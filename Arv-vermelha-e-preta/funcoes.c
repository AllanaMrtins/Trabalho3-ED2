#include "estruturas.h"


int ehVermelho(ArvVermelhaPreta *no) {
    int resultado = 0;
    if (no != NULL && no->cor == RED)
        resultado = 1;
    return resultado;
}

ArvVermelhaPreta *rotacaoEsquerda(ArvVermelhaPreta *raiz) {
    ArvVermelhaPreta *aux = raiz->dir;
    raiz->dir = aux->esq;
    aux->esq = raiz;
    aux->cor = raiz->cor;
    raiz->cor = RED;
    return aux;
}

ArvVermelhaPreta *rotacaoDireita(ArvVermelhaPreta *raiz) {
    ArvVermelhaPreta *aux = raiz->esq;
    raiz->esq = aux->dir;
    aux->dir = raiz;
    aux->cor = raiz->cor;
    raiz->cor = RED;
    return aux;
}

void inverterCores(ArvVermelhaPreta *raiz) {
    if (raiz != NULL) {
        raiz->cor = !raiz->cor;
        if (raiz->esq != NULL)
            raiz->esq->cor = !raiz->esq->cor;
        if (raiz->dir != NULL)
            raiz->dir->cor = !raiz->dir->cor;
    }
}

ArvVermelhaPreta *criarNO(char *chave, int tipo, void *dado) {
    ArvVermelhaPreta *novo = malloc(sizeof(ArvVermelhaPreta));

    if (novo != NULL) {
        strncpy(novo->info.chave, chave, sizeof(novo->info.chave) - 1);
        novo->info.chave[sizeof(novo->info.chave) - 1] = '\0';
        novo->info.tipo = tipo;

        if (tipo == TIPO_ARTISTA)
            novo->info.dado.artista = *(Artista *)dado;
        else if (tipo == TIPO_ALBUM)
            novo->info.dado.album = *(Album *)dado;
        else if (tipo == TIPO_MUSICA)
            novo->info.dado.musica = *(Musica *)dado;
        else if (tipo == TIPO_PLAYLIST)
            novo->info.dado.playlist = *(Playlist *)dado;
        else if (tipo == TIPO_MUSICA_PLAYLIST)
            novo->info.dado.musicaplaylist = *(MusicaPlaylist *)dado;
        else {
            free(novo);
            novo = NULL;
        }

        if (novo != NULL) {
            novo->esq = NULL;
            novo->dir = NULL;
            novo->cor = RED;
        }
    }
    return novo;
}

ArvVermelhaPreta *inserirArvVP(ArvVermelhaPreta *raiz, char *chave, int tipo, void *dado, int *resultado) {
    int comparacao;

    if (raiz == NULL) {
        raiz = criarNO(chave, tipo, dado);
        if (raiz != NULL)
            *resultado = 1;
    } else {
        comparacao = strcmp(chave, raiz->info.chave);

        if (comparacao < 0)
            raiz->esq = inserirArvVP(raiz->esq, chave, tipo, dado, resultado);
        else if (comparacao > 0)
            raiz->dir = inserirArvVP(raiz->dir, chave, tipo, dado, resultado);
        else
            *resultado = 0;

        if (ehVermelho(raiz->dir) && !ehVermelho(raiz->esq))
            raiz = rotacaoEsquerda(raiz);
        if (ehVermelho(raiz->esq) && ehVermelho(raiz->esq->esq))
            raiz = rotacaoDireita(raiz);
        if (ehVermelho(raiz->esq) && ehVermelho(raiz->dir))
            inverterCores(raiz);
    }
    return raiz;
}

int inserir(ArvVermelhaPreta **raiz, char *chave, int tipo, void *dado) {
    int resultado = 0;
    *raiz = inserirArvVP(*raiz, chave, tipo, dado, &resultado);
    if (*raiz != NULL)
        (*raiz)->cor = BLACK;
    return resultado;
}

ArvVermelhaPreta *buscarArvVP(ArvVermelhaPreta *raiz, char *chave) {
    ArvVermelhaPreta *resultado = NULL;
    int comparacao;

    if (raiz != NULL) {
        comparacao = strcmp(chave, raiz->info.chave);
        if (comparacao == 0)
            resultado = raiz;
        else if (comparacao < 0)
            resultado = buscarArvVP(raiz->esq, chave);
        else
            resultado = buscarArvVP(raiz->dir, chave);
    }
    return resultado;
}

ArvVermelhaPreta *menorNo(ArvVermelhaPreta *raiz) {
    ArvVermelhaPreta *resultado = raiz;
    while (resultado != NULL && resultado->esq != NULL)
        resultado = resultado->esq;
    return resultado;
}

static ArvVermelhaPreta *moverVermelhoEsquerda(ArvVermelhaPreta *raiz) {
    inverterCores(raiz);
    if (raiz->dir != NULL && ehVermelho(raiz->dir->esq)) {
        raiz->dir = rotacaoDireita(raiz->dir);
        raiz = rotacaoEsquerda(raiz);
        inverterCores(raiz);
    }
    return raiz;
}

static ArvVermelhaPreta *moverVermelhoDireita(ArvVermelhaPreta *raiz) {
    inverterCores(raiz);
    if (raiz->esq != NULL && ehVermelho(raiz->esq->esq)) {
        raiz = rotacaoDireita(raiz);
        inverterCores(raiz);
    }
    return raiz;
}

static ArvVermelhaPreta *balancear(ArvVermelhaPreta *raiz) {
    if (ehVermelho(raiz->dir))
        raiz = rotacaoEsquerda(raiz);
    if (ehVermelho(raiz->esq) && ehVermelho(raiz->esq->esq))
        raiz = rotacaoDireita(raiz);
    if (ehVermelho(raiz->esq) && ehVermelho(raiz->dir))
        inverterCores(raiz);
    return raiz;
}

static ArvVermelhaPreta *removerMenor(ArvVermelhaPreta *raiz) {
    if (raiz->esq == NULL) {
        free(raiz);
        return NULL;
    }
    if (!ehVermelho(raiz->esq) && !ehVermelho(raiz->esq->esq))
        raiz = moverVermelhoEsquerda(raiz);
    raiz->esq = removerMenor(raiz->esq);
    return balancear(raiz);
}

static ArvVermelhaPreta *removerChave(ArvVermelhaPreta *raiz, char *chave) {
    int comparacao;

    if (strcmp(chave, raiz->info.chave) < 0) {
        if (raiz->esq != NULL) {
            if (!ehVermelho(raiz->esq) && !ehVermelho(raiz->esq->esq))
                raiz = moverVermelhoEsquerda(raiz);
            raiz->esq = removerChave(raiz->esq, chave);
        }
    } else {
        if (ehVermelho(raiz->esq))
            raiz = rotacaoDireita(raiz);

        comparacao = strcmp(chave, raiz->info.chave);
        if (comparacao == 0 && raiz->dir == NULL) {
            free(raiz);
            return NULL;
        }

        if (raiz->dir != NULL) {
            if (!ehVermelho(raiz->dir) && !ehVermelho(raiz->dir->esq))
                raiz = moverVermelhoDireita(raiz);

            comparacao = strcmp(chave, raiz->info.chave);
            if (comparacao == 0) {
                ArvVermelhaPreta *menor = menorNo(raiz->dir);
                raiz->info = menor->info;
                raiz->dir = removerMenor(raiz->dir);
            } else
                raiz->dir = removerChave(raiz->dir, chave);
        }
    }
    return balancear(raiz);
}

ArvVermelhaPreta *removerArvVP(ArvVermelhaPreta *raiz, char *chave, int *resultado) {
    if (buscarArvVP(raiz, chave) != NULL) {
        *resultado = 1;
        if (!ehVermelho(raiz->esq) && !ehVermelho(raiz->dir))
            raiz->cor = RED;
        raiz = removerChave(raiz, chave);
        if (raiz != NULL)
            raiz->cor = BLACK;
    } else
        *resultado = 0;
    return raiz;
}


int cadastrarArtista(ArvVermelhaPreta **raiz, Artista *artista) {
    return inserir(raiz, artista->nome, TIPO_ARTISTA, artista);
}

int cadastrarAlbum(ArvVermelhaPreta *raizArtistas, char *nomeArtista, Album *album) {
    ArvVermelhaPreta *artistaEncontrado = buscarArvVP(raizArtistas, nomeArtista);
    int resultado = 0;

    if (artistaEncontrado != NULL) {
        resultado = inserir(&artistaEncontrado->info.dado.artista.albuns, album->titulo, TIPO_ALBUM, album);
        if (resultado)
            artistaEncontrado->info.dado.artista.numeroAlbuns++;
    }
    return resultado;
}

int cadastrarMusica(ArvVermelhaPreta *raizArtistas, char *nomeArtista, char *tituloAlbum, Musica *musica) {
    ArvVermelhaPreta *artistaEncontrado = buscarArvVP(raizArtistas, nomeArtista);
    ArvVermelhaPreta *albumEncontrado;
    int resultado = 0;

    if (artistaEncontrado != NULL) {
        albumEncontrado = buscarArvVP(artistaEncontrado->info.dado.artista.albuns, tituloAlbum);
        if (albumEncontrado != NULL) {
            resultado = inserir(&albumEncontrado->info.dado.album.musicas, musica->titulo, TIPO_MUSICA, musica);
            if (resultado)
                albumEncontrado->info.dado.album.qtdMusicas++;
        }
    }
    return resultado;
}

void exibirArtista(Artista artista) {
    printf("\nNome: %s", artista.nome);
    printf("\nTipo: %s", artista.tipo);
    printf("\nEstilo: %s", artista.estilo);
    printf("\nNumero de albuns: %d\n", artista.numeroAlbuns);
}

void exibirAlbum(Album album) {
    printf("\nTitulo: %s", album.titulo);
    printf("\nAno: %d", album.ano);
    printf("\nQuantidade de musicas: %d\n", album.qtdMusicas);
}

void exibirMusica(Musica musica) {
    printf("\nTitulo: %s", musica.titulo);
    printf("\nDuracao: %.2f minutos\n", musica.minutos);
}

void mostrarArtistas(ArvVermelhaPreta *raiz) {
    if (raiz != NULL) {
        mostrarArtistas(raiz->esq);
        exibirArtista(raiz->info.dado.artista);
        mostrarArtistas(raiz->dir);
    }
}

void exibirAlbunsAno(ArvVermelhaPreta *raizAlbuns, int ano) {
    if (raizAlbuns != NULL) {
        exibirAlbunsAno(raizAlbuns->esq, ano);
        if (raizAlbuns->info.dado.album.ano == ano)
            exibirAlbum(raizAlbuns->info.dado.album);
        exibirAlbunsAno(raizAlbuns->dir, ano);
    }
}

void exibirMusicaEncontrada(ArvVermelhaPreta *raizMusicas, char *tituloMusica, Artista artista, Album album) {
    if (raizMusicas != NULL) {
        exibirMusicaEncontrada(raizMusicas->esq, tituloMusica, artista, album);
        if (strcmp(raizMusicas->info.dado.musica.titulo, tituloMusica) == 0) {
            exibirMusica(raizMusicas->info.dado.musica);
            printf("\nArtista: %s", artista.nome);
            printf("\nAlbum: %s", album.titulo);
            printf("\nAno de lancamento: %d", album.ano);
            printf("\nEstilo musical: %s", artista.estilo);
            printf("\nTipo de artista: %s\n", artista.tipo);
        }
        exibirMusicaEncontrada(raizMusicas->dir, tituloMusica, artista, album);
    }
}

void procurarMusicaAlbuns(ArvVermelhaPreta *raizAlbuns, char *tituloMusica, Artista artista) {
    if (raizAlbuns != NULL) {
        procurarMusicaAlbuns(raizAlbuns->esq, tituloMusica, artista);
        exibirMusicaEncontrada(raizAlbuns->info.dado.album.musicas, tituloMusica, artista, raizAlbuns->info.dado.album);
        procurarMusicaAlbuns(raizAlbuns->dir, tituloMusica, artista);
    }
}

void procurarMusicaArtistas(ArvVermelhaPreta *raizArtistas, char *tituloMusica) {
    if (raizArtistas != NULL) {
        procurarMusicaArtistas(raizArtistas->esq, tituloMusica);
        procurarMusicaAlbuns(raizArtistas->info.dado.artista.albuns, tituloMusica, raizArtistas->info.dado.artista);
        procurarMusicaArtistas(raizArtistas->dir, tituloMusica);
    }
}

void mostrarDadosMusica(ArvVermelhaPreta *raizArtistas, char *tituloMusica) {
    procurarMusicaArtistas(raizArtistas, tituloMusica);
}

int cadastrarPlaylist(ArvVermelhaPreta **raizPlaylists, Playlist *playlist) {
    return inserir(raizPlaylists, playlist->nome, TIPO_PLAYLIST, playlist);
}

int cadastrarMusicaPlaylist(ArvVermelhaPreta *raizArtistas, ArvVermelhaPreta *raizPlaylists, char *nomePlaylist, char *nomeArtista, char *tituloAlbum, char *tituloMusica) {
    ArvVermelhaPreta *playlistEncontrada = buscarArvVP(raizPlaylists, nomePlaylist);
    ArvVermelhaPreta *artistaEncontrado;
    ArvVermelhaPreta *albumEncontrado;
    ArvVermelhaPreta *musicaEncontrada;
    MusicaPlaylist musicaPlaylist;
    char chave[310];
    int resultado = 0;

    if (playlistEncontrada != NULL) {
        artistaEncontrado = buscarArvVP(raizArtistas, nomeArtista);
        if (artistaEncontrado != NULL) {
            albumEncontrado = buscarArvVP(artistaEncontrado->info.dado.artista.albuns, tituloAlbum);
            if (albumEncontrado != NULL) {
                musicaEncontrada = buscarArvVP(albumEncontrado->info.dado.album.musicas, tituloMusica);
                if (musicaEncontrada != NULL) {
                    snprintf(musicaPlaylist.nomeArtista, sizeof(musicaPlaylist.nomeArtista), "%s", nomeArtista);
                    snprintf(musicaPlaylist.tituloAlbum, sizeof(musicaPlaylist.tituloAlbum), "%s", tituloAlbum);
                    snprintf(musicaPlaylist.tituloMusica, sizeof(musicaPlaylist.tituloMusica), "%s", tituloMusica);
                    snprintf(chave, sizeof(chave), "%s - %s - %s", nomeArtista, tituloAlbum, tituloMusica);
                    resultado = inserir(&playlistEncontrada->info.dado.playlist.musicas, chave, TIPO_MUSICA_PLAYLIST, &musicaPlaylist);
                }
            }
        }
    }
    return resultado;
}

void exibirMusicaPlaylist(MusicaPlaylist musicaPlaylist) {
    printf("\nArtista: %s", musicaPlaylist.nomeArtista);
    printf("\nAlbum: %s", musicaPlaylist.tituloAlbum);
    printf("\nMusica: %s\n", musicaPlaylist.tituloMusica);
}

void mostrarMusicasPlaylist(ArvVermelhaPreta *raizMusicas) {
    if (raizMusicas != NULL) {
        mostrarMusicasPlaylist(raizMusicas->esq);
        exibirMusicaPlaylist(raizMusicas->info.dado.musicaplaylist);
        mostrarMusicasPlaylist(raizMusicas->dir);
    }
}

void mostrarDadosPlaylist(ArvVermelhaPreta *raizPlaylists, char *nomePlaylist) {
    ArvVermelhaPreta *playlistEncontrada = buscarArvVP(raizPlaylists, nomePlaylist);

    if (playlistEncontrada != NULL) {
        printf("\n===== DADOS DA PLAYLIST =====\n");
        printf("Nome: %s\n", playlistEncontrada->info.dado.playlist.nome);
        if (playlistEncontrada->info.dado.playlist.musicas == NULL)
            printf("Nenhuma musica cadastrada.\n");
        else
            mostrarMusicasPlaylist(playlistEncontrada->info.dado.playlist.musicas);
    } else
        printf("\nPlaylist nao encontrada.\n");
}

int removerMusicaPlaylist(ArvVermelhaPreta *raizPlaylists, char *nomePlaylist, char *nomeArtista, char *tituloAlbum, char *tituloMusica) {
    ArvVermelhaPreta *playlistEncontrada = buscarArvVP(raizPlaylists, nomePlaylist);
    char chave[310];
    int resultado = 0;

    if (playlistEncontrada != NULL) {
        snprintf(chave, sizeof(chave), "%s - %s - %s", nomeArtista, tituloAlbum, tituloMusica);
        playlistEncontrada->info.dado.playlist.musicas = removerArvVP(playlistEncontrada->info.dado.playlist.musicas, chave, &resultado);
    }
    return resultado;
}

int musicaEstaEmPlaylist(ArvVermelhaPreta *raizPlaylists, char *nomeArtista, char *tituloAlbum, char *tituloMusica) {
    char chave[310];
    int resultado = 0;

    if (raizPlaylists != NULL) {
        snprintf(chave, sizeof(chave), "%s - %s - %s", nomeArtista, tituloAlbum, tituloMusica);
        if (buscarArvVP(raizPlaylists->info.dado.playlist.musicas, chave) != NULL)
            resultado = 1;
        else {
            resultado = musicaEstaEmPlaylist(raizPlaylists->esq, nomeArtista, tituloAlbum, tituloMusica);
            if (resultado == 0)
                resultado = musicaEstaEmPlaylist(raizPlaylists->dir, nomeArtista, tituloAlbum, tituloMusica);
        }
    }
    return resultado;
}

int removerMusicaAlbum(ArvVermelhaPreta *raizArtistas, ArvVermelhaPreta *raizPlaylists, char *nomeArtista, char *tituloAlbum, char *tituloMusica) {
    ArvVermelhaPreta *artistaEncontrado = buscarArvVP(raizArtistas, nomeArtista);
    ArvVermelhaPreta *albumEncontrado;
    int resultado = 0;

    if (artistaEncontrado != NULL) {
        albumEncontrado = buscarArvVP(artistaEncontrado->info.dado.artista.albuns, tituloAlbum);
        if (albumEncontrado != NULL) {
            if (musicaEstaEmPlaylist(raizPlaylists, nomeArtista, tituloAlbum, tituloMusica) == 0) {
                albumEncontrado->info.dado.album.musicas = removerArvVP(albumEncontrado->info.dado.album.musicas, tituloMusica, &resultado);
                if (resultado)
                    albumEncontrado->info.dado.album.qtdMusicas--;
            } else {
                printf("\nA musica faz parte de uma playlist.");
                printf("\nNao pode ser removida do album.\n");
            }
        }
    }
    return resultado;
}
