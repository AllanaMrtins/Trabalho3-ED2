#include "estruturas.h"

#ifdef _WIN32 //verifica se o programa esta sendo compilado no Windows
#include <windows.h> //declara as funcoes de tempo de alta precisao do Windows
#endif //Encerra a verificacao especifica do Windows

extern int contabilizarRotacoes; //Usa a variavel definida em funcoes.c
extern int chamadasRotacaoEsquerda; //Acessa o total de rotacoes para a esquerda
extern int chamadasRotacaoDireita; //Acessa o total de rotacoes para a direita
extern int chamadasEhVermelho; //Acessa o total de chamadas de ehVermelho
extern int chamadasInverterCores; //Acessa o total de chamadas de inverterCores

#define QUANTIDADE 100
#define TAMANHO_NOME 30
#define QUANTIDADE_BUSCAS 10
#define REPETICOES_INSERCAO_TEMPO 100
#define REPETICOES_BUSCA_TEMPO 1000000

//Retorna o tempo em segundos
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
        if (posicao != i) {
            strcpy(auxiliar, nomes[i]);
            strcpy(nomes[i], nomes[posicao]);
            strcpy(nomes[posicao], auxiliar);
        }
    }
}

//Conta nos visitados e comparacoes feitas ate encontrar a chave
int contar_passos(ArvVermelhaPreta *raiz, char chave[]) {
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

//Conta os nos existentes na arvore
int contar_nos(ArvVermelhaPreta *raiz) {
    int quantidade = 0;

    if (raiz != NULL)
        quantidade = 1 + contar_nos(raiz->esq) + contar_nos(raiz->dir);

    return quantidade;
}

//Calcula a altura: o maior caminho da raiz ate uma folha
int calcular_altura(ArvVermelhaPreta *raiz) {
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

//Confere enlaces, cores e igualdade das alturas pretas
int validar_no(ArvVermelhaPreta *raiz, int *altura_preta) {
    int valido = 1;
    int altura_esquerda = 0;
    int altura_direita = 0;
    int valido_esquerda = 1;
    int valido_direita = 1;

    if (raiz == NULL) {
        *altura_preta = 1;
    } else {
        if (raiz->cor != RED && raiz->cor != BLACK)
            valido = 0;
        if (ehVermelho(raiz->dir))
            valido = 0;
        if (raiz->cor == RED &&
            (ehVermelho(raiz->esq) || ehVermelho(raiz->dir)))
            valido = 0;

        valido_esquerda = validar_no(raiz->esq, &altura_esquerda);
        valido_direita = validar_no(raiz->dir, &altura_direita);

        if (valido_esquerda == 0 || valido_direita == 0)
            valido = 0;
        if (altura_esquerda != altura_direita)
            valido = 0;

        *altura_preta = altura_esquerda;
        if (raiz->cor == BLACK)
            *altura_preta = *altura_preta + 1;
    }
    return valido;
}

//Confere que a raiz e preta e que a arvore vermelha e preta esta balanceada
int validar_arvore(ArvVermelhaPreta *raiz, int *altura_preta) {
    int valido = 1;

    if (raiz == NULL) {
        valido = 0;
        *altura_preta = 0;
    } else {
        if (raiz->cor != BLACK)
            valido = 0;
        if (validar_no(raiz, altura_preta) == 0)
            valido = 0;
    }
    return valido;
}

//Libera a memoria usada pela arvore de teste
void liberar_arvore(ArvVermelhaPreta *raiz) {
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

//mede os lotes e monta uma arvore para contar os passos
void inserir_cenario(char nomes[QUANTIDADE][TAMANHO_NOME], char descricao[],ArvVermelhaPreta **raiz, FILE *arquivo,double *tempo_lote, double *tempo_medio,
int *chamadas_total, int *comparacoes_total,int *rotacoes_esquerda, int *rotacoes_direita,
int *chamadas_eh_vermelho_total, int *chamadas_inverter_cores_total,
int *altura, int *altura_preta, int *validacao){
    ArvVermelhaPreta *arvores_tempo[REPETICOES_INSERCAO_TEMPO];
    ArvVermelhaPreta *arvore_tempo_individual;
    Artista artistas[QUANTIDADE];
    double inicio, fim;
    double tempos_individuais[QUANTIDADE];
    int repeticao, i, passos, chamadas, resultado;
    int rotacao_esquerda_antes, rotacao_direita_antes;
    int eh_vermelho_antes, inverter_cores_antes;
    int rotacoes_esquerda_artista, rotacoes_direita_artista;
    int chamadas_eh_vermelho_artista, chamadas_inverter_cores_artista;

    for (i = 0; i < QUANTIDADE; i++)
        preparar_artista(&artistas[i], nomes[i]);

    for (repeticao = 0; repeticao < REPETICOES_INSERCAO_TEMPO; repeticao++)
        arvores_tempo[repeticao] = NULL;

    contabilizarRotacoes = 0;
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

    /* Mede cada posicao de insercao em varios testes do mesmo cenario. */
    for (i = 0; i < QUANTIDADE; i++)
        tempos_individuais[i] = 0;

    contabilizarRotacoes = 0;
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
    *chamadas_total = 0;
    *comparacoes_total = 0;
    *rotacoes_esquerda = 0;
    *rotacoes_direita = 0;
    chamadasRotacaoEsquerda = 0;
    chamadasRotacaoDireita = 0;
    *chamadas_eh_vermelho_total = 0;
    *chamadas_inverter_cores_total = 0;
    chamadasEhVermelho = 0;
    chamadasInverterCores = 0;
    contabilizarRotacoes = 1;

    printf("  Tabela: tempo medio individual de %d repeticoes por artista.\n",
           REPETICOES_INSERCAO_TEMPO);
    printf("  Chamadas fixas por artista: cadastrarArtista=1, inserir=1, criarNO=1, buscarArvVP=0.\n");
    printf("  %-14s | %14s | %6s | %12s | %6s | %5s | %5s | %10s | %13s\n",
           "Artista", "Tempo (s)", "Passos", "inserirArvVP", "strcmp",
           "Rot E", "Rot D", "ehVermelho", "inverterCores");
    printf("  ----------------+----------------+--------+--------------+--------+-------+-------+------------+---------------\n");
    for (i = 0; i < QUANTIDADE; i++) {
        passos = contar_passos(*raiz, artistas[i].nome);
        chamadas = passos + 1;
        rotacao_esquerda_antes = chamadasRotacaoEsquerda;
        rotacao_direita_antes = chamadasRotacaoDireita;
        eh_vermelho_antes = chamadasEhVermelho;
        inverter_cores_antes = chamadasInverterCores;
        resultado = cadastrarArtista(raiz, &artistas[i]);

        if (resultado == 0) {
            printf("ERRO: nao foi possivel inserir %s.\n", artistas[i].nome);
            exit(1);
        }

        *chamadas_total = *chamadas_total + chamadas;
        *comparacoes_total = *comparacoes_total + passos;
        rotacoes_esquerda_artista = chamadasRotacaoEsquerda - rotacao_esquerda_antes;
        rotacoes_direita_artista = chamadasRotacaoDireita - rotacao_direita_antes;
        chamadas_eh_vermelho_artista = chamadasEhVermelho - eh_vermelho_antes;
        chamadas_inverter_cores_artista = chamadasInverterCores - inverter_cores_antes;
        *rotacoes_esquerda = *rotacoes_esquerda + rotacoes_esquerda_artista;
        *rotacoes_direita = *rotacoes_direita + rotacoes_direita_artista;
        *chamadas_eh_vermelho_total = *chamadas_eh_vermelho_total + chamadas_eh_vermelho_artista;
        *chamadas_inverter_cores_total = *chamadas_inverter_cores_total + chamadas_inverter_cores_artista;

        printf("  %-14s | %14.9f | %6d | %12d | %6d | %5d | %5d | %10d | %13d\n",
               artistas[i].nome, tempos_individuais[i], passos, chamadas,
               passos, rotacoes_esquerda_artista, rotacoes_direita_artista,
               chamadas_eh_vermelho_artista, chamadas_inverter_cores_artista);

        if (arquivo != NULL) {
            fprintf(arquivo,
                    "VERMELHA_PRETA;%s;INSERCAO;%s;%.9f;;1;1;%d;1;0;%d;%d;%d;;;;%d;%d\n",
                    descricao, artistas[i].nome, tempos_individuais[i], chamadas, passos,
                    rotacoes_esquerda_artista, rotacoes_direita_artista,
                    chamadas_eh_vermelho_artista, chamadas_inverter_cores_artista);
        }
    }

    printf("  ----------------+----------------+--------+--------------+--------+-------+-------+------------+---------------\n");

    contabilizarRotacoes = 0;
    *altura = calcular_altura(*raiz);
    *validacao = validar_arvore(*raiz, altura_preta);

    if (arquivo != NULL) {
        fprintf(arquivo,
                "VERMELHA_PRETA;%s;RESUMO_INSERCAO;100 artistas;;%.9f;100;100;%d;100;0;%d;%d;%d;%d;%d;%d;%d;%d\n",
                descricao, *tempo_lote, *chamadas_total,
                *comparacoes_total, *rotacoes_esquerda, *rotacoes_direita,
                *altura, *altura_preta, *validacao,
                *chamadas_eh_vermelho_total, *chamadas_inverter_cores_total);
    }
}

//Testa se a arvore recusa artista duplicado e continua valida
void testar_duplicata(char descricao[], char nome[],
                      ArvVermelhaPreta **raiz, FILE *arquivo) {
    Artista artista;
    int passos, resultado, quantidade_antes, altura_preta, validacao;
    int rotacoes_esquerda, rotacoes_direita;
    int chamadas_eh_vermelho, chamadas_inverter_cores;

    preparar_artista(&artista, nome);
    passos = contar_passos(*raiz, nome);
    quantidade_antes = contar_nos(*raiz);
    chamadasRotacaoEsquerda = 0;
    chamadasRotacaoDireita = 0;
    chamadasEhVermelho = 0;
    chamadasInverterCores = 0;
    contabilizarRotacoes = 1;
    resultado = cadastrarArtista(raiz, &artista);
    contabilizarRotacoes = 0;
    rotacoes_esquerda = chamadasRotacaoEsquerda;
    rotacoes_direita = chamadasRotacaoDireita;
    chamadas_eh_vermelho = chamadasEhVermelho;
    chamadas_inverter_cores = chamadasInverterCores;
    validacao = validar_arvore(*raiz, &altura_preta);

    if (resultado == 0 && contar_nos(*raiz) == quantidade_antes && validacao == 1)
        printf("  OK: duplicata recusada; arvore continua valida com %d artistas.\n",
               quantidade_antes);
    else
        printf("  FALHA: duplicata alterou a arvore ou quebrou uma propriedade.\n");
    printf("  Rotacoes nesta tentativa: esquerda %d; direita %d.\n",
           rotacoes_esquerda, rotacoes_direita);
    printf("  Chamadas auxiliares: ehVermelho %d; inverterCores %d.\n",
           chamadas_eh_vermelho, chamadas_inverter_cores);

    if (arquivo != NULL) {
        fprintf(arquivo,
                "VERMELHA_PRETA;%s;TESTE_DUPLICATA;%s;;;1;1;%d;0;0;%d;%d;%d;%d;%d;%d;%d;%d\n",
                descricao, nome, passos, passos, rotacoes_esquerda,
                rotacoes_direita, calcular_altura(*raiz), altura_preta,
                validacao, chamadas_eh_vermelho, chamadas_inverter_cores);
    }
}

//Mede as buscas dos mesmos 10 artistas nos tres cenarios
void testar_buscas(char descricao[], ArvVermelhaPreta *raiz,char consultas[QUANTIDADE_BUSCAS][TAMANHO_NOME],FILE *arquivo, double *tempo_total,int *chamadas_total, int *comparacoes_total) {
    int i, repeticao, passos;
    double inicio, fim, duracao;
    ArvVermelhaPreta *encontrado;
    int altura_preta, validacao;

    *tempo_total = 0;
    *chamadas_total = 0;
    *comparacoes_total = 0;
    validacao = validar_arvore(raiz, &altura_preta);

    printf("  %-14s | %12s | %8s | %15s\n",
           "Artista", "buscarArvVP", "strcmp", "Tempo medio (s)");
    printf("  ---------------+--------------+----------+-----------------\n");

    for (i = 0; i < QUANTIDADE_BUSCAS; i++) {
        passos = contar_passos(raiz, consultas[i]);
        inicio = obter_tempo();
        for (repeticao = 0; repeticao < REPETICOES_BUSCA_TEMPO; repeticao++)
            encontrado = buscarArvVP(raiz, consultas[i]);
        fim = obter_tempo();
        duracao = (fim - inicio) / REPETICOES_BUSCA_TEMPO;

        if (encontrado == NULL) {
            printf("  FALHA: nao encontrou %s.\n", consultas[i]);
        } else {
            *tempo_total = *tempo_total + duracao;
            *chamadas_total = *chamadas_total + passos;
            *comparacoes_total = *comparacoes_total + passos;

            printf("  %-14s | %12d | %8d | %15.9f\n",
                   consultas[i], passos, passos, duracao);

            if (arquivo != NULL) {
                fprintf(arquivo,
                        "VERMELHA_PRETA;%s;BUSCA_10;%s;%.9f;;0;0;0;0;%d;%d;0;0;%d;%d;%d;0;0\n",
                        descricao, consultas[i], duracao, passos, passos,
                        calcular_altura(raiz), altura_preta, validacao);
            }
        }
    }

    printf("  ---------------+--------------+----------+-----------------\n");

    if (arquivo != NULL) {
        fprintf(arquivo,
                "VERMELHA_PRETA;%s;RESUMO_BUSCA_10;10 artistas;%.9f;%.9f;0;0;0;0;%d;%d;0;0;%d;%d;%d;0;0\n",
                descricao, *tempo_total / QUANTIDADE_BUSCAS, *tempo_total,
                *chamadas_total, *comparacoes_total, calcular_altura(raiz),
                altura_preta, validacao);
    }
}

/* Mede buscas de nomes que nao estao na arvore. */
void testar_busca_ausente(char descricao[], ArvVermelhaPreta *raiz,FILE *arquivo){
    char nomes[2][TAMANHO_NOME] = {"NaoExiste99991", "NaoExiste99992"};
    int i, repeticao, passos, chamadas;
    double inicio, fim, duracao, tempo_total = 0;
    int chamadas_total = 0, comparacoes_total = 0;
    int altura_preta, validacao;
    ArvVermelhaPreta *encontrado;

    validacao = validar_arvore(raiz, &altura_preta);

    for (i = 0; i < 2; i++) {
        passos = contar_passos(raiz, nomes[i]);
        inicio = obter_tempo();
        for (repeticao = 0; repeticao < REPETICOES_BUSCA_TEMPO; repeticao++)
            encontrado = buscarArvVP(raiz, nomes[i]);
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
                    "VERMELHA_PRETA;%s;BUSCA_AUSENTE;%s;%.9f;;0;0;0;0;%d;%d;0;0;%d;%d;%d;0;0\n",
                    descricao, nomes[i], duracao, chamadas, passos,
                    calcular_altura(raiz), altura_preta, validacao);
        }
    }

    if (arquivo != NULL) {
        fprintf(arquivo,
                "VERMELHA_PRETA;%s;RESUMO_BUSCA_AUSENTE;2 nomes;%.9f;%.9f;0;0;0;0;%d;%d;0;0;%d;%d;%d;0;0\n",
                descricao, tempo_total / 2, tempo_total, chamadas_total,
                comparacoes_total, calcular_altura(raiz), altura_preta,
                validacao);
    }
}

//Executa os tres cenarios de desempenho da arvore vermelha e preta
void executar_testes(void) {
    char nomes[QUANTIDADE][TAMANHO_NOME];
    char ordens[3][QUANTIDADE][TAMANHO_NOME];
    char descricao[3][30] = {
        "Ordem crescente",
        "Ordem decrescente",
        "Ordem aleatoria"
    };
    char consultas[QUANTIDADE_BUSCAS][TAMANHO_NOME];
    ArvVermelhaPreta *raizes[3] = {NULL, NULL, NULL};
    double tempo_lote[3], tempo_medio[3], tempo_busca[3];
    int chamadas_insercao[3], comparacoes_insercao[3];
    int chamadas_busca[3], comparacoes_busca[3];
    int alturas[3], alturas_pretas[3], validacoes[3];
    int rotacoes_esquerda[3], rotacoes_direita[3];
    int chamadas_eh_vermelho[3], chamadas_inverter_cores[3];
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

    arquivo = fopen("resultados_arvvp.csv", "w");
    if (arquivo == NULL)
        printf("AVISO: nao foi possivel criar o CSV; os testes aparecerao na tela.\n");

    if (arquivo != NULL) {
        fprintf(arquivo,
                "arvore;cenario;operacao;chave;tempo_medio_operacao_s;"
                "tempo_medio_lote_s;chamadas_cadastrarArtista;chamadas_inserir;"
                "chamadas_inserirArvVP;chamadas_criarNO;chamadas_buscarArvVP;"
                "comparacoes_strcmp;rotacoes_esquerda;rotacoes_direita;"
                "altura;altura_preta;validacao;chamadas_ehVermelho;"
                "chamadas_inverterCores\n");
    }

    printf("\n============================================================\n");
    printf("BATERIA DE TESTES: ARVORE VERMELHA E PRETA\n");
    printf("============================================================\n");
    printf("OBJETIVO\n");
    printf("Medir como a ordem de entrada afeta a arvore vermelha e preta.\n");
    printf("Os mesmos 100 artistas serao usados nos tres cenarios.\n");
    printf("As arvores de teste nao alteram os dados da biblioteca.\n");

    printf("\nCOMO LER AS MEDIDAS\n");
    printf("- Chamadas de inserirArvVP incluem a chamada final ao NULL.\n");
    printf("- strcmp conta comparacoes entre nomes durante a operacao.\n");
    printf("- Altura preta inclui as folhas NIL e valida os caminhos da arvore.\n");
    printf("- Tempo de insercao exclui a preparacao dos dados.\n");
    printf("- O tempo e a media de %d lotes com 100 insercoes.\n",REPETICOES_INSERCAO_TEMPO);
    printf("- Cada artista tambem tem tempo individual, medio de %d repeticoes.\n",
           REPETICOES_INSERCAO_TEMPO);
    printf("- A tabela mostra passos, strcmp, rotacoes e funcoes auxiliares por artista.\n");
    printf("- As chamadas individuais restantes e as metricas completas ficam no CSV.\n");
    printf("- Cada busca e repetida %d vezes para medir o tempo.\n",REPETICOES_BUSCA_TEMPO);
    printf("- Rotacoes sao contadas nas funcoes originais de rotacao.\n");
    printf("- Tambem sao contadas chamadas de ehVermelho e inverterCores.\n");

    printf("\nETAPA 1 DE 4: INSERCOES\n");
    for (i = 0; i < 3; i++) {
        printf("\nCENARIO %d: %s\n", i + 1, descricao[i]);
        inserir_cenario(ordens[i], descricao[i], &raizes[i], arquivo,&tempo_lote[i], &tempo_medio[i],&chamadas_insercao[i], &comparacoes_insercao[i],
        &rotacoes_esquerda[i], &rotacoes_direita[i],&chamadas_eh_vermelho[i], &chamadas_inverter_cores[i],&alturas[i], &alturas_pretas[i], &validacoes[i]);

        printf("  Tempo medio do lote de 100 insercoes: %.9f s\n", tempo_lote[i]);
        printf("  Tempo medio estimado por artista: %.9f s\n", tempo_medio[i]);
        printf("  Chamadas fixas: cadastrarArtista 100; inserir 100; criarNO 100; buscarArvVP 0.\n");
        printf("  Chamadas inserirArvVP: %d; media %.2f por artista.\n",chamadas_insercao[i],(double)chamadas_insercao[i] / QUANTIDADE);
        printf("  Comparacoes strcmp: %d; media %.2f por artista.\n",comparacoes_insercao[i],(double)comparacoes_insercao[i] / QUANTIDADE);
        printf("  Rotacoes: esquerda %d; direita %d; total %d.\n",rotacoes_esquerda[i], rotacoes_direita[i],rotacoes_esquerda[i] + rotacoes_direita[i]);
        printf("  Chamadas auxiliares: ehVermelho %d; inverterCores %d.\n",
               chamadas_eh_vermelho[i], chamadas_inverter_cores[i]);
        printf("  Altura: %d; altura preta: %d.\n", alturas[i], alturas_pretas[i]);

        if (validacoes[i] == 1)
            printf("  Validacao da vermelha e preta: PASSOU.\n");
        else
            printf("  FALHA: uma propriedade da vermelha e preta foi violada.\n");

        if (contar_nos(raizes[i]) == QUANTIDADE)
            printf("  Validacao: os 100 artistas estao na arvore.\n");
        else
            printf("  FALHA: a arvore nao contem os 100 artistas.\n");
    }

    printf("\nETAPA 2 DE 4: BUSCAS DOS MESMOS 10 ARTISTAS\n");
    for (i = 0; i < 3; i++) {
        printf("\nCENARIO: %s\n", descricao[i]);
        testar_buscas(descricao[i], raizes[i], consultas, arquivo,
                      &tempo_busca[i], &chamadas_busca[i],
                      &comparacoes_busca[i]);
        printf("  Chamadas nas 10 buscas: %d; media %.2f por busca.\n",chamadas_busca[i],(double)chamadas_busca[i] / QUANTIDADE_BUSCAS);
        printf("  Tempo medio das 10 buscas: %.9f s.\n", tempo_busca[i]);
    }

    printf("\nETAPA 3 DE 4: TESTES COMPLEMENTARES\n");
    printf("A. Cadastro duplicado: deve ser recusado sem alterar a arvore.\n");
    for (i = 0; i < 3; i++) {
        printf("  %s:\n", descricao[i]);
        testar_duplicata(descricao[i], ordens[i][0], &raizes[i], arquivo);
    }

    printf("B. Busca de nomes ausentes: deve terminar sem encontrar artista.\n");
    for (i = 0; i < 3; i++) {
        printf("  %s:\n", descricao[i]);
        testar_busca_ausente(descricao[i], raizes[i], arquivo);
    }

    printf("\nETAPA 4 DE 4: COMPARACAO ENTRE INSERCAO E BUSCA\n");
    printf("Comparamos as medias de strcmp por operacao.\n");
    printf("Cenario              Insercao/operacao   Busca/operacao\n");
    for (i = 0; i < 3; i++) {
        printf("%-20s %16.2f %15.2f\n",
               descricao[i],(double)comparacoes_insercao[i] / QUANTIDADE,(double)comparacoes_busca[i] / QUANTIDADE_BUSCAS);
    }

    printf("\nCONCLUSAO\n");
    printf("Use comparacoes, altura e validacao; tempos variam por computador.\n");
    printf("As rotacoes esquerda e direita foram contadas durante cada insercao.\n");
    if (arquivo != NULL)
        printf("Detalhes salvos em resultados_arvvp.csv.\n");

    if (arquivo != NULL)
        fclose(arquivo);

    for (j = 0; j < 3; j++)
        liberar_arvore(raizes[j]);
}

void explicar_testes() {
    printf("\n===== COMO FUNCIONAM OS TESTES DA VERMELHA E PRETA =====\n");
    printf("1. Sao criados 100 nomes de artistas, sempre os mesmos.\n");
    printf("2. Os nomes sao inseridos em ordem crescente, decrescente e aleatoria.\n");
    printf("3. O tempo e medido em lotes repetidos, sem preparar dados no cronometro.\n");
    printf("4. Sao contadas chamadas de insercao/busca e comparacoes strcmp.\n");
    printf("5. A validacao confere raiz preta, enlaces vermelhos a esquerda,\n");
    printf("   ausencia de vermelho-vermelho e alturas pretas iguais.\n");
    printf("6. Tambem sao testadas 10 buscas, duplicatas e nomes inexistentes.\n");
    printf("7. As rotacoes esquerda e direita sao contadas em funcoes.c.\n");
    printf("8. Tambem sao contadas chamadas de ehVermelho e inverterCores.\n");
    printf("9. Os resultados detalhados ficam em resultados_arvvp.csv.\n");
    printf("10. O tempo individual de cada artista e a media de 100 repeticoes.\n");
}
