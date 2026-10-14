#ifndef VERMELHO_PRETA_H
#define VERMELHO_PRETA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define RED 1
#define BLACK 0

#define TIPO_ARTISTA         1
#define TIPO_ALBUM           2
#define TIPO_MUSICA          3
#define TIPO_PLAYLIST        4
#define TIPO_MUSICA_PLAYLIST 5

typedef struct ArvVermelhaPreta ArvVermelhaPreta;

typedef struct {
    char nome[30];
    char tipo[30];
    char estilo[50];
    int numeroAlbuns;
    ArvVermelhaPreta *albuns;
} Artista;

typedef struct {
    char titulo[100];
    int ano;
    int qtdMusicas;
    ArvVermelhaPreta *musicas;
} Album;

typedef struct {
    char titulo[100];
    float minutos;
} Musica;

typedef struct {
    char nome[100];
    ArvVermelhaPreta *musicas;
} Playlist;

typedef struct {
    char nomeArtista[100];
    char tituloAlbum[100];
    char tituloMusica[100];
} MusicaPlaylist;

typedef union {
    Artista artista;
    Album album;
    Musica musica;
    Playlist playlist;
    MusicaPlaylist musicaplaylist;
} Dado;

typedef struct {
    char chave[310];
    int tipo;
    Dado dado;
} Info;

struct ArvVermelhaPreta {
    Info info;
    struct ArvVermelhaPreta *esq;
    struct ArvVermelhaPreta *dir;
    int cor;
};


int ehVermelho(ArvVermelhaPreta *no);

ArvVermelhaPreta *rotacaoEsquerda(ArvVermelhaPreta *raiz);

ArvVermelhaPreta *rotacaoDireita(ArvVermelhaPreta *raiz);

void inverterCores(ArvVermelhaPreta *raiz);

ArvVermelhaPreta *criarNO(char *chave, int tipo, void *dado);

ArvVermelhaPreta *inserirArvVP(ArvVermelhaPreta *raiz, char *chave, int tipo, void *dado, int *resultado);

int inserir(ArvVermelhaPreta **raiz, char *chave, int tipo, void *dado);

ArvVermelhaPreta *buscarArvVP(ArvVermelhaPreta *raiz, char *chave);

ArvVermelhaPreta *menorNo(ArvVermelhaPreta *raiz);

ArvVermelhaPreta *removerArvVP(ArvVermelhaPreta *raiz, char *chave, int *resultado);

int cadastrarArtista(ArvVermelhaPreta **raiz, Artista *artista);

int cadastrarAlbum(ArvVermelhaPreta *raizArtistas, char *nomeArtista, Album *album);

int cadastrarMusica(ArvVermelhaPreta *raizArtistas, char *nomeArtista, char *tituloAlbum, Musica *musica);

int cadastrarPlaylist(ArvVermelhaPreta **raizPlaylists, Playlist *playlist);

int cadastrarMusicaPlaylist(ArvVermelhaPreta *raizArtistas, ArvVermelhaPreta *raizPlaylists, char *nomePlaylist, char *nomeArtista, char *tituloAlbum, char *tituloMusica);

void exibirArtista(Artista artista);

void exibirAlbum(Album album);

void exibirMusica(Musica musica);

void mostrarArtistas(ArvVermelhaPreta *raiz);

void exibirAlbunsAno(ArvVermelhaPreta *raizAlbuns, int ano);

void exibirMusicaEncontrada(ArvVermelhaPreta *raizMusicas, char *tituloMusica, Artista artista, Album album);

void procurarMusicaAlbuns(ArvVermelhaPreta *raizAlbuns, char *tituloMusica, Artista artista);

void procurarMusicaArtistas(ArvVermelhaPreta *raizArtistas, char *tituloMusica);

void mostrarDadosMusica(ArvVermelhaPreta *raizArtistas, char *tituloMusica);

void exibirMusicaPlaylist(MusicaPlaylist musicaPlaylist);

void mostrarMusicasPlaylist(ArvVermelhaPreta *raizMusicas);

void mostrarDadosPlaylist(ArvVermelhaPreta *raizPlaylists, char *nomePlaylist);

int removerMusicaPlaylist(ArvVermelhaPreta *raizPlaylists, char *nomePlaylist, char *nomeArtista, char *tituloAlbum, char *tituloMusica);

int musicaEstaEmPlaylist(ArvVermelhaPreta *raizPlaylists, char *nomeArtista, char *tituloAlbum, char *tituloMusica);

int removerMusicaAlbum(ArvVermelhaPreta *raizArtistas, ArvVermelhaPreta *raizPlaylists, char *nomeArtista, char *tituloAlbum, char *tituloMusica);

#endif