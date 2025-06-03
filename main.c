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
    bool cancelado;
    bool eliminado;
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
        if (!listaProjetos[i].eliminado && strcmp(listaProjetos[i].nomeProjeto, nomeProjeto) == 0) {
            return i;
        }
    }
    return -1;
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
void menuTarefasEliminadas(tarefa listaTarefas[100]){
    int opcao;
    int countCase1=0,countCase2=0, countCase3=0;
    int duracaoCase1=0,duracaoCase2=0, duracaoCase3=0, duracaoCase4=0;
    int maxDuracao = -10, maxDuracaoAno = -10;
    int minDuracao = 10000000, minDuracaoAno = 10000000;
    int iMax = -1,iMaxAno = -1;
    int iMin = -1,iMinAno = -1;
    int anoMedia1,anoMedia2;
    int anoMaxMin;
    do
    {
        printf("Indique a opcao que listagem que deseja:\n0 - Sair\n1 - Duracao media de realizacao de tarefas concluidas e nao eliminadas\n2 - Duracao maxima e minima de uma tarefa concluida e nao eliminada\n3 - Duracao media de realizacao de tarefas eliminadas e concluidas para um ano\n4 - Duracao maxima e minima de uma tarefa eliminada e concluida para um ano\n");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            for (int i = 0; i < 100; i++)
            {
                if (!listaTarefas[i].eliminada && listaTarefas[i].concluida)
                {
                    duracaoCase1 += diferencaDias(listaTarefas[i].dataCriacao, listaTarefas[i].dataConclusao);
                    countCase1++;
                }
            }
            if (countCase1 == 0)
            {
                printf("Nao existem tarefas concluidas para elaborar a media!\n");
            }
            else
                printf("A duracao media das tarefas concluidas e %f\n", duracaoCase1/countCase1);
        break;
        case 2:
            for (int i = 0; i < 100; i++)
            {
                if (listaTarefas[i].concluida && !listaTarefas[i].eliminada)
                {
                    duracaoCase2 = diferencaDias(listaTarefas[i].dataCriacao, listaTarefas[i].dataConclusao);
                    if (duracaoCase2 > maxDuracao)
                    {
                        maxDuracao = duracaoCase2;
                        iMax = i;
                    }
                    if (duracaoCase2 < minDuracao)
                    {
                        minDuracao = duracaoCase2;
                        iMin = i;
                    }   
                }
            }
            if (iMax != -1 && iMin != -1)
            {
                printf("Tarefa com maior duracao:\n");
                printf("Nome da tarefa: %s\n", listaTarefas[iMax].nomeTarefa);
                printf("Duracao: %d dias\n", maxDuracao);
                printf("Responsavel: ");
                if (listaTarefas[iMax].responsavel.equipa)
                {
                    for (int j = 0; j < listaTarefas[iMax].responsavel.numMembros; j++)
                    {
                        printf("%s", listaTarefas[iMax].responsavel.nomes[j]);
                        if (j < listaTarefas[iMax].responsavel.numMembros - 1)
                            printf(", ");
                    }
                }
            else{
                printf("%s", listaTarefas[iMax].responsavel.nomes[0]);
            }
            printf("\n");
            
            printf("Tarefa com menor duracao:\n");
            printf("Nome da tarefa: %s\n", listaTarefas[iMin].nomeTarefa);
            printf("Duracao: %d dias\n", minDuracao);
            printf("Responsavel: ");
            if (listaTarefas[iMin].responsavel.equipa)
                {
                    for (int j = 0; j < listaTarefas[iMin].responsavel.numMembros; j++)
                    {
                        printf("%s", listaTarefas[iMin].responsavel.nomes[j]);
                        if (j < listaTarefas[iMin].responsavel.numMembros - 1)
                            printf(", ");
                    }
                }
            else{
                printf("%s", listaTarefas[iMin].responsavel.nomes[0]);
                
            }
            printf("\n");
        }else
            printf("Nao existem tarefas concluidas para analisar duracoes\n");            
        break;
        case 3:
            printf("Indique o ano em que pretende pesquisar a duracao media de conclusao das tarefas: ");
            scanf("%d", &anoMedia1);

            for (int i = 0; i < 100; i++)
            {
                if (listaTarefas[i].concluida && listaTarefas[i].eliminada && listaTarefas[i].dataConclusao.ano == anoMedia1)
                {
                    duracaoCase3 += diferencaDias(listaTarefas[i].dataCriacao, listaTarefas[i].dataConclusao);
                    countCase3++;
                }
            }
            if (countCase3 == 0)
            {
                printf("Nao existem tarefas concluidas do ano %d", anoMedia1);
                break;
            }
            else
                printf("A duracao media das tarefas concluidas do ano %d e %f\n",anoMedia1, (float) duracaoCase3/countCase3);
        break;
        case 4:
            printf("Indique o ano em que pretende pesquisar a duracao maxima e minima de conclusao das tarefas: ");
            scanf("%d", &anoMedia2);

            for (int i = 0; i < 100; i++)
                {
                    if (listaTarefas[i].concluida && listaTarefas[i].eliminada && listaTarefas[i].dataConclusao.ano == anoMedia2)
                    {
                        duracaoCase4 = diferencaDias(listaTarefas[i].dataCriacao, listaTarefas[i].dataConclusao);
                        if (duracaoCase4 > maxDuracaoAno)
                        {
                            maxDuracaoAno = duracaoCase4;
                            iMaxAno = i;
                        }
                        if (duracaoCase4 < minDuracaoAno)
                        {
                            minDuracaoAno = duracaoCase4;
                            iMinAno = i;
                        }   
                    }
                }
                if (iMaxAno != -1 && iMinAno != -1)
                {
                    printf("Tarefa com maior duracao:\n");
                    printf("Nome da tarefa: %s\n", listaTarefas[iMaxAno].nomeTarefa);
                    printf("Duracao: %d dias\n", maxDuracaoAno);
                    printf("Responsavel: ");
                    if (listaTarefas[iMaxAno].responsavel.equipa)
                    {
                        for (int j = 0; j < listaTarefas[iMaxAno].responsavel.numMembros; j++)
                        {
                            printf("%s", listaTarefas[iMaxAno].responsavel.nomes[j]);
                            if (j < listaTarefas[iMaxAno].responsavel.numMembros - 1)
                                printf(", ");
                        }
                    }
                else{
                    printf("%s", listaTarefas[iMaxAno].responsavel.nomes[0]);
                }
                printf("\n");
                printf("Tarefa com menor duracao:\n");
                printf("Nome da tarefa: %s\n", listaTarefas[iMinAno].nomeTarefa);
                printf("Duracao: %d dias\n", minDuracaoAno);
                printf("Responsavel: ");
                if (listaTarefas[iMinAno].responsavel.equipa)
                    {
                        for (int j = 0; j < listaTarefas[iMinAno].responsavel.numMembros; j++)
                        {
                            printf("%s", listaTarefas[iMinAno].responsavel.nomes[j]);
                            if (j < listaTarefas[iMinAno].responsavel.numMembros - 1)
                                printf(", ");
                        }
                    }
                else{
                    printf("%s", listaTarefas[iMinAno].responsavel.nomes[0]);
                }
                printf("\n");
            }else
                printf("Nao existem tarefas concluidas para analisar duracoes\n");            
        break;
        default:
            printf("Escolha uma opcao entre as indicadas!\n");
            break;
        }
    } while (opcao != 0);
}
void menuListagemTarefas(tarefa listaTarefas[100], responsavel listaResponsavel[100]){
    int opcao;
    int count;
    data hoje = obterDataHoje();
    char nomeEquipa[20];

    do
    {
        printf("Indique a opcao que listagem que deseja:\n0 - Sair\n1 - Listar tarefas em execucao\n2 - Listar tarefas concluidas\n3 - Listar tarefas concluidas em execucao\n4 - Listar tarefas concluidas com atraso\n5 - Listar tarefas atribuidas a uma equipa\n6 - Menu de tarefas eliminadas / nao eliminadas\n");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            count = 0;
            printf("Tarefas em execucao: \n");
            for (int i = 0; i < 100; i++)
            {
                if (listaTarefas[i].concluida == 0 && !listaTarefas[i].eliminada)
                {
                    if(listaTarefas[i].nomeTarefa[0] == '\0'){
                        break;
                    }
                    printf("Nome: %s\n", listaTarefas[i].nomeTarefa);
                    printf("Data de criacao da tarefa: %d/%d/%d\n", listaTarefas[i].dataCriacao.dias, listaTarefas[i].dataCriacao.mes, listaTarefas[i].dataCriacao.ano);
                    count++;
                    
                }
            }
            if(count == 0)
                printf("Não existem tarefas em execucao\n"); 
            
        break;
        case 2:
            count = 0;
            int duracao;
            printf("Tarefas concluidas: \n");
            for (int i = 0; i < 100; i++)
            {
                if (listaTarefas[i].concluida == 1 && !listaTarefas[i].eliminada)
                {
                    if(listaTarefas[i].nomeTarefa[0] == '\0'){
                        break;
                    }
                    printf("Nome: %s\n", listaTarefas[i].nomeTarefa);
                    duracao = diferencaDias(listaTarefas[i].dataCriacao, listaTarefas[i].dataConclusao);
                    printf("Esta tarefa demorou %d dias a ser concluida\n", duracao);
                    duracao = 0;
                    count++;
                }
            }
            if (count == 0)
            {
                printf("Nao existem tarefas concluidas\n");
            }
        break;
        case 3:
            count = 0;
            printf("Tarefas em execucao que ultrapassaram o prazo de conclusao: ");
            for (int i = 0; i < 100; i++)
            {
                if (!listaTarefas[i].concluida && !listaTarefas[i].eliminada && diferencaDias(listaTarefas[i].dataLimiteExecucao, hoje) < 0)
                {
                    if(listaTarefas[i].nomeTarefa[0] == '\0'){
                        break;
                    }
                    printf("Nome da tarefa: %s\n", listaTarefas[i].nomeTarefa);
                    printf("Data limite de execucao da tarefa: %d/%d/%d\n", listaTarefas[i].dataLimiteExecucao.dias,listaTarefas[i].dataLimiteExecucao.mes,listaTarefas[i].dataLimiteExecucao.ano);
                    count++;
                }    
            }
            if(count == 0){
                printf("Nao existem tarefas em atraso\n");
            }
        break;
        case 4:
            count = 0;
            printf("Tarefas concluidas com atraso: \n");
            for (int i = 0; i < 100; i++)
            {
                if (listaTarefas[i].concluida && !listaTarefas[i].eliminada && diferencaDias(listaTarefas[i].dataLimiteExecucao, listaTarefas[i].dataConclusao) < 0)
                {
                    if(listaTarefas[i].nomeTarefa[0] == '\0'){
                        break;
                    }
                    int duracao = diferencaDias(listaTarefas[i].dataLimiteExecucao, listaTarefas[i].dataConclusao);
                    printf("Nome da tarefa: %s\n", listaTarefas[i].nomeTarefa);
                    printf("Dias de atraso: %d\n", -(duracao));
                    count++;
                }    
            }
            if(count == 0){
                printf("Não existem tarefas concluidas com atraso\n");
                break;
                }
        break;
        case 5:
            count = 0;
            do
            {
                printf("Indique um nome presente na equipa de que pretende listar as tarefas: ");
                scanf("%s", nomeEquipa);
                for (int i = 0; i < 100; i++)
                {
                    if (!listaTarefas[i].eliminada)
                    {
                        for (int j = 0; j < listaTarefas[i].responsavel.numMembros; j++)
                        {
                            if (strcmp(listaTarefas[i].responsavel.nomes[j], nomeEquipa) == 0)
                            {
                                if(listaTarefas[i].nomeTarefa[0] == '\0'){
                                    break;
                                }
                                printf("Nome da tarefa: %s\n", listaTarefas[i].nomeTarefa);
                                printf("Data de conclusao da tarefa: %d/%d/%d\n", listaTarefas[i].dataConclusao.dias, listaTarefas[i].dataConclusao.mes, listaTarefas[i].dataConclusao.ano);
                                count++;
                                break;
                            }
                        }
                    }
                }
                if (count == 0)
                {
                    printf("Nao foi encontrada nenhuma tarefa com o nome indicado\n");
                }    
            } while (true);
        break;
        case 6:
        menuTarefasEliminadas(listaTarefas);
        break;
        default:
            break;
        }
    } while (opcao != 0);
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
    
    projeto1.cancelado = 0;
    projeto1.eliminado = 0;
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
            if (strcmp(listaProjetos[i].nomeProjeto, nomeProjeto) == 0 && !listaProjetos[i].eliminado)
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
            if (strcmp(listaProjetos[i].nomeProjeto, nomeProjeto) == 0 && !listaProjetos[i].cancelado)
            {
                listaProjetos[i].cancelado = true;
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