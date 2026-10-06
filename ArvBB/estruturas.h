#ifndef ARVORE_BINARIA_BUSCA
#define ARVORE_BINARIA_BUSCA

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

typedef struct ArvBB ArvBB;

typedef struct{
    char nome[30];
    char tipo[30];
    char estilo[50];
    int numeroAlbuns;
    ArvBB *albuns;
}Artista;

typedef struct{
    char titulo[100];
    int ano;
    int qtdMusicas;
    ArvBB *musicas;
}Album;

typedef struct{
    char titulo[100];
    int minutos;
}Musica;

typedef struct{
    char nome[100];
    ArvBB *musicas;
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

struct ArvBB{
    Info info;
    struct ArvBB *esq;
    struct ArvBB *dir;
};

ArvBB *criarNO(char *chave, int tipo, void *dado);

int inserirArvBB(ArvBB **raiz, char *chave, int tipo, void *dado);

ArvBB *buscarArvBB(ArvBB *raiz, char *chave);

int cadastrarArtista(ArvBB **raiz, Artista *artista);

int cadastrarAlbum(ArvBB *raizArtistas, char *nomeArtista, Album *album);

int cadastrarMusica(ArvBB *raizArtistas, char *nomeArtista, char *tituloAlbum, Musica *musica); 

void mostrarArtistas(ArvBB *raiz);

void exibirArtista(Artista artista);

void exibirAlbum(Album album);

void exibirMusica(Musica musica);

void exibirAlbunsAno(ArvBB *raizAlbuns, int ano);

void exibirMusicaEncontrada(ArvBB *raizMusicas, char *tituloMusica, Artista artista, Album album);

void procurarMusicaAlbuns(ArvBB *raizAlbuns, char *tituloMusica, Artista artista);

void procurarMusicaArtistas(ArvBB *raizArtistas, char *tituloMusica);

void mostrarDadosMusica(ArvBB *raizArtistas,char *tituloMusica);

#endif
