#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main() {
    const char *nome_arquivo = "artistas.txt";
    Artista *lista = carregar_dados(nome_arquivo);
    if (lista == NULL) {
        printf("Não foi possível carregar os dados do arquivo. Inicializando com lista vazia.\n");
    }

    int opcao;

    do {
        printf("\nMenu:\n");
        printf("1. Inserir artista\n");
        printf("2. Remover artista\n");
        printf("3. Editar artista\n");
        printf("4. Buscar artista\n");
        printf("5. Buscar álbum\n");
        printf("6. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        printf("\n");
        getchar();  

        char nome[100], genero[100], local[100], novo_genero[100], novo_local[100];
        Album *novosAlbuns = NULL;
        char album[100];

        switch (opcao) {
            case 1:
                printf("Digite o nome do artista: ");
                fgets(nome, sizeof(nome), stdin);
                strtok(nome, "\n");

                printf("Digite o gênero do artista: ");
                fgets(genero, sizeof(genero), stdin);
                strtok(genero, "\n");

                printf("Digite o local de origem do artista: ");
                fgets(local, sizeof(local), stdin);
                strtok(local, "\n");

                int nalbums;
                printf("Quantos álbuns deseja adicionar? ");
                scanf("%d", &nalbums);
                getchar();  

                for (int i = 0; i < nalbums; i++) {
                    Album *novo_album = (Album *)malloc(sizeof(Album));
                    printf("Digite o nome do álbum %d: ", i + 1);
                    fgets(novo_album->nome, sizeof(novo_album->nome), stdin);
                    strtok(novo_album->nome, "\n");
                    novo_album->prox = novosAlbuns;
                    novosAlbuns = novo_album;
                }

                inserir_artista(&lista, nome, genero, local, novosAlbuns);
                salvar_dados(lista, nome_arquivo);
                break;
            case 2:
                printf("Digite o nome do artista que deseja remover: ");
                fgets(nome, sizeof(nome), stdin);
                strtok(nome, "\n");

                remover_artista(&lista, nome);
                salvar_dados(lista, nome_arquivo);
                break;
            case 3:
                printf("Digite o nome do artista que deseja editar: ");
                fgets(nome, sizeof(nome), stdin);
                strtok(nome, "\n");

                printf("Digite o novo gênero do artista: ");
                fgets(novo_genero, sizeof(novo_genero), stdin);
                strtok(novo_genero, "\n");

                printf("Digite o novo local de origem do artista: ");
                fgets(novo_local, sizeof(novo_local), stdin);
                strtok(novo_local, "\n");

                editar_artista(lista, nome, novo_genero, novo_local);
                salvar_dados(lista, nome_arquivo);
                break;
            case 4:
                printf("Digite o nome do artista que deseja buscar: ");
                fgets(nome, sizeof(nome), stdin);
                strtok(nome, "\n");

                Artista *artista_encontrado = buscar_artista(lista, nome);
                if (artista_encontrado != NULL) {
                    printf("Artista encontrado:\n");
                    printf("Nome: %s\n", artista_encontrado->nome);
                    printf("Gênero: %s\n", artista_encontrado->genero);
                    printf("Local de origem: %s\n", artista_encontrado->local);
                } else {
                    printf("Artista não encontrado.\n");
                }
                break;
            case 5:
                printf("Digite o nome do álbum que deseja buscar: ");
                fgets(album, sizeof(album), stdin);
                strtok(album, "\n");

                Album *album_encontrado = buscar_album(lista, album);
                if (album_encontrado != NULL) {
                    printf("Álbum encontrado:\n");
                    printf("Nome: %s\n", album_encontrado->nome);
                } else {
                    printf("Álbum não encontrado.\n");
                }
                break;
            case 6:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 6);

    libera_lista(lista);
    return 0;
}