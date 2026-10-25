#include "estruturas.h"

int main(){
    ArvBB *raizArtistas = NULL;
    ArvBB *raizPlaylists = NULL;

    Artista artista;
    Album album;
    Musica musica;
    Playlist playlist;

    ArvBB *artistaEncontrado;

    int opcao;
    int resultado;
    int ano;

    do{
        printf("\n----------------------------------------\n");
        printf("       SISTEMA DE BIBLIOTECA MUSICAL\n");
        printf("----------------------------------------\n");

        printf("\n1 - Cadastrar artista");
        printf("\n2 - Cadastrar album");
        printf("\n3 - Cadastrar musica");
        printf("\n4 - Mostrar todos os artistas");
        printf("\n5 - Mostrar albuns de um ano");
        printf("\n6 - Buscar musica");
        printf("\n7 - Cadastrar playlist");
        printf("\n8 - Adicionar musica na playlist");
        printf("\n9 - Mostrar playlist");
        printf("\n10 - Remover musica da playlist");
        printf("\n11 - Remover musica do album");
        printf("\n0 - Sair");

        printf("\n\nDigite uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao){
            case 1:
                printf("\n===== CADASTRAR ARTISTA =====\n");

                printf("Nome: ");
                fgets(artista.nome,sizeof(artista.nome),stdin);

                artista.nome[strcspn(artista.nome,"\n")] = '\0';

                printf("Tipo: ");
                fgets(artista.tipo,sizeof(artista.tipo),stdin);
                    
                artista.tipo[strcspn(artista.tipo,"\n")] = '\0';

                printf("Estilo: ");
                fgets(artista.estilo,sizeof(artista.estilo), stdin);

                artista.estilo[strcspn(artista.estilo,"\n")] = '\0';

                artista.numeroAlbuns = 0;
                artista.albuns = NULL;

                resultado = cadastrarArtista(&raizArtistas,&artista);
                    
                if (resultado)
                    printf("\nArtista cadastrado com sucesso!\n");
                
                else
                    printf("\nArtista ja cadastrado!\n");

                break;
            case 2:
                printf("\n===== CADASTRAR ALBUM =====\n");

                printf("Nome do artista: ");
                fgets(artista.nome,sizeof(artista.nome),stdin);

                artista.nome[strcspn(artista.nome,"\n")] = '\0';

                printf("Titulo do album: ");
                fgets(album.titulo,sizeof(album.titulo),stdin);

                album.titulo[strcspn(album.titulo,"\n")] = '\0';

                printf("Ano de lancamento: ");
                scanf("%d", &album.ano);

                getchar();

                album.qtdMusicas = 0;
                album.musicas = NULL;

                resultado = cadastrarAlbum(raizArtistas,artista.nome,&album);

                if (resultado)
                    printf("\nAlbum cadastrado com sucesso!\n");

                else
                    printf("\nNao foi possivel cadastrar o album.\n" );
                        
                break;
            case 3:
                printf("\n===== CADASTRAR MUSICA =====\n");

                printf("Nome do artista: ");
                fgets(artista.nome,sizeof(artista.nome),stdin);

                artista.nome[strcspn(artista.nome,"\n")] = '\0';

                printf("Titulo do album: ");
                fgets(album.titulo,sizeof(album.titulo),stdin);

                album.titulo[strcspn(album.titulo,"\n")] = '\0';

                printf("Titulo da musica: ");
                fgets(musica.titulo,sizeof(musica.titulo),stdin);

                musica.titulo[strcspn(musica.titulo,"\n")] = '\0';

                printf("Duracao em minutos: ");
                scanf("%d", &musica.minutos);

                getchar();

                resultado = cadastrarMusica(raizArtistas,artista.nome,album.titulo,&musica);

                if (resultado)
                    printf("\nMusica cadastrada com sucesso!\n" );
                   
                else
                    printf("\nNao foi possivel cadastrar a musica.\n");
                break;
            case 4:
                printf("\n===== TODOS OS ARTISTAS =====\n");

                if (raizArtistas == NULL)
                    printf("Nenhum artista cadastrado.\n");
                        
                else
                    mostrarArtistas(raizArtistas);
                break;
            case 5:
                printf("\n===== ALBUNS DE DETERMINADO ANO =====\n");

                printf("Nome do artista: ");
                fgets(artista.nome,sizeof(artista.nome),stdin);
                    
                artista.nome[strcspn(artista.nome,"\n")] = '\0';

                printf("Ano: ");
                scanf("%d", &ano);
                getchar();

                artistaEncontrado = buscarArvBB(raizArtistas,artista.nome);

                if (artistaEncontrado != NULL){
                    printf("\nAlbuns de %d:\n",ano);
                    exibirAlbunsAno(artistaEncontrado->info.dado.artista.albuns,ano);
                }
                else
                    printf("\nArtista nao encontrado.\n");

                break;
            case 6:
                printf("\n===== BUSCAR MUSICA =====\n");

                printf("Titulo da musica: ");
                fgets(musica.titulo,sizeof(musica.titulo),stdin);

                musica.titulo[strcspn(musica.titulo,"\n")] = '\0';
                mostrarDadosMusica(raizArtistas,musica.titulo);
                break;
            case 7:
                printf("\n===== CADASTRAR PLAYLIST =====\n");

                printf("Nome da playlist: ");
                fgets(playlist.nome,sizeof(playlist.nome),stdin);

                playlist.nome[strcspn(playlist.nome,"\n")] = '\0'; 
                playlist.musicas = NULL;

                resultado = cadastrarPlaylist(&raizPlaylists,&playlist);

                if (resultado)
                    printf("\nPlaylist cadastrada com sucesso!\n");

                else
                    printf("\nPlaylist ja cadastrada!\n");

                break;
            case 8:
                printf("\n===== ADICIONAR MUSICA NA PLAYLIST =====\n");

                printf("Nome da playlist: ");
                fgets(playlist.nome,sizeof(playlist.nome),stdin);

                playlist.nome[ strcspn(playlist.nome,"\n")] = '\0';

                printf("Nome do artista: ");
                fgets(artista.nome,sizeof(artista.nome),stdin);

                artista.nome[strcspn(artista.nome,"\n")] = '\0';

                printf("Titulo do album: ");
                fgets(album.titulo,sizeof(album.titulo),stdin);

                album.titulo[strcspn(album.titulo,"\n")] = '\0';

                printf("Titulo da musica: ");
                fgets(musica.titulo,sizeof(musica.titulo),stdin);

                musica.titulo[strcspn(musica.titulo,"\n")] = '\0';

                resultado = cadastrarMusicaPlaylist(raizArtistas,raizPlaylists,playlist.nome,artista.nome,album.titulo,musica.titulo);

                if (resultado)
                    printf("\nMusica adicionada a playlist!\n" );

                else
                    printf("\nNao foi possivel adicionar a musica.\n");

                break;
            case 9:
                printf("\n===== MOSTRAR PLAYLIST =====\n");

                printf("Nome da playlist: ");
                fgets(playlist.nome,sizeof(playlist.nome),stdin);

                playlist.nome[strcspn(playlist.nome,"\n")] = '\0';

                mostrarDadosPlaylist( raizPlaylists,playlist.nome);
                break;
            case 10:
                printf("\n===== REMOVER MUSICA DA PLAYLIST =====\n");
                    
                printf("Nome da playlist: ");
                fgets(playlist.nome,sizeof(playlist.nome),stdin);

                playlist.nome[strcspn(playlist.nome,"\n")] = '\0';

                printf("Nome do artista: ");
                fgets( artista.nome,sizeof(artista.nome),stdin);

                artista.nome[strcspn(artista.nome,"\n")] = '\0';

                printf("Titulo do album: ");
                fgets(album.titulo,sizeof(album.titulo),stdin);

                album.titulo[strcspn(album.titulo,"\n")] = '\0';

                printf("Titulo da musica: ");
                fgets(musica.titulo,sizeof(musica.titulo),stdin);

                musica.titulo[strcspn(musica.titulo,"\n")] = '\0';

                resultado =removerMusicaPlaylist(raizPlaylists,playlist.nome,artista.nome,album.titulo,musica.titulo);
                    
                if (resultado)
                    printf("\nMusica removida da playlist!\n");

                else
                    printf("\nMusica nao encontrada na playlist.\n");

                break;
            case 11:
                printf("\n===== REMOVER MUSICA DO ALBUM =====\n");

                printf("Nome do artista: ");
                fgets(artista.nome,sizeof(artista.nome),stdin);

                artista.nome[strcspn(artista.nome,"\n")] = '\0';

                printf("Titulo do album: ");
                fgets(album.titulo,sizeof(album.titulo),stdin);

                album.titulo[strcspn(album.titulo,"\n")] = '\0';

                printf("Titulo da musica: ");
                fgets(musica.titulo,sizeof(musica.titulo),stdin);

                musica.titulo[strcspn(musica.titulo,"\n")] = '\0';

                resultado = removerMusicaAlbum(raizArtistas,raizPlaylists,artista.nome,album.titulo,musica.titulo);

                if (resultado)
                    printf("\nMusica removida do album!\n");

                else
                    printf("\nNao foi possivel remover a musica.\n");

                break;
            case 0:

                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
                break;
        }
    } while (opcao != 0);
    return 0;
}