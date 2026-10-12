#include "estruturas.h"

/* =========================================================
   FUNÇÕES BÁSICAS DA AVL
   ========================================================= */

int altura(ArvAVL *raiz){
    int resultado;

    if (raiz == NULL)
        resultado = 0;
    else
        resultado = raiz->altura;

    return resultado;
}


int maior(int a, int b){
    int resultado;

    if (a > b)
        resultado = a;
    else
        resultado = b;

    return resultado;
}


void atualizarAltura(ArvAVL *raiz){
    if (raiz != NULL)
        raiz->altura = 1 + maior(altura(raiz->esq),altura(raiz->dir)); 
}


int fatorBalanceamento(ArvAVL *raiz){
    int resultado;

    if (raiz == NULL)
        resultado = 0;
    else
        resultado = altura(raiz->esq) - altura(raiz->dir);

    return resultado;
}

/* =========================================================
   ROTAÇÃO À DIREITA
   ========================================================= */

ArvAVL *rotacaoDireita(ArvAVL *raiz){
    ArvAVL *novaRaiz;
    ArvAVL *aux;

    novaRaiz = raiz->esq;
    aux = novaRaiz->dir;

    novaRaiz->dir = raiz;
    raiz->esq = aux;

    atualizarAltura(raiz);
    atualizarAltura(novaRaiz);

    return novaRaiz;
}

/* =========================================================
   ROTAÇÃO À ESQUERDA
   ========================================================= */

ArvAVL *rotacaoEsquerda(ArvAVL *raiz){
    ArvAVL *novaRaiz;
    ArvAVL *aux;

    novaRaiz = raiz->dir;
    aux = novaRaiz->esq;

    novaRaiz->esq = raiz;
    raiz->dir = aux;

    atualizarAltura(raiz);
    atualizarAltura(novaRaiz);

    return novaRaiz;
}

/* =========================================================
   CRIAR NÓ
   ========================================================= */

ArvAVL *criarNO(char *chave,int tipo,void *dado){
    ArvAVL *novo;

    novo = (ArvAVL *)malloc(sizeof(ArvAVL));

    if (novo != NULL){
        strncpy(novo->info.chave,chave,sizeof(novo->info.chave) - 1);

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
    
        else{
            free(novo);
            novo = NULL;
        }

        if (novo != NULL){
            novo->altura = 1;
            novo->esq = NULL;
            novo->dir = NULL;
        }
    }

    return novo;
}

/* =========================================================
   INSERÇÃO AVL
   ========================================================= */

ArvAVL *inserirArvAVL(ArvAVL *raiz,char *chave,int tipo,void *dado,int *resultado){
    int comparacao;
    int fator;

    if (raiz == NULL){
        raiz = criarNO(chave,tipo,dado);

        if (raiz != NULL)
            *resultado = 1;
    }
    else{
        comparacao = strcmp(chave,raiz->info.chave);

        if (comparacao < 0)
            raiz->esq = inserirArvAVL(raiz->esq,chave,tipo,dado,resultado);
        
        else if (comparacao > 0)
            raiz->dir = inserirArvAVL(raiz->dir,chave,tipo,dado,resultado);
                
        else *resultado = 0;
        

        if (comparacao != 0){
            atualizarAltura(raiz);

            fator = fatorBalanceamento(raiz);

            if (fator > 1 && strcmp(chave,raiz->esq->info.chave) < 0)
                raiz = rotacaoDireita(raiz);
  
            else if (fator < -1 && strcmp(chave,raiz->dir->info.chave) > 0)
                raiz = rotacaoEsquerda(raiz);

            else if (fator > 1 && strcmp(chave,raiz->esq->info.chave) > 0){
                raiz->esq = rotacaoEsquerda(raiz->esq);
                raiz = rotacaoDireita(raiz);
            }

            else if (fator < -1 && strcmp(chave,raiz->dir->info.chave) < 0){
                raiz->dir = rotacaoDireita(raiz->dir);
                raiz = rotacaoEsquerda(raiz);
            }
        }
    }

    return raiz;
}

/* =========================================================
   INSERIR
   ========================================================= */

int inserir(ArvAVL **raiz,char *chave,int tipo,void *dado){
    int resultado;

    resultado = 0;

    *raiz = inserirArvAVL(*raiz,chave,tipo,dado,&resultado);

    return resultado;
}

/* =========================================================
   BUSCA
   ========================================================= */

ArvAVL *buscarArvAVL(ArvAVL *raiz,char *chave){
    ArvAVL *resultado;
    int comparacao;

    resultado = NULL;

    if (raiz != NULL){
        comparacao = strcmp(chave, raiz->info.chave);
           
        if (comparacao == 0)
            resultado = raiz;

        else if (comparacao < 0)
            resultado = buscarArvAVL(raiz->esq,chave);

        else
            resultado = buscarArvAVL(raiz->dir,chave);
    }

    return resultado;
}

/* =========================================================
   MENOR NÓ
   ========================================================= */

ArvAVL *menorNo(ArvAVL *raiz){
    ArvAVL *resultado;

    resultado = raiz;

    if (resultado != NULL){
        while (resultado->esq != NULL)
            resultado = resultado->esq;
    }

    return resultado;
}

/* =========================================================
   REMOÇÃO AVL
   ========================================================= */

ArvAVL *removerArvAVL(ArvAVL *raiz,char *chave,int *resultado){
    int comparacao;
    int fator;
    ArvAVL *aux;

    if (raiz != NULL){
        comparacao = strcmp(chave,raiz->info.chave);
   
        if (comparacao < 0)
            raiz->esq = removerArvAVL(raiz->esq,chave,resultado);
            
        else if (comparacao > 0)
            raiz->dir = removerArvAVL(raiz->dir,chave,resultado);
            
        else{
            *resultado = 1;

            if (raiz->esq == NULL){
                aux = raiz->dir;
                free(raiz);
                raiz = aux;
            }

            else if (raiz->dir == NULL){
                aux = raiz->esq;
                free(raiz);
                raiz = aux;
            }

            else{
                aux = menorNo( raiz->dir);
                raiz->info = aux->info;
                raiz->dir = removerArvAVL(raiz->dir, aux->info.chave, resultado);
            }
        }

        if (raiz != NULL){
            atualizarAltura(raiz);

            fator = fatorBalanceamento(raiz);

            if (fator > 1 && fatorBalanceamento(raiz->esq) >= 0)
                raiz = rotacaoDireita(raiz);

            else if (fator > 1 && fatorBalanceamento(raiz->esq) < 0){
                raiz->esq = rotacaoEsquerda(raiz->esq);
                raiz = rotacaoDireita(raiz);
            }

            else if (fator < -1 && fatorBalanceamento(raiz->dir) <= 0)
                raiz = rotacaoEsquerda(raiz);

            else if (fator < -1 && fatorBalanceamento(raiz->dir) > 0){
                raiz->dir = rotacaoDireita(raiz->dir);
                raiz = rotacaoEsquerda(raiz);
            }
        }
    }

    return raiz;
}

/* =========================================================
   CADASTRAR ARTISTA
   ========================================================= */

int cadastrarArtista(ArvAVL **raiz,Artista *artista){
    int resultado;
    resultado = inserir(raiz,artista->nome, TIPO_ARTISTA,artista);

    return resultado;
}

/* =========================================================
   CADASTRAR ÁLBUM
   ========================================================= */

int cadastrarAlbum(ArvAVL *raizArtistas,char *nomeArtista,Album *album){
    ArvAVL *artistaEncontrado;
    int resultado;
    resultado = 0;

    artistaEncontrado = buscarArvAVL(raizArtistas,nomeArtista);

    if (artistaEncontrado != NULL){
        resultado = inserir(&artistaEncontrado->info.dado.artista.albuns,album->titulo,TIPO_ALBUM,album);

        if (resultado)
            artistaEncontrado->info.dado.artista.numeroAlbuns++;       
    }

    return resultado;
}

/* =========================================================
   CADASTRAR MÚSICA
   ========================================================= */

int cadastrarMusica(ArvAVL *raizArtistas,char *nomeArtista,char *tituloAlbum,Musica *musica){
    ArvAVL *artistaEncontrado;
    ArvAVL *albumEncontrado;
    int resultado;

    resultado = 0;

    artistaEncontrado = buscarArvAVL(raizArtistas,nomeArtista);

    if (artistaEncontrado != NULL){
        albumEncontrado = buscarArvAVL(artistaEncontrado->info.dado.artista.albuns,tituloAlbum);

        if (albumEncontrado != NULL){
            resultado = inserir(&albumEncontrado->info.dado.album.musicas,musica->titulo,TIPO_MUSICA,musica);

            if (resultado)
                albumEncontrado->info.dado.album.qtdMusicas++;
        }
    }

    return resultado;
}

/* =========================================================
   EXIBIR ARTISTA
   ========================================================= */

void exibirArtista(Artista artista){
    printf("\nNome: %s", artista.nome);
    printf("\nTipo: %s", artista.tipo);
    printf("\nEstilo: %s", artista.estilo);
    printf("\nNumero de albuns: %d\n", artista.numeroAlbuns);
}

/* =========================================================
   EXIBIR ÁLBUM
   ========================================================= */

void exibirAlbum(Album album){
    printf("\nTitulo: %s", album.titulo);
    printf("\nAno: %d", album.ano);
    printf("\nQuantidade de musicas: %d\n", album.qtdMusicas);
}

/* =========================================================
   EXIBIR MÚSICA
   ========================================================= */

void exibirMusica(Musica musica){
    printf("\nTitulo: %s", musica.titulo);
    printf("\nDuracao: %.2f minutos\n",musica.minutos);
}

/* =========================================================
   IV - MOSTRAR ARTISTAS
   ========================================================= */

void mostrarArtistas(ArvAVL *raiz){
    if (raiz != NULL){
        mostrarArtistas(raiz->esq);
        exibirArtista(raiz->info.dado.artista);
        mostrarArtistas(raiz->dir);
    }
}

/* =========================================================
   V - MOSTRAR ÁLBUNS DE DETERMINADO ANO
   ========================================================= */

void exibirAlbunsAno(ArvAVL *raizAlbuns,int ano){
    if (raizAlbuns != NULL){
        exibirAlbunsAno(raizAlbuns->esq,ano);

        if (raizAlbuns->info.dado.album.ano == ano)
            exibirAlbum(raizAlbuns->info.dado.album);

        exibirAlbunsAno(raizAlbuns->dir,ano);
    }
}

/* =========================================================
   VI - EXIBIR MÚSICA ENCONTRADA
   ========================================================= */

void exibirMusicaEncontrada(ArvAVL *raizMusicas,char *tituloMusica,Artista artista,Album album){
    if (raizMusicas != NULL){
        exibirMusicaEncontrada(raizMusicas->esq,tituloMusica,artista,album);

        if (strcmp(raizMusicas->info.dado.musica.titulo,tituloMusica) == 0){
            exibirMusica(raizMusicas->info.dado.musica);

            printf("\nArtista: %s",artista.nome);
            printf("\nAlbum: %s",album.titulo);
            printf("\nAno de lancamento: %d",album.ano);
            printf("\nEstilo musical: %s",artista.estilo);
            printf("\nTipo de artista: %s\n",artista.tipo);
        }

        exibirMusicaEncontrada(raizMusicas->dir,tituloMusica,artista,album);
    }
}

/* =========================================================
   PROCURAR MÚSICA NOS ÁLBUNS
   ========================================================= */

void procurarMusicaAlbuns(ArvAVL *raizAlbuns,char *tituloMusica,Artista artista){
    if (raizAlbuns != NULL){
        procurarMusicaAlbuns(raizAlbuns->esq,tituloMusica,artista);
        exibirMusicaEncontrada(raizAlbuns->info.dado.album.musicas,tituloMusica,artista,raizAlbuns->info.dado.album);
        procurarMusicaAlbuns(raizAlbuns->dir,tituloMusica,artista);
    }
}

/* =========================================================
   PROCURAR MÚSICA NOS ARTISTAS
   ========================================================= */

void procurarMusicaArtistas(ArvAVL *raizArtistas,char *tituloMusica){
    if (raizArtistas != NULL){
        procurarMusicaArtistas(raizArtistas->esq,tituloMusica);
        procurarMusicaAlbuns(raizArtistas->info.dado.artista.albuns,tituloMusica,raizArtistas->info.dado.artista);
        procurarMusicaArtistas(raizArtistas->dir,tituloMusica);
    }
}

/* =========================================================
   MOSTRAR DADOS DA MÚSICA
   ========================================================= */

void mostrarDadosMusica(ArvAVL *raizArtistas,char *tituloMusica){
    procurarMusicaArtistas(raizArtistas,tituloMusica);
}

/* =========================================================
   VII - CADASTRAR PLAYLIST
   ========================================================= */

int cadastrarPlaylist(ArvAVL **raizPlaylists,Playlist *playlist){
    int resultado;
    resultado = inserir(raizPlaylists,playlist->nome,TIPO_PLAYLIST,playlist);

    return resultado;
}

/* =========================================================
   CADASTRAR MÚSICA NA PLAYLIST
   ========================================================= */

int cadastrarMusicaPlaylist(ArvAVL *raizArtistas,ArvAVL *raizPlaylists,char *nomePlaylist,char *nomeArtista,char *tituloAlbum,char *tituloMusica){
    ArvAVL *playlistEncontrada;
    ArvAVL *artistaEncontrado;
    ArvAVL *albumEncontrado;
    ArvAVL *musicaEncontrada;

    MusicaPlaylist musicaPlaylist;

    char chave[100];

    int resultado;
    resultado = 0;

    playlistEncontrada = buscarArvAVL(raizPlaylists,nomePlaylist);

    if (playlistEncontrada != NULL){
        artistaEncontrado =buscarArvAVL(raizArtistas,nomeArtista);

        if (artistaEncontrado != NULL){
            albumEncontrado = buscarArvAVL(artistaEncontrado->info.dado.artista.albuns,tituloAlbum);  

            if (albumEncontrado != NULL){
                musicaEncontrada = buscarArvAVL(albumEncontrado-> info.dado.album.musicas,tituloMusica);
                    
                if (musicaEncontrada != NULL){
                    strcpy(musicaPlaylist.nomeArtista,nomeArtista);
                    strcpy(musicaPlaylist.tituloAlbum,tituloAlbum);
                    strcpy(musicaPlaylist.tituloMusica,tituloMusica);

                    snprintf(chave,sizeof(chave),"%s - %s - %s",nomeArtista,tituloAlbum,tituloMusica);

                    resultado = inserir(&playlistEncontrada->info.dado.playlist.musicas,chave,TIPO_MUSICA_PLAYLIST,&musicaPlaylist);
                }
            }
        }
    }

    return resultado;
}

/* =========================================================
   EXIBIR MÚSICA DA PLAYLIST
   ========================================================= */

void exibirMusicaPlaylist(MusicaPlaylist musicaPlaylist){
    printf("\nArtista: %s",musicaPlaylist.nomeArtista);
    printf("\nAlbum: %s",musicaPlaylist.tituloAlbum);
    printf("\nMusica: %s\n",musicaPlaylist.tituloMusica);
}

/* =========================================================
   MOSTRAR MÚSICAS DA PLAYLIST
   ========================================================= */

void mostrarMusicasPlaylist(ArvAVL *raizMusicas){
    if (raizMusicas != NULL){
        mostrarMusicasPlaylist(raizMusicas->esq);
        exibirMusicaPlaylist(raizMusicas->info.dado.musicaplaylist);
        mostrarMusicasPlaylist(raizMusicas->dir);
    }
}

/* =========================================================
   VIII - MOSTRAR PLAYLIST
   ========================================================= */

void mostrarDadosPlaylist(ArvAVL *raizPlaylists,char *nomePlaylist){
    ArvAVL *playlistEncontrada;

    playlistEncontrada = buscarArvAVL(raizPlaylists,nomePlaylist);

    if (playlistEncontrada != NULL){
        printf("\n===== DADOS DA PLAYLIST =====\n");
        printf("Nome: %s\n",playlistEncontrada->info.dado.playlist.nome);

        if (playlistEncontrada->info.dado.playlist.musicas == NULL)
            printf("Nenhuma musica cadastrada.\n");
                
        else mostrarMusicasPlaylist(playlistEncontrada->info.dado.playlist.musicas);              
      
    }
    else printf("\nPlaylist nao encontrada.\n");    
}

/* =========================================================
   IX - REMOVER MÚSICA DA PLAYLIST
   ========================================================= */

int removerMusicaPlaylist(ArvAVL *raizPlaylists,char *nomePlaylist,char *nomeArtista,char *tituloAlbum,char *tituloMusica){
    ArvAVL *playlistEncontrada;

    char chave[100];
    int resultado;
    resultado = 0;

    playlistEncontrada = buscarArvAVL(raizPlaylists,nomePlaylist);

    if (playlistEncontrada != NULL){
        snprintf(chave,sizeof(chave),"%s - %s - %s",nomeArtista,tituloAlbum,tituloMusica);

        playlistEncontrada->info.dado.playlist.musicas = removerArvAVL(playlistEncontrada->info.dado.playlist.musicas,chave,&resultado);
    }

    return resultado;
}

/* =========================================================
   VERIFICAR MÚSICA EM PLAYLIST
   ========================================================= */

int musicaEstaEmPlaylist(ArvAVL *raizPlaylists,char *nomeArtista,char *tituloAlbum,char *tituloMusica){
    char chave[100];
    int resultado;
    resultado = 0;

    if (raizPlaylists != NULL){
        snprintf(chave,sizeof(chave),"%s - %s - %s",nomeArtista,tituloAlbum,tituloMusica);

        if (buscarArvAVL(raizPlaylists->info.dado.playlist.musicas,chave) != NULL)
            resultado = 1;

        else{
            resultado = musicaEstaEmPlaylist(raizPlaylists->esq,nomeArtista,tituloAlbum,tituloMusica);

            if (resultado == 0)
                resultado = musicaEstaEmPlaylist(raizPlaylists->dir,nomeArtista,tituloAlbum,tituloMusica);
        }
    }

    return resultado;
}

/* =========================================================
   X - REMOVER MÚSICA DO ÁLBUM
   ========================================================= */

int removerMusicaAlbum(ArvAVL *raizArtistas,ArvAVL *raizPlaylists,char *nomeArtista,char *tituloAlbum,char *tituloMusica){
    ArvAVL *artistaEncontrado;
    ArvAVL *albumEncontrado;

    int resultado;
    resultado = 0;

    artistaEncontrado = buscarArvAVL(raizArtistas,nomeArtista);

    if (artistaEncontrado != NULL){

        albumEncontrado = buscarArvAVL(artistaEncontrado->info.dado.artista.albuns,tituloAlbum);

        if (albumEncontrado != NULL){

            if (musicaEstaEmPlaylist(raizPlaylists,nomeArtista,tituloAlbum,tituloMusica) == 0){
                albumEncontrado->info.dado.album.musicas = removerArvAVL(albumEncontrado->info.dado.album.musicas,tituloMusica,&resultado);

                if (resultado)
                    albumEncontrado->info.dado.album.qtdMusicas--;
            }
            else{
                printf("\nA musica faz parte de uma playlist.");
                printf("\nNao pode ser removida do album.\n");
            }
        }
    }

    return resultado;
}