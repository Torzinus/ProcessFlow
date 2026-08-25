#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tasks.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

Tarefa lista_tarefas[100];
int total_tarefas = 0;

Tarefa * cadastrarTask(char ** tokens, int n){
    Tarefa *t = &lista_tarefas[total_tarefas];
    strcpy(t->nome, tokens[1]);
    strcpy(t->programa, tokens[2]);
    strcpy(t->input, ""); 
    strcpy(t->output, "");
    t->append = false;

    int j = 0;
    t->argumentos[j] = strdup(t->programa);
    j++;
    for (int i = 3; i < n; i ++){
        t->argumentos[j] = strdup(tokens[i]); 
        j++;
    }

    t->argumentos[j] = NULL;

    total_tarefas++;

    printf("Tarefa %s - %s cadastrada com sucesso\n", t->nome, t->programa);
    return t;
}
Tarefa * buscaTask(char *nome){
    Tarefa *t = lista_tarefas;

    for (int i = 0; i < total_tarefas; i++){
        if(strcmp(t->nome, nome) == 0){
            return t;
        }
        t++;
    }
    return NULL;    //se achou, retorna t, caso contrário, retorna NULL
}

void input(char ** tokens, int n){  //adiciona o input à struct
    Tarefa * t = buscaTask(tokens[1]);
    if (t == NULL){ //verifica se a tarefa existe
        printf("ERRO: Tarefa %s não encontrada\n", tokens[1]);
        return;
    }
    strcpy(t->input, tokens[2]);
}

void output(char ** tokens, int n){
    Tarefa * t = buscaTask(tokens[1]);
    if (t == NULL){
        printf("ERRO: Tarefa %s não encontrada\n", tokens[1]);
        return;
    }
    strcpy(t->output, tokens[2]);
    t->append = false;
}

void append(char ** tokens, int n){
    Tarefa * t = buscaTask(tokens[1]);
    if (t == NULL){
        printf("ERRO: Tarefa %s não encontrada\n", tokens[1]);
        return;
    }
    strcpy(t->output, tokens[2]);
    t->append = true;
}

void redirecionar(Tarefa * t){
    if (strcmp(t->input, "") != 0){
        FILE * arquivo = fopen(t->input, "r");
        if(arquivo == NULL){
            printf("ERRO: Não foi possível abrir o arquivo %s\n", t->input);
            exit(1);    //encerra o processo filho
        }
        int fd = fileno(arquivo); 
        dup2(fd, 0);
    }
    if (strcmp(t->output, "") != 0){
        FILE * arquivo;
        if(t->append == true){
            arquivo = fopen(t->output, "a");
        } else{
            arquivo = fopen(t->output, "w");
        }
        if (arquivo == NULL){
            printf("ERRO: Não foi possível abrir o arquivo %s\n", t->output);
            exit(1);
        }
        int fd = fileno(arquivo); 
        dup2(fd, 1);
    }
}


void runTask(char ** tokens, int n){
    if (strcmp(tokens[1],"sequential") == 0){
        for(int i = 2; i < n; i ++){
            Tarefa * t = buscaTask(tokens[i]);    //busca a tarefa a partir do nome
            if (t == NULL){
                printf("ERRO: Tarefa %s não encontrada\n", tokens[i]);
                continue;   //se a tarefa não foi encontrada, ele ignora e passa pra próxima
            }
            runSequential(t);
        }
    } 
    else if (strcmp(tokens[1],"parallel") == 0){
        runParallel(tokens, n);
    } else if (strcmp(tokens[1],"pipe") == 0){
        printf("Executando modo pipe...\n");
    } else{
        for(int i = 1; i < n; i ++){
            Tarefa * t = buscaTask(tokens[i]);   
            if (t == NULL){
                printf("ERRO: Tarefa %s não encontrada\n", tokens[i]);
                continue;   
            }
            runSequential(t);
        }
    }
}

void runSequential(Tarefa * t){
    pid_t pid = fork(); //cria processo

    if(pid == 0){   //processo filho
        if (t->input != ""){
            redirecionar(t);
        }
        execvp(t->programa, t->argumentos);

        printf("ERRO: Não foi possível executar o programa %s\n", t->programa);
        exit(1);
    } else if (pid > 0){
        int status;
        waitpid(pid, &status, 0);
    }
}

void runParallel(char ** tokens, int n){
    pid_t pids[10];
    int qtd_tarefas = n - 2;

    for (int i = 2; i < n; i++){
        Tarefa * t = buscaTask(tokens[i]);  //busca a tarefa várias vezes, para cada nome digitado pelo usuário

        if (t == NULL){
            printf("ERRO: Tarefa %s não encontrada\n", tokens[i]);
            continue;
        }

        pids[i - 2] = fork();
        
        if(pids[i - 2] == 0){
            if(t->input != ""){
                redirecionar(t);
            }
            execvp(t->programa, t->argumentos);

            printf("ERRO: Não foi possível executar o programa %s\n", t->programa);
            exit(1);
        } 
    }

    for (int i = 0; i < qtd_tarefas; i++){
        int status;
        waitpid(pids[i], &status, 0);
    }
}