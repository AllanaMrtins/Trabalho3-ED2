
<div align="center">

# Biblioteca Musical — Estruturas de Dados II

**Sistema de gerenciamento de artistas, álbuns, músicas e playlists desenvolvido em C**

</div>

---

## Sobre o projeto

Este projeto implementa um sistema de biblioteca musical em linguagem C, utilizando três estruturas diferentes de árvores binárias para organizar e consultar os dados:

- Árvore Binária de Busca (ABB)
- Árvore AVL
- Árvore Vermelha e preta (rubro-negra)

As três implementações oferecem as mesmas funcionalidades principais de gerenciamento musical. A proposta é permitir a comparação do comportamento das estruturas, especialmente em operações de inserção e busca, observando métricas como tempo de execução, quantidade de chamadas, comparações e altura das árvores.

O projeto foi desenvolvido como atividade prática da disciplina **Estruturas de Dados II**.

## Objetivos

- Aplicar árvores binárias na organização de dados relacionados.
- Implementar operações de inserção, busca e remoção.
- Utilizar árvores balanceadas para reduzir o impacto de entradas ordenadas.
- Comparar o desempenho da ABB, AVL e Rubro-Negra.
- Analisar métricas experimentais em diferentes cenários de inserção.

## Funcionalidades do sistema

O programa possui um menu interativo no terminal com as seguintes operações:

| Opção | Funcionalidade | Descrição |
|:---:|---|---|
| 1 | Cadastrar artista | Registra nome, tipo e estilo musical. |
| 2 | Cadastrar álbum | Associa um álbum a um artista e informa o ano de lançamento. |
| 3 | Cadastrar música | Adiciona uma música a um álbum e registra sua duração em minutos. |
| 4 | Listar artistas | Exibe os artistas cadastrados e seus dados. |
| 5 | Filtrar álbuns por ano | Exibe os álbuns de determinado artista lançados no ano informado. |
| 6 | Buscar música | Procura uma música e apresenta os dados relacionados ao artista e ao álbum. |
| 7 | Cadastrar playlist | Cria uma playlist. |
| 8 | Adicionar música à playlist | Adiciona à playlist uma música já cadastrada na biblioteca. |
| 9 | Consultar playlist | Exibe as músicas de uma playlist. |
| 10 | Remover música da playlist | Retira uma música específica de uma playlist. |
| 11 | Remover música do álbum | Remove uma música do álbum, respeitando a verificação de vínculo com playlists. |
| 12 | Executar testes | Executa a bateria de testes da estrutura correspondente. |
| 13 | Explicar os testes | Apresenta no terminal a metodologia utilizada nos experimentos. |
| 0 | Sair | Encerra o programa. |

## Organização dos dados

O sistema trabalha com cinco tipos de registros:

- **Artista:** nome, tipo, estilo, quantidade de álbuns e árvore de álbuns.
- **Álbum:** título, ano de lançamento, quantidade de músicas e árvore de músicas.
- **Música:** título e duração em minutos.
- **Playlist:** nome e árvore de músicas associadas.
- **Música da playlist:** identificação do artista, álbum e música.

Os registros são armazenados em nós que possuem uma chave de pesquisa, um identificador de tipo e uma união (`union`) com os dados correspondentes. Dessa forma, cada implementação reutiliza a estrutura de árvore para organizar diferentes categorias de informação.

## Estruturas de dados implementadas

### 1. Árvore Binária de Busca (ABB)

A ABB organiza os nós de acordo com a comparação entre suas chaves:

- Chaves menores são armazenadas à esquerda.
- Chaves maiores são armazenadas à direita.
- A inserção não realiza balanceamento automático.
- A remoção considera os casos de nó folha, nó com um filho e nó com dois filhos.

**Ponto de análise:** quando os dados são inseridos em ordem crescente ou decrescente, a árvore pode ficar inclinada, aumentando sua altura e o custo das operações.

### 2. Árvore AVL

A AVL é uma árvore binária de busca autobalanceada. Cada nó mantém sua altura, e o fator de balanceamento é utilizado para identificar desequilíbrios.

A implementação utiliza:

- Cálculo e atualização de alturas.
- Fator de balanceamento.
- Rotação simples à direita.
- Rotação simples à esquerda.
- Rebalanceamento após inserções e remoções.

**Ponto de análise:** o balanceamento mantém a árvore com altura controlada, favorecendo operações de busca, inserção e remoção.

### 3. Árvore Rubro-Negra

A árvore Rubro-Negra é uma árvore binária de busca autobalanceada que utiliza cores para manter suas propriedades estruturais.

A implementação contém:

- Nós vermelhos e pretos.
- Verificação da cor dos nós.
- Rotações à esquerda e à direita.
- Inversão de cores.
- Ajustes durante a inserção.
- Ajustes durante a remoção.

**Ponto de análise:** as regras de coloração e balanceamento evitam que a árvore se torne excessivamente alta.

## Experimentos e testes

Cada implementação possui uma bateria de testes própria, acessível pela opção **12** do menu. Os experimentos utilizam 100 nomes de artistas gerados para comparar três cenários de inserção:

1. **Ordem crescente:** os nomes são inseridos em ordem alfabética.
2. **Ordem decrescente:** os mesmos nomes são inseridos na ordem inversa.
3. **Ordem aleatória:** os nomes são embaralhados antes da inserção.

Além das inserções, os testes realizam buscas dos mesmos 10 artistas nos três cenários e verificam situações complementares, como:

- Tentativa de cadastrar um artista duplicado.
- Busca por nomes que não existem na árvore.
- Quantidade de nós armazenados.
- Altura final da árvore.
- Validação das propriedades de balanceamento, conforme a estrutura.

### Métricas analisadas

| Métrica | O que representa |
|---|---|
| Tempo médio | Tempo médio gasto por operação ou por lote de operações. |
| Chamadas | Quantidade de chamadas realizadas pelas rotinas analisadas. |
| Comparações | Quantidade de comparações entre chaves. |
| Altura | Comprimento do maior caminho da raiz até uma folha. |
| Rotações | Operações de rotação realizadas para manter o balanceamento. |
| Validação | Verificação da quantidade de nós e das propriedades da árvore. |

Os tempos podem variar conforme o computador e as condições de execução. Por isso, a análise não deve considerar apenas o tempo: altura, comparações, chamadas e validações também são importantes.

### Arquivos de resultados

Ao executar os testes, o programa tenta gerar um arquivo CSV na pasta da implementação:

| Implementação | Arquivo gerado |
|---|---|
| ABB | `resultados_arvbb.csv` |
| AVL | `resultados_avl.csv` |
| Rubro-Negra | `resultados_arvvp.csv` |

Os arquivos podem ser abertos em editores de planilhas para facilitar a análise e a comparação dos resultados. Caso não seja possível criar o CSV, os testes continuam apresentando as informações no terminal.

## Estrutura do repositório

```text
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
|---|---|
| `estruturas.h` | Declara as estruturas, tipos de dados e protótipos das funções. |
| `funcoes.c` | Implementa as operações da árvore e as funcionalidades da biblioteca musical. |
| `main.c` | Contém o menu interativo e a execução das opções escolhidas. |
| `testes.c` | Implementa os experimentos, as métricas e a geração dos arquivos CSV. |

> **Importante:** cada pasta é uma implementação independente. Os arquivos de uma pasta devem ser compilados juntos; não compile os três `main.c` em um único executável.

## Tecnologias e ferramentas

- **Linguagem:** C
- **Bibliotecas padrão:** `stdio.h`, `stdlib.h`, `string.h`, `ctype.h` e `time.h`
- **Biblioteca específica do Windows:** `windows.h`, utilizada para obter medidas de tempo de alta resolução na implementação que oferece esse suporte.
- **Compilador:** GCC (ou outro compilador C compatível).

## Como executar

### Pré-requisitos

1. Ter um compilador C instalado, como o GCC.
2. Ter um terminal ou ambiente de desenvolvimento configurado.
3. Clonar ou baixar este repositório.

### 1. Clonar o repositório

```bash
git clone <URL_DO_REPOSITORIO>
```

Entre na pasta do projeto:

```bash
cd Trabalho3-ED2
```

### 2. Escolher a implementação

Entre na pasta da árvore que deseja executar.

**ABB:**

```bash
cd ArvBB
```

**AVL:**

```bash
cd ArvAVL
```

**Rubro-Negra:**

```bash
cd Arv-vermelha-e-preta
```

### 3. Compilar

Execute o comando correspondente dentro da pasta escolhida:

```bash
gcc main.c funcoes.c testes.c -o biblioteca
```

### 4. Executar

No Linux:

```bash
./biblioteca
```

No Windows:

```bash
biblioteca.exe
```

Depois da execução, utilize as opções apresentadas no menu para cadastrar e consultar os dados ou executar os testes.

## Comparação teórica das estruturas

| Estrutura | Busca | Inserção | Remoção | Balanceamento automático |
|---|---:|---:|---:|:---:|
| ABB | O(h) | O(h) | O(h) | Não |
| AVL | O(log n) | O(log n) | O(log n) | Sim |
| Rubro-Negra | O(log n) | O(log n) | O(log n) | Sim |

`n` representa a quantidade de nós e `h` representa a altura da árvore. Na ABB, a altura pode chegar a `n` em casos desfavoráveis; nas árvores AVL e Rubro-Negra, as regras de balanceamento mantêm altura logarítmica.

## Conceitos praticados

- Tipos abstratos de dados e estruturas dinâmicas.
- Alocação e liberação de memória.
- Ponteiros e recursividade.
- Árvores binárias de busca.
- Árvores AVL e fatores de balanceamento.
- Árvores Rubro-Negras e propriedades de coloração.
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
