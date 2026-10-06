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

ArvBB *buscarArvBB(ArvBB *raiz, char *chave){
    ArvBB *resultado = NULL;
    int comparacao;

    if (raiz !=  NULL){
        comparacao = strcmp(chave, raiz->info.chave);

        if (comparacao == 0)
            resultado = raiz;
        
        else if (comparacao < 0)
            resultado = buscarArvBB(raiz->esq, chave);

        else 
            resultado = buscarArvBB(raiz->dir, chave);
    }
    return resultado;
}

int removerArvBB(ArvBB **raiz, char *chave){
    int resultado = 0, comparacao;
    ArvBB *aux;
    ArvBB *pai;

    if (*raiz != NULL)
    {
        comparacao = strcmp(chave, (*raiz)->info.chave);
        if (comparacao < 0)
            resultado = removerArvBB(&(*raiz)->esq, chave);

    }else if (comparacao > 0)
        resultado = removerArvBB(&(*raiz)->dir, chave);

    else if ((*raiz)->esq == NULL && (*raiz)->dir == NULL){
        free(*raiz);
        *raiz = NULL;
        resultado = 1;

    }else if ((*raiz)->esq == NULL){
        aux = *raiz;
        *raiz = (*raiz)->dir;
        free(aux);
        resultado = 1;

    }else if ((*raiz)->dir == NULL){
        aux = *raiz;
        *raiz = (*raiz)->esq;
        free(aux);
        resultado = 1;

    }else{
        pai = *raiz;
        aux = (*raiz)->dir;

        while (aux->esq != NULL)
        {
            pai = aux;
            aux = aux->esq;
        }
        (*raiz)->info = aux->info;
        if (pai == *raiz)
            pai->dir = aux->dir;
        else
            pai->esq = aux->dir;

        free(aux);
        resultado = 1;
    }
    return resultado;
}

int cadastrarArtista(ArvBB **raiz, Artista *artista){
    int resultado;

    resultado = inserirArvBB(raiz, artista->nome, TIPO_ARTISTA, artista);
    return resultado;
}

int cadastrarAlbum(ArvBB *raizArtistas, char *nomeArtista, Album *album){
    ArvBB *artistaEncontrado;
    int resultado = 0;

    artistaEncontrado = buscarArvBB(raizArtistas, nomeArtista);

    if (artistaEncontrado != NULL)
    {
        resultado = inserirArvBB(&artistaEncontrado->info.dado.artista.albuns, 
        album->titulo, TIPO_ALBUM, album);

        if (resultado)
            artistaEncontrado->info.dado.artista.numeroAlbuns++;
    }
    return resultado;
}

int cadastrarMusica(ArvBB *raizArtistas, char *nomeArtista, char *tituloAlbum, Musica *musica){
    ArvBB *artistaEncontrado;
    ArvBB *albumEncontrado;
    int resultado = 0;

    artistaEncontrado = buscarArvBB(raizArtistas, nomeArtista);

    if (artistaEncontrado != NULL){
        albumEncontrado = buscarArvBB(artistaEncontrado->info.dado.artista.albuns, tituloAlbum);

        if (albumEncontrado != NULL)
            resultado = inserirArvBB(&albumEncontrado->info.dado.album.musicas, musica->titulo, 
            TIPO_MUSICA, musica);

            if (resultado)
                albumEncontrado->info.dado.album.qtdMusicas++;      
    }
    return resultado;
}

void mostrarArtistas(ArvBB *raiz){
    if ( raiz != NULL)
    {
        mostrarArtistas(raiz->esq);
        exibirArtista(raiz->info.dado.artista);
        mostrarArtistas(raiz->dir);
    }
    
}

void exibirArtista(Artista artista){
    printf("\nNome: %s", artista.nome);
    printf("\nTipo: %s", artista.tipo);
    printf("\nEstilo: %s", artista.estilo);
    printf("\nNumero de albuns: %d\n", artista.numeroAlbuns);
}

void exibirAlbum(Album album) {
    printf("\nTitulo: %s", album.titulo);
    printf("\nAno: %d", album.ano);
    printf("\nQuantidade de musicas: %d\n",album.qtdMusicas);
}

void exibirMusica(Musica musica){
    printf("\nTitulo:  %s", musica.titulo);
    printf("\nDuracao:  %d minutos\n", musica.minutos);
}

void exibirAlbunsAno(ArvBB *raizAlbuns, int ano){
    if (raizAlbuns != NULL)
    {
        exibirAlbunsAno(raizAlbuns->esq, ano);
        
        if (raizAlbuns->info.dado.album.ano == ano)
            exibirAlbum(raizAlbuns->info.dado.album);

        exibirAlbunsAno(raizAlbuns->dir, ano);
    }
    
}

void exibirMusicaEncontrada(ArvBB *raizMusicas, char *tituloMusica, Artista artista, Album album){
    if (raizMusicas != NULL)
    {
        exibirMusicaEncontrada(raizMusicas->esq, tituloMusica, artista, album);

        if (strcmp(raizMusicas->info.dado.musica.titulo, tituloMusica) == 0)
        {
            exibirMusica(raizMusicas->info.dado.musica);
            printf("\nArtista: %s", artista.nome);
            printf("\nAlbum: %s", album.titulo);
            printf("\nAno de lançamento: %d", album.ano);
            printf("\nEstilo musical: %s", artista.estilo);
            printf("\nTipo de artista: %s\n", artista.tipo);
        }
        exibirMusicaEncontrada(raizMusicas->dir, tituloMusica, artista, album);
    }
    
}

void procurarMusicaAlbuns(ArvBB *raizAlbuns, char *tituloMusica, Artista artista){
    if (raizAlbuns != NULL)
    {
        procurarMusicaAlbuns(raizAlbuns->esq, tituloMusica, artista);
        exibirMusicaEncontrada(raizAlbuns->info.dado.album.musicas, tituloMusica, artista, raizAlbuns->info.dado.album);
        procurarMusicaAlbuns(raizAlbuns->dir, tituloMusica, artista);
    }
}

void procurarMusicaArtistas(ArvBB *raizArtistas, char *tituloMusica){
    if (raizArtistas != NULL)
    {
        procurarMusicaArtistas(raizArtistas->esq, tituloMusica);
        procurarMusicaAlbuns(raizArtistas->info.dado.artista.albuns, tituloMusica, raizArtistas->info.dado.artista);
        procurarMusicaArtistas(raizArtistas->dir, tituloMusica);
    }
}

void mostrarDadosMusica(ArvBB *raizArtistas,char *tituloMusica){
    procurarMusicaArtistas(raizArtistas, tituloMusica);
}

int cadastrarPlaylist(ArvBB **raizPlaylists, Playlist *playlist){
    int resultado;

    resultado = inserirArvBB(raizPlaylists, playlist->nome, TIPO_PLAYLIST, playlist);
    return resultado;
}

int cadastrarMusicaPlaylist(ArvBB *raizArtistas, ArvBB *raizPlaylists, char *nomePlaylist, char *nomeArtista, char *tituloAlbum, char *tituloMusica){
    ArvBB *playlistEncontrada;
    ArvBB *artistaEncontrado;
    ArvBB *albumEncontrado;
    ArvBB *musicaEncontrada;
    MusicaPlaylist musicaplaylsit;
    char chave[100];
    int resultado = 0;

    playlistEncontrada = buscarArvBB(raizPlaylists, nomePlaylist);
    artistaEncontrado = buscarArvBB(raizArtistas, nomeArtista);

    if (albumEncontrado != NULL)
    {
        musicaEncontrada = buscarArvBB(albumEncontrado->info.dado.album.musicas, tituloMusica);

        if (musicaEncontrada != NULL)
        {
            strcpy(musicaplaylsit.nomeArtista, nomeArtista);
            strcpy(musicaplaylsit.tituloAlbum, tituloAlbum);
            strcpy(musicaplaylsit.tituloMusica, tituloMusica);

            snprintf(chave, sizeof(chave), "%s - %s - %s", nomeArtista, tituloAlbum, tituloMusica);
            resultado = inserirArvBB(&playlistEncontrada->info.dado.playlist.musicas, chave, TIPO_MUSICA_PLAYLIST, &musicaplaylsit);
        }
        
    }
    return resultado;
}

void exibirMusicaPlaylist(MusicaPlaylist musicaPlaylist){
    printf("\nArtista: %s", musicaPlaylist.nomeArtista);
    printf("\nAlbum: %s", musicaPlaylist.tituloAlbum);
    printf("\nMusica: %s\n", musicaPlaylist.tituloMusica);
}

void mostrarMusicasPlaylist(ArvBB *raizMusicas){
    if (raizMusicas != NULL)
    {
        mostrarMusicasPlaylist(raizMusicas->esq);
        exibirMusicaPlaylist(raizMusicas->info.dado.musicaplaylist);
        mostrarMusicasPlaylist(raizMusicas->dir);
    }
}

void mostrarPlaylists(ArvBB *raizPlaylists){
    if (raizPlaylists != NULL)
    {
        mostrarPlaylists(raizPlaylists->esq);
        printf("\nPlaylist: %s\n", raizPlaylists->info.dado.playlist.nome);

        if (raizPlaylists->info.dado.playlist.musicas == NULL)
            printf("Nenhuma musica cadastrada.\n");
        else
            mostrarMusicasPlaylist(raizPlaylists->info.dado.playlist.musicas);

        mostrarPlaylists(raizPlaylists->dir);
    }
}

void mostrarDadosPlaylist(ArvBB *raizPlaylists, char *nomePlaylist){
    ArvBB *playlistEncontrada;
    playlistEncontrada = buscarArvBB(raizPlaylists, nomePlaylist);

    if (playlistEncontrada != NULL)
    {
        printf("\n===== DADOS DA PLAYLIST ====\n");
        printf("Nome: %s\n", playlistEncontrada->info.dado.playlist.nome);

        if (playlistEncontrada->info.dado.playlist.musicas == NULL)
            printf("Nenhuma musica cadastrada.\n");
        else
            mostrarMusicasPlaylist(playlistEncontrada->info.dado.playlist.musicas);
    }else
        printf("\nPlatlist nao encontrada.\n"); 
}

int removerMusicaPlaylist(ArvBB *raizPlaylists, char *nomePlaylist, char *nomeArtista, char *tituloAlbum, char *tituloMusica){
    ArvBB *playlistEncontrada;
    char chave[100];
    int resultado = 0;

    playlistEncontrada = buscarArvBB(raizPlaylists, nomePlaylist);

    if (playlistEncontrada != NULL)
    {
        snprintf(chave, sizeof(chave), "%s - %s - %s", nomeArtista, tituloAlbum, tituloMusica);
        resultado = removerArvBB(&playlistEncontrada->info.dado.playlist.musicas, chave);
    }
    return resultado;
}