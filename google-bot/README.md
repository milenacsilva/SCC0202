# Projeto 1 - Mini Googlebot

## Participantes
<a href="https://github.com/roselino-quatro"><img src="https://avatars3.githubusercontent.com/u/43501582?s=460&u=607eaa89f7d1d4117ad8e3b2059d7fd24e1c0b45&v=4" title="rafa-quatro" width="80" height="80"></a>
<a href="https://github.com/milenacsilva"><img src="https://avatars2.githubusercontent.com/u/61664263?s=460&v=4" title="milenacsilva" width="80" height="80"></a>

Lourenço de Salles Roselino - **11796805**\
Milena Corrêa da Silva - **11795401**

## Especificações
Compílador GCC 7.5.0 com as flags -std=c99 -Wall -Wextra -Werror -O1

## Instruções Para Uso

## Justificativas

No projeto foi utilizado Lista Encadeada Simples com inserção ordenada por ser mais eficiente em manter os sites ordenados, por consequencia, não utilizamos busca binaria por não ser possivel implementar ela nessa estrutura, logo utilizamos busca linear simples, decidimos pedir como primeiro input um arquivo incial com os sites e manter como possibilidade o uso do comando (1) pelo usuario para inserir novos items, dando flexibilidade de uso de arquivos mas também permitindo entradas durante a execução.

Decidimos separar as funções em 4 duplas de arquivo: utils.h, site.h, commands.h e site_list.h. Site seria equivalente ao TAD item dos trabalhos anteriores, paralelamente, o site_list é equivalente a uma lista ligada, já o utils.h seriam funções genericas que não encaixariam nos grupos anteriores, além disso o commands une todos os comandos oferecidos pelo bot.
