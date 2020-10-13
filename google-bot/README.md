# Projeto 1 - Mini Googlebot

## Participantes
<a href="https://github.com/roselino-quatro"><img src="https://avatars3.githubusercontent.com/u/43501582?s=460&u=607eaa89f7d1d4117ad8e3b2059d7fd24e1c0b45&v=4" title="rafa-quatro" width="80" height="80"></a>
<a href="https://github.com/milenacsilva"><img src="https://avatars2.githubusercontent.com/u/61664263?s=460&v=4" title="milenacsilva" width="80" height="80"></a>

Lourenço de Salles Roselino - **11796805**\
Milena Corrêa da Silva - **11795401**

## Especificações
Compílador GCC 7.5.0 com as flags -std=c99 -Wall -Wextra -Werror -O1

## Instruções Para Uso

Para realizar a compilação normal basta utilizar o comando "make", podendo utilizar também "make debug" para realizar execução com informaç~oes extras fornecidas pelo Valgrind, depois de compilado simplesmente executar e você será apresentado a seguinte tela incial:
```
  +-------------------------------------------------+
  |               INCIANDO GOOGLE BOT...            |
  +-------------------------------------------------+
  Arquivo de entrada: 
```

O usuario pode inserir um arquivo de entrada customizado ou utilizar um dos que se encontram dentro da pasta test-cases. Inserido o nome do arquivo se entra no loop principal:

```
O que você deseja fazer?
1 - Inserir sites
2 - Remover site
3 - Atualizar a relevância de um site
4 - Adicionar uma palavra-chave a um site
5 - Mostrar banco de dados
6 - Finalizar o programa
```

| Comando      | Função |
| :---        |    :----: |
| 1 - Inserir sites    | Permite a inserção de novos sites passado o nome de outro arquivo de entrada    |
| 2 - Remover site   | Remove um único site passado sua chave     |
| 3 - Atualizar a relevância de um site   | Modifica a relevância de um site passado a chave e um novo valor     |
| 4 - Adicionar uma palavra-chave a um site   | Adciona uma nova palavra chave a um site passado a chave e a palavra chave, apenas se o site ainda não ultrapassou o limite     |
| 5 - Mostrar banco de dados   | Comando que não foi pedido nas especificações do projeto, mas permite vizualização facil de todas as entradas no rpojeto     |
| 6 - Finalizar o programa   | Finaliza o programa, dando a oportunidade do usuario salvar o estado atual do banco de dados     |

## Justificativas

No projeto foi utilizado Lista Encadeada Simples com inserção ordenada por ser mais eficiente em manter os sites ordenados, por consequencia, não utilizamos busca binaria por não ser possivel implementar ela nessa estrutura, logo utilizamos busca linear simples. Decidimos pedir como primeiro input um arquivo incial com os sites e manter como possibilidade a adição de arquivos extras no resto da execução. Além disso foi necessário adcionar nossas versões dos comandos strdup e strndup, já que esses não são padronizados pelo C99 mas são úteis para nossa implementação.

Decidimos separar as funções em 4 duplas de arquivo: utils.h, site.h, commands.h e site_list.h. Site seria equivalente ao TAD item dos trabalhos anteriores, paralelamente, o site_list é equivalente a uma lista ligada, já o utils.h seriam funções genericas que não encaixariam nos grupos anteriores, além disso o commands une todos os comandos oferecidos pelo bot.
