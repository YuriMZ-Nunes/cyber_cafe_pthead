# Gerenciamento de Recursos em um Internet Café Futurista

## Sobre o Projeto
Este trabalho tem como objetivo o estudo de programação concorrente simulando um ambiente de concorrencia de recursos entre diferentes tipos de clientes com diferentes prioridades.

## Como Rodar
1. Compile:
   ```sh
   gcc cyber_cafe.c -o cyber_cafe -lpthread
   ```
2. Execute:
   ```sh
   ./cyber_cafe
   ```

## O Que Faz
- Simula clientes disputando recursos.
- Controla acesso com semáforos.
- Mostra estatísticas em tempo real.
- Gera um relatório final sobre o uso dos recursos.
