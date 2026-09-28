# Decisoes da pratica A

## 1. Quem lanca, quem propaga, quem recupera

C++: quem lanca e a funcao `adquirir` (em `include/estacao.hpp`), com `throw FalhaCalibracao("fonte sem calibracao")` quando a fonte esta disponivel mas nao calibrada; a indisponibilidade continua sendo lancada antes como `FalhaLeitura`. Quem apenas propaga e `lerServico`, que so chama `adquirir` sem capturar nada, deixando a excecao atravessar a camada. Quem recupera e `executarCiclo`, que captura `FalhaCalibracao` (e, como base, `FalhaLeitura`) e devolve `{false, 0, "calibracao"}`.

Python: quem lanca e `adquirir`, com `raise FalhaCalibracao("fonte sem calibracao")`. Quem propaga e `ler_servico`, que retorna direto a chamada a `adquirir` sem `try`. Quem recupera e `executar_ciclo`, com `except FalhaCalibracao: return False, 0, "calibracao"`. Chamada observada em `make run`: `Sem calibracao: sem leitura (calibracao) | sessoes: 0`.

## 2. Ordem das capturas e liberacao da sessao

`FalhaCalibracao` deriva de `FalhaLeitura`. Em C++ e Python, um bloco `catch`/`except` da base capturaria tambem as derivadas. Se `FalhaLeitura` viesse primeiro, todo caso de calibracao cairia no rotulo `"indisponivel"`, perdendo a informacao especifica. Por isso a captura especifica (`FalhaCalibracao`) vem antes da generica (`FalhaLeitura`).

A sessao e liberada antes de a excecao sair de `adquirir`. Em C++, o destrutor de `Sessao` roda quando o escopo de `adquirir` e desenrolado (RAII) durante a propagacao. Em Python, o bloco `finally: sessao.fechar()` executa antes de a excecao subir para `ler_servico` e `executar_ciclo`. Por isso a saida mostra sempre `sessoes: 0`, inclusive nos casos de falha.

## 3. Mesmo contrato, duas fontes

`FonteNivel` e `FonteConstante` implementam `IFonteLeitura`, oferecendo `valor()` e `unidade()`. Nada em `adquirir`, `lerServico` ou `executarCiclo` menciona classes concretas: todos recebem `const IFonteLeitura&` (ou `IFonteLeitura` em Python) e consultam apenas o contrato. Exemplo observado em `make run`: `Fonte simulada: 42.5 % | sessoes: 0` vem de `FonteConstante`, e `Leitura: 20 | sessoes: 0` vem de `FonteNivel` sobre `SensorNivel`. As duas passam pela mesma funcao `adquirir` e pelo mesmo ciclo de captura, sem ramificacoes por tipo.