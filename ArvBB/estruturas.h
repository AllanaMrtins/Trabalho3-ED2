#ifndef ARVORE_BINARIA_BUSCA
#define ARVORE_BINARIA_BUSCA

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

typedef struct ArvBB ArBB;

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

#endif