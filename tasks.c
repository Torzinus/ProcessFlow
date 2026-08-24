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
    for (int i = 3; i < n; i ++){
        t->argumentos[j] = tokens[i];
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
}

void input(char ** tokens, int n){  //adiciona o input à struct
    Tarefa * t = buscaTask(tokens[1]);
    strcpy(t->input, tokens[2]);
}

void output(char ** tokens, int n){
    Tarefa * t = buscaTask(tokens[1]);
    strcpy(t->output, tokens[2]);
    t->append = false;
}

void append(char ** tokens, int n){
    Tarefa * t = buscaTask(tokens[1]);
    strcpy(t->output, tokens[2]);
    t->append = true;
}

void redirecionar(Tarefa * t){
    if (strcmp(t->input, "") != 0){
        FILE * arquivo = fopen(t->input, "r");
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
        int fd = fileno(arquivo); 
        dup2(fd, 1);
    }
}


void runTask(char ** tokens, int n){
    if (strcmp(tokens[1],"sequential") == 0){
        for(int i = 2; i < n; i ++){
            Tarefa * t = buscaTask(tokens[i]);    //busca a tarefa a partir do nome
            runSequential(t);
        }
    } 
    else if (strcmp(tokens[1],"parallel") == 0){
        runParallel(tokens, n);
    } else if (strcmp(tokens[1],"pipe") == 0){
        printf("Executando modo pipe...\n");
    } else{
        printf("Erro. Insira um modo de execução válido.\n");
    }
}

void runSequential(Tarefa * t){
    pid_t pid = fork(); //cria processo

    if(pid == 0){   //processo filho
        if (t->input != ""){
            redirecionar(t);
        }
        execvp(t->programa, t->argumentos);
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
        pids[i - 2] = fork();
        
        if(pids[i - 2] == 0){
            if(t->input != ""){
                redirecionar(t);
            }
            execvp(t->programa, t->argumentos);
        } 
    }

    for (int i = 0; i < qtd_tarefas; i++){
        int status;
        waitpid(pids[i], &status, 0);
    }
}