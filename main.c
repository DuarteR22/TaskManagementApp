#include<stdio.h>
#include<string.h>
#include<stdbool.h>

typedef struct 
{
    char nomes[20][20];
    bool equipa; //1 se foi elaborada por uma equipa, 0 se não foi
    int numMembros;
}responsavel;

typedef struct 
{
    char nomeTarefa[10];
    char dataCriacao[10];
    char dataLimiteExecucao[10];
    char dataConclusao[10];
    char descricao [50];
    bool concluida; //1 se está concluída, 0 se está por concluir
    responsavel responsavel;
}tarefa;


tarefa menuRegistoTarefa(){

    tarefa tarefa1;
    char nomeTarefa[10],dataCriacao[10],dataLimiteExecucao[10],dataConclusao[10],descricao[50];
    printf("Indique o nome da tarefa a registar: ");
    scanf("%s", nomeTarefa);
    strcpy(tarefa1.nomeTarefa, nomeTarefa);
    printf("Indique a data de criacao da tarefa [dd/mm/aaaa]: ");
    scanf("%s", dataCriacao);
    strcpy(tarefa1.dataCriacao, dataCriacao);
    printf("Indique a data limite de execucao da tarefa [dd/mm/aaaa]: ");
    scanf("%s", dataLimiteExecucao);
    strcpy(tarefa1.dataLimiteExecucao, dataLimiteExecucao);
    printf("Indique a data limite de conclusao da tarefa [dd/mm/aaaa]: ");
    scanf("%s", dataConclusao);
    strcpy(tarefa1.dataConclusao, dataConclusao);
    printf("Indique a descricao da tarefa a executar: ");
    scanf("%s", descricao);
    strcpy(tarefa1.descricao, descricao);
    tarefa1.concluida = 0;

    return tarefa1;
}

void menuCriacaoEquipa(){
    int opcao1;
    int numMembros;
    responsavel responsavel1;
    printf("Indique se a equipa de adicionar se trata de uma pessoa ou uma equipa de varias [1 - Unica pessoa, 2 - Equipa]");
    scanf("%d", &opcao1);
    if (opcao1 == 1)
    {
        char nome[20];
        printf("Indique o nome do responsavel: ");
        scanf("%s", nome);
        strcpy(responsavel1.nomes[0][20], nome);
        responsavel1.numMembros = 1;
        responsavel1.equipa = 0;
    }
    else{
        char nome[20];
        printf("Indique o numero de membros da equipa a adicionar: ");
        scanf("%d", &numMembros);
        responsavel1.numMembros = numMembros;
        for (int i = 0; i < numMembros; i++)
        {
            printf("Indique o nome do %dº membro da equipa", i+1);
            scanf("%s", nome);
            strcpy(nome, responsavel1.nomes[i][30]);
        }
    }
}
void menuPrincipal(){
    int opcao;
    do
    {
        printf("Indique a opcao que deseja: [0 - Sair, 1 - Registar nova tarefa, 2 - Criar equipa, 3 - Alterar tarefa, 4 - Concluir tarefa, 5 - Eliminar tarefa, 6 - Alocar equipa a uma tarefa, 7 - Menu de listagem de tarefas, 8 - Menu de equipas]\n");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:

            menuRegistoTarefa();
            break;
        default:
            printf("Termino do programa");
            break;
        }
    } while (opcao != 0);
    
}
int main(){

    menuPrincipal();
    return 0;
}


