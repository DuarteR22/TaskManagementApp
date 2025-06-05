#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include<stdlib.h>


typedef struct
{
    int dias;
    int mes;
    int ano;
}data;
typedef struct 
{
    char nomes[20][20];
    bool equipa; //1 se foi elaborada por uma equipa, 0 se não foi
    int numMembros;
}responsavel;

typedef struct tarefa
{
    char nomeTarefa[50];
    data dataCriacao;
    data dataLimiteExecucao;
    data dataConclusao;
    char descricao [50];
    bool concluida; //1 se está concluída, 0 se está por concluir
    bool eliminada; //1 se foi eliminada, 0 se continua presente
    responsavel responsavel;
    struct tarefa *seguinte;
}tarefa;

typedef struct projeto
{
    char nomeProjeto[50];
    char nomeResponsavel[20];
    data dataInicio;
    data dataConclusao;
    tarefa *tarefasConcluidas;
    tarefa *tarefasNaoConcluidas;
}projeto;

data obterDataHoje(){
    time_t t= time(NULL);
    struct tm tm = *localtime(&t);

    data hoje;
    hoje.dias = tm.tm_mday;
    hoje.mes = tm.tm_mon + 1;
    hoje.ano = tm.tm_year + 1900;

    return hoje;
}
void guardarFicheiroTarefasResponsavel(tarefa listaTarefas[100], responsavel listaResponsavel[100]){
    FILE *ftarefas = fopen("tarefas.dat", "wb");
    FILE *fresponsavel = fopen("responsavel.dat", "wb");

    if (ftarefas == NULL || fresponsavel == NULL)
    {
        printf("Erro ao abrir os ficheiros de tarefas ou responsaveis\n");
        return;
    }
    fwrite(listaTarefas, sizeof(tarefa), 100, ftarefas);
    fwrite(listaResponsavel, sizeof(responsavel), 100, fresponsavel);

    fclose(ftarefas);
    fclose(fresponsavel);

}
void carregaDadosFicheiroTarefasResponsavel(tarefa listaTarefas[100], responsavel listaResponsavel[100]){

    FILE *ftarefas = fopen("tarefas.dat", "rb");
    FILE *fresponsavel = fopen("responsavel.dat", "rb");

    if (ftarefas != NULL)
    {
        fread(listaTarefas, sizeof(tarefa), 100, ftarefas);
    }
    if (fresponsavel != NULL)
    {
        fread(listaResponsavel,  sizeof(responsavel), 100, fresponsavel);
    }
    if (ftarefas != NULL)
        fclose(ftarefas);
    if(fresponsavel != NULL)
        fclose(fresponsavel);
}

int diferencaDias(data dataInicio,data dataFim ){
    int diasInicio = dataInicio.ano * 365 + dataInicio.mes*30 + dataInicio.dias;
    int diasFim = dataFim.ano * 365 + dataFim.mes * 30 + dataFim.dias;

    return diasFim - diasInicio;
}
//Função auxiliar para ir buscar o índice do projeto através do seu nome
int indiceProjetoAux(projeto listaProjetos[100], char nomeProjeto[50]) {
    for (int i = 0; i < 100; i++) {
        if (strcmp(listaProjetos[i].nomeProjeto, nomeProjeto) == 0) {
            return i;
        }
    }
    return -1;
}
//Função auxiliar que pretende indicar se o projeto se encontra cancelado
bool verificaCancelado(projeto p){

    if(p.tarefasConcluidas == NULL && p.tarefasNaoConcluidas == NULL)
        return true;

    return false;
}
void menuEquipas(responsavel listaResponsavel [100], tarefa listaTarefas[100]){
    char nomeEquipa[20];
    int opcao;
    do
    {
        printf("0 - Sair\n1 - Determinar duracao media de realizacao de tarefas de uma equipa\n2 - Determinar numero de tarefas concluidas por uma equipa dentro e fora do prazo\n3 - Determinar o numero de tarefas em execucao de uma equipa\nIndique a opcao que deseja: ");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            printf("Indique o nome da equipa a pesquisar: ");
            scanf("%s", nomeEquipa);
            int duracoes = 0;
            int count = 0;
            for (int i = 0; i < 100; i++)
            {
                if (listaTarefas[i].concluida && !listaTarefas[i].eliminada)
                {
                    for (int j = 0; j < 20; j++)
                    {
                        if (strcmp(listaTarefas[i].responsavel.nomes[j], nomeEquipa)== 0)
                        {
                            int duracao = diferencaDias(listaTarefas[i].dataConclusao, listaTarefas[i].dataCriacao);
                            duracoes += duracao;
                            count++;
                            break;
                        }
                    }
                }  
            }
            if (count > 0)
            {
                float media = (float)duracoes / count;
                printf("Duracao media de realizacao das tarefas concluidas pela equipa %s em dias e %f\n", nomeEquipa, -(media));
            }
            if (count == 0)
            {
                printf("Nao existe nenhuma tarefa concluida por uma equipa com o nome %s ou introduziu um nome errado!\n", nomeEquipa);
            }
            break;  
        case 2:
            printf("Indique o nome da equipa a pesquisar: ");
            scanf("%s", nomeEquipa);
            int countTarefasDentroPrazo = 0;
            int countTarefasForaPrazo = 0;
            for (int i = 0; i < 100; i++)
            {
                if (listaTarefas[i].concluida && !listaTarefas[i].eliminada)
                {
                    for (int j = 0; j < 20; j++)
                    {
                        if (strcmp(listaTarefas[i].responsavel.nomes[j], nomeEquipa)== 0)
                        {
                            if (diferencaDias(listaTarefas[i].dataConclusao, listaTarefas[i].dataLimiteExecucao) <= 0)
                            {
                                countTarefasDentroPrazo++;
                                break;
                            }
                            else{
                                countTarefasForaPrazo++;
                                break;
                            }
                        }
                    }
                }  
            }
            if (countTarefasDentroPrazo == 0 && countTarefasForaPrazo == 0)
            {
                printf("Nao existe nenhuma tarefa concluida por uma equipa com o nome %s ou introduziu um nome errado!\n", nomeEquipa);
            }
            break;
        case 3:
            printf("Indique o nome da equipa a pesquisar: ");
            scanf("%s", nomeEquipa);
            int countExecucao = 0;
            int duracao;
            data hoje = obterDataHoje();
            for (int i = 0; i < 100; i++)
            {
                if (!listaTarefas[i].concluida && !listaTarefas[i].eliminada)
                {
                    for (int j = 0; j < 20; j++)
                    {
                        if (strcmp(listaTarefas[i].responsavel.nomes[j], nomeEquipa)== 0)
                        {
                            duracao = diferencaDias(hoje, listaTarefas[i].dataCriacao);
                            printf("A tarefa de nome %s esta em execucao a %d dias\n", listaTarefas[i].nomeTarefa, duracao);
                            countExecucao++;
                            break;
                        }
                    }
                }  
            }
            if (countExecucao == 0)
            {
                printf("Nao existe nenhuma tarefa concluida por uma equipa com o nome %s ou introduziu um nome errado!\n", nomeEquipa);
                break;
            }
            else{
                printf("Existem %d tarefas em execucao", countExecucao);
                break;
            }
            default:
                printf("Indique uma opcao dentro das indicadas!");
                break;
        }
    } while (opcao != 0);
}

void alocarEquipa(projeto listaProjetos[100], responsavel listaResponsavel[100]) {
    char nomeProjeto[50];
    char nomeTarefa[50];
    char nomeEquipa[50];

    bool encontrouTarefa;
    bool encontrouResponsavel;

    printf("Indique o nome do projeto a que pretende alocar uma equipa: ");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado errado!\n");
        return;
    }
    printf("Indique o nome da tarefa a que pretende alocar uma equipa: ");
    scanf("%s", nomeTarefa);
    tarefa *t = listaProjetos[indiceProjeto].tarefasNaoConcluidas;
    do
    {
        if (strcmp(t->nomeTarefa, nomeTarefa) == 0 && !t->eliminada)
        {
            encontrouTarefa = true;
            char nomeResponsavel[50];

            printf("Indique o nome de um responsavel da equipa que pretende alocar a tarefa: ");
            scanf("%s", nomeResponsavel);

            for (int i = 0; i < 100; i++)
            {
                for (int j = 0; j < listaResponsavel[i].numMembros; j++)
                {
                    if (strcmp(listaResponsavel[i].nomes[j], nomeResponsavel)== 0)
                    {
                        t->responsavel = listaResponsavel[i];
                        t->responsavel.equipa = true;
                        encontrouResponsavel = true;
                        printf("Equipa alocada com sucesso a tarefa\n");
                        break;
                    }   
                }
                break; 
            }
            if (!encontrouResponsavel)
            {
                printf("Nome de responsavel incorreto ou nao presente na lista de responsaveis\n");
            }
            return;
        }
        t = t->seguinte;
        
    } while (t != NULL);
    if (!encontrouTarefa)    
        {
            printf("Nome de tarefa incorreto ou nao presente na lista de tarefas\n");
        }
}
void alterarTarefa(projeto listaProjetos[100], responsavel listaResponsavel[100]){

    char nomeProjeto[50];
    printf("Indique o nome do projeto que pretende alterar a tarefa: ");
    scanf("%s", nomeProjeto);

    int indiceProjeto = indiceProjetoAux(listaProjetos,nomeProjeto);

    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado errado!\n");
        return;
    }
    char nomeTarefa[20];
    printf("Indique o nome da tarefa a alterar: ");
    scanf("%s", nomeTarefa);
    tarefa *t = listaProjetos[indiceProjeto].tarefasNaoConcluidas;
    while(t != NULL){

        if (strcmp(t->nomeTarefa, nomeTarefa) == 0 && !t->eliminada)
        {
            int opcao;
            do
            {
                printf("Indique que dados pretende alterar da tarefa:\n0 - Sair\n1 - Data limite de execucao\n2 - Responsavel\n3 - Descricao\n");
                scanf("%d", &opcao);
                switch (opcao)
                {
                    case 1:
                        char novaDataLimite[10];
                        printf("Indique a nova data a inserir na tarefa [dd/mm/aaaa]: ");
                        scanf("%s", novaDataLimite);
                        sscanf(novaDataLimite, "%d/%d/%d",&t->dataLimiteExecucao.dias,&t->dataLimiteExecucao.mes,&t->dataLimiteExecucao.ano);
                        printf("Data de execucao limite alterada com sucesso! \n");                 
                        break;
                    case 2:
                        char nomeResponsavel[50];
                        bool responsavelEncontrado = false;
                        printf("Indique o nome do responsavel/equipa responsavel a atribuir");
                        for (int i = 0; i < 100; i++)
                        {
                            for (int j = 0; j < listaResponsavel[j].numMembros; j++)
                            {
                                if(strcmp(listaResponsavel[i].nomes[j], nomeResponsavel) == 0){
                                    t->responsavel = listaResponsavel[i];
                                    printf("Responsavel alterado com sucesso\n");
                                    responsavelEncontrado = true;
                                    break;
                                }
                            }
                        }
                        if (!responsavelEncontrado)
                        {
                            printf("Nome de responsavel incorreto ou nao presente na lista de responsaveis");
                            break;
                        }
                        break;
                    case 3:
                        char novaDescricao[50];
                        printf("Indique nova descricao a inserir na tarefa: ");
                        scanf("%s\n", novaDescricao);
                        strcpy(t->descricao, novaDescricao);
                        printf("Descricao alterada com sucesso!\n");
                        break;
                    case 0:
                        printf("Sair da edicao de tarefas\n");
                        break;
                    default:
                        printf("Insira uma opcao entre as indicadas!\n");
                        break;
                    }
                } while (opcao != 0);
                return;
            }
        t = t->seguinte;
    }
    printf("Nenhuma tarefa encontrada com o nome indicado\n");

}
void eliminarTarefa(projeto listaProjetos[100]){
    char nomeProjeto[50],nomeTarefa[50];
    bool eliminada = false;
    bool encontrou = true;
    int opcao;
    printf("Indique o nome do projeto que pretende eliminar a tarefa: ");
    scanf("%s", nomeProjeto);

    int indiceProjeto = indiceProjetoAux(listaProjetos,nomeProjeto);

    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado errado!\n");
        return;
    }
    printf("Indique o nome da tarefa a eliminar: ");
    scanf("%s", nomeTarefa);

    tarefa *t = listaProjetos[indiceProjeto].tarefasNaoConcluidas;
    tarefa *anterior = NULL;

    do
    {
       if (strcmp(t->nomeTarefa, nomeTarefa) == 0 && !t->eliminada)
       {
        encontrou = true;
        if (anterior == NULL)
        {
            listaProjetos[indiceProjeto].tarefasNaoConcluidas = t->seguinte;
        }
        else
            anterior->seguinte = t->seguinte;

        free(t);
        printf("Tarefa eliminada com sucesso!\n");
        return;
       }
       anterior = t;
       t = t->seguinte;
    } while (t!=NULL);
    if (!encontrou)
    {
        printf("Nenhuma tarefa encontrada com o nome indicado!\n");
    }
    
}
void concluirTarefa(projeto listaProjetos[100]){
    char nomeProjeto[50],nomeTarefa[50];
    bool encontrada = false;
    printf("Indique o nome do projeto que pretende alterar a tarefa: ");
    scanf("%s", nomeProjeto);

    int indiceProjeto = indiceProjetoAux(listaProjetos,nomeProjeto);

    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado errado!\n");
        return;
    }
    printf("Indique o nome da tarefa a concluir: ");
    scanf("%s", nomeTarefa);

    tarefa *t = listaProjetos[indiceProjeto].tarefasNaoConcluidas;
    tarefa *anterior = NULL;
    do
    {
        if (strcmp(t->nomeTarefa, nomeTarefa) == 0 && !t->eliminada && !t->concluida)
        {
            if (anterior == NULL)
            {
                listaProjetos[indiceProjeto].tarefasNaoConcluidas = t->seguinte;
            }
            else
                anterior->seguinte = t->seguinte;

            t->concluida = true;
            t->dataConclusao = obterDataHoje();
            t->seguinte = listaProjetos[indiceProjeto].tarefasConcluidas;
            listaProjetos[indiceProjeto].tarefasConcluidas = t;
            printf("Tarefa concluida com sucesso!\n");
            encontrada = true;
            return;
        }
        anterior = t;
        t = t->seguinte;
    } while (t!=NULL);
    if (!encontrada)
    {
        printf("Nenhuma tarefa encontrada com o nome indicado!\n");
    }
}

void menuRegistoTarefa(projeto listaProjetos[100]){

    tarefa tarefa1;
    char nomeProjeto[50];
    int indiceProjeto = indiceProjetoAux(listaProjetos,nomeProjeto);
    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado errado!\n");
         return;
    }
    char descricao[50];
    tarefa *nova = malloc(sizeof(tarefa));
    if(!nova){
        printf("Erro ao alocar memoria para a nova tarefa!\n");
        return;
    }

    printf("Indique o nome da tarefa a registar: ");
    scanf("%s", nova->nomeTarefa);
    printf("Indique a data de criacao da tarefa [dd/mm/aaaa]: ");
    scanf("%d/%d/%d", &nova->dataCriacao.dias, &nova->dataCriacao.mes, &nova->dataCriacao.ano);
    printf("Indique a data limite de execucao da tarefa [dd/mm/aaaa]: ");
    scanf("%d/%d/%d", &nova->dataLimiteExecucao.dias, &nova->dataLimiteExecucao.mes, &nova->dataLimiteExecucao.ano);
    printf("Indique a data de conclusao da tarefa [dd/mm/aaaa]: ");
    scanf("%d/%d/%d", &nova->dataConclusao.dias, &nova->dataConclusao.mes, &nova->dataConclusao.ano);
    printf("Indique a descricao da tarefa a executar: ");
    getchar();
    fgets(descricao, sizeof(descricao), stdin);
    strcpy( nova->descricao, descricao);
    nova->concluida = 0;
    nova->eliminada = 0;
    nova->responsavel.numMembros = 0;
    nova->seguinte = NULL;

    nova->seguinte = listaProjetos[indiceProjeto].tarefasNaoConcluidas;
    listaProjetos[indiceProjeto].tarefasNaoConcluidas = nova;

    printf("Tarefa adicionada com sucesso ao projeto %s\n", nomeProjeto);

}

void menuCriacaoEquipa(responsavel listaResponsavel[100]) {
    int opcao1;
    int numMembros;
    char nome[20];
    responsavel responsavel1;

    do {
        printf("Indique se a equipa de adicionar se trata de uma pessoa ou uma equipa de varias: \n0 - Sair\n1 - Unica pessoa\n2 - Equipa\n");
        scanf("%d", &opcao1);
        switch (opcao1) {
            case 1:
                printf("Indique o nome do responsavel: ");
                scanf("%s", nome); 
                strcpy(responsavel1.nomes[0], nome);
                responsavel1.numMembros = 1;
                responsavel1.equipa = 0;
                for (int i = 0; i < 100; i++) {
                    if (listaResponsavel[i].nomes[0][0] == '\0') {

                        listaResponsavel[i] = responsavel1;
                        break;
                    }
                }
                printf("Responsavel adicionado com sucesso!\n");
                break;

            case 2:
                printf("Indique o numero de membros da equipa a adicionar: ");
                scanf("%d", &numMembros);
                if (numMembros > 20) {
                    printf("Numero de membros excede o limite de 20.\n");
                    break;
                }
                responsavel1.numMembros = numMembros;
                responsavel1.equipa = 1;

                for (int i = 0; i < numMembros; i++) {
                    printf("Indique o nome do %dº membro da equipa: ", i + 1);
                    scanf("%s",nome); 
                    strcpy(responsavel1.nomes[i], nome);
                }
                for (int i = 0; i < 100; i++) {
                    if (listaResponsavel[i].nomes[0][0] == '\0') {
                        listaResponsavel[i] = responsavel1;
                        break;
                    }
                }
                printf("Equipa de responsaveis adicionada com sucesso!\n");
                break;
            default:
                break;
        }
    } while (opcao1 != 0);
}
void menuPrincipalTarefas(tarefa listaTarefas[100], responsavel listaResponsavel[100], projeto listaProjetos[100]){
    int opcao;
    do
    {
        printf("0 - Sair\n1 - Registar nova tarefa\n2 - Criar equipa\n3 - Alterar tarefa\n4 - Concluir tarefa\n5 - Eliminar tarefa\n6 - Alocar equipa a uma tarefa\n7 - Menu de listagem de tarefas\n8 - Ordenar tarefas nao concluidas por ordem de urgencia\n9 - Menu de equipas\nIndique a opcao que deseja: ");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            menuRegistoTarefa(listaProjetos);
            break;  
        case 2:
            menuCriacaoEquipa(listaResponsavel);
            break;
        case 3:
            alterarTarefa(listaProjetos, listaResponsavel);
            break;
        case 4:
            concluirTarefa(listaTarefas);
            break;
        case 5:
            eliminarTarefa(listaTarefas);
            break;
        case 6:
            alocarEquipa(listaProjetos, listaResponsavel);
            break;
        case 8:
            break;
        case 7:
            menuListagemTarefas(listaTarefas, listaResponsavel);
            break;
        case 9:
            menuEquipas(listaResponsavel, listaTarefas);
            break;
        case 10:
            break;
        default:
            printf("Termino do programa");
            break;
        }
    } while (opcao != 0);
}


void menuRegistoProjeto(projeto listaProjetos[100]){

    projeto projeto1;
    char nomeProjeto[50],nomeResponsavel[20], dataInicio[10],dataConclusao[10];
    printf("Indique o nome do projeto a registar: ");
    scanf("%s", nomeProjeto);
    strcpy(projeto1.nomeProjeto, nomeProjeto);
    printf("Indique a data prevista de inicio do projeto [dd/mm/aaaa]: ");
    scanf("%s", dataInicio);
    sscanf(dataInicio, "%d/%d/%d", &projeto1.dataInicio.dias,&projeto1.dataInicio.mes,&projeto1.dataInicio.ano);
    printf("Indique a data prevista de conclusao do projeto [dd/mm/aaaa]: ");
    scanf("%s", dataConclusao);
    sscanf(dataConclusao, "%d/%d/%d", &projeto1.dataConclusao.dias,&projeto1.dataConclusao.mes,&projeto1.dataConclusao.ano);
    
    for (int i = 0; i < 100; i++)
    {
        if(listaProjetos[i].nomeProjeto[0] == '\0'){
            listaProjetos[i] = projeto1;
            printf("Projeto adicionado com sucesso!\n");
            char c;
            printf("Prima qualquer tecla para regressar ao menu principal");
            getchar();
            scanf("%c", &c);
            break;
        }
        if(i == 99){
            printf("Lista de projetos cheia!\n");
        }
    }
}
void alterarProjeto(projeto listaProjetos[100]){
    char nomeProjeto[50];
    char nomeResponsavel [50];
    bool responsavelEncontrado = false;
    char novaDataInicio[10];
    char novaDataConclusao[10];

    int opcao;
    do
    {
        printf("Indique o nome do projeto a alterar: ");
        scanf("%s", nomeProjeto);

        bool encontrada = false;
        for (int i = 0; i < 100; i++)
        {
            if (strcmp(listaProjetos[i].nomeProjeto, nomeProjeto) == 0)
            {
                
                int opcao=0;
                do
                {
                    printf("Indique que dados pretende alterar do projeto:\n0 - Sair\n1 - Nome do projeto\n2 - Responsavel do projeto\n3 - Data prevista de inicio\n4 - Data prevista de conclusao\n");
                    scanf("%d", &opcao);
                    switch (opcao)
                    {
                    case 1:
                        printf("Indique o novo nome do projeto: ");
                        scanf("%s", nomeProjeto);
                        strcpy(listaProjetos[i].nomeProjeto, nomeProjeto);
                        break;
                    case 2:
                        printf("Indique o nome do novo responsavel do projeto: ");
                        scanf("%s", nomeResponsavel);
                        strcpy(listaProjetos[i].nomeResponsavel, nomeResponsavel);
                        break;
                    case 3:
                        printf("Indique a nova data de inicio a inserir no projeto [dd/mm/aaaa]: ");
                        scanf("%s", novaDataInicio);
                        sscanf(novaDataInicio, "%d/%d/%d", &listaProjetos[i].dataInicio.dias, &listaProjetos[i].dataInicio.mes, &listaProjetos[i].dataInicio.ano);
                        printf("Data de inicio alterada com sucesso! \n");                 
                        break;
                    case 4:
                        printf("Indique a nova data de conclusao a inserir no projeto [dd/mm/aaaa]: ");
                        scanf("%s", novaDataConclusao);
                        sscanf(novaDataConclusao, "%d/%d/%d", &listaProjetos[i].dataConclusao.dias, &listaProjetos[i].dataConclusao.mes, &listaProjetos[i].dataConclusao.ano);
                        printf("Data de conclusao do projeto alterada com sucesso! \n");         
                        break;
                    default:
                        printf("Insira uma opcao entre as indicadas!\n");
                        break;
                    }
                } while (opcao != 0);
                printf("Nenhuma tarefa encontrada com o nome indicado\n");
            }
        }

        if (!encontrada && opcao != 0) {
            printf("Nenhuma tarefa encontrada com o nome indicado!\n0 - Sair\n1 - Tentar novamente\n");
            scanf("%d", &opcao);
            if (opcao == 0) break;
        } else {
            break; 
        }

    } while (true);
}
void cancelarProjeto(projeto listaProjetos[100]){
    char nomeProjeto[50];
    int opcao;
    do
    {
        printf("Indique o nome do projeto a cancelar: ");
        scanf("%s", nomeProjeto);

        bool eliminada = false; 
        for (int i = 0; i < 100; i++)
        {
            if (strcmp(listaProjetos[i].nomeProjeto, nomeProjeto) == 0)
            {
                printf("Projeto cancelado com sucesso!\n");
                eliminada = true;
                char c;
                printf("Prima qualquer tecla para regressar ao menu principal");
                getchar();
                scanf("%c", &c);
                break;
            }
        }

        if (!eliminada) {
            printf("Nenhum projeto encontrado com o nome indicado!\n0 - Sair\n1 - Tentar novamente\n");
            scanf("%d", &opcao);
            if (opcao == 0) break;
        } else {
            break; 
        }
    } while (true);
}
void menuPrincipalProjetos(projeto listaProjetos[100]){
    int opcao;
    do
    {
        printf("0 - Sair\n1 - Registar novo projeto\n2 - Editar projeto\n3 - Eliminar projeto\n4 - Cancelar projeto\n5 - Menu de listagem de projetos\n6 - Menu de ordenacao de projetos\n7 - Apresentar resumo de projeto concluido\nIndique a opcao que deseja: ");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            menuRegistoProjeto(listaProjetos);
            break;  
        case 2:
            alterarProjeto(listaProjetos);
            break;
        case 3:
            
            break;
        case 4:
            cancelarProjeto(listaProjetos);
            break;
        case 5:
            break;
        case 6:
            break;
        case 8:
            break;
        case 7:
            break;
        case 9:
            break;
        case 10:
            break;
        default:
            printf("Termino do programa");
            break;
        }
    } while (opcao != 0);
}
void listarTarefasExecucao(projeto listaProjetos[100]){
    char nomeProjeto[20];
    printf("Indique o nome do projeto a listar: ");
    scanf("%s", nomeProjeto);
    for (int i = 0; i < 100; i++)
    {
        
    }  
}

void listarTarefasEmExecucao(projeto listaProjetos[100]){

    char nomeProjeto[50];
    bool encontrouProjeto = false;
    bool encontrouTarefa = false;

    printf("Indique o nome do projeto que pretende listar as tarefas em exeucao");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == 1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado erradO!\n");
        return;
    }
    encontrouProjeto = true;
    tarefa *t = listaProjetos[indiceProjeto].tarefasNaoConcluidas;
    printf("Tarefas em execucao do projeto %s", listaProjetos[indiceProjeto]);
    do
    {
        encontrouTarefa = true;
        printf("Nome da tarefa: %s Data de inicio da tarefa: %d/%d/%d", t->nomeTarefa, t->dataCriacao.dias,t->dataCriacao.mes,t->dataCriacao.ano);
    
        if (t != NULL)
        {
            t = t->seguinte;
        }
        
    } while (t != NULL);
    if(!encontrouTarefa){
        printf("Nao existem tarefas em execucao no projeto de nome %s\n", nomeProjeto);
    }    
}
void listarTarefasConcluidas(projeto listaProjetos[100]){

    char nomeProjeto[50];
    bool encontrouProjeto = false;
    bool encontrouTarefa = false;

    printf("Indique o nome do projeto que pretende listar as tarefas em exeucao");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == 1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado erradO!\n");
        return;
    }
    encontrouProjeto = true;
    tarefa *t = listaProjetos[indiceProjeto].tarefasConcluidas;
    printf("Tarefas em execucao do projeto %s", listaProjetos[indiceProjeto]);
    do
    {
        encontrouTarefa = true;
        printf("Nome da tarefa: %s Duracao da conclusao da tarefa: %d", t->nomeTarefa,"Dias de duracao da tarefa: " diferencaDias(t->dataCriacao, t->dataConclusao));
    
        if (t != NULL)
        {
            t = t->seguinte;
        }
        
    } while (t != NULL);
    if(!encontrouTarefa){
        printf("Nao existem tarefas em execucao no projeto de nome %s\n", nomeProjeto);
    }    
}
void menuListagemProjetos(projeto listaProjetos[100]){
char nomeProjeto[20];
int opcao;
do
{
    printf("Indique 0 - Sair\n1 - Listagem de tarefas em execucao\n2 - Listagem de tarefas concluidas\n3 - Listagem de tarefas que ultrapassaram o prazo de conclusao\n4 - Listagem de tarefas concluidas com atraso\n5 - Listagem de tarefas atribuidas a uma equipa\n");
    switch (opcao)
    {
    case 1:
        listarTarefasEmExecucao(listaProjetos);
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    default:
        break;
    }
} while (opcao != 0);


}
void menuPrincipal(tarefa listaTarefas[100], responsavel listaResponsavel[100], projeto listaProjetos[100]){
    int opcao;
    do
    {
        printf("0 - Sair\n1 - Menu de Projetos\n2 - Menu de Tarefas\nIndique a opcao que deseja: ");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            break;  
        case 2:
            menuPrincipalTarefas(listaTarefas, listaResponsavel, listaProjetos);
            break;
               default:
            printf("Termino do programa");
            break;
        }
    } while (opcao != 0);
}

int main(){


    responsavel listaResponsavel[100] ;
    tarefa listaTarefas[100] ;
    projeto listaProjetos[100];
    carregaDadosFicheiroTarefasResponsavel(listaTarefas,listaResponsavel);
    menuPrincipal(listaTarefas, listaResponsavel, listaProjetos);
    guardarFicheiroTarefasResponsavel(listaTarefas,listaResponsavel);
    return 0;
}