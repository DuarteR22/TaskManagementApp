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
    int dataCriacao;
    int dataLimiteExecucao;
    int dataConclusao;
    char descricao [50];
    bool concluida; //1 se está concluída, 0 se está por concluir
    responsavel responsavel;
}tarefa;

void menuListagemTarefas(tarefa listaTarefas[100]){
    int opcao;
    do
    {
        printf("Indique a opcao que listagem que deseja: 1 - Listar tarefas em execucao, 2 - Listar tarefas concluidas, 3 - Listar tarefas concluidas em execucao, 4 - Listar tarefas concluidas com atraso, 5 - Listar tarefas atribuidas a uma equipa");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            int count = 0;
            printf("Tarefas em execucao: \n");
            for (int i = 0; i < 100; i++)
            {
                if (listaTarefas[i].concluida == 0)
                {
                    printf("Nome: %s\n", listaTarefas[i].nomeTarefa);
                    printf("Data de criacao da tarefa: %s\n", listaTarefas[i].dataCriacao);
                    count++;
                }
            }
            if(count == 0)
                printf("Não existem tarefas em execucao\n"); 
            
            break;
        case 2:
            int count = 0;
            printf("Tarefas concluidas: \n");
            for (int i = 0; i < 100; i++)
            {
                if (listaTarefas[i].concluida == 1)
                {
                    printf("Nome: %s\n", listaTarefas[i].nomeTarefa);
                    int duracao = listaTarefas[i].dataConclusao - listaTarefas[i].dataCriacao;
                    printf("A tarefa demorou %d dias a ser concluida", duracao);
                    count++;
                }
                
            }
            
            break;
        default:
            break;
        }
    } while (opcao != 0);
    
    
}
void menuRegistoTarefa(tarefa listaTarefas[100]){

    tarefa tarefa1;
    char nomeTarefa[10],dataCriacao[10],dataLimiteExecucao[10],dataConclusao[10],descricao[50];
    printf("Indique o nome da tarefa a registar: ");
    scanf("%s", nomeTarefa);
    strcpy(tarefa1.nomeTarefa, nomeTarefa);
    printf("Indique a data de criacao da tarefa [ddmmaaaa]: ");
    scanf("%d", &tarefa1.dataCriacao);
    printf("Indique a data limite de execucao da tarefa [ddmmaaaa]: ");
    scanf("%d", &tarefa1.dataLimiteExecucao);
    printf("Indique a data limite de conclusao da tarefa [ddmmaaaa]: ");
    scanf("%d", &tarefa1.dataConclusao);
    printf("Indique a descricao da tarefa a executar: ");
    getchar();
    fgets(descricao, sizeof(descricao), stdin);
    strcpy(tarefa1.descricao, descricao);
    tarefa1.concluida = 0;

    for (int i = 0; i < 100; i++)
    {
        if(listaTarefas[i].nomeTarefa[0] == '\0'){
            listaTarefas[i] = tarefa1;
            printf("Tarefa adicionada com sucesso!");
            break;
        }
        if(i == 100){
            printf("Lista de tarefas cheia!");
        }
    }
    
    
    
}

void menuCriacaoEquipa(){
    int opcao1;
    int numMembros;
    char nome[20];
    responsavel responsavel1;
    do
    {
        printf("Indique se a equipa de adicionar se trata de uma pessoa ou uma equipa de varias: \n0 - Sair\n1 - Unica pessoa\n2 - Equipa\n");
        scanf("%d", &opcao1);
        switch (opcao1)
        {
        case 1:
            printf("Indique o nome do responsavel: ");
            scanf("%s", nome);
            strcpy(responsavel1.nomes[0], nome);
            responsavel1.numMembros = 1;
            responsavel1.equipa = 0;
            break;
        case 2:
            printf("Indique o numero de membros da equipa a adicionar: ");
            scanf("%d", &numMembros);
            responsavel1.numMembros = numMembros;
            for (int i = 0; i < numMembros; i++)
            {
                printf("Indique o nome do %dº membro da equipa: ", i+1);
                scanf("%s", nome);
                strcpy(responsavel1.nomes[i], nome);
                
            }
            break;
        default:
            break;
        }
        
    } while (opcao1 != 0);
    printf("Equipa adicionada com sucesso!\n");
}
void menuPrincipal(tarefa listaTarefas[100], responsavel listaResponsavel[100]){
    int opcao;
    do
    {
        printf("0 - Sair\n1 - Registar nova tarefa\n2 - Criar equipa\n3 - Alterar tarefa\n4 - Concluir tarefa\n5 - Eliminar tarefa\n6 - Alocar equipa a uma tarefa\n7 - Menu de listagem de tarefas\n8 - Menu de equipas\nIndique a opcao que deseja: ");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:

            menuRegistoTarefa(listaTarefas);
            break;  
        case 2:
            menuCriacaoEquipa();
            break;
        case 3:
            menuListagemTarefas(listaTarefas);
        default:
            printf("Termino do programa");
            break;
        }
    } while (opcao != 0);
    
}
int main(){



    responsavel listaResponsavel[100];
    tarefa listaTarefas[100];

    menuPrincipal(listaTarefas, listaResponsavel);

    return 0;
}


