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
// void guardarFicheiroTarefasResponsavel(responsavel listaResponsavel[100]){
//     FILE *ftarefas = fopen("tarefas.dat", "wb");
//     FILE *fresponsavel = fopen("responsavel.dat", "wb");

//     if (ftarefas == NULL || fresponsavel == NULL)
//     {
//         printf("Erro ao abrir os ficheiros de tarefas ou responsaveis\n");
//         return;
//     }
//     fwrite(listaTarefas, sizeof(tarefa), 100, ftarefas);
//     fwrite(listaResponsavel, sizeof(responsavel), 100, fresponsavel);

//     fclose(ftarefas);
//     fclose(fresponsavel);

// }
// void carregaDadosFicheiroTarefasResponsavel(responsavel listaResponsavel[100]){

//     FILE *ftarefas = fopen("tarefas.dat", "rb");
//     FILE *fresponsavel = fopen("responsavel.dat", "rb");

//     if (ftarefas != NULL)
//     {
//         fread(listaTarefas, sizeof(tarefa), 100, ftarefas);
//     }
//     if (fresponsavel != NULL)
//     {
//         fread(listaResponsavel,  sizeof(responsavel), 100, fresponsavel);
//     }
//     if (ftarefas != NULL)
//         fclose(ftarefas);
//     if(fresponsavel != NULL)
//         fclose(fresponsavel);
// }
//Função auxiliar para obter a diferença entre duas datas em dias
int diferencaDias(data dataInicio,data dataFim ){
    int diasInicio = dataInicio.ano * 365 + dataInicio.mes*30 + dataInicio.dias;
    int diasFim = dataFim.ano * 365 + dataFim.mes * 30 + dataFim.dias;

    return diasFim - diasInicio;
}
//Função auxiliar que pretende indicar se o projeto se encontra cancelado
bool verificaCancelado(projeto p){

    if(p.tarefasConcluidas == NULL && p.tarefasNaoConcluidas == NULL)
        return true;

    return false;
}
//Função auxiliar que permite contar o número total de projetos não cancelados na lista de projetos
int countProjetosNaoCancelados(projeto listaProjetos[100]){
    int count = 0;

    for (int i = 0; i < 100; i++)
    {
        if (!verificaCancelado(listaProjetos[i]))
        {
            count++;
        }
    }
    return count;
}
int countProjetosCancelados(projeto listaProjetos[100]){
    int count = 0;

    for (int i = 0; i < 100; i++)
    {
        if (verificaCancelado(listaProjetos[i]))
        {
            count++;
        }
    }
    return count;
}
//Função auxiliar que permite contar o total de projetos presentes na lista de projetos
int countProjetos(projeto listaProjetos[100]){
    int count = 0;

    for (int i = 0; i < 100; i++)
    {
        if (listaProjetos[i].nomeProjeto[0] != '\0')
        {
            count++;
        }
    }
    return count;
}
//Função auxiliar para obter a duração média das tarefas de uma tarefa em específico
int obterDuracaoMediaTarefas(tarefa *t){

    int dias = 0;
    int countTarefas = 0;

    while (t != NULL)
    {
        int duracao = diferencaDias(t->dataCriacao, t->dataConclusao);
        dias +=duracao;
        countTarefas++;
        t = t->seguinte;
    }
    if (countTarefas == 0)
    {
        return 0;
    }
    return dias / countTarefas;
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
                    }   
                    break;

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

    char novaDataLimite[10];
    char nomeResponsavel[50];
    bool responsavelEncontrado = false;
    char novaDescricao[50];

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
                        printf("Indique a nova data a inserir na tarefa [dd/mm/aaaa]: ");
                        scanf("%s", novaDataLimite);
                        sscanf(novaDataLimite, "%d/%d/%d",&t->dataLimiteExecucao.dias,&t->dataLimiteExecucao.mes,&t->dataLimiteExecucao.ano);
                        printf("Data de execucao limite alterada com sucesso! \n");                 
                        break;
                    case 2:
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
    printf("Indique o nome do projeto da tarefa que pretende concluir: ");
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
    printf("Indique o nome do projeto da tarefa que pretende concluir: ");
    scanf("%s", nomeProjeto);
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
                encontrada = true;
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
                printf("Nenhum projeto encontrado com o nome indicado\n");
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
//Função auxiliar para eliminar uma tarefa individualmente, utilizada pela função de eliminar projeto
void eliminarTarefaAux(tarefa *listaTarefa){
    tarefa *atual = listaTarefa;
    while (atual != NULL)
    {
        tarefa *auxiliar = atual;
        atual = atual ->seguinte;
        free(auxiliar);
    }
}

void eliminarProjeto(projeto listaProjetos[100]){

    char nomeProjeto[50];
    int opcao;
    do
    {
        printf("Indique o nome do projeto a eliminar: ");
        scanf("%s", nomeProjeto);

        bool eliminado = false; 
        for (int i = 0; i < 100; i++)
        {
            if (strcmp(listaProjetos[i].nomeProjeto, nomeProjeto) == 0)
            {
                eliminado = true;
                eliminarTarefaAux(listaProjetos[i].tarefasConcluidas);
                eliminarTarefaAux(listaProjetos[i].tarefasNaoConcluidas);
                char c;
                printf("Prima qualquer tecla para regressar ao menu principal");
                getchar();
                scanf("%c", &c);
                break;
            }
        }

        if (!eliminado) {
            printf("Nenhum projeto encontrado com o nome indicado!\n0 - Sair\n1 - Tentar novamente\n");
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


void listarTarefasEmExecucao(projeto listaProjetos[100]){

    char nomeProjeto[50];
    bool encontrouProjeto = false;
    bool encontrouTarefa = false;

    printf("Indique o nome do projeto que pretende listar as tarefas em exeucao");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == -1)
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
    if (indiceProjeto == -1)
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
        printf("Nome da tarefa: %s Duracao da conclusao da tarefa: %d Dias de duracao da tarefa: ", t->nomeTarefa, diferencaDias(t->dataCriacao, t->dataConclusao));
    
        if (t != NULL)
        {
            t = t->seguinte;
        }
        
    } while (t != NULL);
    if(!encontrouTarefa){
        printf("Nao existem tarefas em execucao no projeto de nome %s\n", nomeProjeto);
    }    
}

void listarTarefasUltrapassaramPrazo(projeto listaProjetos[100]){

    char nomeProjeto[50];
    bool encontrouProjeto = false;
    bool encontrouTarefa = false;

    printf("Indique o nome do projeto que pretende listar as tarefas em execucao que ultrapassaram o prazo de conclusao: ");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado erradO!\n");
        return;
    }
    encontrouProjeto = true;
    tarefa *t = listaProjetos[indiceProjeto].tarefasNaoConcluidas;
    printf("Tarefas em execucao do projeto %s", listaProjetos[indiceProjeto]);
    do
    {
        if (t != NULL && diferencaDias(t->dataLimiteExecucao, obterDataHoje()) < 0)
        {
            printf("Nome da tarefa: %s Data de conclusao da tarefa: %d/%d/%d", t->nomeTarefa, t->dataConclusao.dias, t->dataConclusao.mes,t->dataConclusao.ano);
        }
        
        encontrouTarefa = true;        
        if (t != NULL)
        {
            t = t->seguinte;
        }
        
    } while (t != NULL);
    if(!encontrouTarefa){
        printf("Nao existem tarefas em execucao no projeto de nome %s\n", nomeProjeto);
    }        
}
void listarTarefasConcluidasAtraso(projeto listaProjetos[100]){

    char nomeProjeto[50];
    bool encontrouProjeto = false;
    bool encontrouTarefa = false;

    printf("Indique o nome do projeto que pretende listar as tarefas em concluidas que ultrapassaram o prazo de conclusao: ");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado erradO!\n");
        return;
    }
    encontrouProjeto = true;
    tarefa *t = listaProjetos[indiceProjeto].tarefasConcluidas;
    printf("Tarefas em execucao do projeto %s", listaProjetos[indiceProjeto]);
    do
    {
        if (t != NULL && diferencaDias(t->dataLimiteExecucao, t->dataConclusao) < 0)
        {
            printf("Nome da tarefa: %s Numero de dias de atraso: %d", t->nomeTarefa, -diferencaDias(t->dataLimiteExecucao,t->dataConclusao));
        }
        
        encontrouTarefa = true;        
        if (t != NULL)
        {
            t = t->seguinte;
        }
        
    } while (t != NULL);
    if(!encontrouTarefa){
        printf("Nao existem tarefas em execucao no projeto de nome %s\n", nomeProjeto);
    }
}

void listarTarefasEquipaEspecifica(projeto listaProjetos[100]){

    char nomeProjeto[50];
    bool encontrouProjeto = false;
    bool encontrouTarefa = false;
    bool encontrouEquipa = false;
    char nomeEquipa[50];
    printf("Indique o nome do projeto que pretende listar as tarefas em concluidas que ultrapassaram o prazo de conclusao: ");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado erradO!\n");
        return;
    }
    encontrouProjeto = true;

    printf("Indique o nome da equipa que pretende listar as tarefas atribuidas");
    scanf("%s", nomeEquipa);
    tarefa *t = listaProjetos[indiceProjeto].tarefasNaoConcluidas;

    do
    {
        if (t!=NULL && t->responsavel.equipa)
        {
            for (int i = 0; i < t->responsavel.numMembros; i++)
            {
                if (strcmp(t->responsavel.nomes[i],nomeEquipa) == 0)
                {
                    encontrouTarefa = true;
                    printf("Nome da tarefa: %s Data de conclusao da tarefa: %d/%d/%d", t->nomeTarefa, t->dataConclusao.dias, t->dataConclusao.mes,t->dataConclusao.ano);
                    break;
                }   
            }
        }
        if(t!=NULL)
            t = t->seguinte;
    } while (t!=NULL);

    if(!encontrouTarefa){
        printf("Nao existem tarefas em execucao no projeto de nome %s com a equipa de nome %s\n", nomeProjeto, nomeEquipa);
    } 
}

void duracaoMediaProjeto(projeto listaProjetos[100]){

    char nomeProjeto[50];
    bool encontrouProjeto = false;
    bool encontrouTarefa = false;
    printf("Indique o nome do projeto que pretende obter a media de realizacao das tarefas concluidas: ");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado erradO!\n");
        return;
    }
    encontrouProjeto = true;
    int media = obterDuracaoMediaTarefas(listaProjetos[indiceProjeto].tarefasConcluidas);

    if(media == 0){
        printf("O projeto submetido nao possui tarefas associadas ou a media de realizacao das tarefas esta errada!");
        return;
    }
    printf("Media de duracao das tarefas concluidas do projeto %s: %d", nomeProjeto, media);     
}

void tarefasConcluidasDentroForaPrazo(projeto listaProjetos[100]){

    char nomeProjeto[50];
    printf("Indique o nome do projeto que pretende obter a media de realizacao das tarefas concluidas: ");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado erradO!\n");
        return;
    }

}

void duracaoTarefasConcluidasPrazo(projeto listaProjetos[100]){

    char nomeProjeto[50];
    bool encontrouProjeto = false;
    int countDentroPrazo = 0;
    int countForaPrazo = 0;
    printf("Indique o nome do projeto que pretende obter a media de realizacao das tarefas concluidas: ");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado erradO!\n");
        return;
    }
    tarefa *t = listaProjetos[indiceProjeto].tarefasConcluidas;

     do
    {
        if (t!=NULL)
        {
            {
                if (diferencaDias(t->dataLimiteExecucao, t->dataConclusao)>= 0)
                {
                    countDentroPrazo++;
                }   
                else if(diferencaDias(t->dataLimiteExecucao, t->dataConclusao) < 0)
                    countForaPrazo++;
            }
        }
        if(t!=NULL)
            t = t->seguinte;
    } while (t!=NULL);
    if(countDentroPrazo == 0 && countForaPrazo == 0){
        printf("Nao existe nenhuma tarefa concluida dentro ou fora do prazo indicado!\n");
    }
        else if(countDentroPrazo == 0){
            printf("Nao existe nenhuma tarefa concluida dentro do prazo indicado!\n");
            printf("Numero de tarefas concluidas fora do prazo indicado: %d", countForaPrazo);
        }
        else if(countForaPrazo == 0){
            printf("Nao existe nenhuma tarefa concluida fora do prazo indicado!\n");
            printf("Numero de tarefas concluidas dentro do prazo indicado: %d", countDentroPrazo);
        }
    
    else{
        printf("Numero de tarefas concluidas dentro do prazo indicado: %d", countDentroPrazo);
        printf("Numero de tarefas concluidas fora do prazo indicado: %d", countForaPrazo);
    }
}

void numeroTarefasExecucao(projeto listaProjetos[100]){

    char nomeProjeto[50];
    bool encontrouProjeto = false;
    int countTarefas = 0;
    printf("Indique o nome do projeto que pretende obter a media de realizacao das tarefas concluidas: ");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado erradO!\n");
        return;
    }
    tarefa *t = listaProjetos[indiceProjeto].tarefasNaoConcluidas;

    do
    {
        if (t!=NULL)
        {   
            int duracao = diferencaDias(t->dataCriacao, obterDataHoje());
            printf("Nome da tarefa: %s Duracao ate a data: %d dias", t->nomeTarefa, duracao);
            countTarefas++;
        }
            t = t->seguinte;
    } while (t!=NULL);
    printf("Numero total de tarefas em execucao: %d", countTarefas);
}
void projetosCancelados(projeto listaProjetos[100]){

    int count = countProjetosCancelados(listaProjetos);
    if (count == 0)
    {
        printf("Nao existem projetos cancelados na lista de projetos!\n");
        return;
    }
    printf("Projetos cancelados:\n");
    for (int i = 0; i < 100; i++)
    {
        if (verificaCancelado(listaProjetos[i]))
        {
            printf("Nome do projeto: %s\n", listaProjetos[i].nomeProjeto);
            printf("Responsavel pelo projeto: %s", listaProjetos[i].nomeResponsavel);
            printf("Data de inicio do projeto: %d/%d/%d", listaProjetos[i].dataInicio.dias,listaProjetos[i].dataInicio.mes,listaProjetos[i].dataInicio.ano);
        }
    }
}
void menuListagemProjetos(projeto listaProjetos[100]){
char nomeProjeto[20];
int opcao;
do
{
    printf("Indique 0 - Sair\n1 - Listagem de tarefas em execucao\n2 - Listagem de tarefas concluidas\n3 - Listagem de tarefas que ultrapassaram o prazo de conclusao\n4 - Listagem de tarefas concluidas com atraso\n5 - Listagem de tarefas atribuidas a uma equipa\n6 - Duracao media de realizacao das tarefas concluidas\n7 - Numero de tarefas concluidas dentro e fora do prazo\n8 - Numero de tarefas em execucao com a duracao atual\n9 - Listar projetos cancelados\n");
    scanf("%d", &opcao);
    switch (opcao)
    {
    case 1:
        listarTarefasEmExecucao(listaProjetos);
        break;
    case 2:
        listarTarefasConcluidas(listaProjetos);
        break;
    case 3:
        listarTarefasEmExecucao(listaProjetos);
        break;
    case 4:
        listarTarefasConcluidasAtraso(listaProjetos);
        break;
    case 5:
        listarTarefasEquipaEspecifica(listaProjetos);
        break;
    case 6: 
        duracaoMediaProjeto(listaProjetos);
        break;
    case 7:
        tarefasConcluidasDentroForaPrazo(listaProjetos);
        break;
    case 8:
        numeroTarefasExecucao(listaProjetos);
        break;
    case 9:
        projetosCancelados(listaProjetos);
    default:
        printf("Escolha uma opcao entre as indicadas!\n");
        break;
    }
} while (opcao != 0);
}
//Função utiliza bubblesort
void ordenarTarefasNaoConcluidas(projeto listaProjetos[100]){

    char nomeProjeto[50];
     printf("Indique o nome do projeto que pretende obter a media de realizacao das tarefas concluidas: ");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado erradO!\n");
        return;
    }
    tarefa *inicio = listaProjetos[indiceProjeto].tarefasNaoConcluidas;
    
    if (inicio == NULL || inicio->seguinte == NULL)
    {
        printf("Nao existem tarefas suficientes para ordenar!\n");
        return;
    }
    bool troca;
    do
    {
        troca = false;
        tarefa *atual = inicio;
        tarefa *anterior = NULL;
        tarefa *seguinte = NULL;

        while (atual != NULL && atual->seguinte != NULL)
        {
            seguinte = atual ->seguinte;
            if (diferencaDias(atual->dataLimiteExecucao, seguinte->dataLimiteExecucao) > 0)
            {
                troca = true;
                if (anterior == NULL)
                {
                    atual ->seguinte = seguinte->seguinte;
                    seguinte->seguinte = atual;
                    inicio = seguinte;
                    anterior = seguinte;

                }else{
                    anterior -> seguinte = seguinte;
                    atual -> seguinte = seguinte -> seguinte;
                    seguinte -> seguinte = atual;
                    anterior = seguinte;
                }    
            }
            else{
                anterior = atual;
                atual = atual -> seguinte;
            }
        }
    } while (troca);
    listaProjetos[indiceProjeto].tarefasNaoConcluidas = inicio;
    printf("As tarefas do projeto de nome: %s foram ordenadas com sucesso", nomeProjeto);
}
//Função auxiliar que conta o número de tarefas da lista de tarefas de cada projeto
int countTarefasProjeto(tarefa *listaTarefa){

    int count = 0;
    while (listaTarefa != NULL)
    {
        count++;
        listaTarefa = listaTarefa -> seguinte;
    }
    return count;
}

void ordenarProjetosTarefasPorConcluir(projeto listaProjetos[100]){

    int totalProjetosAtivos = countProjetosNaoCancelados(listaProjetos);
    for (int i = 0; i < totalProjetosAtivos-1; i++)
    {
        for (int j = 0; j < totalProjetosAtivos; j++)
        {
            int tarefasI = countTarefasProjeto(listaProjetos[i].tarefasNaoConcluidas);
            int tarefasJ = countTarefasProjeto(listaProjetos[j].tarefasNaoConcluidas);

            if (tarefasJ > tarefasI)
            {
                projeto temp = listaProjetos[i];
                listaProjetos[i] = listaProjetos[j];
                listaProjetos[j] = temp;
            }
        }
    }
    printf("Projetos ordenados por ordem decrescente de numero de tarefas por concluir\n");
}

void menuOrdenacaoProjetos(projeto listaProjetos[100]){
int opcao;
do
{
    printf("Indique 0 - Sair\n1 - Ordenar as tarefas nao concluidas de cada projeto por ordem de urgencia\n2 - Ordenas os projetos por ordem decrescente de numero de tarefas por concluir\n");
    scanf("%d", &opcao);
    switch (opcao)
    {
    case 1:
        ordenarTarefasNaoConcluidas(listaProjetos);
        break;
    case 2:
        ordenarProjetosTarefasPorConcluir(listaProjetos);
        break;
    default:
        printf("Escolha uma opcao entre as indicadas!\n");
        break;
    }
} while (opcao != 0);
}
void resumoPrazoDesenvolvimentoCumprido(projeto listaProjetos[100]){

    bool prazoCumprido = false; 
    char nomeProjeto[50];
    printf("Indique o nome do projeto que pretende obter a media de realizacao das tarefas concluidas: ");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado erradO!\n");
        return;
    }
    projeto projetoAuxiliar = listaProjetos[indiceProjeto];

    if (projetoAuxiliar.tarefasNaoConcluidas == NULL)
    {
        printf("O projeto de nome %s ainda nao tem tarefas concluidas\n", nomeProjeto);
        return;
    }
    int dias = 0;
    if (diferencaDias(projetoAuxiliar.dataConclusao, obterDataHoje()) >= 0)
    {
        prazoCumprido = true;
    }
    else
        prazoCumprido = false;
        dias = -diferencaDias(projetoAuxiliar.dataConclusao, obterDataHoje());

    if (prazoCumprido)
    {
        printf("O projeto de nome %s cumpriu o prazo de desenvolvimento\n", nomeProjeto);
        return;
    }
    else
        printf("O projeto de nome %s nao cumpriu o prazo de desenvolvimento, possui um atraso de %d dias\n", nomeProjeto, dias);
}
void resumoTarefasDesenvolvidas(projeto listaProjetos[100]){

    int duracaoMedia=0, duracaoMax=0,duracaoMin=-1, somaDuracao=0;
    char nomeProjeto[50];
    printf("Indique o nome do projeto que pretende obter a media de realizacao das tarefas concluidas: ");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado erradO!\n");
        return;
    }
    projeto projetoAuxiliar = listaProjetos[indiceProjeto];

    if (projetoAuxiliar.tarefasConcluidas == NULL)
    {
        printf("O projeto de nome %s ainda nao tem tarefas concluidas\n", nomeProjeto);
        return;
    }
    tarefa *atual = projetoAuxiliar.tarefasConcluidas;
    
    int count = 0;

    while (atual != NULL)
    {
        if (!atual->eliminada && atual -> concluida)
        {
            int duracao = diferencaDias(atual->dataCriacao, atual->dataConclusao);
            somaDuracao += duracao;

            if (duracao > duracaoMax)
                duracaoMax = duracao;
            if(duracao < duracaoMin)
                duracaoMin = duracao;
            count++;
        }
        atual = atual ->seguinte;
        
    }
    duracaoMedia = somaDuracao / count;
    printf("Numero total de tarefas desenvolvidas do projeto %s: %d",nomeProjeto, count);
    printf("Duracao media de realizacao de tarefas do projeto: %d dias\n",duracaoMedia);
    printf("Duracao maxima de realizacao de uma tarefa do projeto: %d dias\n", duracaoMax);
    printf("Duracao minima de realizacao de uma tarefa do projeto: %d dias\n", duracaoMin);
}

void resumoTarefasPrazoUltrapassado(projeto listaProjetos[100]){
    int count;
    char nomeProjeto[50];
    printf("Indique o nome do projeto que pretende obter a media de realizacao das tarefas concluidas: ");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado erradO!\n");
        return;
    }
    projeto projetoAuxiliar = listaProjetos[indiceProjeto];

    if (projetoAuxiliar.tarefasConcluidas == NULL)
    {
        printf("O projeto de nome %s ainda nao tem tarefas concluidas\n", nomeProjeto);
        return;
    }
    tarefa *atual = projetoAuxiliar.tarefasNaoConcluidas;

    while (atual != NULL)
    {
        if (!atual->eliminada && !atual -> concluida)
        {   
            if (diferencaDias(atual->dataLimiteExecucao, obterDataHoje()) < 0)
            {
                count++;
            }
        }
        atual = atual ->seguinte;   
    }
    if (count == 0)
    {
        printf("O projeto %s nao possui tarefas que ultrapassaram o prazo de conclusao\n", nomeProjeto);
        return;
    }
    else
        printf("O projeto %s possui %d tarefas que ultrapassaram o prazo de conclusao\n", nomeProjeto, count);
    
}
void resumoTarefaMaiorIncumprimento(projeto listaProjetos[100]){

     char nomeProjeto[50];
    printf("Indique o nome do projeto que pretende obter a media de realizacao das tarefas concluidas: ");
    scanf("%s", nomeProjeto);
    int indiceProjeto = indiceProjetoAux(listaProjetos, nomeProjeto);
    if (indiceProjeto == -1)
    {
        printf("Projeto nao encontrado com o nome indicado ou nome indicado erradO!\n");
        return;
    }
    projeto projetoAuxiliar = listaProjetos[indiceProjeto];

    if (projetoAuxiliar.tarefasConcluidas == NULL)
    {
        printf("O projeto de nome %s ainda nao tem tarefas concluidas\n", nomeProjeto);
        return;
    }
    tarefa *atual = projetoAuxiliar.tarefasConcluidas;
    tarefa *tarefaMaiorIncumprimento = NULL;
    int incumprimentoDias = 0;
    while (atual != NULL)
    {
        if (!atual->eliminada && atual -> concluida)
        {   
            if (diferencaDias(atual->dataLimiteExecucao,atual->dataConclusao) < 0)
            {
                if(diferencaDias(atual->dataLimiteExecucao,atual->dataConclusao) > incumprimentoDias){
                    incumprimentoDias = diferencaDias(atual->dataLimiteExecucao,atual->dataConclusao);
                    tarefaMaiorIncumprimento = atual;
                }
            }
        }
        atual = atual ->seguinte;   
    }

    if (tarefaMaiorIncumprimento == NULL)
    {
        printf("Nenhuma tarefa do projeto de nome %s ultrapassou o prazo de conclusao\n", nomeProjeto);
        return;
    }
     if (!tarefaMaiorIncumprimento->responsavel.equipa)
    {
        printf("Tarefa com maior incumprimento de prazo do projeto %s tem o nome %s, teve a duracao de %d dias, e o responsavel foi o/a %s\n", nomeProjeto, tarefaMaiorIncumprimento->nomeTarefa,incumprimentoDias, tarefaMaiorIncumprimento->responsavel.nomes[0]);
    }
    else{
        printf("Tarefa com maior incumprimento de prazo do projeto %s tem o nome %s e teve duracao de %d dias\n",nomeProjeto, tarefaMaiorIncumprimento->nomeTarefa, incumprimentoDias);
        printf("Os nomes dos responsaveis pela tarefa sao:\n");
        for (int i = 0; i < tarefaMaiorIncumprimento->responsavel.numMembros; i++)
        {
            printf("%s,\n", tarefaMaiorIncumprimento->responsavel.nomes[i]);
        }
        printf("\n");
    }
}
void menuResumoProjetos(projeto listaProjetos[100]){
int opcao;
do
{
    printf("Indique 0 - Sair\n1 - Apresentar um resumo quanto ao prazo de desenvolvimento de cada projeto concluido\n2 - Apresentar um resumo do numero total de tarefas desenvolvidas\n3 - Apresentar um resumo do numero de tarefas que ultrapassaram o prazo de conclusao\n4 - Apresentar um resumo da duracao de tarefa com maior incumprimento de prazo\n");
    scanf("%d", &opcao);
    switch (opcao)
    {
    case 1:
        resumoPrazoDesenvolvimentoCumprido(listaProjetos);
        break;
    case 2:
        resumoTarefasDesenvolvidas(listaProjetos);
        break;
    case 3:
        resumoTarefasPrazoUltrapassado(listaProjetos);
        break;
    case 4:
        resumoTarefaMaiorIncumprimento(listaProjetos);
        break;
    default:
        printf("Escolha uma opcao entre as indicadas!\n");
        break;
    }
} while (opcao != 0);
}
void menuPrincipalTarefas(responsavel listaResponsavel[100], projeto listaProjetos[100]){
    int opcao;
    do
    {
        printf("0 - Sair\n1 - Registar nova tarefa\n2 - Criar equipa\n3 - Alterar tarefa\n4 - Concluir tarefa\n5 - Eliminar tarefa\n6 - Alocar equipa a uma tarefa\n7 - Menu de equipas\nIndique a opcao que deseja: ");
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
            concluirTarefa(listaProjetos);
            break;
        case 5:
            eliminarTarefa(listaProjetos);
            break;
        case 6:
            alocarEquipa(listaProjetos, listaResponsavel);
            break;
        case 7:
            break;
        default:
            printf("Termino do programa\n");
            break;
        }
    } while (opcao != 0);
}
void menuPrincipalProjetos(projeto listaProjetos[100]){
    int opcao;
    do
    {
        printf("0 - Sair\n1 - Registar novo projeto\n2 - Editar projeto\n3 - Eliminar projeto\n4 - Cancelar projeto\n5 - Menu de listagem de projetos\n6 - Menu de ordenacao de projetos\n7 - Menu de resumos de projeto\nIndique a opcao que deseja: ");
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
            eliminarProjeto(listaProjetos);
            break;
        case 4:
            cancelarProjeto(listaProjetos);
            break;
        case 5:
            menuListagemProjetos(listaProjetos);
            break;
        case 6:
            menuOrdenacaoProjetos(listaProjetos);
            break;
        case 7:
            menuResumoProjetos(listaProjetos);
            break;
        default:
            printf("Termino do programa\n");
            break;
        }
    } while (opcao != 0);
}

void menuPrincipal(responsavel listaResponsavel[100], projeto listaProjetos[100]){
    int opcao;
    do
    {
        printf("0 - Sair\n1 - Menu de Projetos\n2 - Menu de Tarefas\nIndique a opcao que deseja: ");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 0:
            printf("Termino do programa");
            break;
        case 1:
            menuPrincipalProjetos(listaProjetos);
            break;  
        case 2:
            menuPrincipalTarefas(listaResponsavel, listaProjetos);
            break;
        default:
            printf("Escolha uma opcao entre as indicadas!\n");
            break;
        }
    } while (opcao != 0);
}

int main(){


    responsavel listaResponsavel[100] ;
    projeto listaProjetos[100];
    //carregaDadosFicheiroTarefasResponsavel(listaResponsavel);
    menuPrincipal( listaResponsavel, listaProjetos);
    //guardarFicheiroTarefasResponsavel(listaResponsavel);
    return 0;
}