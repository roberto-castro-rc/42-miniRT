# Tutorial: Testando os Bonus do miniRT

## Índice
1. [Visão Geral](#visão-geral)
2. [Setup Rápido](#setup-rápido)
3. [Modos de Uso](#modos-de-uso)
4. [Testando Cada Bonus](#testando-cada-bonus)
5. [Comparação Mandatory vs Bonus](#comparação-mandatory-vs-bonus)
6. [Dashboard de Progresso](#dashboard-de-progresso)
7. [Detecção de Leaks](#detecção-de-leaks)
8. [Guia Visual por Step](#guia-visual-por-step)
9. [Dicas de Desenvolvimento](#dicas-de-desenvolvimento)

---

## Visão Geral

O `test_bonus.sh` testa os **5 bonus do subject** individualmente:

| Step | Bonus | O que testa |
|------|-------|-------------|
| 1 | **Multi-light colorida** | Múltiplas luzes, cor da luz, cor do ambient |
| 2 | **Specular Phong** | Reflexão especular (hotspot brilhante) |
| 3 | **Cone** | Novo objeto `cn`, parse + interseção |
| 4 | **Checkerboard** | Padrão xadrez em superfícies |
| 5 | **Bump mapping** | Perturbação de normais via textura |

Cada step tem:
- **Cenas de teste** (`.rt`) desenhadas para exercitar a feature
- **Teste de crash** — verifica se a cena roda sem segfault
- **Guia visual** — explica o que observar na janela
- **Teste de leak** — valgrind em cada cena

---

## Setup Rápido

```bash
# 1. Compile o bonus
make bonus

# 2. Dê permissão ao script
chmod +x tests/test_bonus.sh

# 3. Rode (a partir da raiz do projeto)
./tests/test_bonus.sh
```

Isso abre o **menu interativo**:

```
╔══════════════════════════════════════════════════════════════╗
║    miniRT Bonus Test Suite                                  ║
╚══════════════════════════════════════════════════════════════╝

  Testes por Step:
    1)  Step 1 — Multi-light colorida
    2)  Step 2 — Specular Phong
    3)  Step 3 — Cone
    4)  Step 4 — Checkerboard
    5)  Step 5 — Bump mapping

  Modos Especiais:
    6)  Rodar TODOS os steps
    7)  Comparacao visual (mandatory vs bonus)
    8)  Dashboard de progresso
    9)  Leak detection (valgrind)

  Visual (abre cenas no bonus build):
    v1) Visual Step 1    v2) Visual Step 2
    ...

    0)  Sair
```

---

## Modos de Uso

### Linha de comando (sem menu)

```bash
# Testa um step específico
./tests/test_bonus.sh step1
./tests/test_bonus.sh step2
./tests/test_bonus.sh step3
./tests/test_bonus.sh step4
./tests/test_bonus.sh step5

# Testa todos de uma vez
./tests/test_bonus.sh all

# Comparação visual mandatory vs bonus
./tests/test_bonus.sh compare

# Dashboard de progresso
./tests/test_bonus.sh progress

# Leak detection
./tests/test_bonus.sh leak
```

### Via Makefile

```bash
make test-bonus-step1   # Testa multi-light
make test-bonus-step2   # Testa specular
make test-bonus-step3   # Testa cone
make test-bonus-step4   # Testa checkerboard
make test-bonus-step5   # Testa bump map
make test-bonus-all     # Testa tudo
make test-bonus-compare # Comparação visual
make test-bonus-progress # Dashboard
make test-bonus-leak    # Leak detection
```

---

## Testando Cada Bonus

### Step 1: Multi-light colorida

**Quando testar:** Assim que desbloquear múltiplas luzes no parser e usar `light.color` na iluminação.

**Cenas:**

| Cena | O que mostra |
|------|-------------|
| `01_two_white_lights` | Duas luzes brancas → esfera iluminada dos dois lados |
| `02_rgb_color_mix` | Três luzes R,G,B → mistura de cores na esfera branca |
| `03_colored_ambient` | Ambient laranja → tom quente nas sombras |
| `04_multi_shadows` | Três luzes → três direções de sombra |
| `05_warm_cold_contrast` | Luz quente (esq) + fria (dir) → contraste dramático |

**Como validar:**
```bash
# Roda os testes automatizados
./tests/test_bonus.sh step1

# Abre cada cena para inspeção visual
# (no menu, digite: v1)

# Ou pela linha de comando, rode manualmente:
./miniRT tests/bonus_tests/step1_multilight/02_rgb_color_mix.rt
```

**O que procurar:**
- Com mandatory (1 luz): a esfera fica iluminada só de um lado
- Com bonus (3 luzes): iluminação vem de múltiplas direções, cores se misturam

---

### Step 2: Specular Phong

**Quando testar:** Após implementar `calc_specular()` e `vec_reflect()`.

**Cenas:**

| Cena | O que mostra |
|------|-------------|
| `01_basic_specular` | Hotspot branco na esfera vermelha |
| `02_multi_specular` | Três esferas com highlights de cores diferentes |
| `03_specular_cylinder` | Streak especular no corpo de um cilindro |
| `04_frontal_specular` | Luz frontal → highlight forte no centro da esfera |

**Como validar:**
```bash
./tests/test_bonus.sh step2
```

**O que procurar:**
- **Mandatory:** esferas com iluminação "flat" — sem brilho pontual
- **Bonus:** ponto branco brilhante onde a luz reflete na direção da câmera
- O highlight deve se mover conforme a posição relativa câmera/luz

---

### Step 3: Cone

**Quando testar:** Após implementar `parse_cone()` e `intersect_cone()`.

**Cenas:**

| Cena | O que mostra |
|------|-------------|
| `01_basic_cone` | Cone vertical simples |
| `02_cone_vs_cylinder` | Cone ao lado de cilindro — compara formas |
| `03_tilted_cone` | Cone inclinado a 45° |
| `04_ice_cream` | Três cones com esferas (sorvete) |
| `05_inverted_cone` | Cone de cabeça para baixo (funil) |

**Como validar:**
```bash
./tests/test_bonus.sh step3
```

**Importante:** Estas cenas usam o identificador `cn`. O **mandatory build vai dar erro** nelas (comportamento esperado). O script detecta isso automaticamente.

**O que procurar:**
- Formato cônico (afunila até a ponta)
- Sombra com formato de cone no chão
- Caps (tampa circular na base) renderizados corretamente
- Cone inclinado mantém a forma

---

### Step 4: Checkerboard

**Quando testar:** Após implementar `apply_checkerboard()` com UV mapping.

**Cenas:**

| Cena | O que mostra |
|------|-------------|
| `01_checker_sphere` | Esfera com xadrez preto/vermelho |
| `02_checker_floor` | Chão clássico preto/branco |
| `03_checker_cylinder` | Cilindro com xadrez no corpo |
| `04_full_checker_scene` | Tudo com xadrez |

**Como validar:**
```bash
./tests/test_bonus.sh step4
```

**O que procurar:**
- **Mandatory:** cores sólidas em todas as superfícies
- **Bonus:** padrão xadrez alternando entre cor do objeto e preto
- Na esfera, o xadrez deve se "curvar" acompanhando a superfície
- No plano, quadrados regulares até o horizonte
- Sem distorção excessiva nos polos da esfera

---

### Step 5: Bump Mapping

**Quando testar:** Após implementar perturbação de normais.

**Cenas:**

| Cena | O que mostra |
|------|-------------|
| `01_bump_sphere` | Esfera com superfície rugosa |
| `02_bump_plane` | Plano com textura de ondulação |
| `03_bump_cylinder` | Cilindro com aspecto de casca de árvore |

**Como validar:**
```bash
./tests/test_bonus.sh step5
```

**O que procurar:**
- **Mandatory:** superfícies perfeitamente lisas
- **Bonus:** variações de iluminação que simulam relevo
- A silhueta do objeto NÃO muda (bump map só afeta normais, não geometria)

---

## Comparação Mandatory vs Bonus

O modo `compare` é a ferramenta principal para ver a diferença entre as builds:

```bash
./tests/test_bonus.sh compare
```

**O que acontece:**
1. O script compila o **mandatory** (`make re`) e salva como `miniRT_mandatory`
2. Compila o **bonus** (`make bonus`)
3. Para cada cena de comparação:
   - Abre primeiro com o mandatory → você observa
   - Fecha (ESC) → abre com o bonus → compara mentalmente

**Cenas de comparação:**

| Cena | Propósito |
|------|-----------|
| `01_baseline_single_light` | **Baseline** — deve ser IGUAL em ambos |
| `02_multilight_diff` | Multi-light — bonus mais brilhante e colorido |
| `03_specular_diff` | Specular — bonus tem hotspots brilhantes |

**Dica:** Use esta funcionalidade após cada step implementado para verificar que:
- A cena baseline continua **idêntica** (não quebrou nada)
- As cenas de diff mostram as melhorias do bonus

---

## Dashboard de Progresso

```bash
./tests/test_bonus.sh progress
```

Mostra um resumo rápido dos 5 steps:

```
  Dashboard de Progresso dos Bonus

  Legenda: ■=OK  ◧=Parcial  □=Nao funciona  ○=Sem cenas

    ■ Step 1: Multi-light colorida : 5/5 sem crash
    ■ Step 2: Specular Phong       : 4/4 sem crash
    □ Step 3: Cone                 : 0/5 sem crash
    ■ Step 4: Checkerboard         : 4/4 sem crash
    □ Step 5: Bump mapping         : 0/3 sem crash
```

**Use isso para:** acompanhar o progresso durante o desenvolvimento. Rode após cada commit para ver a evolução.

---

## Detecção de Leaks

```bash
./tests/test_bonus.sh leak
```

Executa **valgrind** em cada cena bonus, separado por step. Usa `DISPLAY=:99` para evitar abrir janela.

**Quando rodar:**
- Após implementar cone (novo `malloc` para `scene->cones`)
- Após implementar texturas/bump map (novo `malloc` para dados de textura)
- Antes de cada commit importante

---

## Guia Visual por Step

### Workflow recomendado para desenvolvimento

```
┌─────────────────────────────────────────────┐
│  1. Implementar feature no código           │
│  2. make bonus                              │
│  3. ./tests/test_bonus.sh stepN             │
│     → Verifica sem crash                    │
│  4. Menu: vN (visual do step)               │
│     → Verifica resultado visual             │
│  5. ./tests/test_bonus.sh progress          │
│     → Confirma no dashboard                 │
│  6. ./tests/test_bonus.sh compare           │
│     → Confirma que nada quebrou             │
│  7. ./tests/test_bonus.sh leak              │
│     → Sem leaks                             │
│  8. git commit                              │
└─────────────────────────────────────────────┘
```

---

## Dicas de Desenvolvimento

### 1. Teste incremental
Não espere tudo ficar pronto. A cada mudança:
```bash
make bonus && ./tests/test_bonus.sh stepN
```

### 2. Cenas customizadas
Você pode criar suas próprias cenas em `tests/bonus_tests/stepN/`:
```
# Minha cena de teste
A 0.2 255,255,255
C 0,3,-10 0,0,1 70
L 5,8,-3 0.8 255,0,0
sp 0,1,5 3 255,255,255
```

### 3. Debugging visual
Se algo parece errado visualmente, compare com a cena baseline:
```bash
./miniRT tests/bonus_tests/comparison/01_baseline_single_light.rt
```
Se esta cena parecer diferente do mandatory, algo no bonus quebrou o comportamento base.

### 4. Isolar problemas
Rode apenas a cena problemática:
```bash
./miniRT tests/bonus_tests/step3_cone/01_basic_cone.rt
```

### 5. Valgrind em cena específica
```bash
valgrind --leak-check=full ./miniRT tests/bonus_tests/step3_cone/01_basic_cone.rt
```

---

## Estrutura de Arquivos

```
tests/
  test_bonus.sh                          ← Script principal
  TEST_BONUS_TUTORIAL.md                 ← Este arquivo
  bonus_tests/
    step1_multilight/
      01_two_white_lights.rt
      02_rgb_color_mix.rt
      03_colored_ambient.rt
      04_multi_shadows.rt
      05_warm_cold_contrast.rt
    step2_specular/
      01_basic_specular.rt
      02_multi_specular.rt
      03_specular_cylinder.rt
      04_frontal_specular.rt
    step3_cone/
      01_basic_cone.rt
      02_cone_vs_cylinder.rt
      03_tilted_cone.rt
      04_ice_cream.rt
      05_inverted_cone.rt
    step4_checkerboard/
      01_checker_sphere.rt
      02_checker_floor.rt
      03_checker_cylinder.rt
      04_full_checker_scene.rt
    step5_bumpmap/
      01_bump_sphere.rt
      02_bump_plane.rt
      03_bump_cylinder.rt
    comparison/
      01_baseline_single_light.rt        ← Igual em ambos builds
      02_multilight_diff.rt              ← Multi-light: bonus mais rico
      03_specular_diff.rt                ← Specular: bonus com hotspots
```
