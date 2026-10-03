#ifndef VERMELHO_PRETA_H
#define VERMELHO_PRETA_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

#define RED 1
#define BLACK 0

typedef struct ArvVermelhaPreta ArvVermelhaPreta;

typedef struct{
    char nome[30];
    char tipo[30];
    char estilo[50];
    int numeroAlbuns;
    ArvVermelhaPreta *albuns;
}Artista;

typedef struct{
    char titulo[100];
    int ano;
    int qtdMusicas;
    ArvVermelhaPreta *musicas;
}Album;

typedef struct{
    char titulo[100];
    int minutos;
}Musica;

typedef struct{
    char nome[100];
    ArvVermelhaPreta *musicas;
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

struct ArvVermelhaPreta
{
    Info info;
    struct ArvVermelhaPreta *esq;
    struct ArvVermelhaPreta *dir;
    int cor;
};

#endif