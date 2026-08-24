#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tasks.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

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
        
        if (fgets(prompt, sizeof(prompt), stdin) == NULL) {
            printf("Saindo...\n");
            break;
        }    
        
        prompt[strcspn(prompt, "\n")] = '\0';

        char *tokens[20];
        int n = tokenizar(prompt, tokens);  //n guarda o tamanho da lista de tokens
        if (n == 0){
            continue;
        }
        char *comando = tokens[0];   //comandos: task, run, input, output, start, exit

        if (strcmp(comando, "task") == 0){
            cadastrarTask(tokens, n);
        } 
        else if(strcmp(comando, "run") == 0){
            runTask(tokens, n);
        }
        else if(strcmp(comando, "input") == 0){
            input(tokens, n);
        }
        else if(strcmp(comando, "output") == 0){
            output(tokens, n);
        } 
        else if(strcmp(comando, "append") == 0){
            append(tokens, n);
        }
        else if (strcmp(comando, "start") == 0){
            printf("Ainda não implementado\n");
        } else if(strcmp(comando, "exit") == 0){
            rodando = 0;
        } else{
            printf("ERRO: Comando desconhecido\n");
        }
    }

}

int main(int argc, char ** argv){
    if (argc > 2){
        printf("ERRO: Apenas 2 argumentos são suportados\n");
        return 1;
    }
    else if (argc == 2){
        printf("Modo workflow ainda não implementado\n");
        return 1;
    } 
    else if (argc == 1){
        printf("Iniciando modo interativo...\n");
        interativo();
    }
    return 0;
}