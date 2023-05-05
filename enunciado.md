# Descrição do Trabalho:

Implementar a arquitetura para um conjunto de instruções (ISA, Instruction-Set
Architecture) composta por Parte de Controle e Parte Operativa, compatíveis com
a filosofia RISC (Reduced Instruction-Set Computer) e que executam em “Pipeline”
o seguinte conjunto de instruções:

- AND —> operação booleana AND
- OR —> operação booleana OR
- XOR —> operação booleana XOR
- NOT —> operação booleana NOT
- CMP —> comparação
- ADD —> soma
- SUB —> subtração
- LD —> leitura em memória
- ST —> armazenamento em memória
- J —> salto incondicional
- JN —> salto condicional; salta se (N)egativo
- JZ —> salto condicional; salta se (Z)ero

O pipeline pode ser implementado com 4 ou 5 estágios.

# Decisões de projeto:

- o tamanho da palavra do processador;
- o formato da palavra de instrução;
- os modos de endereçamento de operandos;
- o tamanho do banco de registradores;
- o tamanho das memórias de instruções e de dados;
- o número e tipos de barramentos (ou canais dedicados) da parte operativa; e
- a organização do pipeline.

# O trabalho consiste de:

- Implementação do modelo da arquitetura em linguagem para descrição de hardware
  (VHDL ou SystemC);

- Resultados de simulações da execução de instruções de pelo menos 3 algoritmos
  na arquitetura. Os resultados podem ser apresentados em diagramas de forma de
  onda e/ou textual. Esses resultados serão colocados no relatório e
  apresentados em sala de aula;

- Relatório explicando e exemplificando a implementação da arquitetura e
  justificando as decisões de projeto acima elencadas (seção 2). O relatório
  deve conter pelo menos:

  1. os diagramas de bloco da parte de controle;

  2. os diagramas de bloco da parte operativa; e

  3. análises de desempenho (ciclos de relógio necessários à execução das
     instruções) em função dos comportamentos do pipeline. Entende-se por
     comportamentos do pipeline, os tratamentos implementados para as
     dependências de dados e de controle;

# Apresentação dias 08 e 10/05 em sala de aula, exemplificando o funcionando do modelo de processador implementado. A apresentação deve mostrar:

- Os diagramas de bloco da organização do processador, os quais foram projetados
  de acordo com as decisões de projeto (seção 2) e o número de estágios do
  Pipeline; e

- Resultados de simulação (telas ou demonstração).

# Entrega dia 07/05/22 via tarefa SIGAA.
