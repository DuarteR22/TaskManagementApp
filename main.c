#include<stdio.h>
#include<string.h>
#include<stdbool.h>


typedef struct 

{
    char nomeTarefa[10];
    char dataCriacao[10];
    char dataLimiteExecucao[10];
    char dataConclusao[10];
    char descricao;
    bool concluida; //1 se está concluída, 0 se está por concluir
    responsavel responsavel;
}tarefa;

typedef struct 
{
    char nomes[20][20];
    bool equipa; //1 se foi elaborada por uma equipa, 0 se não foi
    int numMembros;
}responsavel;

void menuPrincipal(){
    int opcao;
    do
    {
        printf("Indique a opção que deseja: \n");
        switch (opcao)
        {
        case 1:

            printf("I");
            break;
        
        default:

            break;
        }
    } while (opcao != 0);
    
}
int main(){


    return 0;
}


