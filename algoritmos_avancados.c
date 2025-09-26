#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 50

// =============================
// Estruturas de Dados
// =============================

// Nó da mansão (árvore binária de salas)
typedef struct Sala {
    char nome[TAM];
    struct Sala *esquerda;
    struct Sala *direita;
    char pista[TAM]; // Pista encontrada nesta sala (ou vazio se não houver)
} Sala;

// Nó da árvore de pistas (BST)
typedef struct Pista {
    char nome[TAM];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;


// =============================
// Funções da Mansão
// =============================

/*
 * criarSala()
 * Cria dinamicamente uma sala com nome e (opcional) pista.
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) { 
        printf("Erro de memória!\n"); 
        exit(1); 
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}


// =============================
// Funções da BST de Pistas
// =============================

/*
 * inserirPista()
 * Insere uma nova pista na BST em ordem alfabética.
 */
Pista* inserirPista(Pista *raiz, const char *nome) {
    if (raiz == NULL) {
        Pista *nova = (Pista*) malloc(sizeof(Pista));
        strcpy(nova->nome, nome);
        nova->esquerda = nova->direita = NULL;
        return nova;
    }
    if (strcmp(nome, raiz->nome) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, nome);
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->direita = inserirPista(raiz->direita, nome);
    return raiz;
}

/*
 * listarPistas()
 * Percorre a BST em ordem e imprime as pistas em ordem alfabética.
 */
void listarPistas(Pista *raiz) {
    if (raiz != NULL) {
        listarPistas(raiz->esquerda);
        printf(" - %s\n", raiz->nome);
        listarPistas(raiz->direita);
    }
}

/*
 * liberarPistas()
 * Libera a memória da árvore de pistas.
 */
void liberarPistas(Pista *raiz) {
    if (raiz) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}


// =============================
// Exploração da Mansão
// =============================

/*
 * explorarSalas()
 * Permite ao jogador navegar pela mansão e coleta pistas automaticamente.
 */
void explorarSalas(Sala *atual, Pista **raizPistas) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Se a sala contém pista, coleta automaticamente
        if (strlen(atual->pista) > 0) {
            printf("💡 Você encontrou uma pista: %s!\n", atual->pista);
            *raizPistas = inserirPista(*raizPistas, atual->pista);
        }

        // Verifica se é nó-folha
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim da exploração! Você chegou a um beco sem saída.\n");
            break;
        }

        printf("Escolha um caminho:\n");
        if (atual->esquerda) printf("(e) Esquerda ");
        if (atual->direita) printf("(d) Direita ");
        printf("| (p) Ver pistas | (s) Sair\n-> ");
        scanf(" %c", &escolha);

        if ((escolha == 'e' || escolha == 'E') && atual->esquerda)
            atual = atual->esquerda;
        else if ((escolha == 'd' || escolha == 'D') && atual->direita)
            atual = atual->direita;
        else if (escolha == 'p' || escolha == 'P') {
            printf("\n📜 Pistas coletadas até agora:\n");
            if (*raizPistas)
                listarPistas(*raizPistas);
            else
                printf("Nenhuma pista encontrada.\n");
        }
        else if (escolha == 's' || escolha == 'S') {
            printf("Exploração encerrada!\n");
            break;
        }
        else
            printf("Opção inválida!\n");
    }
}

/*
 * liberarSalas()
 * Libera memória da árvore da mansão.
 */
void liberarSalas(Sala *raiz) {
    if (raiz != NULL) {
        liberarSalas(raiz->esquerda);
        liberarSalas(raiz->direita);
        free(raiz);
    }
}


// =============================
// FUNÇÃO MAIN (AGORA AO FINAL)
// =============================

int main() {
    // Criar salas (algumas com pistas)
    Sala *hall        = criarSala("Hall de Entrada", "");
    Sala *salaEstar   = criarSala("Sala de Estar", "Chave enferrujada");
    Sala *cozinha     = criarSala("Cozinha", "");
    Sala *biblioteca  = criarSala("Biblioteca", "Livro antigo");
    Sala *jardim      = criarSala("Jardim", "Mapa misterioso");
    Sala *quarto      = criarSala("Quarto Secreto", "Colar de esmeralda");

    // Montar a árvore da mansão
    hall->esquerda = salaEstar;
    hall->direita  = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita  = jardim;
    cozinha->direita = quarto;

    Pista *pistas = NULL; // Raiz da BST de pistas

    printf("=== Mansão Misteriosa - Nível Aventureiro ===\n");
    printf("Explore os cômodos e encontre pistas!\n");

    explorarSalas(hall, &pistas);

    // Exibe as pistas ao final
    printf("\n=== Pistas Coletadas ===\n");
    if (pistas)
        listarPistas(pistas);
    else
        printf("Nenhuma pista encontrada.\n");

    // Libera memória
    liberarSalas(hall);
    liberarPistas(pistas);

    printf("\nPrograma encerrado.\n");
    return 0;
}
