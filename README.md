# erenciamento de Recursos em um Internet Café Futurista

## Sobre o Projeto
Este trabalho tem como objetivo o desenvolvimento de um simulador que gerencie de maneira eficiente a alocação de recursos dentro do ambiente do CyberFlux, utilizando técnicas de sincronização de threads e semáforos para controlar o acesso aos PCs, headsets VR e cadeiras.

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
