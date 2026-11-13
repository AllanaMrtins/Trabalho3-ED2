<div align="center">

# Biblioteca Musical — Estruturas de Dados II

**Sistema de gerenciamento de artistas, álbuns, músicas e playlists desenvolvido em C**

</div>

---

## Sobre o projeto

Este projeto implementa um sistema de biblioteca musical em linguagem C. Os dados são organizados por três estruturas de árvores binárias:

- Árvore Binária de Busca (ABB)

- Árvore AVL

- Árvore vermelha e preta inclinada à esquerda (LLRB, também chamada rubro-negra)

As implementações oferecem funcionalidades semelhantes de cadastro, consulta e remoção. O projeto também permite comparar as árvores em diferentes ordens de inserção, observando tempo, passos, chamadas de funções, comparações, rotações e altura.

O trabalho foi desenvolvido para a disciplina **Estruturas de Dados II**.

## Objetivos

- Aplicar árvores binárias na organização de dados relacionados.

- Implementar operações de inserção, busca e remoção.

- Observar o efeito da ordem de entrada nas árvores.

- Comparar a ABB, a AVL e a árvore vermelha e preta.

- Analisar métricas experimentais além do tempo de execução.

## Funcionalidades do sistema

O programa possui um menu interativo no terminal. As opções podem variar ligeiramente entre as implementações.

| Opção | Funcionalidade | Descrição |
| --- | --- | --- |
| 1 | Cadastrar artista | Registra nome, tipo e estilo musical. |
| 2 | Cadastrar álbum | Associa um álbum a um artista e informa o ano de lançamento. |
| 3 | Cadastrar música | Adiciona uma música a um álbum e registra sua duração em minutos. |
| 4 | Listar artistas | Exibe os artistas cadastrados e seus dados. |
| 5 | Filtrar álbuns por ano | Exibe álbuns lançados no ano informado. |
| 6 | Buscar música | Procura uma música e apresenta os dados relacionados ao artista e ao álbum. |
| 7 | Cadastrar playlist | Cria uma playlist. |
| 8 | Adicionar música à playlist | Adiciona à playlist uma música já cadastrada na biblioteca. |
| 9 | Consultar playlist | Exibe as músicas de uma playlist. |
| 10 | Remover música da playlist | Retira uma música específica de uma playlist. |
| 11 | Remover música do álbum | Remove uma música do álbum, respeitando a verificação de vínculo com playlists. |
| 12 | Executar testes | Executa a bateria de testes da implementação. |
| 13 | Explicar os testes | Apresenta informações sobre a metodologia dos testes, quando disponível no menu. |
| 0 | Sair | Encerra o programa. |

## Organização dos dados

O sistema trabalha com cinco tipos de registros:

- **Artista:** nome, tipo, estilo, quantidade de álbuns e árvore de álbuns.

- **Álbum:** título, ano de lançamento, quantidade de músicas e árvore de músicas.

- **Música:** título e duração em minutos.

- **Playlist:** nome e árvore de músicas associadas.

- **Música da playlist:** identificação do artista, álbum e música.

Os registros são armazenados em nós que possuem uma chave de pesquisa, um identificador de tipo e uma união (`union`) com os dados correspondentes. As árvores são utilizadas para organizar as diferentes categorias de informação.

## Estruturas de dados implementadas

### 1. Árvore Binária de Busca (ABB)

A ABB organiza os nós de acordo com a comparação entre suas chaves:

- Chaves menores são armazenadas à esquerda.

- Chaves maiores são armazenadas à direita.

- A inserção não realiza balanceamento automático.

- A remoção considera os casos de nó folha, nó com um filho e nó com dois filhos.

**Ponto de análise:** com inserções crescentes ou decrescentes, a árvore pode ficar inclinada. Isso aumenta sua altura e pode elevar o custo das operações.

### 2. Árvore AVL

A AVL é uma árvore binária de busca autobalanceada. Cada nó mantém sua altura, e o fator de balanceamento é usado para identificar desequilíbrios.

A implementação utiliza:

- Cálculo e atualização de alturas.

- Cálculo do fator de balanceamento.

- Rotações à esquerda e à direita, simples ou combinadas.

- Rebalanceamento após inserções e remoções.

**Ponto de análise:** o balanceamento mantém a altura controlada e favorece as operações de busca, inserção e remoção.

### 3. Árvore vermelha e preta (LLRB)

A árvore vermelha e preta implementada é uma variante inclinada à esquerda (*Left-Leaning Red-Black*, LLRB). Ela usa cores e rotações para manter a árvore balanceada.

A implementação contém:

- Nós vermelhos e pretos.

- Verificação das cores dos nós.

- Rotações à esquerda e à direita.

- Inversão de cores.

- Ajustes durante a inserção e a remoção.

**Ponto de análise:** as regras de cor e balanceamento evitam que a árvore fique excessivamente alta. As rotações e chamadas auxiliares são contabilizadas nos testes.

## Experimentos e testes

Cada implementação possui sua própria bateria de testes, acessível pela opção **12** do menu. Os testes usam 100 nomes de artistas sem repetição e os inserem em três ordens:

1. **Ordem crescente:** nomes em ordem alfabética.

1. **Ordem decrescente:** os mesmos nomes na ordem inversa.

1. **Ordem aleatória:** os nomes embaralhados.

Em cada cenário, os testes constroem uma árvore separada. A árvore usada nos experimentos não altera a biblioteca musical cadastrada pelo usuário.

### Medição das inserções

Os testes apresentam duas medidas de tempo:

- **Tempo médio do lote:** tempo para inserir os 100 artistas, calculado a partir de 100 repetições do lote completo.

- **Tempo individual por artista:** tempo médio da inserção de cada artista, calculado em 100 repetições do mesmo cenário.

A preparação dos dados fica fora das regiões cronometradas. Na tabela do terminal, **passos** representa os nós visitados. A chamada recursiva final que chega ao ponteiro `NULL` também é contada em `inserirArvBB`, `inserirArvAVL` ou `inserirArvVP`; por isso, esse número pode ser um a mais que os passos.

As métricas específicas variam por estrutura:

- **ABB:** chamadas de inserção e comparações; como não há balanceamento, as rotações são zero.

- **AVL:** chamadas das funções de inserção, altura, fator de balanceamento, atualização de altura e rotações.

- **Vermelha e preta:** chamadas de inserção, comparações, rotações e funções auxiliares de cor.

### Medição das buscas e testes complementares

Os mesmos 10 nomes de artistas são buscados em cada ordem de inserção. Cada busca é repetida **1.000.000 de vezes** para estimar seu tempo médio. Também são testados:

- Cadastro de um artista duplicado.

- Busca de nomes ausentes.

- Quantidade de artistas armazenados.

- Altura final da árvore.

- Propriedades de balanceamento, quando aplicáveis.

### Métricas analisadas

| Métrica | O que representa |
| --- | --- |
| Tempo médio do lote | Tempo médio para executar um lote de operações. |
| Tempo individual | Tempo médio da operação de inserção de cada artista, com repetição do cenário. |
| Passos | Nós visitados no caminho percorrido pela operação. |
| Chamadas | Quantidade de chamadas realizadas pelas funções monitoradas. |
| Comparações (`strcmp`) | Comparações entre chaves durante inserções ou buscas, conforme a métrica registrada por cada implementação. |
| Altura | Número de níveis do maior caminho da raiz até uma folha. |
| Rotações | Operações de rotação usadas para balancear AVL e árvore vermelha e preta; não são usadas pela ABB. |
| Validação | Conferência da quantidade de nós e das propriedades estruturais ou de balanceamento. |

Os tempos podem variar conforme o computador, o sistema operacional e as condições de execução. Pequenas diferenças ou valores atípicos podem ocorrer em medições muito curtas. Por isso, a análise não deve considerar apenas o tempo: passos, chamadas, comparações, rotações e altura também são importantes.

### Arquivos de resultados

Ao executar os testes, o programa tenta gerar um arquivo CSV na pasta da implementação:

| Implementação | Arquivo gerado |
| --- | --- |
| ABB | `resultados_arvbb.csv` |
| AVL | `resultados_avl.csv` |
| Vermelha e preta | `resultados_arvvp.csv` |

Os CSVs incluem os registros por operação e os resumos dos cenários. Podem ser abertos em editores de planilhas para facilitar a análise. Caso não seja possível criar o arquivo, os testes continuam apresentando os resultados no terminal.

## Estrutura do repositório

```
Trabalho3-ED2/
│
├── ArvBB/
│   ├── estruturas.h
│   ├── funcoes.c
│   ├── main.c
│   └── testes.c
│
├── ArvAVL/
│   ├── estruturas.h
│   ├── funcoes.c
│   ├── main.c
│   └── testes.c
│
├── Arv-vermelha-e-preta/
│   ├── estruturas.h
│   ├── funcoes.c
│   ├── main.c
│   └── testes.c
│
└── README.md
```

### Responsabilidade dos arquivos

| Arquivo | Responsabilidade |
| --- | --- |
| `estruturas.h` | Declara as estruturas, os tipos de dados e os protótipos das funções. |
| `funcoes.c` | Implementa as operações da árvore e as funcionalidades da biblioteca musical. |
| `main.c` | Contém o menu interativo e a execução das opções escolhidas. |
| `testes.c` | Implementa os experimentos, as métricas e a geração dos arquivos CSV. |

> **Importante:** cada pasta é uma implementação independente. Compile os arquivos de uma única pasta juntos; não compile os três `main.c` em um mesmo executável.

## Tecnologias e ferramentas

- **Linguagem:** C, padrão C99.

- **Bibliotecas padrão:** `stdio.h`, `stdlib.h`, `string.h`, `ctype.h` e `time.h`.

- **Biblioteca específica do Windows:** `windows.h`, incluída condicionalmente para usar um contador de tempo de alta resolução.

- **Compilador:** GCC ou outro compilador C compatível.

No Windows, as medições usam `QueryPerformanceCounter` quando disponível. Em outros ambientes, o código usa `clock()`.

## Como executar

### Pré-requisitos

1. Ter um compilador C instalado, como o GCC.

1. Ter um terminal ou ambiente de desenvolvimento configurado.

1. Clonar ou baixar o repositório.

### 1. Clonar o repositório

```bash
git clone <URL_DO_REPOSITORIO>
cd Trabalho3-ED2
```

### 2. Escolher a implementação

Entre na pasta da árvore que deseja executar:

```bash
cd ArvBB
```

Para testar outra árvore, use `cd ArvAVL` ou `cd Arv-vermelha-e-preta` a partir da pasta `Trabalho3-ED2`.

### 3. Compilar

No Linux, macOS ou outro ambiente com GCC:

```bash
gcc -std=c99 main.c funcoes.c testes.c -o biblioteca
```

No Windows com GCC/MinGW, dentro da pasta da implementação:

```
gcc -std=c99 main.c funcoes.c testes.c -o biblioteca.exe
```

### 4. Executar

No Linux ou macOS:

```bash
./biblioteca
```

No Windows PowerShell:

```
.\biblioteca.exe
```

Depois de iniciar o programa, use o menu para cadastrar e consultar dados ou executar os testes. Para gerar os resultados de desempenho, selecione a opção **12**.

## Conceitos praticados

- Tipos abstratos de dados e estruturas dinâmicas.

- Alocação e liberação de memória.

- Ponteiros e recursividade.

- Árvores binárias de busca.

- Árvores AVL e fatores de balanceamento.

- Árvores vermelhas e pretas e propriedades de coloração.

- Rotações e operações de remoção.

- Organização de dados com `struct` e `union`.

- Análise de algoritmos e experimentação.

- Medição de tempo e contagem de operações.

- Exportação de resultados para CSV.

## Contexto acadêmico

Projeto desenvolvido para a disciplina **Estruturas de Dados II**, com foco na implementação e análise comparativa de estruturas de árvores em linguagem C.

---

<div align="center">

**Estruturas de Dados II • Linguagem C**

*Organização, busca e balanceamento de dados.*

</div>
