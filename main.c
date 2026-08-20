#include <stdio.h>
#include <string.h>

void interativo(){
    char prompt[100];
    int rodando = 1;
    while(rodando == 1){
        printf("processflow> ");
        fgets(prompt, sizeof(prompt), stdin);

        char  *comando = strtok(prompt, " ");
        if (strcmp(comando, "task") == 0){
            printf("Cadastrando tarefa...\n");
        } 
        else if(strcmp(comando, "run") == 0){
            printf("Executando tarefa...\n");
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
            rodando == 0;
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