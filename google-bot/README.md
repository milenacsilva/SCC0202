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
Assim que o programa inicia, é pedido o nome do arquivo (.csv) de entrada de onde os primeiros sites do banco de dados seram lidos. Após isso, o usuário encrontra-se livre para fazer as seguintes operações:

```
O que você deseja fazer? (Digite o número equivalente ao comando)
0 - Inserir site manualmente
1 - Inserir sites a partir de um csv
2 - Remover site
3 - Atualizar a relevância de um site
4 - Adicionar uma palavra-chave a um site
5 - Mostrar banco de dados
6 - Finalizar o programa
```

A seguir, disponibilizamos a tabela explicando cada uma das funções/operações disponveis:

| Comando      | Função | Complexidade |
| :---        | --- |    :----: |
| Inserir site manualmente | Permite a inserção de um novo site diretamente pelo `stdin`, com as entradas fornecidas uma por uma pelo usuário | (Inserção ordenada): O(n) |
| Inserir sites a partir de um csv    | Permite a inserção de novos sites passado o nome de outro arquivo csv. Vale ressaltar que os sites antigos continuam no programa. Dito isso, essa é só uma maneira simplificada para o usuário inserir vários sites de uma vez | (Inserção ordenada): O(n) |
| Remover site   | Remove um único site passado sua chave     | (busca necessária): O(n) |
| Atualizar a relevância de um site   | Modifica a relevância de um site passado sua chave e um novo valor     | (busca necessária): O(n) |
| Adicionar uma palavra-chave a um site   | Adiciona uma nova palavra chave a um site passado sua chave e a palavra chave a ser adicionada, apenas se o site ainda não ultrapassou o limite de palavras chave|  (busca necessária) : O(n) |
| Mostrar banco de dados   | Comando que não foi pedido nas especificações do projeto, mas permite vizualização fácil de todas as entradas no projeto    |
| Finalizar o programa   | Finaliza o programa, dando a oportunidade do usuario salvar o estado atual do banco de dados     |


## Estruturas e justificativas

No projeto foi utilizado **Lista Encadeada Simples** por ser mais eficiente no quesito de ordenação, já que não precisamos trocar todos os sites de lugar para fazer inserção ordenada (pedida nas especificações do projeto). Dito isso, para as outra operações envolvendo busca, foi utilizada uma **sequencial simples** devido a estrutura  escolhida. Nesse sentido, é possível perceber que chegamos em um impasse: a inserção na lista encadeada de maneira ordenada é mais eficiente, porém ao utilizar esse tipo de estrutura perdemos a possibilidade de usar uma busca binária. Numa futura remodelação do projeto, idealmente seria aplicado uma àrvore binária de busca para resolver esse dilema, porém como ainda não vimos esse método, decidimos focar na inserção de dados e, logo, na lista encadeada.

Decidimos pedir como primeiro input um arquivo incial com os sites e manter como possibilidade a adição de arquivos extras no resto da execução para deixar mais "_user friendly_". Além disso, foi necessário adcionar nossas versões dos comandos strdup e strndup, já que esses não são padronizados pelo C99 mas são úteis para nossa implementação.

## Modularização

Separamos as funções em 4 duplas de arquivo: utils.h, site.h, commands.h e site_list.h. **site.h** seria equivalente ao **TAD item** dos trabalhos anteriores; o **site_list.h**, a implementação de uma lista ligada; o **utils.h** seriam funções genéricas que não encaixariam nos grupos anteriores, mas que seriam utilizados no decorrer do código (O.b.s.: é bom ressaltar que o intuito desse arquivo não é ser um TAD e sim um módulo de operações auxiliares); e o **mini_google_bot.h** que une todos os comandos oferecidos e a implementação do bot em si. Por fim, temos o arquivo **main.c** que apenas chama os comandos do bot
