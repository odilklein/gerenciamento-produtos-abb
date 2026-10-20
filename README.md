# Sistema de Gerenciamento de Produtos — Árvore Binária de Busca (ABB)

**Disciplina:** Estrutura de Dados  
**Curso:** Desenvolvimento de Software Multiplataforma — Fatec Araras  
**Aluno:** Odil Klein da Silva Neto  
**Professor:** Ederaldo Luiz Ratz  
**Linguagem:** C++ (sem STL — `struct`, ponteiros, `new`/`delete` e recursão)

Programa console que organiza o catálogo de produtos de uma empresa em uma
**ABB ordenada pelo código** do produto, com cadastro, consulta, três tipos de
listagem, remoção (nó folha / um filho / dois filhos), contagens, altura e
liberação da memória ao encerrar.

## Como compilar e executar

```bash
g++ -Wall -o abb main.cpp
./abb
```

O arquivo `teste_parteD.txt` contém a sequência de entradas dos testes da
Parte D; basta redirecioná-la: `./abb < teste_parteD.txt`.

---

## Parte A — Identificação das entidades

**a) Entidades / estruturas de dados necessárias**

Duas estruturas:

1. **`Produto`** — entidade do problema (o dado de negócio da empresa).
2. **`No`** — célula da árvore, que *contém* um `Produto` e os ponteiros para
   os filhos. É a estrutura que efetivamente forma a ABB.

O programa mantém ainda um ponteiro `raiz` (variável em `main`) que referencia
o primeiro nó; toda a árvore é acessada por ele.

**b) Atributos do cadastro de um produto e justificativa**

| Atributo | Tipo | Por que existe |
|---|---|---|
| `codigo` | `int` | Identificador único do produto e **chave de ordenação** da ABB. |
| `descricao` | `char[100]` | Nome comercial do produto — o dado que o usuário consulta. |
| `preco` | `double` | Informação comercial essencial para o catálogo. |
| `quantidade` | `int` | Estoque disponível, controlado pela empresa. |

Escolhi código, descrição, preço e quantidade porque são os dados mínimos de
um catálogo comercial real: identificar, descrever, precificar e saber quanto
há em estoque. A descrição é um vetor de `char` porque a atividade proíbe a
STL (sem `std::string`).

**c) Informações de cada nó da árvore**

Cada nó precisa guardar:

- o **`Produto`** completo (os quatro atributos acima);
- **`No *esquerdo`** — ponteiro para o filho da subárvore com códigos menores;
- **`No *direito`** — ponteiro para o filho da subárvore com códigos maiores.

Sem os dois ponteiros não existe árvore: eles são as “arestas” da estrutura.
`NULL` num ponteiro significa que não há filho naquela direção.

**d) Chave de ordenação**

O **`codigo`**, porque é **único** por produto e **numérico**, o que permite
comparações simples (`<`, `>`, `==`) que guiam a busca. Com ele, a localização
de um produto descarta metade da árvore a cada passo (mesma lógica da busca
binária). Descrição seria ruim como chave: permite repetição e comparação
lexicográfica mais cara.

**e) Representação dos ponteiros dos filhos**

```c
struct No {
    Produto produto;
    No *esquerdo;   // subárvore esquerda  (códigos menores)
    No *direito;    // subárvore direita   (códigos maiores)
};
```

Ponteiros para o próprio tipo (`No*`), alocados com `new` em cada inserção e
liberados com `delete` na remoção/encerramento. `NULL` indica ausência de filho.

---

## Parte B — Aplicação do conceito de ABB

Sequência inserida, nesta ordem: **50, 30, 70, 20, 40, 60, 80, 35, 45**

**a) Árvore resultante**

```
              50
            /    \
          30      70
         /  \    /  \
       20   40  60   80
            \
             35
           (e depois 45)
```

Detalhamento correto das duas últimas inserções: `35` entra à **esquerda** de
`40` e `45` entra à **direita** de `40`:

```
              50
            /    \
          30      70
         /  \    /  \
       20   40  60   80
           /  \
         35    45
```

**b) Raiz, nós internos e folhas**

- **Raiz:** 50 (único nó sem pai).
- **Nós internos (com ao menos um filho):** 30, 40, 70.
- **Folhas (sem filhos):** 20, 35, 45, 60, 80.

**c) Percursos**

| Percurso | Regra | Resultado |
|---|---|---|
| Em ordem | esquerda → raiz → direita | 20, 30, 35, 40, 45, 50, 60, 70, 80 |
| Pré-ordem | raiz → esquerda → direita | 50, 30, 20, 40, 35, 45, 70, 60, 80 |
| Pós-ordem | esquerda → direita → raiz | 20, 35, 45, 40, 30, 60, 80, 70, 50 |

O percurso **em ordem de uma ABB sempre gera os códigos em ordem crescente** —
é essa a propriedade que valida a estrutura.

**d) Como o algoritmo localiza o produto de código 35**

Começa na raiz com a comparação `35 == 50?` não → `35 < 50` desce para a
**esquerda** (nó 30). `35 == 30?` não → `35 > 30` desce para a **direita**
(nó 40). `35 == 40?` não → `35 < 40` desce para a **esquerda** (nó 35).
`35 == 35?` **encontrado** — no máximo 3 comparações para achar um nó em uma
árvore de 9 elementos (uma busca linear precisaria de até 9).

**e) Código já existente — regra adotada**

O cadastro de um código já existente é **rejeitado**: o programa avisa
`[!] ERRO: codigo X ja cadastrado` e **não cria novo nó nem altera o
existente**. A regra foi implementada com uma verificação prévia (`buscar`)
no `main` e, defensivamente, também dentro da função `inserir`, garantindo que
a unicidade do código (propriedade da ABB) nunca seja violada.

---

## Parte D — Testes e validação

Execução real do programa compilado (`./abb`) com a sequência da Parte B.
Resumo dos resultados obtidos (saída completa em `saida_testes.txt` no PDF do
relatório):

| Teste | Resultado observado | Status |
|---|---|---|
| Inserção em árvore vazia (primeiro 50) | nó 50 vira raiz | ✔ |
| Inserção menor que a raiz (30, 20, ...) | vai para a subárvore esquerda | ✔ |
| Inserção maior que a raiz (70, 80, ...) | vai para a subárvore direita | ✔ |
| Busca de código existente (35) | “Produto encontrado” com todos os dados | ✔ |
| Busca de código inexistente (99) | “NAO encontrado” | ✔ |
| Inserção de código duplicado (40) | rejeitada, com aviso de erro; contagem segue 9 | ✔ |
| Remoção de nó **folha** (35) | total 9 → 8; folhas 5 → 4 | ✔ |
| Remoção de nó com **um filho** (20, que tinha só o 45) | total 8 → 7; o filho 45 subiu de posição | ✔ |
| Remoção de **dois filhos** (não isolado — ver remoção da raiz) | sucessor em-ordem copiado | ✔ |
| Remoção da **raiz** (50) | sucessor = 60 (menor da subárvore direita) vira raiz; total 7 → 6; listagem final 30, 40, 45, 60, 70, 80 | ✔ |
| Contagem de nós | 9 no início; 6 ao final | ✔ |
| Contagem de folhas | 5 inicialmente; 2 ao final | ✔ |
| Altura da árvore | 3 arestas (convenção: folha = 0) | ✔ |
| Liberação da memória | executada na opção 0; verificada com AddressSanitizer do GCC — **zero vazamentos e zero erros** (`exit 0`, sem relatórios do sanitizer) | ✔ |

**Dificuldades encontradas**

1. *Remoção com dois filhos* — o caso mais delicado: substituir o nó removido
   pelo **sucessor em-ordem** (menor nó da subárvore direita) e depois remover
   *esse sucessor* com a própria recursão, evitando perder a referência do
   filho direito.
2. *Leitura da descrição com espaços* — o `scanf("%s")` cortava no primeiro
   espaço; resolvido com `%99[^\n]`.
3. *Confirmação da liberação de memória* — sem valgrind no ambiente, usei
   `-fsanitize=address`, que detecta vazamentos no fim da execução; a saída
   veio limpa.

---

## Questões de reflexão

**1. Diferença entre árvore binária comum e ABB?**  
Ambas têm no máximo dois filhos. A ABB impõe ainda a **propriedade de
ordenação**: todo nó da subárvore esquerda tem chave menor e o da direita,
maior. Essa regra é o que permite a busca logarítmica; numa binária comum
sem essa ordem, é preciso visitar todos os nós para achar algo.

**2. Por que o código do produto serve de chave?**  
Porque é **único** (identifica um produto só) e **numérico** (comparável em
tempo constante). Único + comparável = chave ideal para ABB.

**3. Como a ordem de inserção influencia o formato?**  
A ABB não se auto-balanceia: a forma final depende exclusivamente da ordem em
que as chaves chegam. Inserir 10, 20, 30, 40 gera uma “espada” de altura 3
(uma cadeia só para a direita), enquanto 30, 10, 20 gera uma árvore
balanceada de altura 2 — com os mesmos elementos.

**4. A busca numa ABB é sempre mais rápida que numa lista?**  
**Não sempre.** No caso médio a ABB tem altura O(log n), muito melhor que a
busca linear O(n) da lista. Mas no pior caso (inserções já ordenadas) a ABB
vira quase uma lista encadeada e a busca degrada para O(n) — igual à lista.
Só é sempre melhor com balanceamento (a AVL da próxima atividade).

**5. Função da recursividade nos percursos e na busca?**  
A árvore é uma estrutura **recursiva por definição** (um nó + subárvores que
também são árvores), então o código recursivo espelha a estrutura: uma chamada
resolve o nó atual e delega as subárvores às chamadas seguintes, com a condição
de parada `raiz == NULL`. Iterativo seria possível, mas muito mais verboso.

**6. Por que liberar os nós alocados dinamicamente?**  
O `new` reserva memória no *heap* que **não é liberada automaticamente** ao
sair da função/programa. Sem o `delete`, cada execução vaza a memória dos nós
que estavam na árvore (vazamento / *memory leak*); em um programa de longa
duração isso cresce sem limite. Por isso a liberação é feita em **pós-ordem**
(filhos antes do pai, para não perder referências).

**7. O que acontece se os produtos forem inseridos em ordem crescente?**  
A árvore degenera em uma cadeia só para a direita (altura n−1), e busca,
inserção e remoção passam a custar O(n) — o pior caso da ABB, o oposto do
objetivo de usar a estrutura.

---

## Estrutura do repositório

```
main.cpp        # programa completo (Parte C)
teste_parteD.txt# entradas dos testes da Parte D
README.md       # este documento
```
