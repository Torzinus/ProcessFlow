#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tasks.h"
#include <unistd.h>
#include <sys/wait.h>

int tokenizar(char * prompt, char ** tokens){
    int i = 0;
    char *token = strtok(prompt, " ");
    while(token != NULL){
        tokens[i] = token;
        i ++;
        token = strtok(NULL, " ");
    }
    tokens[i] = NULL;
    return i;
}

void interativo(){
    char prompt[100];
    int rodando = 1;
    while(rodando == 1){
        printf("processflow> ");
        fgets(prompt, sizeof(prompt), stdin);
        prompt[strcspn(prompt, "\n")] = '\0';

        char *tokens[20];
        int n = tokenizar(prompt, tokens);  //n guarda o tamanho da lista de tokens
        char *comando = tokens[0];   //comandos: task, run, input, output, start, exit

        if (strcmp(comando, "task") == 0){
            cadastrarTask(tokens, n);
        } 
        else if(strcmp(comando, "run") == 0){
            runTask(tokens, n);
        }
        else if(strcmp(comando, "input") == 0){
            printf("Recebendo entrada de arquivo...\n");
        }
        else if(strcmp(comando, "output") == 0){
            printf("Enviando saída para arquivo...\n");
        }
        else if (strcmp(comando, "start") == 0){
            printf("Iniciando tarefa...\n");
        } else if(strcmp(comando, "exit") == 0){
            printf("Saindo...\n");
            rodando = 0;
        }
    }

}

int main(int argc, char ** argv){
    if (argc == 2){
        printf("Iniciando modo workflow...\n");
    } 
    else if (argc == 1){
        printf("Iniciando modo interativo...\n");
        interativo();
    }
    return 0;
}