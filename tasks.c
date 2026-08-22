#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tasks.h"
#include <unistd.h>
#include <sys/wait.h>

Tarefa lista_tarefas[100];
int total_tarefas = 0;

Tarefa * cadastrarTask(char ** tokens, int n){
    Tarefa *t = &lista_tarefas[total_tarefas];
    strcpy(t->nome, tokens[1]);
    strcpy(t->programa, tokens[2]);
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

void runTask(char ** tokens, int n){
    if (strcmp(tokens[1],"sequential") == 0){
        for(int i = 2; i < n; i ++){
            Tarefa * t = buscaTask(tokens[i]);    //busca a tarefa a partir do nome
            runSequential(t);
        }
    } 
    else if (strcmp(tokens[1],"parallel") == 0){
        printf("Executando modo paralelo...\n");
    } else if (strcmp(tokens[1],"pipe") == 0){
        printf("Executando modo pipe...\n");
    } else{
        printf("Erro. Insira um modo de execução válido.\n");
    }
}

void runSequential(Tarefa * t){
    pid_t pid = fork(); //cria processo

    if(pid == 0){   //processo filho
        execvp(t->programa, t->argumentos);
    } else if (pid > 0){
        int status;
        waitpid(pid, &status, 0);
    }
}