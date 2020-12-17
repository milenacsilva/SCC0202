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
* **Inserção de sites a partir de um csv**: mesmo após a inicialização do programa, pode-se adicionar multiplos sites de uma vez no formato csv 
* **Mostrar o banco de dados**: mostra todos os sites contidos no bot, na ordem de sua chave
* **Salvar nova lista de sites ao finalizar programa**: Pode-se salvar toda a lista de sites gerada durante a execução como um arquivo csv. 

## Estruturas Utilizadas

### Estrutura principal
Para a estrutura principal, foi utilizado uma árvore `Avl`, já que, tratando-se de uma árvore de busca auto-balanceada, ela mantêm o custo das operações constante.

### Estrutura de retorno p/ busca de palavra chave e sugestão de sites
Para o retorno dessas funções, foi se utilizado uma `Lista Encadeada Simples` visto que precisamos ordená-la por relevância (a utilização de uma avl não seria possível pois diferentes sites podem ter a mesma relevância). Nesse sentido, para não precisarmos trocar todos os sites de lugar, foi utilizado uma inserção ordenada. Além disso, como as únicas operações que temos que fazer nessa estrutura é inserir ordenado, printar os sites e deletar a instância criada, um encadeamento simples foi o suficiente. Por fim, considerando que não sabemos o total de sites a serem adicionados, foi utilizado alocaçâo dinâmica. 

### Estrutura auxiliar para a sugestão de sites
Para auxiliar a sugestão de sites, utilizamos uma `Trie` ou ainda `Árvore de prefixos`. O motivo de utilização dessa estrutura foi que, mesmo utilizando bastante memória, todas as operações tem uma complexidade de tempo `O(m)` onde `m` é o tamanho da palavra chave (que, segundo as especificações da primeira parte do projeto é no máximo 50).

## Novas funções

### Buscar palavra chave
Para buscar os sites contendo uma palavra-chave, rodamos por toda a `Avl` (`O(n)`) e a cada site contendo a palavra, inserimos ele na lista ordenado por sua relevância. Assim, no pior caso temos uma complexidade de tempo de `O(n²)`.

### Sugestão de sites
Primeiramente, rodamos busca de palavra-chave com a palavra-chave base (`O(n²)`) e rodamos pela lista retornada. Se um site conter ela, rodados por todas suas palavras chaves e adicionamos na `Trie` (`O(10m)`, com `m` sendo o tamanho das palavras-chaves); totalizando  uma complexidade assíntótica de `O(n)` para a primeira parte. Após isso, rodamos novamente por todos os sites da `Avl` (`O(n)`) e para cada site, rodados por todas as suas palavras-chaves e vemos se alguma delas está na `Trie` (`O(10m)`, com `m` sendo o tamanho das palavras-chaves). Se estiver, adicionamos o site a lista de sugestões, ordenado por sua relevância (`O(n)`). Assim no final de tudo, temos uma complexidade de tempo `O(n²)`.


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
