#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Album {
    char nome[100];
    struct Album *prox;
} Album;

typedef struct Artista {
    char nome[100];
    char genero[100];
    char local[100];
    Album *albuns;
    struct Artista *prox;
} Artista;

void libera_album(Album *albuns) {
    while (albuns != NULL) {
        Album *proximoalbum = albuns->prox;
        free(albuns);
        albuns = proximoalbum;
    }
}

void libera_lista(Artista *lista) {
    while (lista != NULL) {
        Album *albuns = lista->albuns;
        libera_album(albuns);

        Artista *proximoartista = lista->prox;
        free(lista);
        lista = proximoartista;
    }
}

void inserir_artista(Artista **lista, char *nome, char *genero, char *local, Album *albuns) {
    Artista *novo_artista = (Artista *)malloc(sizeof(Artista));
    if (novo_artista == NULL) {
        printf("Erro: falha na alocação de memória.\n");
        exit(1);
    }

    strcpy(novo_artista->nome, nome);
    strcpy(novo_artista->genero, genero);
    strcpy(novo_artista->local, local);
    novo_artista->albuns = albuns;
    novo_artista->prox = NULL;

    if (*lista == NULL || strcmp((*lista)->nome, nome) > 0) {
        novo_artista->prox = *lista;
        *lista = novo_artista;
        return;
    }

    Artista *atual = *lista;
    Artista *anterior = NULL;
    while (atual != NULL && strcmp(atual->nome, nome) < 0) {
        anterior = atual;
        atual = atual->prox;
    }

    anterior->prox = novo_artista;
    novo_artista->prox = atual;
}

void remover_artista(Artista **lista, char *nome) {
    if (*lista == NULL) {
        printf("Lista de artistas vazia.\n");
        return;
    }

    Artista *atual = *lista;
    Artista *anterior = NULL;

    while (atual != NULL && strcmp(atual->nome, nome) != 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Artista não encontrado.\n");
        return;
    }

    if (anterior == NULL) {
        *lista = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    libera_album(atual->albuns);
    free(atual);
}

void editar_artista(Artista *lista, char *nome, char *novo_genero, char *novo_local) {
    while (lista != NULL) {
        if (strcmp(lista->nome, nome) == 0) {
            strcpy(lista->genero, novo_genero);
            strcpy(lista->local, novo_local);

            
            libera_album(lista->albuns);
            lista->albuns = NULL;

            int op;
            printf("Deseja adicionar álbuns? (1 para Sim, 0 para Não): ");
            scanf("%d", &op);
            getchar(); 

            if (op) {
                int nalbums;
                printf("Quantos álbuns deseja adicionar? ");
                scanf("%d", &nalbums);
                getchar();  
                for (int i = 0; i < nalbums; i++) {
                    Album *novo_album = (Album *)malloc(sizeof(Album));
                    printf("Digite o nome do álbum %d: ", i + 1);
                    fgets(novo_album->nome, sizeof(novo_album->nome), stdin);
                    strtok(novo_album->nome, "\n");
                    novo_album->prox = lista->albuns;
                    lista->albuns = novo_album;
                }
            }

            return;
        }
        lista = lista->prox;
    }

    printf("Artista não encontrado.\n");
}

void salvar_dados(Artista *lista, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        exit(1);
    }

    Artista *atual = lista;
    while (atual != NULL) {
        fprintf(arquivo, "%s\n", atual->nome);
        fprintf(arquivo, "%s\n", atual->genero);
        fprintf(arquivo, "%s\n", atual->local);

        Album *albuns = atual->albuns;
        while (albuns != NULL) {
            fprintf(arquivo, "%s\n", albuns->nome);
            albuns = albuns->prox;
        }

        fprintf(arquivo, "==========\n");
        atual = atual->prox;
    }

    fclose(arquivo);
}
Album *buscar_album(Artista *listaArtistas, char *nomeAlbum) {
    while (listaArtistas != NULL) {
        Album *albuns = listaArtistas->albuns;
        while (albuns != NULL) {
            if (strcmp(albuns->nome, nomeAlbum) == 0) {
                return albuns;
            }
            albuns = albuns->prox;
        }
        listaArtistas = listaArtistas->prox;
    }
    return NULL;
}
Artista *buscar_artista(Artista *lista, char *nome) {
    while (lista != NULL) {
        if (strcmp(lista->nome, nome) == 0) {
            return lista;
        }
        lista = lista->prox;
    }
    return NULL;
}
Artista *carregar_dados(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return NULL;
    }

    Artista *lista = NULL;
    char buffer[100];
    char nome[100], genero[100], local[100];
    Album *albuns = NULL;

    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        if (strcmp(buffer, "==========\n") == 0) {
            inserir_artista(&lista, nome, genero, local, albuns);
            albuns = NULL;
        } else if (albuns == NULL) {
            strcpy(nome, buffer);
            strtok(nome, "\n");
            fgets(genero, sizeof(genero), arquivo);
            strtok(genero, "\n");
            fgets(local, sizeof(local), arquivo);
            strtok(local, "\n");
            albuns = NULL;
        } else {
            Album *novo_album = (Album *)malloc(sizeof(Album));
            strcpy(novo_album->nome, buffer);
            strtok(novo_album->nome, "\n");
            novo_album->prox = albuns;
            albuns = novo_album;
        }
    }

    fclose(arquivo);
    return lista;
}