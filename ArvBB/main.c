#include "estruturas.h"

int main(void){
    ArvBB *artistas = NULL;
    Artista artista;
    Album album;
    Musica musica;
    ArvBB *artistaEncontrado;
    ArvBB *albumEncontrado;

    char entrada[100], nomeArtista[100], tituloAlbum[100], tituloMusica[100];
    int opcao = -1, resultado, ano, minutos;

    strcpy(artista.nome, "Legiao Urbana");
    strcpy(artista.tipo, "Banda");
    strcpy(artista.estilo, "Rock nacional");
    artista.numeroAlbuns = 0;
    artista.albuns = NULL;
    cadastrarArtista(&artistas, &artista);

    strcpy(album.titulo, "Dois");
    album.ano = 1986;
    album.qtdMusicas = 0;
    album.musicas = NULL;
    cadastrarAlbum(artistas, "Legiao Urbana", &album);

    strcpy(album.titulo, "Que pis e Este");
    album.ano = 1987;
    album.qtdMusicas = 0;
    album.musicas = NULL;
    cadastrarAlbum(artistas, "Legiao Urbana", &album);

    strcpy(album.titulo, "Cabeca Dinossauro");
    album.ano = 1986;
    album.qtdMusicas = 0;
    album.musicas = NULL;
    cadastrarAlbum(artistas, "Titas", &album);

    strcpy(musica.titulo, "Tempo perdido");
    musica.minutos = 5;
    cadastrarMusica(artistas, "Legiao Urbana", "Dois", &musica);

    strcpy(musica.titulo, "Policia");
    musica.minutos = 3;
    cadastrarMusica(artistas, "Titas", "Cabeca Dinossauro", &musica);

    printf("\nDados iniciais cadastrados com sucesso!\n");

    while (opcao != 0)
    {
         printf("\n==============================\n");
        printf("        MENU PRINCIPAL\n");
        printf("==============================\n");
        printf("1 - Cadastrar artista\n");
        printf("2 - Cadastrar album\n");
        printf("3 - Cadastrar musica\n");
        printf("4 - Mostrar artistas\n");
        printf("5 - Mostrar albuns de um ano\n");
        printf("6 - Mostrar dados de uma musica\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        fgets(entrada, 30, stdin);
        opcao = atoi(entrada);

        if (opcao == 1) {
            printf("\nNome do artista: ");
            fgets(artista.nome, 30, stdin);
            artista.nome[strcspn(artista.nome, "\n")] = '\0';

            printf("Tipo do artista: ");
            fgets(artista.tipo, 30, stdin);
            artista.tipo[strcspn(artista.tipo, "\n")] = '\0';

            printf("Estilo musical: ");
            fgets(artista.estilo, 50, stdin);
            artista.estilo[strcspn(artista.estilo, "\n")] = '\0';

            artista.numeroAlbuns = 0;
            artista.albuns = NULL;

            resultado = cadastrarArtista(
                &artistas,
                &artista
            );

            if (resultado)
                printf("\nArtista cadastrado com sucesso!\n");
            else
                printf("\nArtista ja cadastrado!\n");
        }

        else if (opcao == 2) {
            printf("\nNome do artista: ");
            fgets(nomeArtista, 100, stdin);
            nomeArtista[strcspn(nomeArtista, "\n")] = '\0';

            printf("Titulo do album: ");
            fgets(album.titulo, 100, stdin);
            album.titulo[strcspn(album.titulo, "\n")] = '\0';

            printf("Ano de lancamento: ");
            fgets(entrada, 30, stdin);
            album.ano = atoi(entrada);
            album.qtdMusicas = 0;
            album.musicas = NULL;

            resultado = cadastrarAlbum(
                artistas,
                nomeArtista,
                &album
            );

            if (resultado)
                printf("\nAlbum cadastrado com sucesso!\n");
            else
                printf("\nArtista inexistente ou album repetido!\n");
        }

        else if (opcao == 3) {
            printf("\nNome do artista: ");
            fgets(nomeArtista, 100, stdin);
            nomeArtista[strcspn(nomeArtista, "\n")] = '\0';

            printf("Titulo do album: ");
            fgets(tituloAlbum, 100, stdin);
            tituloAlbum[strcspn(tituloAlbum, "\n")] = '\0';

            artistaEncontrado = buscarArvBB(
                artistas,
                nomeArtista
            );

            if (artistaEncontrado != NULL) {
                albumEncontrado = buscarArvBB(
                    artistaEncontrado->info.dado.artista.albuns,
                    tituloAlbum
                );

                if (albumEncontrado != NULL) {
                    printf("Titulo da musica: ");
                    fgets(musica.titulo, 100, stdin);
                    musica.titulo[strcspn(musica.titulo, "\n")] = '\0';

                    printf("Duracao em minutos: ");
                    fgets(entrada, 30, stdin);
                    minutos = atoi(entrada);
                    musica.minutos = minutos;

                    resultado = cadastrarMusica(
                        artistas,
                        nomeArtista,
                        tituloAlbum,
                        &musica
                    );

                    if (resultado)
                        printf("\nMusica cadastrada com sucesso!\n");
                    else
                        printf("\nMusica ja cadastrada!\n");
                }
                else
                    printf("\nAlbum inexistente!\n");
            }
            else
                printf("\nArtista inexistente!\n");
        }

        else if (opcao == 4) {
            printf("\n===== ARTISTAS CADASTRADOS =====\n");
            mostrarArtistas(artistas);
        }

        else if (opcao == 5) {
            printf("\nNome do artista: ");
            fgets(nomeArtista, 100, stdin);
            nomeArtista[strcspn(nomeArtista, "\n")] = '\0';

            printf("Ano de lancamento: ");
            fgets(entrada, 30, stdin);
            ano = atoi(entrada);

            artistaEncontrado = buscarArvBB(
                artistas,
                nomeArtista
            );

            if (artistaEncontrado != NULL) {
                printf("\n===== ALBUNS DO ANO %d =====\n", ano);
                exibirAlbunsAno(
                    artistaEncontrado->info.dado.artista.albuns,
                    ano
                );
            }
            else
                printf("\nArtista inexistente!\n");
        }

        else if (opcao == 6) {
            printf("\nTitulo da musica: ");
            fgets(tituloMusica, 100, stdin);
            tituloMusica[strcspn(tituloMusica, "\n")] = '\0';

            printf("\n===== DADOS DA MUSICA =====\n");
            mostrarDadosMusica(
                artistas,
                tituloMusica
            );
        }

        else if (opcao != 0)
            printf("\nOpcao invalida!\n");
    }

    printf("\nPrograma encerrado.\n");

    return 0;
}