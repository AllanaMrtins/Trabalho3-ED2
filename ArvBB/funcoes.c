#include "estruturas.h"

ArvBB *criarNO(char *chave, int tipo, void *dado){
    ArvBB *novo = (ArvBB*)malloc(sizeof(ArvBB));
    if (novo != NULL)
    {
        strncpy(novo->info.chave, chave, sizeof(novo->info.chave) - 1);

        novo->info.chave[sizeof(novo->info.chave) -  1] = '\0';
        novo->info.tipo = tipo;
        
        if (tipo == TIPO_ARTISTA)
            novo->info.dado.artista = *(Artista *)dado;

        else if(tipo == TIPO_ALBUM)
            novo->info.dado.album = *(Album *)dado;

        else if(tipo == TIPO_MUSICA)
            novo->info.dado.musica = *(Musica *)dado;

        else if(tipo == TIPO_PLAYLIST)
            novo->info.dado.playlist = *(Playlist *)dado;

        else if(tipo == TIPO_MUSICA_PLAYLIST)
            novo->info.dado.musicaplaylist = *(MusicaPlaylist *)dado;

        else{
            free(novo);
            novo = NULL;
        }

        if (novo != NULL)
        {
            novo->esq = NULL;
            novo->dir = NULL;
        }
        
    }
    return novo;
}

int inserirArvBB(ArvBB **raiz, char *chave, int tipo, void *dado){
    int resultado = 0;
    int comparacao;

    if (*raiz == NULL)
    {
        *raiz = criarNO(chave, tipo, dado);
        resultado = (*raiz != NULL);
    }else{
        comparacao = strcmp(chave, (*raiz)->info.chave);

        if (comparacao < 0)
            resultado = inserirArvBB(&(*raiz)->esq, chave, tipo, dado);

        else if (comparacao > 0)
            resultado = inserirArvBB(&(*raiz)->dir, chave, tipo, dado);
    }
    return resultado;
}