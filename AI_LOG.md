# Uso de IA

## Registro

Usei um assistente de IA (ChatGPT) durante a pratica integrada A para:

1. Entender o contrato do exercicio (quem lanca, quem propaga, quem captura) antes de editar os arquivos.
2. Revisar a ordem das capturas em C++ e Python apos implementar.
3. Redigir o arquivo `docs/decisoes.md`.

## O que foi aceito

- Sugestao de lancar `FalhaCalibracao` em `adquirir` (C++ e Python) **depois** da checagem de disponibilidade e **antes** do `return fonte.valor()` / `fonte.valor()`.
- Sugestao de colocar a captura de `FalhaCalibracao` **antes** da de `FalhaLeitura` em `executarCiclo` / `executar_ciclo`, justificada pela relacao de heranca.
- Texto do `docs/decisoes.md`.

## O que foi rejeitado

- Nenhuma sugestao foi rejeitada nesta pratica.

## Justificativa tecnica

A ordem das capturas e obrigatoria porque `FalhaCalibracao` deriva de `FalhaLeitura`: inverter faria a falha especifica cair no rotulo generico `"indisponivel"`. A verificacao de disponibilidade continua vindo primeiro para preservar o comportamento previsto pelo contrato. Nenhum teste foi enfraquecido; a saida de `make test ETAPA=A` permanece verde em C++ e Python.