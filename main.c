#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>

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

typedef struct 
{
    char nomeTarefa[20];
    data dataCriacao;
    data dataLimiteExecucao;
    data dataConclusao;
    char descricao [50];
    bool concluida; //1 se está concluída, 0 se está por concluir
    bool eliminada; //1 se foi eliminada, 0 se continua presente
    responsavel responsavel;
}tarefa;

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
    if (ftarefas == NULL)
        fclose(ftarefas);
    if(fresponsavel == NULL)
        fclose(fresponsavel);
}
void guardarFicheiroTarefasConcluidas(tarefa listaTarefas[100]){
    data hoje = obterDataHoje();
    FILE *ficheiro;
    ficheiro = fopen("ficheiro.csv", "w");

    if (ficheiro == NULL)
    {
        printf("Erro ao abrir o ficheiro para escrita\n");
        return;
    }
    fprintf(ficheiro, "Tarefa;Realizada por;Data de criação;Data limite;Data conclusão;Descrição\n");
    for (int i = 0; i < 100; i++)
    {
        if (listaTarefas[i].concluida && !listaTarefas[i].eliminada)
        {
            if (listaTarefas[i].dataConclusao.ano == hoje.ano)
            {
                listaTarefas[i].eliminada = true;
                fprintf(ficheiro, "%s;", listaTarefas[i].nomeTarefa);
                if (listaTarefas[i].responsavel.equipa)
                {
                    fprintf(ficheiro, "Equipa: ");
                    for (int j = 0; j < listaTarefas[i].responsavel.numMembros; j++)
                    {
                        fprintf(ficheiro, "%s", listaTarefas[i].responsavel.nomes[j]);
                        if (j < listaTarefas[i].responsavel.numMembros - 1) //Verifica se a vírgula não está a ser colocada no final da lista de nomes
                        {
                            fprintf(ficheiro, ", ");
                        }
                    }
                }else{
                    fprintf(ficheiro, "%s", listaTarefas[i].responsavel.nomes[0]);
                }
                fprintf(ficheiro, ";");
                fprintf(ficheiro, "%d%d%d;", listaTarefas[i].dataCriacao.dias, listaTarefas[i].dataCriacao.mes, listaTarefas[i].dataCriacao.ano);
                fprintf(ficheiro, "%d%d%d;", listaTarefas[i].dataLimiteExecucao.dias, listaTarefas[i].dataLimiteExecucao.mes, listaTarefas[i].dataLimiteExecucao.ano);
                fprintf(ficheiro, "%d%d%d;", listaTarefas[i].dataConclusao.dias, listaTarefas[i].dataConclusao.mes, listaTarefas[i].dataConclusao.ano);

                for (int k = 0; k < strlen(listaTarefas[i].descricao); k++)
                {
                    if (listaTarefas[i].descricao[k] != '\n')
                    {
                        fputc(listaTarefas[i].descricao[k], ficheiro);
                    }
                }
                fprintf(ficheiro, "\n");
            }  
        }
    }
    fclose(ficheiro);
    printf("Ficheiro CSV criado com sucesso!\n");
}
int diferencaDias(data dataInicio,data dataFim ){
    int diasInicio = dataInicio.ano * 360 + dataInicio.mes*30 + dataInicio.dias;
    int diasFim = dataFim.ano * 360 + dataFim.mes * 30 + dataFim.dias;

    return diasFim - diasInicio;
}
//Utiliza Bubblesort
void ordenarTarefas(tarefa listaTarefas[100]){
    tarefa temp;
    for (int i = 0; i < 99; i++)
    {
        for (int j = 0; j < 99-i; j++)
        {
            if (!listaTarefas[j].concluida && !listaTarefas[j+1].concluida)
            {
                if (diferencaDias(listaTarefas[j].dataLimiteExecucao, listaTarefas[j+1].dataLimiteExecucao) > 0)
                {
                    temp = listaTarefas[j];
                    listaTarefas[j] = listaTarefas[j+1];
                    listaTarefas[j+1] = temp;
                }
            }
        }
    }
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
                printf("Duracao media de realizacao das tarefas concluidas pela equipa %s e %f\n", nomeEquipa, media);
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
                        }
                    }
                }  
            }
            if (count == 0)
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
void alocarEquipa(tarefa listaTarefas[100], responsavel listaResponsavel[100]){

    char nomeTarefa[20];
    char nomeEquipa[20];

    bool encontrouTarefa = false;
    bool encontrouResponsavel = false;
    do
    {
        printf("Indique a tarefa a que pretende alocar uma equipa: \n");
        scanf("%s", nomeTarefa);
        for (int i = 0; i < 100; i++)
        {
            if (strcmp(listaTarefas[i].nomeTarefa, nomeTarefa) == 0 && !listaTarefas[i].eliminada)
            {
                encontrouTarefa = true;
                printf("Indique o nome de um responsavel da equipa que pretende alocar a tarefa de nome %s", listaTarefas[i].nomeTarefa);
                scanf("%s", nomeEquipa);
                for (int j = 0; j < listaResponsavel[j].numMembros; j++)
                {
                    if(strcmp(listaResponsavel[i].nomes[j], nomeEquipa) == 0){
                        listaTarefas[i].responsavel = listaResponsavel[j];
                        printf("Equipa alocada com sucesso a tarefa de nome %s", listaTarefas[i].nomeTarefa);
                        encontrouResponsavel = true;
                    }
                }
            }
        }
        if (encontrouResponsavel == false)
        {
            printf("Nome de responsavel incorreto ou nao presente na lista de responsaveis");
        }
        if (encontrouTarefa == false)
        {
            printf("Nome de tarefa incorreto ou nao presente na lista de tarefas");
        }
    } while (true);
    
}
void alterarTarefa(tarefa listaTarefas[100], responsavel listaResponsavel[100]){
    char nomeTarefa[20];
    char novaDataLimite [10];
    bool responsavelEncontrado = false;
    char nomeResponsavel[20];
    char novaDescricao[50];

    int opcao;
    do
    {
        printf("Indique o nome da tarefa a alterar: ");
        scanf("%s", nomeTarefa);

        bool encontrada = false;
        for (int i = 0; i < 100; i++)
        {
            if (strcmp(listaTarefas[i].nomeTarefa, nomeTarefa) == 0 && !listaTarefas[i].eliminada)
            {
                
                int opcao=0;
                do
                {
                    printf("Indique que dados pretende alterar da tarefa:\n0 - Sair\n1 - Data limite de execucao\n2 - Responsavel\n3 - Descricao\n");
                    scanf("%d", &opcao);
                    switch (opcao)
                    {
                    case 1:
                        printf("Indique a nova data a inserir na tarefa [dd/mm/aaaa]: ");
                        scanf("%s", novaDataLimite);
                        sscanf(novaDataLimite, "%d/%d/%d", &listaTarefas[i].dataLimiteExecucao.dias, &listaTarefas[i].dataLimiteExecucao.mes, &listaTarefas[i].dataLimiteExecucao.ano);
                        printf("Data limite de execucao alterada com sucesso! \n");                 
                        break;
                    case 2:
                        
                        printf("Indique o nome do responsavel/equipa responsavel a atribuir");
                        for (int i = 0; i < 100; i++)
                        {
                            for (int j = 0; j < listaResponsavel[j].numMembros; j++)
                            {
                                if(strcmp(listaResponsavel[i].nomes[j], nomeResponsavel) == 0){
                                    listaTarefas[i].responsavel = listaResponsavel[i];
                                    printf("Responsavel alterado com sucesso!\n");
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

                        printf("Indique nova descricao a inserir na tarefa");
                        scanf("%s", novaDescricao);
                        strcpy(listaTarefas[i].descricao, novaDescricao);
                        printf("Descricao alterada com sucesso!\n");
                        break;
                    default:
                        break;
                    }
                } while (opcao != 0);
                
            }
        }

        if (!encontrada) {
            printf("Nenhuma tarefa encontrada com o nome indicado!\n0 - Sair\n1 - Tentar novamente\n");
            scanf("%d", &opcao);
            if (opcao == 0) break;
        } else {
            break; 
        }

    } while (true);
    
}
void eliminarTarefa(tarefa listaTarefas[100]){
    char nomeTarefa[20];
    int opcao;
    do
    {
        printf("Indique o nome da tarefa a eliminar: ");
        scanf("%s", nomeTarefa);

        bool eliminada = false; // nova flag
        for (int i = 0; i < 100; i++)
        {
            if (strcmp(listaTarefas[i].nomeTarefa, nomeTarefa) == 0 && !listaTarefas[i].eliminada)
            {
                listaTarefas[i].eliminada = true;
                printf("Tarefa eliminada com sucesso!\n");
                eliminada = true;
                char c;
                printf("Prima qualquer tecla para regressar ao menu principal");
                getchar();
                scanf("%c", &c);
                break;
            }
        }

        if (!eliminada) {
            printf("Nenhuma tarefa encontrada com o nome indicado!\n0 - Sair\n1 - Tentar novamente\n");
            scanf("%d", &opcao);
            if (opcao == 0) break;
        } else {
            break; 
        }

    } while (true);
}
void concluirTarefa(tarefa listaTarefas[100]){
    char nomeTarefa[20];
    int opcao;
    do
    {
        printf("Indique o nome da tarefa a concluir: ");
        scanf("%s", nomeTarefa);

        bool encontrada = false; // nova flag
        for (int i = 0; i < 100; i++)
        {
            if (strcmp(listaTarefas[i].nomeTarefa, nomeTarefa) == 0 && !listaTarefas[i].eliminada)
            {
                listaTarefas[i].concluida = true;
                printf("Tarefa concluida com sucesso!\n");
                encontrada = true;
                char c;
                printf("Prima qualquer tecla para regressar ao menu principal");
                getchar();
                scanf("%c", &c);
                break;
            }
        }

        if (!encontrada) {
            printf("Nenhuma tarefa encontrada com o nome indicado!\n0 - Sair\n1 - Tentar novamente\n");
            scanf("%d", &opcao);
            if (opcao == 0) break;
        } else {
            break; 
        }

    } while (true);
}
void menuTarefasEliminadas(tarefa listaTarefas[100]){
    int opcao;
    int countCase1,countCase2, countCase3;
    int duracaoCase1,duracaoCase2, duracaoCase3, duracaoCase4;
    int maxDuracao = -10, maxDuracaoAno = -10;
    int minDuracao = 10000000, minDuracaoAno = 10000000;
    int iMax,iMaxAno;
    int iMin,iMinAno;
    int anoMedia;
    int anoMaxMin;
    do
    {
        printf("Indique a opcao que listagem que deseja:\n0 - Sair\n1 - Duracao media de realizacao de tarefas nao eliminadas\n2 - Duracao maxima e minima de uma tarefa nao eliminada\n3 - Duracao media de realizacao de tarefas nao eliminadas para um ano\n4 - Duracao maxima e minima de uma tarefa nao eliminada\n");
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
            scanf("%d", &anoMedia);

            for (int i = 0; i < 100; i++)
            {
                if (listaTarefas[i].concluida && !listaTarefas[i].eliminada && listaTarefas[i].dataConclusao.ano == anoMedia)
                {
                    duracaoCase3 += diferencaDias(listaTarefas[i].dataCriacao, listaTarefas[i].dataConclusao);
                    countCase3++;
                }
            }
            if (countCase3 == 0)
            {
                printf("Nao existem tarefas concluidas do ano %d", anoMedia);
                break;
            }
            else
                printf("A duracao media das tarefas concluidas do ano %d e %f\n",anoMedia, (float) duracaoCase3/countCase3);
        break;
        case 4:
            printf("Indique o ano em que pretende pesquisar a duracao maxima e minima de conclusao das tarefas: ");
            scanf("%d", &anoMedia);

            for (int i = 0; i < 100; i++)
                {
                    if (listaTarefas[i].concluida && !listaTarefas[i].eliminada && listaTarefas[i].dataConclusao.ano == anoMedia)
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
        case 5:
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
        printf("Indique a opcao que listagem que deseja:\n0 - Sair\n1 - Listar tarefas em execucao\n2 - Listar tarefas concluidas\n3 - Listar tarefas concluidas em execucao\n4 - Listar tarefas concluidas com atraso\n5 - Listar tarefas atribuidas a uma equipa\n6 - Menu de listagem de tarefas eliminadas\n");
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
                    printf("Data limite de execucao da tarefa: %d/%d/%d", listaTarefas[i].dataLimiteExecucao.dias,listaTarefas[i].dataLimiteExecucao.mes,listaTarefas[i].dataLimiteExecucao.ano);
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
    printf("Indique a data de criacao da tarefa [dd/mm/aaaa]: ");
    scanf("%s", dataCriacao);
    sscanf(dataCriacao, "%d/%d/%d", &tarefa1.dataCriacao.dias,&tarefa1.dataCriacao.mes,&tarefa1.dataCriacao.ano);
    printf("Indique a data limite de execucao da tarefa [dd/mm/aaaa]: ");
    scanf("%s", dataLimiteExecucao);
    sscanf(dataLimiteExecucao, "%d/%d/%d", &tarefa1.dataLimiteExecucao.dias,&tarefa1.dataLimiteExecucao.mes,&tarefa1.dataLimiteExecucao.ano);
    printf("Indique a data limite de conclusao da tarefa [dd/mm/aaaa]: ");
    scanf("%s", dataConclusao);
    sscanf(dataConclusao, "%d/%d/%d", &tarefa1.dataConclusao.dias,&tarefa1.dataConclusao.mes,&tarefa1.dataConclusao.ano);
    printf("Indique a descricao da tarefa a executar: ");
    getchar();
    fgets(descricao, sizeof(descricao), stdin);
    strcpy(tarefa1.descricao, descricao);
    tarefa1.concluida = 0;
    tarefa1.eliminada = 0;
    for (int i = 0; i < 100; i++)
    {
        if(listaTarefas[i].nomeTarefa[0] == '\0'){
            listaTarefas[i] = tarefa1;
            printf("Tarefa adicionada com sucesso!\n");
            char c;
            printf("Prima qualquer tecla para regressar ao menu principal");
            getchar();
            scanf("%c", &c);
            break;
        }
        if(i == 99){
            printf("Lista de tarefas cheia!\n");
        }
    }
    
    
    
}

void menuCriacaoEquipa(responsavel listaResponsavel[100]){
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
            for (int i = 0; i < 100; i++)
            {
                if (listaResponsavel[i].nomes[0][0] == '\0')
                {
                    listaResponsavel[i] = responsavel1;
                    break;
                }
            }
            printf("Responsavel adicionado com sucesso!\n");
            char c;
            printf("Prima qualquer tecla para regressar ao menu de criacao de equipas");
            getchar();
            scanf("%c", &c);
            break;
        case 2:
            printf("Indique o numero de membros da equipa a adicionar: ");
            scanf("%d", &numMembros);
            responsavel1.numMembros = numMembros;
            responsavel1.equipa = 1;
            for (int i = 0; i < numMembros; i++)
            {
                printf("Indique o nome do %dº membro da equipa: ", i+1);
                scanf("%s", nome);
                strcpy(responsavel1.nomes[i], nome);
                for (int i = 0; i < 100; i++)
            {
                if (listaResponsavel[i].nomes[0][0] == '\0')
                {
                    listaResponsavel[i] = responsavel1;
                }
            }
            }
            printf("Equipa de responsaveis adicionado com sucesso!\n");
            printf("Prima qualquer tecla para regressar ao menu principal");
            getchar();
            scanf("%c", &c);
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
        printf("0 - Sair\n1 - Registar nova tarefa\n2 - Criar equipa\n3 - Alterar tarefa\n4 - Concluir tarefa\n5 - Eliminar tarefa\n6 - Alocar equipa a uma tarefa\n7 - Menu de listagem de tarefas\n8 - Ordenar tarefas nao concluidas por ordem de urgencia\n9 - Menu de equipas\n10 - Criar ficheiro com as tarefas concluidas do ano\nIndique a opcao que deseja: ");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:

            menuRegistoTarefa(listaTarefas);
            break;  
        case 2:
            menuCriacaoEquipa(listaResponsavel);
            break;
        case 3:
            alterarTarefa(listaTarefas, listaResponsavel);
            break;
        case 4:
            concluirTarefa(listaTarefas);
            break;
        case 5:
            eliminarTarefa(listaTarefas);
            break;
        case 6:
            alocarEquipa(listaTarefas, listaResponsavel);
            break;
        case 7:
            menuListagemTarefas(listaTarefas, listaResponsavel);
            break;
        case 10:
            guardarFicheiroTarefasConcluidas(listaTarefas);
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
    
    carregaDadosFicheiroTarefasResponsavel(listaTarefas,listaResponsavel);
    menuPrincipal(listaTarefas, listaResponsavel);
    guardarFicheiroTarefasResponsavel(listaTarefas,listaResponsavel);
    return 0;
}


