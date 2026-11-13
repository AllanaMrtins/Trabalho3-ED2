#include "estruturas.h"

#ifdef _WIN32
#include <windows.h>
#endif

extern int contabilizarMetricasAVL;
extern int chamadasAlturaAVL;
extern int chamadasMaiorAVL;
extern int chamadasAtualizarAlturaAVL;
extern int chamadasFatorBalanceamentoAVL;
extern int chamadasRotacaoDireitaAVL;
extern int chamadasRotacaoEsquerdaAVL;
extern int chamadasCriarNOAVL;
extern int chamadasInserirArvAVL;
extern int chamadasInserirAVL;
extern int chamadasBuscarArvAVL;
extern int chamadasCadastrarArtistaAVL;
extern int comparacoesStrcmpAVL;

#define QUANTIDADE 100
#define TAMANHO_NOME 30
#define QUANTIDADE_BUSCAS 10
#define REPETICOES_INSERCAO_TEMPO 100
#define REPETICOES_BUSCA_TEMPO 1000000

typedef struct {
    int cadastrarArtista;
    int inserir;
    int inserirArvAVL;
    int criarNO;
    int buscarArvAVL;
    int comparacoes;
    int altura;
    int maior;
    int atualizarAltura;
    int fatorBalanceamento;
    int rotacaoEsquerda;
    int rotacaoDireita;
} Metricas;

//Retorna o tempo atual em segundos
double obter_tempo(void) {
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

//Zera todos os contadores das funcoes AVL
void zerar_metricas(void) {
    chamadasAlturaAVL = 0;
    chamadasMaiorAVL = 0;
    chamadasAtualizarAlturaAVL = 0;
    chamadasFatorBalanceamentoAVL = 0;
    chamadasRotacaoDireitaAVL = 0;
    chamadasRotacaoEsquerdaAVL = 0;
    chamadasCriarNOAVL = 0;
    chamadasInserirArvAVL = 0;
    chamadasInserirAVL = 0;
    chamadasBuscarArvAVL = 0;
    chamadasCadastrarArtistaAVL = 0;
    comparacoesStrcmpAVL = 0;
}

//Copia os contadores globais para uma estrutura local
Metricas ler_metricas() {
    Metricas m;
    m.cadastrarArtista = chamadasCadastrarArtistaAVL;
    m.inserir = chamadasInserirAVL;
    m.inserirArvAVL = chamadasInserirArvAVL;
    m.criarNO = chamadasCriarNOAVL;
    m.buscarArvAVL = chamadasBuscarArvAVL;
    m.comparacoes = comparacoesStrcmpAVL;
    m.altura = chamadasAlturaAVL;
    m.maior = chamadasMaiorAVL;
    m.atualizarAltura = chamadasAtualizarAlturaAVL;
    m.fatorBalanceamento = chamadasFatorBalanceamentoAVL;
    m.rotacaoEsquerda = chamadasRotacaoEsquerdaAVL;
    m.rotacaoDireita = chamadasRotacaoDireitaAVL;
    return m;
}

//Soma as metricas de uma operacao ao total do cenario
void somar_metricas(Metricas *total, Metricas atual) {
    total->cadastrarArtista += atual.cadastrarArtista;
    total->inserir += atual.inserir;
    total->inserirArvAVL += atual.inserirArvAVL;
    total->criarNO += atual.criarNO;
    total->buscarArvAVL += atual.buscarArvAVL;
    total->comparacoes += atual.comparacoes;
    total->altura += atual.altura;
    total->maior += atual.maior;
    total->atualizarAltura += atual.atualizarAltura;
    total->fatorBalanceamento += atual.fatorBalanceamento;
    total->rotacaoEsquerda += atual.rotacaoEsquerda;
    total->rotacaoDireita += atual.rotacaoDireita;
}

//Prepara os campos do artista usado nos testes
void preparar_artista(Artista *artista, char nome[]) {
    strcpy(artista->nome, nome);
    strcpy(artista->tipo, "Banda");
    strcpy(artista->estilo, "Teste");
    artista->numeroAlbuns = 0;
    artista->albuns = NULL;
}

//Gera nomes aleatorios e sem repeticao
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

//Inverte a lista para criar a ordem decrescente
void inverter_nomes(char nomes[QUANTIDADE][TAMANHO_NOME]) {
    int i;
    char auxiliar[TAMANHO_NOME];
    for (i = 0; i < QUANTIDADE / 2; i++) {
        strcpy(auxiliar, nomes[i]);
        strcpy(nomes[i], nomes[QUANTIDADE - 1 - i]);
        strcpy(nomes[QUANTIDADE - 1 - i], auxiliar);
    }
}

//Embaralha os nomes para criar a ordem aleatoria
void embaralhar_nomes(char nomes[QUANTIDADE][TAMANHO_NOME]) {
    int i, posicao;
    char auxiliar[TAMANHO_NOME];
    for (i = QUANTIDADE - 1; i > 0; i--) {
        posicao = rand() % (i + 1);
        if (posicao != i) {
            strcpy(auxiliar, nomes[i]);
            strcpy(nomes[i], nomes[posicao]);
            strcpy(nomes[posicao], auxiliar);
        }
    }
}


int contar_passos(ArvAVL *raiz, char chave[]) {
    int passos = 0;
    int comparacao;
    while (raiz != NULL) {
        passos++;
        comparacao = strcmp(chave, raiz->info.chave);
        if (comparacao == 0)
            break;
        if (comparacao < 0)
            raiz = raiz->esq;
        else
            raiz = raiz->dir;
    }
    return passos;
}

int contar_nos(ArvAVL *raiz) {
    int quantidade = 0;
    if (raiz != NULL)
        quantidade = 1 + contar_nos(raiz->esq) + contar_nos(raiz->dir);
    return quantidade;
}

//Calcula a altura real para conferir o campo armazenado em cada no
int calcular_altura_real(ArvAVL *raiz) {
    int esquerda, direita, resultado = 0;
    if (raiz != NULL) {
        esquerda = calcular_altura_real(raiz->esq);
        direita = calcular_altura_real(raiz->dir);
        if (esquerda > direita)
            resultado = esquerda + 1;
        else
            resultado = direita + 1;
    }
    return resultado;
}

int validar_avl_no(ArvAVL *raiz, int *altura_calculada) {
    int valido = 1;
    int esquerda = 0, direita = 0;
    int altura_esquerda = 0, altura_direita = 0;
    int maior_altura;
    if (raiz == NULL) {
        *altura_calculada = 0;
    } else {
        valido = validar_avl_no(raiz->esq, &altura_esquerda);
        if (validar_avl_no(raiz->dir, &altura_direita) == 0)
            valido = 0;
        esquerda = altura_esquerda;
        direita = altura_direita;
        if (esquerda > direita)
            maior_altura = esquerda;
        else
            maior_altura = direita;
        if (raiz->altura != 1 + maior_altura)
            valido = 0;
        if (esquerda - direita > 1 || direita - esquerda > 1)
            valido = 0;
        *altura_calculada = 1 + maior_altura;
    }
    return valido;
}

//confere se a arvore toda respeita as regras AVL
int validar_avl(ArvAVL *raiz, int *altura_calculada) {
    int valido = 1;
    if (raiz == NULL) {
        *altura_calculada = 0;
        valido = 0;
    } else {
        valido = validar_avl_no(raiz, altura_calculada);
    }
    return valido;
}

//Libera os nos criados para os testes
void liberar_arvore(ArvAVL *raiz) {
    if (raiz != NULL) {
        liberar_arvore(raiz->esq);
        liberar_arvore(raiz->dir);
        free(raiz);
    }
}

//Imprime um grupo de metricas de uma operacao
void mostrar_metricas(Metricas m) {
    printf("  Cadastro: cadastrarArtista %d; inserir %d; inserirArvAVL %d; criarNO %d.\n",
           m.cadastrarArtista, m.inserir, m.inserirArvAVL, m.criarNO);
    printf("  Busca/comparacoes: buscarArvAVL %d; strcmp %d.\n",
           m.buscarArvAVL, m.comparacoes);
    printf("  Balanceamento: altura %d; maior %d; atualizarAltura %d; fatorBalanceamento %d.\n",
           m.altura, m.maior, m.atualizarAltura, m.fatorBalanceamento);
    printf("  Rotacoes: esquerda %d; direita %d; total %d.\n",
           m.rotacaoEsquerda, m.rotacaoDireita,
           m.rotacaoEsquerda + m.rotacaoDireita);
}

//Escreve as metricas no CSV, na mesma ordem do cabecalho
void escrever_metricas(FILE *arquivo, Metricas m) {
    fprintf(arquivo, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d",
            m.cadastrarArtista, m.inserir, m.inserirArvAVL, m.criarNO,
            m.buscarArvAVL, m.comparacoes, m.altura, m.maior,
            m.atualizarAltura, m.fatorBalanceamento,
            m.rotacaoEsquerda, m.rotacaoDireita);
}

//Mede o tempo, conta chamadas e constroi uma arvore para um cenario
void inserir_cenario(char nomes[QUANTIDADE][TAMANHO_NOME], char descricao[],
                     ArvAVL **raiz, FILE *arquivo,
                     double *tempo_lote, double *tempo_medio,
                     Metricas *metricas_total, int *altura_final, int *validacao) {
    ArvAVL *arvores_tempo[REPETICOES_INSERCAO_TEMPO];
    ArvAVL *arvore_tempo_individual;
    Artista artistas[QUANTIDADE];
    double inicio, fim;
    double tempos_individuais[QUANTIDADE];
    int repeticao, i, resultado, altura_calculada, passos;
    Metricas atual;

    for (i = 0; i < QUANTIDADE; i++)
        preparar_artista(&artistas[i], nomes[i]);
    for (repeticao = 0; repeticao < REPETICOES_INSERCAO_TEMPO; repeticao++)
        arvores_tempo[repeticao] = NULL;

    contabilizarMetricasAVL = 0;
    inicio = obter_tempo();
    for (repeticao = 0; repeticao < REPETICOES_INSERCAO_TEMPO; repeticao++) {
        for (i = 0; i < QUANTIDADE; i++) {
            if (!cadastrarArtista(&arvores_tempo[repeticao], &artistas[i])) {
                printf("ERRO: falha na medicao de insercao.\n");
                exit(1);
            }
        }
    }
    fim = obter_tempo();
    *tempo_lote = (fim - inicio) / REPETICOES_INSERCAO_TEMPO;
    *tempo_medio = *tempo_lote / QUANTIDADE;
    for (repeticao = 0; repeticao < REPETICOES_INSERCAO_TEMPO; repeticao++)
        liberar_arvore(arvores_tempo[repeticao]);

    /* Mede cada artista em 100 repeticoes do mesmo cenario. */
    for (i = 0; i < QUANTIDADE; i++)
        tempos_individuais[i] = 0;

    contabilizarMetricasAVL = 0;
    for (repeticao = 0; repeticao < REPETICOES_INSERCAO_TEMPO; repeticao++) {
        arvore_tempo_individual = NULL;
        for (i = 0; i < QUANTIDADE; i++) {
            inicio = obter_tempo();
            resultado = cadastrarArtista(&arvore_tempo_individual, &artistas[i]);
            fim = obter_tempo();

            if (resultado == 0) {
                printf("ERRO: falha ao medir a insercao individual de %s.\n",
                       artistas[i].nome);
                exit(1);
            }

            tempos_individuais[i] = tempos_individuais[i] + (fim - inicio);
        }
        liberar_arvore(arvore_tempo_individual);
    }

    for (i = 0; i < QUANTIDADE; i++)
        tempos_individuais[i] = tempos_individuais[i] / REPETICOES_INSERCAO_TEMPO;

    *raiz = NULL;
    zerar_metricas();
    metricas_total->cadastrarArtista = 0;
    metricas_total->inserir = 0;
    metricas_total->inserirArvAVL = 0;
    metricas_total->criarNO = 0;
    metricas_total->buscarArvAVL = 0;
    metricas_total->comparacoes = 0;
    metricas_total->altura = 0;
    metricas_total->maior = 0;
    metricas_total->atualizarAltura = 0;
    metricas_total->fatorBalanceamento = 0;
    metricas_total->rotacaoEsquerda = 0;
    metricas_total->rotacaoDireita = 0;

    printf("\nCENARIO: %s\n", descricao);
    printf("Tabela: tempo individual medio de %d repeticoes por artista.\n",
           REPETICOES_INSERCAO_TEMPO);
    printf("Chamadas fixas por artista: cadastrarArtista=1, inserir=1, criarNO=1, buscarArvAVL=0.\n");
    printf("Passos contam os nos visitados; inserirArvAVL inclui a chamada final ao NULL.\n");
    printf("As colunas alt(), maior(), atualiza() e fator() mostram chamadas dessas funcoes.\n");
    printf("  %-14s | %14s | %6s | %13s | %6s | %5s | %5s | %7s | %5s | %9s | %8s\n",
           "Artista", "Tempo (s)", "Passos", "inserirArvAVL", "strcmp",
           "Rot E", "Rot D", "alt()", "maior()", "atualiza", "fator()");
    printf("  ---------------+----------------+--------+---------------+--------+-------+-------+---------+-------+-----------+----------\n");
    for (i = 0; i < QUANTIDADE; i++) {
        passos = contar_passos(*raiz, artistas[i].nome);
        zerar_metricas();
        contabilizarMetricasAVL = 1;
        resultado = cadastrarArtista(raiz, &artistas[i]);
        contabilizarMetricasAVL = 0;
        atual = ler_metricas();
        somar_metricas(metricas_total, atual);

        if (resultado == 0) {
            printf("ERRO: nao foi possivel inserir %s.\n", artistas[i].nome);
            exit(1);
        }

        printf("  %-14s | %14.9f | %6d | %13d | %6d | %5d | %5d | %7d | %5d | %9d | %8d\n",
               artistas[i].nome, tempos_individuais[i], passos,
               atual.inserirArvAVL, atual.comparacoes,
               atual.rotacaoEsquerda, atual.rotacaoDireita, atual.altura,
               atual.maior, atual.atualizarAltura, atual.fatorBalanceamento);

        if (arquivo != NULL) {
            fprintf(arquivo, "AVL;%s;INSERCAO;%s;%.9f;;", descricao,
                    artistas[i].nome, tempos_individuais[i]);
            escrever_metricas(arquivo, atual);
            fprintf(arquivo, ";;\n");
        }
    }

    printf("  ---------------+----------------+--------+---------------+--------+-------+-------+---------+-------+-----------+----------\n");

    *validacao = validar_avl(*raiz, &altura_calculada);
    *altura_final = altura(*raiz);
    if (arquivo != NULL) {
        fprintf(arquivo, "AVL;%s;RESUMO_INSERCAO;100 artistas;;%.9f;",
                descricao, *tempo_lote);
        escrever_metricas(arquivo, *metricas_total);
        fprintf(arquivo, ";%d;%d\n", *altura_final, *validacao);
    }
}

//mede as buscas dos mesmos artistas nos tres cenarios
void testar_buscas(char descricao[], ArvAVL *raiz,char consultas[QUANTIDADE_BUSCAS][TAMANHO_NOME],FILE *arquivo, double *tempo_total, Metricas *total) {
    int i, repeticao, altura_calculada, validacao;
    double inicio, fim, duracao;
    ArvAVL *encontrado;
    Metricas atual;

    *tempo_total = 0;
    total->cadastrarArtista = 0;
    total->inserir = 0;
    total->inserirArvAVL = 0;
    total->criarNO = 0;
    total->buscarArvAVL = 0;
    total->comparacoes = 0;
    total->altura = 0;
    total->maior = 0;
    total->atualizarAltura = 0;
    total->fatorBalanceamento = 0;
    total->rotacaoEsquerda = 0;
    total->rotacaoDireita = 0;
    validacao = validar_avl(raiz, &altura_calculada);

    for (i = 0; i < QUANTIDADE_BUSCAS; i++) {
        inicio = obter_tempo();
        for (repeticao = 0; repeticao < REPETICOES_BUSCA_TEMPO; repeticao++)
            encontrado = buscarArvAVL(raiz, consultas[i]);
        fim = obter_tempo();
        duracao = (fim - inicio) / REPETICOES_BUSCA_TEMPO;

        zerar_metricas();
        contabilizarMetricasAVL = 1;
        encontrado = buscarArvAVL(raiz, consultas[i]);
        contabilizarMetricasAVL = 0;
        atual = ler_metricas();

        if (encontrado == NULL) {
            printf("  FALHA: nao encontrou %s.\n", consultas[i]);
        } else {
            somar_metricas(total, atual);
            *tempo_total += duracao;
            printf("  Artista %-12s | chamadas buscarArvAVL: %d | strcmp: %d | media %.9f s\n",
                   consultas[i], atual.buscarArvAVL, atual.comparacoes, duracao);
            if (arquivo != NULL) {
                fprintf(arquivo, "AVL;%s;BUSCA;%s;%.9f;;", descricao,
                        consultas[i], duracao);
                escrever_metricas(arquivo, atual);
                fprintf(arquivo, ";%d;%d\n", altura(raiz), validacao);
            }
        }
    }

    if (arquivo != NULL) {
        fprintf(arquivo, "AVL;%s;RESUMO_BUSCA;10 artistas;%.9f;%.9f;",
                descricao, *tempo_total / QUANTIDADE_BUSCAS, *tempo_total);
        escrever_metricas(arquivo, *total);
        fprintf(arquivo, ";%d;%d\n", altura(raiz), validacao);
    }
}

//Verifica que uma duplicata e recusada sem criar outro no
void testar_duplicata(char descricao[], char nome[], ArvAVL **raiz,
                      FILE *arquivo) {
    Artista artista;
    int resultado, quantidade_antes, altura_calculada, validacao;
    Metricas m;
    preparar_artista(&artista, nome);
    quantidade_antes = contar_nos(*raiz);
    zerar_metricas();
    contabilizarMetricasAVL = 1;
    resultado = cadastrarArtista(raiz, &artista);
    contabilizarMetricasAVL = 0;
    m = ler_metricas();
    validacao = validar_avl(*raiz, &altura_calculada);

    if (resultado == 0 && contar_nos(*raiz) == quantidade_antes && validacao == 1)
        printf("  OK: duplicata recusada; arvore continua valida com %d artistas.\n", quantidade_antes);
    else
        printf("  FALHA: duplicata alterou a arvore ou quebrou uma regra AVL.\n");
    mostrar_metricas(m);
    if (arquivo != NULL) {
        fprintf(arquivo, "AVL;%s;TESTE_DUPLICATA;%s;;;", descricao, nome);
        escrever_metricas(arquivo, m);
        fprintf(arquivo, ";%d;%d\n", altura(*raiz), validacao);
    }
}

void testar_ausentes(char descricao[], ArvAVL *raiz, FILE *arquivo) {
    char nomes[2][TAMANHO_NOME] = {"NaoExiste99991", "NaoExiste99992"};
    int i, altura_calculada, validacao;
    ArvAVL *encontrado;
    Metricas m;
    validacao = validar_avl(raiz, &altura_calculada);

    for (i = 0; i < 2; i++) {
        zerar_metricas();
        contabilizarMetricasAVL = 1;
        encontrado = buscarArvAVL(raiz, nomes[i]);
        contabilizarMetricasAVL = 0;
        m = ler_metricas();
        if (encontrado == NULL)
            printf("  OK: %s nao existe; chamadas buscarArvAVL %d; strcmp %d.\n",
                   nomes[i], m.buscarArvAVL, m.comparacoes);
        else
            printf("  FALHA: encontrou nome inexistente %s.\n", nomes[i]);
        if (arquivo != NULL) {
            fprintf(arquivo, "AVL;%s;BUSCA_AUSENTE;%s;;;", descricao, nomes[i]);
            escrever_metricas(arquivo, m);
            fprintf(arquivo, ";%d;%d\n", altura(raiz), validacao);
        }
    }
}

void executar_testes_avl() {
    char nomes[QUANTIDADE][TAMANHO_NOME];
    char ordens[3][QUANTIDADE][TAMANHO_NOME];
    char descricao[3][30] = {"Ordem crescente", "Ordem decrescente", "Ordem aleatoria"};
    char consultas[QUANTIDADE_BUSCAS][TAMANHO_NOME];
    ArvAVL *raizes[3] = {NULL, NULL, NULL};
    double tempo_lote[3], tempo_medio[3], tempo_busca[3];
    Metricas metricas_insercao[3], metricas_busca[3];
    int alturas[3], validacoes[3];
    int i, j, posicao, altura_calculada;
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

    arquivo = fopen("resultados_avl.csv", "w");
    if (arquivo == NULL)
        printf("AVISO: nao foi possivel criar o CSV; os resultados aparecerao na tela.\n");
    else
        fprintf(arquivo, "arvore;cenario;operacao;chave;tempo_medio_operacao_s;tempo_medio_lote_s;chamadas_cadastrarArtista;chamadas_inserir;chamadas_inserirArvAVL;chamadas_criarNO;chamadas_buscarArvAVL;comparacoes_strcmp;chamadas_altura;chamadas_maior;chamadas_atualizarAltura;chamadas_fatorBalanceamento;rotacoes_esquerda;rotacoes_direita;altura;validacao\n");

    printf("\n============================================================\n");
    printf("BATERIA DE TESTES: ARVORE AVL\n");
    printf("============================================================\n");
    printf("Os mesmos 100 artistas serao inseridos em tres ordens.\n");
    printf("As metricas contam funcoes de cadastro, busca e balanceamento.\n");
    printf("O tempo de insercao e a media de %d lotes de 100 artistas.\n",
           REPETICOES_INSERCAO_TEMPO);
    printf("O tempo individual de cada artista tambem e a media de %d repeticoes.\n",
           REPETICOES_INSERCAO_TEMPO);
    printf("As tabelas mostram as metricas por artista; o CSV guarda os nomes completos das funcoes.\n");
    printf("Cada busca cronometrada e repetida %d vezes.\n", REPETICOES_BUSCA_TEMPO);

    printf("\nETAPA 1 DE 4: INSERCOES E BALANCEAMENTO\n");
    for (i = 0; i < 3; i++) {
        inserir_cenario(ordens[i], descricao[i], &raizes[i], arquivo,
                        &tempo_lote[i], &tempo_medio[i], &metricas_insercao[i],
                        &alturas[i], &validacoes[i]);
        printf("  Tempo medio do lote de 100 insercoes: %.9f s\n",
               tempo_lote[i]);
        printf("  Tempo medio estimado por artista: %.9f s\n",
               tempo_medio[i]);
        printf("  Chamadas fixas: cadastrarArtista %d; inserir %d; criarNO %d; buscarArvAVL %d.\n",
               metricas_insercao[i].cadastrarArtista,
               metricas_insercao[i].inserir,
               metricas_insercao[i].criarNO,
               metricas_insercao[i].buscarArvAVL);
        printf("  Chamadas inserirArvAVL: %d; media %.2f por artista.\n",
               metricas_insercao[i].inserirArvAVL,
               (double)metricas_insercao[i].inserirArvAVL / QUANTIDADE);
        printf("  Comparacoes strcmp: %d; media %.2f por artista.\n",
               metricas_insercao[i].comparacoes,
               (double)metricas_insercao[i].comparacoes / QUANTIDADE);
        printf("  Chamadas de balanceamento: altura %d; maior %d; atualizarAltura %d; fatorBalanceamento %d.\n",
               metricas_insercao[i].altura, metricas_insercao[i].maior,
               metricas_insercao[i].atualizarAltura,
               metricas_insercao[i].fatorBalanceamento);
        printf("  Rotacoes: esquerda %d; direita %d; total %d.\n",
               metricas_insercao[i].rotacaoEsquerda,
               metricas_insercao[i].rotacaoDireita,
               metricas_insercao[i].rotacaoEsquerda +
               metricas_insercao[i].rotacaoDireita);
        printf("  Altura final da AVL: %d.\n", alturas[i]);
        /* A validacao e exibida logo abaixo, junto com o total de nos. */
        if (validacoes[i] == 1)
            printf("  Validacao AVL: PASSOU.\n");
        else
            printf("  Validacao AVL: FALHOU.\n");
        if (contar_nos(raizes[i]) == QUANTIDADE)
            printf("  Validacao: os 100 artistas estao na arvore.\n");
        else
            printf("  FALHA: a arvore nao contem os 100 artistas.\n");
    }

    printf("\nETAPA 2 DE 4: BUSCAS DOS MESMOS 10 ARTISTAS\n");
    for (i = 0; i < 3; i++) {
        printf("\nCENARIO: %s\n", descricao[i]);
        testar_buscas(descricao[i], raizes[i], consultas, arquivo,
                      &tempo_busca[i], &metricas_busca[i]);
        printf("  Media das 10 buscas: %.9f s; chamadas buscarArvAVL: %d; strcmp: %d.\n",
               tempo_busca[i] / QUANTIDADE_BUSCAS,
               metricas_busca[i].buscarArvAVL, metricas_busca[i].comparacoes);
    }

    printf("\nETAPA 3 DE 4: TESTES COMPLEMENTARES\n");
    printf("A. Cadastro duplicado: deve ser recusado.\n");
    for (i = 0; i < 3; i++) {
        printf("  %s:\n", descricao[i]);
        testar_duplicata(descricao[i], ordens[i][0], &raizes[i], arquivo);
    }
    printf("B. Busca de nomes ausentes: deve retornar NULL.\n");
    for (i = 0; i < 3; i++) {
        printf("  %s:\n", descricao[i]);
        testar_ausentes(descricao[i], raizes[i], arquivo);
    }

    printf("\nETAPA 4 DE 4: RESUMO DA ALTURA\n");
    for (i = 0; i < 3; i++) {
        altura_calculada = calcular_altura_real(raizes[i]);
        if (validacoes[i] == 1)
            printf("%-20s altura armazenada: %d; altura calculada: %d; validacao: PASSOU.\n",
                   descricao[i], alturas[i], altura_calculada);
        else
            printf("%-20s altura armazenada: %d; altura calculada: %d; validacao: FALHOU.\n",
                   descricao[i], alturas[i], altura_calculada);
    }
    printf("Compare principalmente comparacoes, chamadas auxiliares e rotacoes; os tempos variam por computador.\n");
    if (arquivo != NULL) {
        fclose(arquivo);
        printf("Detalhes salvos em resultados_avl.csv.\n");
    }
    for (j = 0; j < 3; j++)
        liberar_arvore(raizes[j]);
}

//explica as medidas que a bateria apresenta
void explicar_testes_avl() {
    printf("\n===== COMO FUNCIONAM OS TESTES AVL =====\n");
    printf("1. Sao criados 100 nomes de artistas sem repeticao.\n");
    printf("2. Os mesmos nomes entram em ordem crescente, decrescente e aleatoria.\n");
    printf("3. O tempo e medido em lotes repetidos, sem preparar dados no cronometro.\n");
    printf("   Tambem e medido o tempo individual de cada insercao, pela media de 100 repeticoes.\n");
    printf("4. Contamos chamadas de cadastro, insercao, busca e funcoes de balanceamento.\n");
    printf("5. As rotacoes para a esquerda e para a direita sao contadas separadamente.\n");
    printf("6. A validacao confere alturas armazenadas e fator de balanceamento de cada no.\n");
    printf("7. Tambem sao testadas buscas existentes, duplicatas e nomes ausentes.\n");
    printf("8. Os resultados detalhados ficam em resultados_avl.csv.\n");
}