# miniRT - Requisitos do Projeto

## Informações Gerais

| Item | Descrição |
|------|-----------|
| **Nome do programa** | miniRT |
| **Arquivos para entregar** | Todos os seus arquivos |
| **Makefile** | all, clean, fclean, re, bonus |
| **Argumentos** | Uma cena no formato *.rt |
| **Funções externas autorizadas** | open, close, read, write, printf, malloc, free, perror, strerror, exit<br>Todas as funções da biblioteca math (compilar com flag -lm)<br>Todas as funções da biblioteca MiniLibX<br>gettimeofday() |
| **Libft autorizada** | Sim |
| **Descrição** | O objetivo do programa é gerar imagens usando o protocolo Raytracing. Essas imagens representarão cenas vistas de ângulos e posições específicos, definidas por objetos geométricos simples, cada uma com seu próprio sistema de iluminação. |

---

## Requisitos Obrigatórios

### Biblioteca e Janela

- Usar a biblioteca MiniLibX (versão do sistema operacional ou a partir das fontes)
- O gerenciamento da janela deve permanecer fluido: alternar para outra janela, minimizar, etc.
- O uso de images da biblioteca minilibX é altamente recomendado

### Objetos Geométricos

Pelo menos três objetos geométricos simples são obrigatórios:

1. **Plano**
2. **Esfera**
3. **Cilindro**

#### Tratamento de Objetos

- Todas as possíveis interseções e o interior dos objetos devem ser tratados corretamente
- O programa deve redimensionar as propriedades exclusivas dos objetos:
  - Diâmetro para uma esfera
  - Largura e altura para um cilindro

### Transformações

O programa deve aplicar transformações de:

- **Translação**: objetos, luzes e câmeras
- **Rotação**: objetos, luzes e câmeras
  - Exceção: esferas e luzes não podem ser giradas

### Sistema de Iluminação

Implementar os seguintes elementos de iluminação:

- **Brilho do ponto de luz**
- **Sombras duras**
- **Iluminação ambiente** (objetos nunca estão completamente no escuro)
- **Iluminação difusa**

---

## Controles de Janela

O programa deve respeitar as seguintes regras:

- Pressionar **ESC** deve fechar a janela e encerrar o programa de forma limpa
- Clicar no **X vermelho** na moldura da janela deve fechar a janela e encerrar o programa de forma limpa

---

## Formato do Arquivo de Cena (.rt)

### Regras Gerais

- O programa deve receber como primeiro argumento um arquivo de descrição de cena com a extensão **.rt**
- Cada tipo de elemento pode ser separado por um ou mais finais de linha
- Cada tipo de informação de um elemento pode ser separado por um ou mais espaços
- Cada tipo de elemento pode ser definido em qualquer ordem no arquivo
- Elementos definidos por uma letra maiúscula só podem ser declarados **uma vez** na cena
- A primeira informação para cada elemento é o identificador de tipo (composto por um ou dois caracteres), seguido por informações específicas em ordem estrita

### Elementos da Cena

#### 1. Iluminação Ambiente (A)

```
A 0.2 255,255,255
```

- **Identificador**: A
- **Taxa de iluminação ambiente**: intervalo [0.0, 1.0]
- **Cores RGB**: intervalo [0-255]

#### 2. Câmera (C)

```
C -50.0,0,20 0,0,1 70
```

- **Identificador**: C
- **Coordenadas x,y,z do ponto de vista**: -50.0,0,20
- **Vetor de orientação 3D normalizado**: intervalo [-1,1] para cada eixo
- **FOV (Campo de Visão)**: graus horizontais no intervalo [0,180]

#### 3. Luz (L)

```
L -40.0,50.0,0.0 0.6 10,0,255
```

- **Identificador**: L
- **Coordenadas x,y,z do ponto de luz**: -40.0,50.0,0.0
- **Taxa de brilho da luz**: intervalo [0.0, 1.0]
- **Cores RGB** (não utilizado na parte obrigatória): intervalo [0-255]

#### 4. Esfera (sp)

```
sp 0.0,0.0,20.6 12.6 10,0,255
```

- **Identificador**: sp
- **Coordenadas x,y,z do centro**: 0.0,0.0,20.6
- **Diâmetro**: 12.6
- **Cores RGB**: intervalo [0-255]

#### 5. Plano (pl)

```
pl 0.0,0.0,-10.0 0.0,1.0,0.0 0,0,225
```

- **Identificador**: pl
- **Coordenadas x,y,z de um ponto no plano**: 0.0,0.0,-10.0
- **Vetor normal 3D normalizado**: intervalo [-1,1] para cada eixo
- **Cores RGB**: intervalo [0-255]

#### 6. Cilindro (cy)

```
cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255
```

- **Identificador**: cy
- **Coordenadas x,y,z do centro**: 50.0,0.0,20.6
- **Vetor 3D normalizado do eixo**: intervalo [-1,1] para cada eixo
- **Diâmetro**: 14.2
- **Altura**: 21.42
- **Cores RGB**: intervalo [0-255]

### Exemplo de Cena Minimalista

```
A 0.2 255,255,255
C -50,0,20 0,0,1 70
L -40,0,30 0.7 255,255,255
pl 0,0,0 0,1.0,0 255,0,225
sp 0,0,20 20 255,0,0
cy 50.0,0.0,20.6 0,0,1.0 14.2 21.42 10,0,255
```

---

## Tratamento de Erros

- Se qualquer má configuração de qualquer tipo for encontrada no arquivo, o programa deve sair corretamente e retornar:
  ```
  Error\n
  ```
  Seguido por uma mensagem de erro explícita de sua escolha

---

## Requisitos Bônus

> **⚠️ IMPORTANTE**: Bônus serão avaliados apenas se a parte obrigatória for perfeita. Por perfeita, significa que precisa estar completa, que não pode falhar mesmo em casos de erros desagradáveis. Se a parte obrigatória não obtiver todos os pontos durante a avaliação, os bônus serão totalmente ignorados.

### Lista de Bônus

1. **Reflexão especular**: para obter um modelo de reflexão de Phong completo
2. **Disrupção de cor**: padrão de tabuleiro de xadrez
3. **Luzes coloridas e multi-spot**
4. **Outro objeto de segundo grau**: cone, hiperboloide, paraboloide
5. **Texturas de mapa de relevo** (bump mapping)

### Observações sobre Bônus

- Você pode usar outras funções e adicionar recursos à descrição de cena, desde que o uso seja justificado durante a avaliação
- Você pode modificar o arquivo de cena esperado para atender às suas necessidades
- Seja esperto!

---

## Recomendações para Defesa

- Ter um conjunto completo de cenas focadas no que é funcional
- Facilitar o controle dos elementos a serem criados
- Estar preparado para demonstrar compreensão real do código
- Estar preparado para fazer pequenas modificações durante a avaliação (se solicitado)