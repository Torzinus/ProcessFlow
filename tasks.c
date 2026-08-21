#include <stdio.h>
#include <string.h>
#include "tasks.h"

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

