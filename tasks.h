#define TASKS_H

typedef struct Tarefa{
    char nome[50];
    char programa[50];
    char *argumentos[10];
} Tarefa;

Tarefa * cadastrarTask(char ** tokens, int n);
