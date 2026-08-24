#define TASKS_H

typedef struct Tarefa{
    char nome[50];
    char programa[50];
    char *argumentos[10];
    char input[100];
    char output[100];
    bool append;    //guarda se é append ou output
} Tarefa;

Tarefa * cadastrarTask(char ** tokens, int n);
Tarefa * buscaTask(char * nome);
void input(char ** tokens, int n);
void output(char ** tokens, int n);
void append(char ** tokens, int n);
void redirecionar(Tarefa * t);
void runTask(char ** tokens, int n);
void runSequential(Tarefa * t);
void runParallel(char ** tokens, int n);