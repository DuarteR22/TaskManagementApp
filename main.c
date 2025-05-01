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

int diferencaDias(data dataInicio,data dataFim ){
    int diasInicio = dataInicio.ano * 360 + dataInicio.mes*30 + dataInicio.dias;
    int diasFim = dataFim.ano * 360 + dataFim.mes * 30 + dataFim.dias;

    return diasFim - diasInicio;
}
void ordenarTarefas(tarefa listaTarefas[100]){

    for (int i = 0; i < 100; i++)
    {
        
    }
    

}
void menuEquipas(responsavel listaResponsavel){

    int opcao;
    do
    {
        printf("0 - Sair\n1 - Determinar duracao media de realizacao de tarefas de uma equipa\n2 - Determinar numero de tarefas concluidas por uma equipa dentro e fora do prazo\n3 - Determinar o numero de tarefas em execucao de uma equipa\nIndique a opcao que deseja: ");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:

            
            break;  
        case 2:
            
            break;
        case 3:
            break;
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
void menuListagemTarefas(tarefa listaTarefas[100], responsavel listaResponsavel[100]){
    int opcao;
    int count;
    data hoje = obterDataHoje();
    char nomeEquipa[20];

    do
    {
        printf("Indique a opcao que listagem que deseja:\n0 - Sair\n1 - Listar tarefas em execucao\n2 - Listar tarefas concluidas\n3 - Listar tarefas concluidas em execucao\n4 - Listar tarefas concluidas com atraso\n5 - Listar tarefas atribuidas a uma equipa\n");
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
        printf("0 - Sair\n1 - Registar nova tarefa\n2 - Criar equipa\n3 - Alterar tarefa\n4 - Concluir tarefa\n5 - Eliminar tarefa\n6 - Alocar equipa a uma tarefa\n7 - Menu de listagem de tarefas\n8 - Ordenar tarefas nao concluidas por ordem de urgencia\n9 - Menu de equipas\nIndique a opcao que deseja: ");
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
        default:
            printf("Termino do programa");
            break;
        }
    } while (opcao != 0);
    
}
int main(){



    responsavel listaResponsavel[100] ;
    tarefa listaTarefas[100] ;

    menuPrincipal(listaTarefas, listaResponsavel);

    return 0;
}


