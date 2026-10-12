#ifndef ARVORE_AVL
#define ARVORE_AVL

#define TIPO_ARTISTA         1
#define TIPO_ALBUM           2
#define TIPO_MUSICA          3
#define TIPO_PLAYLIST        4
#define TIPO_MUSICA_PLAYLIST 5

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

typedef struct ArvAVL ArvAVL;

typedef struct{
    char nome[30];
    char tipo[30];
    char estilo[50];
    int numeroAlbuns;
    ArvAVL *albuns;
}Artista;

typedef struct{
    char titulo[100];
    int ano;
    int qtdMusicas;
    ArvAVL *musicas;
}Album;

typedef struct{
    char titulo[100];
    float minutos;
}Musica;

typedef struct{
    char nome[100];
    ArvAVL *musicas;
}Playlist;

typedef struct{
    char nomeArtista[100];
    char tituloAlbum[100];
    char tituloMusica[100];
}MusicaPlaylist;

typedef union{
    Artista artista;
    Album album;
    Musica musica;
    Playlist playlist;
    MusicaPlaylist musicaplaylist;
}Dado;

typedef struct{
    char chave[100];
    int tipo;
    Dado dado;
}Info;

struct ArvAVL{
    Info info;
    int altura;
    struct ArvAVL *esq;
    struct ArvAVL *dir;
};

//------------FUNÇÕES-----------

int altura(ArvAVL *raiz);

int maior(int a, int b);

void atualizarAltura(ArvAVL *raiz);

int fatorBalanceamento(ArvAVL *raiz);

ArvAVL *rotacaoDireita(ArvAVL *raiz);

ArvAVL *rotacaoEsquerda(ArvAVL *raiz);

ArvAVL *criarNO(char *chave,int tipo,void *dado);

ArvAVL *inserirArvAVL(ArvAVL *raiz,char *chave,int tipo,void *dado,int *resultado);

int inserir(ArvAVL **raiz,char *chave,int tipo,void *dado);

ArvAVL *buscarArvAVL(ArvAVL *raiz,char *chave);

ArvAVL *menorNo(ArvAVL *raiz);

ArvAVL *removerArvAVL(ArvAVL *raiz,char *chave,int *resultado);

int cadastrarArtista(ArvAVL **raiz,Artista *artista);

int cadastrarAlbum(ArvAVL *raizArtistas,char *nomeArtista,Album *album);

int cadastrarMusica(ArvAVL *raizArtistas,char *nomeArtista,char *tituloAlbum,Musica *musica);

int cadastrarPlaylist(ArvAVL **raizPlaylists,Playlist *playlist);

int cadastrarMusicaPlaylist(ArvAVL *raizArtistas,ArvAVL *raizPlaylists,char *nomePlaylist,char *nomeArtista,char *tituloAlbum, char *tituloMusica);

void exibirArtista(Artista artista);

void exibirAlbum(Album album);

void exibirMusica(Musica musica);

void mostrarArtistas(ArvAVL *raiz);

void exibirAlbunsAno(ArvAVL *raizAlbuns,int ano);

void exibirMusicaEncontrada(ArvAVL *raizMusicas,char *tituloMusica,Artista artista,Album album);

void procurarMusicaAlbuns(ArvAVL *raizAlbuns,char *tituloMusica,Artista artista);

void procurarMusicaArtistas(ArvAVL *raizArtistas,char *tituloMusica);

void mostrarDadosMusica(ArvAVL *raizArtistas,char *tituloMusica);

void exibirMusicaPlaylist(MusicaPlaylist musicaPlaylist);

void mostrarMusicasPlaylist(ArvAVL *raizMusicas);

void mostrarDadosPlaylist(ArvAVL *raizPlaylists,char *nomePlaylist);

int removerMusicaPlaylist(ArvAVL *raizPlaylists,char *nomePlaylist,char *nomeArtista,char *tituloAlbum,char *tituloMusica);

int musicaEstaEmPlaylist(ArvAVL *raizPlaylists,char *nomeArtista,char *tituloAlbum,char *tituloMusica);
    
int removerMusicaAlbum(ArvAVL *raizArtistas,ArvAVL *raizPlaylists,char *nomeArtista,char *tituloAlbum,char *tituloMusica);

#endif