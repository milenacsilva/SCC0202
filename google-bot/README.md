# Projeto 1 - Mini Googlebot

## Participantes
<a href="https://github.com/roselino-quatro"><img src="https://avatars3.githubusercontent.com/u/43501582?s=460&u=607eaa89f7d1d4117ad8e3b2059d7fd24e1c0b45&v=4" title="rafa-quatro" width="80" height="80"></a>
<a href="https://github.com/milenacsilva"><img src="https://avatars2.githubusercontent.com/u/61664263?s=460&v=4" title="milenacsilva" width="80" height="80"></a>

Lourenço de Salles Roselino - **11796805**\
Milena Corrêa da Silva - **11795401**

## Especificações
Compílador GCC 7.5.0 com as flags -std=c99 -O1


## Instruções Para Uso
Para realizar a compilação normal basta utilizar o comando `make` ou `make all`, depois para execução utiliza `make run`. É possível também utilizar `make debug` para realizar execução com informações de depuração extras fornecidas pelo _valgrind_.

Depois de compilado e executado você será apresentado a seguinte tela incial:
```
  +-------------------------------------------------+
  |               INCIANDO GOOGLE BOT...            |
  +-------------------------------------------------+
  Arquivo de entrada: 
```

## Execução e usabilidade
Assim que o programa inicia, é pedido o nome do arquivo (.csv) de entrada de onde os primeiros sites do banco de dados serão lidos. Após isso, o usuário encrontra-se livre para fazer as seguintes operações:

```
O que você deseja fazer? (Digite o número equivalente ao comando)
0 - Inserir site manualmente
1 - Inserir sites a partir de um csv
2 - Remover site
3 - Atualizar a relevância de um site
4 - Adicionar uma palavra-chave a um site
5 - Mostrar banco de dados
6 - Buscar palavra chave
7 - Sugestão de sites
8 - Finalizar o programa
```

## Funcionalidades Extras
* **Inserção de sites a partir de um csv**: mesmo após a inicialização do programa, pode-se adicionar múltiplos sites de uma vez só no formato csv. 
* **Mostrar o banco de dados**: mostra todos os sites contidos no bot, ordenados por sua chave.
* **Salvar nova lista de sites ao finalizar programa**: pode-se salvar toda a lista de sites gerada durante a execução como um arquivo csv. 

## Estruturas Utilizadas

### Estrutura principal (AVL)
#### Visão Geral:

Para a estrutura principal, foi utilizado uma árvore `Avl` (estrutura dinâmica) ordenada pela chave dos sites. Isso porquê, tratando-se de uma árvore de busca balanceada, ela mantêm o custo das operações tendendo a `O(log n)` mesmo no seu pior caso.

#### Implementação:

```cpp
typedef struct node *Node;
struct node {
    Site site;
    Node left;
    Node right;
    int height;
};

struct avl {
    Node root;
    int amnt_sites;
};

```

#### Como funciona:

O balanceamento de uma `Avl` é feito de modo que a altura cada uma de suas sub-árvores podem diferir em no máximo 1 unidade, nós dando assim uma árvore de altura constante igual à `O(log n)`. Para garantir isso, a cada inserção ou remoção a altura, e por consequência, o fator de balanceamento (altura do nó esquerdo - altura do nó direito) deve ser atualizada a partir do nó inserido até a raiz da árvore. Na inserção basta encontrar apenas o primeiro nó desregulado e aplicar o operação de rotação necessária. Na remoção essa verificação deverá prosseguir até a raiz, podendo ser necessáio mais de uma rotação.

#### Complexidade:

Sua complexidade de espaço é `O(n)` e todas as operações bases tem complexidade de tempo `O(log n)`. 


### Estrutura de retorno p/ busca de palavra chave e sugestão de sites (LISTA ENCADEADA)
#### Visão Geral:

Para o retorno dessas funções, foi se utilizado uma `Lista Encadeada Simples` com inserção ordenada (a utilização de uma avl não seria possível pois diferentes sites podem ter a mesma relevância). Além disso, como as únicas operações que temos que fazer nessa estrutura é inserir, printar os sites e deletar a instância criada, um encadeamento simples foi o suficiente. Por fim, considerando que não sabemos o total de sites a serem adicionados, foi preferida uma alocaçâo dinâmica. 

#### Implementação:

```cpp
typedef struct node *Node;
struct node {
    Site site;
    Node next;
};

struct list {
    Node head;
    Node cur;
    int amnt_sites;
};
```

#### Como funciona:

A inserção de sites foi feita de acordo com a ordem decrescente de relevância, sendo que sites com chaves iguais não podem se fazer presentes na lista. Para facilitar o código posteriormente, foi utilizado também um ponteiro `Node cur` que inicia no mesmo nó correspondente à `head` da lista e que, com ajuda da função `list_go_to_next_node()`, anda pela lista até chegar no seu fim.

#### Complexidade:

Sua complexidade de espaço e complexidade de tempo da impressão, inserção ordenada e de `delete()` são todas `O(n)`.

### Estrutura auxiliar para a sugestão de sites (TRIE)
#### Visão Geral:

Para auxiliar a sugestão de sites, utilizamos uma `Trie` ou ainda `Árvore de prefixos`. O motivo de utilização dessa estrutura foi que, mesmo utilizando bastante memória, todas as operações tem uma complexidade de tempo `O(m)` onde `m` é o tamanho da palavra chave (que, segundo as especificações da primeira parte do projeto é no máximo 50).

#### Implementação:
```cpp
typedef struct node *Node;
struct node {
    bool is_leaf;
    Node *children;
};

struct trie {
    Node root;
};
```

#### Como funciona:
![Exemplo](https://www.ime.usp.br/~pf/estruturas-de-dados/aulas/figuressw/Chapter5/TrieAnatomy.png)

Como podemos ver na ilustração acima disponiblizada pelo [Ime](https://www.ime.usp.br), cada nível da árvore que se desce corresponde a avançar uma letra na palavra-chave. Desse modo, quando chegamos no nó com `ìs_leaf` igual a `true`, temos a palavra-chave completa.

#### Complexidade:

Sua inserção e busca possuem uma complexidade de tempo `O(m)` onde `m` é o tamanho da palavra-chave em questão, enquanto sua complexidade de espaço e de `delete()` são `O(n*m)`.
## Novas funções

### Buscar palavra chave
#### Visão Geral:

Para buscar os sites contendo uma palavra-chave, rodamos por toda a `Avl` (`O(n)`) e a cada site contendo a palavra, inserimos ele na lista ordenado por sua relevância. Assim, no pior caso temos uma complexidade de tempo de `O(n²)`.

#### Implementação:

```cpp
/* Gets a `List` of `Site`s containing a `keyword` from an `Avl`. */ 
static void _get_sites_with_keyword_in_avl(Node root, List s_list, string keyword) {
    if (root == NULL) return;

    if (site_search_keyword(root->site, keyword) == FOUND) {
        list_insert_site(s_list, root->site);
    }
    _get_sites_with_keyword_in_avl(root->left, s_list, keyword);
    _get_sites_with_keyword_in_avl(root->right, s_list, keyword);
} 

/* Searchs for an `keyword` in a tree and returns a array of `Site` containing it. */ 
List avl_search_keyword(Avl avl, string keyword) {
    assert(avl != NULL);
    List matches = list_init();
    _get_sites_with_keyword_in_avl(avl->root, matches, keyword);
    return matches;
}

```
### Sugestão de sites
* Visão Geral:

Primeiramente, rodamos busca de palavra-chave com a palavra-chave base (`O(n²)`) e rodamos pela lista retornada. Se um site conter ela, rodados por todas suas palavras chaves e adicionamos na `Trie` (`O(10m)`, com `m` sendo o tamanho das palavras-chaves); totalizando  uma complexidade assíntótica de `O(n)` para a primeira parte. Após isso, rodamos novamente por todos os sites da `Avl` (`O(n)`) e para cada site, rodados por todas as suas palavras-chaves e vemos se alguma delas está na `Trie` (`O(10m)`, com `m` sendo o tamanho das palavras-chaves). Se estiver, adicionamos o site a lista de sugestões, ordenado por sua relevância (`O(n)`). Assim no final de tudo, temos uma complexidade de tempo `O(n²)`.

* Implementação:

```cpp
/* Gets a `Trie` instance of all keywords from the sites with `base_keyword`. */
static Trie _get_suggested_keywords(Avl avl,  string base_keyword) {
    List matches_with_base_keyword = avl_search_keyword(avl, base_keyword); // n^2

    Trie suggested_keywords = trie_init(); 
    trie_insert_word(suggested_keywords, base_keyword);

    Site cur_site;
    while ((cur_site = list_get_cur_site(matches_with_base_keyword)) != NULL) {       // n 
        
        int amnt_keywords = site_get_amnt_keywords(cur_site);
        string *keywords = site_get_keywords(cur_site);   
        for (int j = 0; j < amnt_keywords; ++j) {
            trie_insert_word(suggested_keywords, keywords[j]);
        }
        list_go_to_next_site(matches_with_base_keyword);
    }

    list_delete(&matches_with_base_keyword);
    return suggested_keywords;
}

/* Gets a list of `suggested_sites` that contain any word in the `suggested_keywords`. */
static void _search_suggestions_in_avl(Node root, Trie suggested_keywords, List suggested_sites) {
    if (root == NULL) return;

    int amnt_keywords = site_get_amnt_keywords(root->site);
    string *keywords = site_get_keywords(root->site);
    for (int i = 0; i < amnt_keywords; ++i) { 
        if (trie_search_word(suggested_keywords, keywords[i]) == FOUND) {
            list_insert_site(suggested_sites, root->site); 
            break;
        }
    }
    _search_suggestions_in_avl(root->left, suggested_keywords, suggested_sites);
    _search_suggestions_in_avl(root->right, suggested_keywords, suggested_sites);
}

/* Get a suggestion of `Site`s based on a `keyword`. */
static List _get_suggestions(Avl avl, string base_keyword) {
    Trie suggested_keywords = _get_suggested_keywords(avl, base_keyword);
    List suggested_sites = list_init();
    _search_suggestions_in_avl(avl->root, suggested_keywords, suggested_sites);
    trie_delete(&suggested_keywords);

    return suggested_sites;
}

/* Get a suggestion of `Site`s based on a `keyword`. */
List get_suggestions(Avl avl, string keyword, int max_amnt_suggestions) {
    assert(avl != NULL && keyword != NULL);
    
    return _get_suggestions(avl, keyword);
}
```


## Análise Assíntótica das Operações
* **Inserção de sites dado uma chave**: `O(log n)`
* **Busca de sites dado uma chave**: `O(log n)`
* **Remover site dado um chave**: `O(log(n))`
* **Adicionar uma palavra-chave | Atualizar a relevância de um site**: `O(log(n))` - visto que a operação de sub-rotina é uma busca 
* **Mostrar banco de dados**: `O(n)`
* **Buscar palavra-chave**: `O(n²)`
* **Sugestão de Sites**: `O(n²)`

## Modularização
Temos duas bibliotecas auxiliares com funções genéricas para lidar com leitura e arquivos: utils.h e parser.h (O.b.s.: é bom ressaltar que o intuito desses arquivos não é ser um TAD e sim módulos de operações auxiliares). Temos também 4 estruturas de dados: avl.h (`Estrura Principal`), list.h (`Estrutura de retorno p/ busca de palavra chave e sugestão de sites`), trie.h (`Estrutura auxiliar para a sugestão de sites`) e site.h (Correspondente ao TAD item). Por fim, temos o mini_google_bot.h que une todos os comandos oferecidos e a main.c que apenas chama os comandos do bot.
