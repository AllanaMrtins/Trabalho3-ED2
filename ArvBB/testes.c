#include "estruturas.h"

#ifdef _WIN32
#include <windows.h>
#endif

#define QUANTIDADE 100
#define TAMANHO_NOME 30
#define QUANTIDADE_BUSCAS 10
#define REPETICOES_INSERCAO_TEMPO 100
#define REPETICOES_BUSCA_TEMPO 1000000

//Retorna o tempo em segundos usando um contador de alta resolucao
double obter_tempo() {
    double tempo = 0;

#ifdef _WIN32
    LARGE_INTEGER contador;
    LARGE_INTEGER frequencia;

    if (QueryPerformanceFrequency(&frequencia) &&
        QueryPerformanceCounter(&contador))
        tempo = (double)contador.QuadPart / (double)frequencia.QuadPart;
#else
    tempo = (double)clock() / CLOCKS_PER_SEC;
#endif

    return tempo;
}

//Cria 100 nomes aleatorios e sem repeticao
void gerar_nomes(char nomes[QUANTIDADE][TAMANHO_NOME]) {
    int i, j, numero, repetido;

    for (i = 0; i < QUANTIDADE; i++) {
        do {
            numero = rand() % 10000;
            sprintf(nomes[i], "Artista%04d", numero);
            repetido = 0;
            for (j = 0; j < i; j++) {
                if (strcmp(nomes[i], nomes[j]) == 0)
                    repetido = 1;
            }
        } while (repetido == 1);
    }
}

//Coloca os nomes em ordem alfabetica crescente
void ordenar_crescente(char nomes[QUANTIDADE][TAMANHO_NOME]) {
    int i, j;
    char auxiliar[TAMANHO_NOME];

    for (i = 0; i < QUANTIDADE - 1; i++) {
        for (j = i + 1; j < QUANTIDADE; j++) {
            if (strcmp(nomes[i], nomes[j]) > 0) {
                strcpy(auxiliar, nomes[i]);
                strcpy(nomes[i], nomes[j]);
                strcpy(nomes[j], auxiliar);
            }
        }
    }
}

//Inverte a lista crescente para criar a ordem decrescente
void inverter_nomes(char nomes[QUANTIDADE][TAMANHO_NOME]) {
    int i;
    char auxiliar[TAMANHO_NOME];

    for (i = 0; i < QUANTIDADE / 2; i++) {
        strcpy(auxiliar, nomes[i]);
        strcpy(nomes[i], nomes[QUANTIDADE - 1 - i]);
        strcpy(nomes[QUANTIDADE - 1 - i], auxiliar);
    }
}

//Embaralha os mesmos 100 nomes para o terceiro cenario
void embaralhar_nomes(char nomes[QUANTIDADE][TAMANHO_NOME]) {
    int i, posicao;
    char auxiliar[TAMANHO_NOME];

    for (i = QUANTIDADE - 1; i > 0; i--) {
        posicao = rand() % (i + 1);
        strcpy(auxiliar, nomes[i]);
        strcpy(nomes[i], nomes[posicao]);
        strcpy(nomes[posicao], auxiliar);
    }
}

//Conta quantos nos sao visitados ate chegar a chave
int contar_passos(ArvBB *raiz, char chave[]) {
    int passos = 0;

    while (raiz != NULL) {
        passos++;
        if (strcmp(chave, raiz->info.chave) == 0)
            break;
        if (strcmp(chave, raiz->info.chave) < 0)
            raiz = raiz->esq;
        else
            raiz = raiz->dir;
    }

    return passos;
}

//Conta quantos artistas existem na arvore
int contar_artistas(ArvBB *raiz) {
    int quantidade = 0;

    if (raiz != NULL)
        quantidade = 1 + contar_artistas(raiz->esq) + contar_artistas(raiz->dir);

    return quantidade;
}

//Calcula a altura: o maior caminho da raiz ate uma folha
int calcular_altura(ArvBB *raiz) {
    int esquerda, direita;
    int altura = 0;

    if (raiz != NULL) {
        esquerda = calcular_altura(raiz->esq);
        direita = calcular_altura(raiz->dir);
        if (esquerda > direita)
            altura = esquerda + 1;
        else
            altura = direita + 1;
    }

    return altura;
}

//Libera a memoria usada pela arvore de teste
void liberar_arvore(ArvBB *raiz) {
    if (raiz != NULL) {
        liberar_arvore(raiz->esq);
        liberar_arvore(raiz->dir);
        free(raiz);
    }
}

//Prepara os dados de um artista de teste
void preparar_artista(Artista *artista, char nome[]) {
    strcpy(artista->nome, nome);
    strcpy(artista->tipo, "Banda");
    strcpy(artista->estilo, "Teste");
    artista->numeroAlbuns = 0;
    artista->albuns = NULL;
}

//Mede o tempo medio do lote e guarda uma arvore para contar os passos
void inserir_cenario(char nomes[QUANTIDADE][TAMANHO_NOME], char descricao[],
                     ArvBB **raiz, FILE *arquivo, double *tempo_lote,
                     double *tempo_medio, int *chamadas_total,
                     int *comparacoes_total) {
    ArvBB *arvores_tempo[REPETICOES_INSERCAO_TEMPO];
    Artista artistas[QUANTIDADE];
    double inicio, fim;
    int repeticao, i, passos, chamadas, resultado;

    //Prepara os registros antes do cronometro para medir so as insercoes
    for (i = 0; i < QUANTIDADE; i++)
        preparar_artista(&artistas[i], nomes[i]);

    for (repeticao = 0; repeticao < REPETICOES_INSERCAO_TEMPO; repeticao++)
        arvores_tempo[repeticao] = NULL;

    //Repetimos o lote completo para o clock medir um intervalo maior
    inicio = obter_tempo();
    for (repeticao = 0; repeticao < REPETICOES_INSERCAO_TEMPO; repeticao++) {
        for (i = 0; i < QUANTIDADE; i++) {
            if (!cadastrarArtista(&arvores_tempo[repeticao], &artistas[i])) {
                printf("ERRO: falha durante a medicao das insercoes.\n");
                exit(1);
            }
        }
    }
    fim = obter_tempo();

    *tempo_lote = (fim - inicio) / REPETICOES_INSERCAO_TEMPO;
    *tempo_medio = *tempo_lote / QUANTIDADE;

    for (repeticao = 0; repeticao < REPETICOES_INSERCAO_TEMPO; repeticao++)
        liberar_arvore(arvores_tempo[repeticao]);

    //Constroi uma arvore definitiva para contar chamadas e fazer buscas
    *raiz = NULL;
    *chamadas_total = 0;
    *comparacoes_total = 0;

    for (i = 0; i < QUANTIDADE; i++) {
        passos = contar_passos(*raiz, artistas[i].nome);
        chamadas = passos + 1;
        resultado = cadastrarArtista(raiz, &artistas[i]);

        if (resultado == 0) {
            printf("ERRO: nao foi possivel inserir %s.\n", artistas[i].nome);
            exit(1);
        }

        *chamadas_total = *chamadas_total + chamadas;
        *comparacoes_total = *comparacoes_total + passos;

        if (arquivo != NULL) {
            fprintf(arquivo,
                    "ABB;%s;INSERCAO;%s;%.9f;;1;%d;1;0;%d;0;\n",
                    descricao, artistas[i].nome, *tempo_medio, chamadas, passos);
        }
    }

    if (arquivo != NULL) {
        fprintf(arquivo,
                "ABB;%s;RESUMO_INSERCAO;100 artistas;;%.9f;100;%d;100;0;%d;0;%d\n",
                descricao, *tempo_lote, *chamadas_total,
                *comparacoes_total, calcular_altura(*raiz));
    }
}

//Confirma que a arvore recusa o cadastro de um artista repetido
void testar_duplicata(char descricao[], char nome[], ArvBB *raiz, FILE *arquivo) {
    Artista artista;
    int passos, resultado, quantidade_antes, aprovado = 0;

    preparar_artista(&artista, nome);
    passos = contar_passos(raiz, nome);
    quantidade_antes = contar_artistas(raiz);
    resultado = cadastrarArtista(&raiz, &artista);

    if (resultado == 0 && contar_artistas(raiz) == quantidade_antes)
        aprovado = 1;

    if (aprovado == 1)
        printf("  OK: artista duplicado recusado; arvore continua com %d artistas.\n",
               quantidade_antes);
    else
        printf("  FALHA: artista duplicado foi aceito ou alterou a arvore.\n");

    if (arquivo != NULL) {
        fprintf(arquivo,
                "ABB;%s;TESTE_DUPLICATA;%s;;;1;%d;0;0;%d;0;%d\n",
                descricao, nome, passos, passos, calcular_altura(raiz));
    }
}

//Mede cada busca em um lote repetido e conta os passos de uma busca
void testar_buscas(char descricao[], ArvBB *raiz,char consultas[QUANTIDADE_BUSCAS][TAMANHO_NOME],FILE *arquivo, double *tempo_total,
int *chamadas_total, int *comparacoes_total) {
    int i, repeticao, passos;
    double inicio, fim;
    double duracao;
    ArvBB *encontrado;

    *tempo_total = 0;
    *chamadas_total = 0;
    *comparacoes_total = 0;

    for (i = 0; i < QUANTIDADE_BUSCAS; i++) {
        passos = contar_passos(raiz, consultas[i]);
        inicio = obter_tempo();
        for (repeticao = 0; repeticao < REPETICOES_BUSCA_TEMPO; repeticao++)
            encontrado = buscarArvBB(raiz, consultas[i]);
        fim = obter_tempo();
        duracao = (fim - inicio) / REPETICOES_BUSCA_TEMPO;

        if (encontrado == NULL) {
            printf("  FALHA: nao encontrou %s.\n", consultas[i]);
        } else {
            *tempo_total = *tempo_total + duracao;
            *chamadas_total = *chamadas_total + passos;
            *comparacoes_total = *comparacoes_total + passos;

            printf("  Artista %-12s | chamadas: %3d | media: %.9f s por busca\n",
                   consultas[i], passos, duracao);

            if (arquivo != NULL) {
                fprintf(arquivo,
                        "ABB;%s;BUSCA_10;%s;%.9f;;0;0;0;%d;%d;0;\n",
                        descricao, consultas[i], duracao, passos, passos);
            }
        }
    }

    if (arquivo != NULL) {
        fprintf(arquivo,
                "ABB;%s;RESUMO_BUSCA_10;10 artistas;%.9f;%.9f;0;0;0;%d;%d;0;\n",
                descricao, *tempo_total / QUANTIDADE_BUSCAS, *tempo_total,
                *chamadas_total, *comparacoes_total);
    }
}

//Mede buscas sem resultado e mostra a chamada final feita com ponteiro NULL
void testar_busca_ausente(char descricao[], ArvBB *raiz, FILE *arquivo) {
    char nomes[2][TAMANHO_NOME] = {"NaoExiste99991", "NaoExiste99992"};
    int i, repeticao, passos, chamadas;
    double inicio, fim;
    double duracao, tempo_total = 0;
    int chamadas_total = 0, comparacoes_total = 0;
    ArvBB *encontrado;

    for (i = 0; i < 2; i++) {
        passos = contar_passos(raiz, nomes[i]);
        inicio = obter_tempo();
        for (repeticao = 0; repeticao < REPETICOES_BUSCA_TEMPO; repeticao++)
            encontrado = buscarArvBB(raiz, nomes[i]);
        fim = obter_tempo();
        duracao = (fim - inicio) / REPETICOES_BUSCA_TEMPO;

        chamadas = passos + 1;
        tempo_total = tempo_total + duracao;
        chamadas_total = chamadas_total + chamadas;
        comparacoes_total = comparacoes_total + passos;

        if (encontrado == NULL)
            printf("  OK: %s nao existe; %d chamadas; media %.9f s.\n",
                   nomes[i], chamadas, duracao);
        else
            printf("  FALHA: busca encontrou nome inexistente %s.\n", nomes[i]);

        if (arquivo != NULL) {
            fprintf(arquivo,
                    "ABB;%s;BUSCA_AUSENTE;%s;%.9f;;0;0;0;%d;%d;0;\n",
                    descricao, nomes[i], duracao, chamadas, passos);
        }
    }

    if (arquivo != NULL) {
        fprintf(arquivo,
                "ABB;%s;RESUMO_BUSCA_AUSENTE;2 nomes;%.9f;%.9f;0;0;0;%d;%d;0;\n",
                descricao, tempo_total / 2, tempo_total,
                chamadas_total, comparacoes_total);
    }
}

//Executa a bateria completa chamada pela opcao 12 do menu
void executar_testes() {
    char nomes[QUANTIDADE][TAMANHO_NOME];
    char ordens[3][QUANTIDADE][TAMANHO_NOME];
    char descricao[3][30] = {
        "Ordem crescente",
        "Ordem decrescente",
        "Ordem aleatoria"
    };
    char consultas[QUANTIDADE_BUSCAS][TAMANHO_NOME];
    ArvBB *raizes[3] = {NULL, NULL, NULL};
    double tempo_lote[3], tempo_medio[3], tempo_busca[3];
    int chamadas_insercao[3], comparacoes_insercao[3];
    int chamadas_busca[3], comparacoes_busca[3];
    int alturas[3];
    int i, j, posicao;
    FILE *arquivo;

    srand(2026);
    gerar_nomes(nomes);

    for (i = 0; i < QUANTIDADE; i++)
        strcpy(ordens[0][i], nomes[i]);
    ordenar_crescente(ordens[0]);

    for (i = 0; i < QUANTIDADE; i++)
        strcpy(ordens[1][i], ordens[0][i]);
    inverter_nomes(ordens[1]);

    for (i = 0; i < QUANTIDADE; i++)
        strcpy(ordens[2][i], nomes[i]);
    embaralhar_nomes(ordens[2]);

    for (i = 0; i < QUANTIDADE_BUSCAS; i++) {
        posicao = i * 11;
        strcpy(consultas[i], ordens[0][posicao]);
    }

    arquivo = fopen("resultados_arvbb.csv", "w");
    if (arquivo == NULL)
        printf("AVISO: nao foi possivel criar o CSV; os testes aparecerao na tela.\n");

    if (arquivo != NULL) {
        fprintf(arquivo,"arvore;cenario;operacao;chave;tempo_medio_operacao_s;""tempo_medio_lote_s;chamadas_cadastrarArtista;chamadas_inserirArvBB;"
         "chamadas_criarNO;chamadas_buscarArvBB;comparacoes_strcmp;""chamadas_rotacao;altura_final\n");
    }

    printf("\n============================================================\n");
    printf("BATERIA DE TESTES: ARVORE BINARIA DE BUSCA (ABB)\n");
    printf("============================================================\n");
    printf("OBJETIVO\n");
    printf("Medir como a ordem de entrada altera o trabalho da ABB.\n");
    printf("Os mesmos 100 artistas serao usados nos tres cenarios.\n");
    printf("As arvores de teste nao alteram os dados da biblioteca.\n");

    printf("\nCOMO LER AS MEDIDAS\n");
    printf("- Chamadas inserirArvBB: chamadas recursivas feitas na insercao.\n");
    printf("- strcmp: comparacoes entre nomes durante a operacao.\n");
    printf("- Altura: maior caminho da raiz ate uma folha.\n");
    printf("- O tempo de insercao exclui a preparacao dos dados.\n");
    printf("- O CSV separa media por operacao e media do lote.\n");
    printf("- Tempo de busca: media de %d repeticoes por nome.\n",
           REPETICOES_BUSCA_TEMPO);
    printf("- A ABB nao usa rotacoes; o total de rotacoes e zero.\n");

    printf("\nETAPA 1 DE 4: INSERCOES\n");
    printf("Cada cenario foi repetido %d vezes para medir o lote completo.\n",
           REPETICOES_INSERCAO_TEMPO);

    for (i = 0; i < 3; i++) {
        printf("\nCENARIO %d: %s\n", i + 1, descricao[i]);
        inserir_cenario(ordens[i], descricao[i], &raizes[i], arquivo,&tempo_lote[i], &tempo_medio[i],
        &chamadas_insercao[i], &comparacoes_insercao[i]);
        alturas[i] = calcular_altura(raizes[i]);

        printf("  Tempo medio do lote de 100 insercoes: %.9f s\n", tempo_lote[i]);
        printf("  Tempo medio estimado por artista: %.9f s\n", tempo_medio[i]);
        printf("  Chamadas inserirArvBB: %d; media %.2f por artista.\n",chamadas_insercao[i],(double)chamadas_insercao[i] / QUANTIDADE);
        printf("  Comparacoes strcmp: %d.\n", comparacoes_insercao[i]);
        printf("  Altura final: %d.\n", alturas[i]);

        if (contar_artistas(raizes[i]) == QUANTIDADE)
            printf("  Validacao: os 100 artistas estao na arvore.\n");
        else
            printf("  FALHA: a arvore nao contem os 100 artistas.\n");
    }

    printf("\nETAPA 2 DE 4: BUSCAS DOS MESMOS 10 ARTISTAS\n");
    printf("A busca e repetida para medir o tempo medio por operacao.\n");

    for (i = 0; i < 3; i++) {
        printf("\nCENARIO: %s\n", descricao[i]);
        testar_buscas(descricao[i], raizes[i], consultas, arquivo,&tempo_busca[i], &chamadas_busca[i],&comparacoes_busca[i]);
        printf("  Chamadas nas 10 buscas: %d; media %.2f por busca.\n",chamadas_busca[i],(double)chamadas_busca[i] / QUANTIDADE_BUSCAS);
        printf("  Tempo estimado das 10 buscas: %.9f s.\n", tempo_busca[i]);
    }

    printf("\nETAPA 3 DE 4: TESTES COMPLEMENTARES\n");
    printf("A. Cadastro duplicado: deve ser recusado.\n");

    for (i = 0; i < 3; i++) {
        printf("  %s:\n", descricao[i]);
        testar_duplicata(descricao[i], ordens[i][0], raizes[i], arquivo);
    }

    printf("B. Busca de nomes ausentes: devem terminar sem encontrar artista.\n");
    for (i = 0; i < 3; i++) {
        printf("  %s:\n", descricao[i]);
        testar_busca_ausente(descricao[i], raizes[i], arquivo);
    }

    printf("\nETAPA 4 DE 4: COMPARACAO ENTRE INSERCAO E BUSCA\n");
    printf("A tabela compara quantas chaves sao comparadas por operacao.\n");
    printf("Chamadas de insercao incluem a chamada final ao ponteiro NULL.\n");
    printf("Por isso, usamos strcmp para comparar insercao e busca de forma justa.\n");
    printf("Cenario              Insercao/operacao   Busca/operacao\n");

    for (i = 0; i < 3; i++) {
        printf("%-20s %16.2f %15.2f\n",descricao[i],(double)comparacoes_insercao[i] / QUANTIDADE,(double)comparacoes_busca[i] / QUANTIDADE_BUSCAS);
    }

    printf("\nCONCLUSAO\n");
    printf("Altura alta indica que a ABB ficou inclinada.\n");
    printf("Use comparacoes e altura para comparar; tempos variam por computador.\n");
    if (arquivo != NULL)
        printf("Detalhes salvos em resultados_arvbb.csv.\n");

    if (arquivo != NULL)
        fclose(arquivo);

    for (j = 0; j < 3; j++)
        liberar_arvore(raizes[j]);

    return;
}
