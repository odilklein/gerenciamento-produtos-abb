/*
 * ===========================================================================
 *  Sistema de Gerenciamento de Produtos com Arvore Binaria de Busca (ABB)
 * ===========================================================================
 *  Disciplina: Estrutura de Dados
 *  Curso: Analise e Desenvolvimento de Sistemas - Fatec Araras
 *  Aluno:  Odil Klein da Silva Neto
 *  Professor: Ederaldo Luiz Ratz
 * --------------------------------------------------------------------------
 *  Regras de organizacao (ABB):
 *    - A chave de ordenacao e o CODIGO do produto.
 *    - Todo no da subarvore ESQUERDA tem codigo MENOR que o no atual.
 *    - Todo no da subarvore DIREITA tem codigo MAIOR que o no atual.
 *    - Codigo duplicado e REJEITADO (cada produto e unico pelo codigo).
 *
 *  Convencao de ALTURA (usada neste programa):
 *    - Altura medida em NUMERO DE ARESTAS.
 *    - No folha tem altura 0; arvore vazia tem altura -1.
 *
 *  Restricoes da atividade: sem STL (sem string, vector, etc.), apenas
 *  struct, ponteiros, alocacao dinamica (new/delete) e recursao.
 * ===========================================================================
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>

/* --------------------------- ENTIDADES ---------------------------------- */

/* Produto: dados cadastrais do item do catalogo da empresa. */
struct Produto {
    int  codigo;               /* chave unica de ordenacao da ABB         */
    char descricao[100];       /* nome/descricao do produto               */
    double preco;              /* preco unitario                          */
    int  quantidade;           /* quantidade em estoque                   */
};

/* No: celula da arvore. Guarda o produto e os ponteiros para os filhos.
 * Os ponteiros de filho representam as ligacoes da arvore: NULL significa
 * "nao ha filho nessa direcao" (usado nas folhas e em arvores vazias). */
struct No {
    Produto produto;
    No    *esquerdo;           /* subarvore esquerda (codigos menores)    */
    No    *direito;            /* subarvore direita  (codigos maiores)    */
};

/* ------------------------ FUNCOES DA ABB --------------------------------- */

/* Cria um novo no dinamicamente a partir de um produto ja preenchido. */
No* criarNo(const Produto &p) {
    No *novo   = new No;
    novo->produto   = p;
    novo->esquerdo  = NULL;
    novo->direito   = NULL;
    return novo;
}

/* INSERCAO (recursiva): percorre a arvore comparando o codigo e insere
 * o novo no na posicao correta. Retorna a nova raiz da subarvore.
 * Codigo repetido: avisa e nao insere nada (regra definida na Parte B, e). */
No* inserir(No *raiz, const Produto &p) {
    if (raiz == NULL)                    /* posicao encontrada        */
        return criarNo(p);

    if (p.codigo == raiz->produto.codigo) {
        printf("  [!] ERRO: codigo %d ja cadastrado para \"%s\". "
               "Produto nao inserido.\n",
               p.codigo, raiz->produto.descricao);
        return raiz;                     /* mantem a arvore como esta */
    }

    if (p.codigo < raiz->produto.codigo)          /* vai para a esquerda */
        raiz->esquerdo = inserir(raiz->esquerdo, p);
    else                                         /* vai para a direita  */
        raiz->direito  = inserir(raiz->direito, p);

    return raiz;
}

/* BUSCA (recursiva): desce pela arvore usando a propriedade da ABB.
 * Retorna o no encontrado ou NULL se o codigo nao existir. */
No* buscar(No *raiz, int codigo) {
    if (raiz == NULL)              /* chegou a uma folha sem achar */
        return NULL;
    if (codigo == raiz->produto.codigo)
        return raiz;               /* achou                        */
    if (codigo < raiz->produto.codigo)
        return buscar(raiz->esquerdo, codigo);
    return buscar(raiz->direito, codigo);
}

/* PERCURSO EM ORDEM: esquerda -> raiz -> direita.
 * Em uma ABB, esse percurso emite os codigos em ORDEM CRESCENTE. */
void listarEmOrdem(No *raiz) {
    if (raiz == NULL) return;
    listarEmOrdem(raiz->esquerdo);
    printf("  %-6d %-30s R$ %8.2f  qtd:%3d\n",
           raiz->produto.codigo, raiz->produto.descricao,
           raiz->produto.preco, raiz->produto.quantidade);
    listarEmOrdem(raiz->direito);
}

/* PERCURSO PRE-ORDEM: raiz -> esquerda -> direita. */
void listarPreOrdem(No *raiz) {
    if (raiz == NULL) return;
    printf("  %-6d %-30s R$ %8.2f  qtd:%3d\n",
           raiz->produto.codigo, raiz->produto.descricao,
           raiz->produto.preco, raiz->produto.quantidade);
    listarPreOrdem(raiz->esquerdo);
    listarPreOrdem(raiz->direito);
}

/* PERCURSO POS-ORDEM: esquerda -> direita -> raiz. */
void listarPosOrdem(No *raiz) {
    if (raiz == NULL) return;
    listarPosOrdem(raiz->esquerdo);
    listarPosOrdem(raiz->direito);
    printf("  %-6d %-30s R$ %8.2f  qtd:%3d\n",
           raiz->produto.codigo, raiz->produto.descricao,
           raiz->produto.preco, raiz->produto.quantidade);
}

/* Encontra o no com MENOR codigo de uma subarvore (mais a esquerda).
 * Usado na remocao do no que tem dois filhos. */
No* menorNo(No *raiz) {
    No *atual = raiz;
    while (atual && atual->esquerdo != NULL)
        atual = atual->esquerdo;
    return atual;
}

/* REMOCAO (recursiva) com os tres casos:
 *   Caso 1: no folha         -> simplesmente remove o no.
 *   Caso 2: no com UM filho  -> o filho substitui o no removido.
 *   Caso 3: no com DOIS filhos -> substitui pelo SUCESSOR (menor codigo
 *            da subarvore direita) e remove o sucessor da la. */
No* remover(No *raiz, int codigo) {
    if (raiz == NULL) {
        printf("  [!] Codigo %d nao encontrado na arvore.\n", codigo);
        return NULL;
    }

    if (codigo < raiz->produto.codigo)
        raiz->esquerdo = remover(raiz->esquerdo, codigo);
    else if (codigo > raiz->produto.codigo)
        raiz->direito  = remover(raiz->direito, codigo);
    else {
        /* achou o no a remover */
        if (raiz->esquerdo == NULL && raiz->direito == NULL) {
            /* Caso 1: folha */
            delete raiz;
            return NULL;
        }
        if (raiz->esquerdo == NULL) {
            /* Caso 2: so tem filho direito */
            No *tmp = raiz->direito;
            delete raiz;
            return tmp;
        }
        if (raiz->direito == NULL) {
            /* Caso 2: so tem filho esquerdo */
            No *tmp = raiz->esquerdo;
            delete raiz;
            return tmp;
        }
        /* Caso 3: dois filhos -> substitui pelo sucessor em-ordem */
        No *sucessor = menorNo(raiz->direito);
        raiz->produto   = sucessor->produto;   /* copia os dados     */
        raiz->direito   = remover(raiz->direito, sucessor->produto.codigo);
    }
    return raiz;
}

/* CONTAR PRODUTOS: total de nos da arvore (recursivo). */
int contarNos(No *raiz) {
    if (raiz == NULL) return 0;
    return 1 + contarNos(raiz->esquerdo) + contarNos(raiz->direito);
}

/* CONTAR FOLHAS: nos sem nenhum filho (esquerdo e direito == NULL). */
int contarFolhas(No *raiz) {
    if (raiz == NULL) return 0;
    if (raiz->esquerdo == NULL && raiz->direito == NULL) return 1;
    return contarFolhas(raiz->esquerdo) + contarFolhas(raiz->direito);
}

/* ALTURA: numero de ARESTAS do maior caminho da raiz ate uma folha.
 * Convencao documentada: folha = 0 e arvore vazia = -1. */
int altura(No *raiz) {
    if (raiz == NULL) return -1;
    int he = altura(raiz->esquerdo);
    int hd = altura(raiz->direito);
    return (he > hd ? he : hd) + 1;
}

/* LIBERAR MEMORIA (pos-ordem): apaga os filhos antes do proprio no,
 * evitando acesso a memoria ja liberada. */
void liberar(No *raiz) {
    if (raiz == NULL) return;
    liberar(raiz->esquerdo);
    liberar(raiz->direito);
    delete raiz;
}

/* ------------------------ PROGRAMA PRINCIPAL ---------------------------- */

void imprimirProduto(No *no) {
    printf("  Codigo......: %d\n",      no->produto.codigo);
    printf("  Descricao...: %s\n",      no->produto.descricao);
    printf("  Preco.......: R$ %.2f\n", no->produto.preco);
    printf("  Quantidade..: %d\n",      no->produto.quantidade);
}

int main() {
    No      *raiz = NULL;      /* arvore comeca vazia */
    int      opcao;
    int      codigo;
    Produto  p;

    do {
        printf("\n+--------------------------------------------+\n");
        printf("|   GERENCIAMENTO DE PRODUTOS - ABB          |\n");
        printf("+--------------------------------------------+\n");
        printf("| 1) Cadastrar produto                       |\n");
        printf("| 2) Consultar produto (por codigo)          |\n");
        printf("| 3) Listar em ordem CRESCENTE (em-ordem)    |\n");
        printf("| 4) Listar em PRE-ORDEM                     |\n");
        printf("| 5) Listar em POS-ORDEM                     |\n");
        printf("| 6) Remover produto                         |\n");
        printf("| 7) Contar produtos cadastrados             |\n");
        printf("| 8) Contar produtos sem filhos (folhas)     |\n");
        printf("| 9) Altura da arvore (em arestas)           |\n");
        printf("| 0) Encerrar (libera a memoria)             |\n");
        printf("+--------------------------------------------+\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) break;

        switch (opcao) {
        case 1:                                    /* CADASTRAR */
            printf("Codigo: ");
            scanf("%d", &p.codigo);
            printf("Descricao: ");
            scanf(" %99[^\n]", p.descricao);       /* le texto com espacos */
            printf("Preco: ");
            scanf("%lf", &p.preco);
            printf("Quantidade: ");
            scanf("%d", &p.quantidade);
            if (buscar(raiz, p.codigo) != NULL) {
                printf("  [!] ERRO: codigo %d ja cadastrado para \"%s\". "
                       "Produto nao inserido.\n",
                       p.codigo, buscar(raiz, p.codigo)->produto.descricao);
            } else {
                raiz = inserir(raiz, p);
                printf("  [ok] Produto %d inserido.\n", p.codigo);
            }
            break;

        case 2:                                    /* CONSULTAR */
            printf("Codigo a consultar: ");
            scanf("%d", &codigo);
            {
                No *achado = buscar(raiz, codigo);
                if (achado == NULL)
                    printf("  [!] Produto de codigo %d NAO encontrado.\n", codigo);
                else {
                    printf("  Produto encontrado:\n");
                    imprimirProduto(achado);
                }
            }
            break;

        case 3:                                    /* EM ORDEM */
            if (raiz == NULL) printf("  Arvore vazia.\n");
            else {
                printf("  %-6s %-30s %-12s %s\n", "COD", "DESCRICAO", "PRECO", "QTD");
                listarEmOrdem(raiz);
            }
            break;

        case 4:                                    /* PRE-ORDEM */
            if (raiz == NULL) printf("  Arvore vazia.\n");
            else {
                printf("  %-6s %-30s %-12s %s\n", "COD", "DESCRICAO", "PRECO", "QTD");
                listarPreOrdem(raiz);
            }
            break;

        case 5:                                    /* POS-ORDEM */
            if (raiz == NULL) printf("  Arvore vazia.\n");
            else {
                printf("  %-6s %-30s %-12s %s\n", "COD", "DESCRICAO", "PRECO", "QTD");
                listarPosOrdem(raiz);
            }
            break;

        case 6:                                    /* REMOVER */
            printf("Codigo a remover: ");
            scanf("%d", &codigo);
            raiz = remover(raiz, codigo);
            printf("  [ok] Remocao processada para o codigo %d.\n", codigo);
            break;

        case 7:
            printf("  Total de produtos cadastrados: %d\n", contarNos(raiz));
            break;

        case 8:
            printf("  Produtos sem filhos (folhas): %d\n", contarFolhas(raiz));
            break;

        case 9:
            if (raiz == NULL)
                printf("  Arvore vazia: altura = -1 (nenhum no).\n");
            else
                printf("  Altura da arvore = %d arestas.\n", altura(raiz));
            break;

        case 0:
            printf("  Liberando memoria e encerrando...\n");
            liberar(raiz);      /* requisito 10: free antes de sair */
            raiz = NULL;
            break;

        default:
            printf("  Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
