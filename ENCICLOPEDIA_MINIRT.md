# 📚 ENCICLOPÉDIA COMPLETA DO miniRT

## A Bíblia do Ray Tracing na Ecole 42

---

# 📖 ÍNDICE GERAL

1. [Introdução: O Que é Ray Tracing?](#-parte-1-introdução-o-que-é-ray-tracing)
2. [Arquitetura do Projeto](#-parte-2-arquitetura-do-projeto)
3. [Fundamentos Matemáticos](#-parte-3-fundamentos-matemáticos---a-base-de-tudo)
4. [Sistema de Parsing](#-parte-4-sistema-de-parsing)
5. [Intersecções com Objetos](#-parte-5-intersecções-com-objetos---o-coração-do-ray-tracing)
6. [Sistema de Iluminação](#-parte-6-sistema-de-iluminação)
7. [Câmera e Projeção](#-parte-7-câmera-e-projeção)
8. [Análise do Projeto Atual](#-parte-8-análise-do-projeto-atual)
9. [Melhorias Propostas](#-parte-9-melhorias-propostas)
10. [Guia de Implementação do Bônus](#-parte-10-guia-de-implementação-do-bônus)
11. [Bônus do Bônus: Indo Além](#-parte-11-bônus-do-bônus---indo-além)
12. [Dicas para a Defesa](#-parte-12-dicas-para-a-defesa)
13. [Referências e Recursos](#-parte-13-referências-e-recursos)

---

# 🎯 PARTE 1: Introdução - O Que é Ray Tracing?

## 1.1 Conceito Fundamental

**Ray Tracing** (traçado de raios) é uma técnica de renderização que simula como a luz se comporta no mundo real, mas ao contrário! Em vez de simular a luz saindo das fontes e chegando aos nossos olhos (o que seria computacionalmente impossível), fazemos o inverso:

```
┌─────────────────────────────────────────────────────────────────┐
│                                                                  │
│   OLHO (Câmera)                                                 │
│        ○ ─────────────────────▶ Raio                            │
│         \                          │                            │
│          \                         ▼                            │
│           \              ┌───────────────┐                      │
│            \             │    OBJETO     │                      │
│             \            │   (Esfera)    │                      │
│              \           └───────────────┘                      │
│               \                 │                               │
│                \                │ Raio de sombra                │
│                 \               ▼                               │
│                  \         💡 LUZ                               │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Por que ao contrário?

Imagine uma lâmpada emitindo bilhões de fótons por segundo em todas as direções. Apenas uma fração minúscula desses fótons chegaria aos nossos olhos. Simular todos seria um desperdício computacional absurdo!

**Solução inteligente**: Lançamos raios DA câmera PARA a cena. Só calculamos os raios que realmente importam para formar a imagem.

## 1.2 O Pipeline do Ray Tracing

```
┌────────────────────────────────────────────────────────────────────────┐
│                        PIPELINE DO RAY TRACING                         │
├────────────────────────────────────────────────────────────────────────┤
│                                                                        │
│  1. GERAR RAIO        2. ENCONTRAR           3. CALCULAR              │
│     (por pixel)          INTERSECÇÃO            ILUMINAÇÃO            │
│                                                                        │
│  ┌─────────┐          ┌─────────────┐        ┌────────────────┐       │
│  │  PIXEL  │ ───────▶ │ Qual objeto │ ─────▶ │ Cor final com  │       │
│  │ (x, y)  │   Raio   │ o raio bate │  Hit   │ luz + sombra   │       │
│  └─────────┘          │ primeiro?   │  info  │                │       │
│                       └─────────────┘        └────────────────┘       │
│                              │                       │                │
│                              ▼                       ▼                │
│                       Para cada objeto:        - Luz ambiente         │
│                       - Esfera?               - Luz difusa            │
│                       - Plano?                - Sombras               │
│                       - Cilindro?             - (Especular)*          │
│                                                                        │
└────────────────────────────────────────────────────────────────────────┘
```

## 1.3 Anatomia de um Raio

Um raio é definido por dois componentes:

```c
typedef struct s_ray
{
    t_vector    origin;      // Ponto de origem (posição da câmera)
    t_vector    direction;   // Direção normalizada do raio
}   t_ray;

// Qualquer ponto no raio pode ser calculado por:
// P(t) = origin + t * direction
// onde t >= 0 (valores positivos = frente da câmera)
```

**Visualização**:
```
      origin (O)
         ●─────────────────────────────────▶ direction (D)
         │
         │  t=0    t=1    t=2    t=3    t=4
         ●──────●──────●──────●──────●─────▶
         O     P1     P2     P3     P4
```

---

# 🏗️ PARTE 2: Arquitetura do Projeto

## 2.1 Estrutura de Diretórios

```
miniRT/
├── includes/
│   ├── minirt.h          # Header principal com todas as estruturas
│   └── vectors.h         # Operações vetoriais
├── src/
│   ├── core/
│   │   ├── main.c        # Ponto de entrada
│   │   ├── cleanup.c     # Liberação de memória
│   │   └── error.c       # Tratamento de erros
│   ├── parser/
│   │   ├── parse_scene.c      # Parser principal
│   │   ├── parse_elements.c   # Ambient, Camera, Light
│   │   ├── parse_objects.c    # Plane, Cylinder
│   │   ├── parse_sphere.c     # Esfera
│   │   ├── parse_utils.c      # Utilitários de parsing
│   │   └── validate.c         # Validações
│   ├── math/
│   │   ├── vec_basic.c     # add, subtract, scale, negate
│   │   ├── vec_products.c  # dot, cross
│   │   └── vec_utils.c     # magnitude, normalize, distance
│   ├── render/
│   │   ├── render.c        # Loop de renderização
│   │   ├── ray_trace.c     # Traçado de raios
│   │   ├── lighting.c      # Cálculo de iluminação
│   │   └── shadows.c       # Cálculo de sombras
│   ├── scene/
│   │   ├── init_scene.c    # Inicialização da cena
│   │   └── camera.c        # Configuração da câmera
│   ├── shapes/
│   │   ├── intersect_sphere.c    # Intersecção raio-esfera
│   │   ├── intersect_plane.c     # Intersecção raio-plano
│   │   ├── intersect_cylinder.c  # Intersecção raio-cilindro
│   │   └── cylinder_utils.c      # Utilitários do cilindro
│   ├── mlx/
│   │   ├── mlx_init.c      # Inicialização MLX42
│   │   ├── mlx_events.c    # Eventos (ESC, fechar)
│   │   └── mlx_utils.c     # Utilitários gráficos
│   └── utils/
│       └── color.c         # Operações com cores
├── lib/
│   ├── libft/              # Biblioteca pessoal
│   └── MLX42/              # Biblioteca gráfica
├── scenes/                 # Arquivos .rt de exemplo
└── tests/                  # Suite de testes
```

## 2.2 Estruturas de Dados Principais

### A Cena (t_scene) - O Coração do Projeto

```c
typedef struct s_scene
{
    // Elementos únicos (só pode haver um de cada)
    t_ambient       ambient;     // Luz ambiente global
    t_camera        camera;      // Câmera (ponto de vista)

    // Fontes de luz (podem ser múltiplas)
    t_light         *lights;     // Array de luzes
    int             light_count;

    // Objetos da cena
    t_sphere        *spheres;
    int             sphere_count;
    t_plane         *planes;
    int             plane_count;
    t_cylinder      *cylinders;
    int             cylinder_count;

    // MLX
    mlx_t           *mlx;        // Contexto MLX
    mlx_image_t     *image;      // Imagem a ser renderizada
    int             width;       // 1280
    int             height;      // 720
}   t_scene;
```

### Fluxo de Dados

```
┌─────────────────────────────────────────────────────────────────────┐
│                    FLUXO DE EXECUÇÃO DO MINIRT                      │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ./miniRT scene.rt                                                  │
│         │                                                           │
│         ▼                                                           │
│  ┌─────────────────┐                                               │
│  │ check_extension │──▶ Valida se termina em .rt                   │
│  └────────┬────────┘                                               │
│           ▼                                                         │
│  ┌─────────────────┐                                               │
│  │   init_scene    │──▶ Zera todos os contadores                   │
│  └────────┬────────┘                                               │
│           ▼                                                         │
│  ┌─────────────────┐     ┌─────────────────────────────────┐       │
│  │   parse_scene   │────▶│ Lê arquivo linha por linha      │       │
│  └────────┬────────┘     │ Identifica: A, C, L, sp, pl, cy │       │
│           │              │ Valida e popula t_scene         │       │
│           │              └─────────────────────────────────┘       │
│           ▼                                                         │
│  ┌─────────────────┐                                               │
│  │    init_mlx     │──▶ Cria janela 1280x720                       │
│  └────────┬────────┘                                               │
│           ▼                                                         │
│  ┌─────────────────┐                                               │
│  │   init_camera   │──▶ Calcula vetores right e up                 │
│  └────────┬────────┘                                               │
│           ▼                                                         │
│  ┌─────────────────┐     ┌─────────────────────────────────┐       │
│  │  render_scene   │────▶│ Para cada pixel (x, y):         │       │
│  └────────┬────────┘     │   1. generate_ray(x, y)         │       │
│           │              │   2. find_closest_hit()         │       │
│           │              │   3. calculate_lighting()       │       │
│           │              │   4. put_pixel()                │       │
│           │              └─────────────────────────────────┘       │
│           ▼                                                         │
│  ┌─────────────────┐                                               │
│  │    mlx_loop     │──▶ Exibe janela até ESC/fechar                │
│  └────────┬────────┘                                               │
│           ▼                                                         │
│  ┌─────────────────┐                                               │
│  │  cleanup_scene  │──▶ Libera toda memória                        │
│  └─────────────────┘                                               │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

---

# 🧮 PARTE 3: Fundamentos Matemáticos - A Base de Tudo

## 3.1 Vetores 3D

Um vetor representa uma direção e magnitude no espaço 3D:

```c
typedef struct s_vector
{
    double  x;  // Componente horizontal
    double  y;  // Componente vertical
    double  z;  // Componente de profundidade
}   t_vector;
```

### Visualização do Sistema de Coordenadas

```
                 +Y (cima)
                  │
                  │
                  │
                  │
                  ●───────────────▶ +X (direita)
                 /
                /
               /
              ▼
            +Z (frente/profundidade)
```

## 3.2 Operações Vetoriais Essenciais

### 3.2.1 Adição e Subtração

```c
// Adição: combina dois vetores
t_vector vec_add(t_vector a, t_vector b)
{
    return (t_vector){a.x + b.x, a.y + b.y, a.z + b.z};
}

// Subtração: vetor de a para b
t_vector vec_subtract(t_vector a, t_vector b)
{
    return (t_vector){a.x - b.x, a.y - b.y, a.z - b.z};
}
```

**Visualização**:
```
        Adição                    Subtração
         ▲ B                       ▲ B
        /                         /
       /                         / A - B
      /       A + B             /   ▲
     ●──────▶────────▶         ●────│────▶ A
     │       A                      │
     │
```

### 3.2.2 Produto Escalar (Dot Product)

O produto escalar retorna um **número** que representa:
- O quanto dois vetores apontam na mesma direção
- Usado para calcular ângulos, iluminação, projeções

```c
double vec_dot(t_vector a, t_vector b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}
```

**Propriedades mágicas do dot product**:

```
┌─────────────────────────────────────────────────────────────────────┐
│                    DOT PRODUCT E ÂNGULOS                            │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│    a · b = |a| × |b| × cos(θ)                                      │
│                                                                     │
│    Se a e b são normalizados (|a| = |b| = 1):                      │
│    a · b = cos(θ)                                                  │
│                                                                     │
│    ┌──────────────┬────────────┬────────────────────────────┐      │
│    │   RESULTADO  │   ÂNGULO   │        SIGNIFICADO         │      │
│    ├──────────────┼────────────┼────────────────────────────┤      │
│    │    = 1       │    0°      │  Mesma direção (paralelos) │      │
│    │    > 0       │   < 90°    │  Apontam "parecido"        │      │
│    │    = 0       │    90°     │  Perpendiculares           │      │
│    │    < 0       │   > 90°    │  Direções opostas          │      │
│    │    = -1      │   180°     │  Direções opostas exatas   │      │
│    └──────────────┴────────────┴────────────────────────────┘      │
│                                                                     │
│    USO NO MINIRT:                                                   │
│    - Iluminação difusa: dot(normal, light_dir)                     │
│    - Se positivo: superfície voltada para a luz                    │
│    - Se negativo: superfície de costas para a luz                  │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

### 3.2.3 Produto Vetorial (Cross Product)

O produto vetorial retorna um **vetor** perpendicular aos dois vetores de entrada:

```c
t_vector vec_cross(t_vector a, t_vector b)
{
    t_vector result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return (result);
}
```

**Visualização**:
```
            a × b
              ▲
              │
              │
              ●──────▶ b
             /
            /
           ▼
          a

    O resultado é PERPENDICULAR ao plano formado por a e b!
    Usado para calcular os vetores "right" e "up" da câmera.
```

### 3.2.4 Magnitude e Normalização

```c
// Magnitude: "comprimento" do vetor
double vec_magnitude(t_vector v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    // Teorema de Pitágoras em 3D!
}

// Normalização: transforma em vetor unitário (comprimento = 1)
t_vector vec_normalize(t_vector v)
{
    double mag = vec_magnitude(v);
    if (mag < EPSILON)  // Evita divisão por zero
        return v;
    return (t_vector){v.x / mag, v.y / mag, v.z / mag};
}
```

**Por que normalizar?**
```
┌─────────────────────────────────────────────────────────────────────┐
│  Vetores normalizados simplificam cálculos de ângulos!             │
│                                                                     │
│  Vetor original: (3, 4, 0)  →  magnitude = 5                       │
│  Normalizado:    (0.6, 0.8, 0)  →  magnitude = 1                   │
│                                                                     │
│  Agora dot(n1, n2) = cos(θ) diretamente!                           │
└─────────────────────────────────────────────────────────────────────┘
```

## 3.3 Equações de Intersecção

### 3.3.1 Raio-Esfera: A Magia da Equação Quadrática

**O Problema**: Onde um raio penetra uma esfera?

```
Equação do raio:     P(t) = O + t·D    (O=origem, D=direção)
Equação da esfera:   |P - C|² = r²     (C=centro, r=raio)

Substituindo P(t) na equação da esfera:
|O + t·D - C|² = r²

Seja OC = O - C (vetor do centro da esfera até a origem do raio)
|(OC) + t·D|² = r²

Expandindo:
(OC + t·D) · (OC + t·D) = r²
OC·OC + 2·t·(OC·D) + t²·(D·D) = r²

Reorganizando (forma at² + bt + c = 0):
(D·D)t² + 2(OC·D)t + (OC·OC - r²) = 0

Onde:
  a = D·D                    (sempre 1 se D é normalizado)
  b = 2(OC·D)
  c = OC·OC - r²

Discriminante: Δ = b² - 4ac

Se Δ < 0: Raio não atinge a esfera (0 soluções)
Se Δ = 0: Raio tangencia a esfera (1 solução)
Se Δ > 0: Raio atravessa a esfera (2 soluções)
```

**Visualização**:
```
                        Δ < 0: Não atinge
                           ○
                          / \
          Ray ──────────▶/   \
                        └─────┘
                         Esfera

                        Δ = 0: Tangencia
                           ○
                          /│\
          Ray ──────────▶─●─
                        └─┴─┘
                         Esfera

                        Δ > 0: Atravessa
                           ○
                          /│\
          Ray ──────────●─┼─●──────▶
                       t1 │ t2
                        └─┴─┘
                         Esfera
```

**Implementação no projeto**:
```c
t_hit intersect_sphere(t_ray ray, t_sphere sp, double closest)
{
    t_vector    oc;           // Vetor do centro da esfera até a origem do raio
    double      abc[3];       // Coeficientes a, b, c da equação quadrática
    double      discriminant;
    double      t;
    t_hit       hit;

    // OC = origem do raio - centro da esfera
    oc = vec_subtract(ray.origin, sp.center);

    // a = D·D (como D é normalizado, a ≈ 1)
    abc[0] = vec_dot(ray.direction, ray.direction);

    // b = 2(OC·D)
    abc[1] = 2.0 * vec_dot(oc, ray.direction);

    // c = OC·OC - r²
    abc[2] = vec_dot(oc, oc) - sp.radius * sp.radius;

    // Δ = b² - 4ac
    discriminant = abc[1] * abc[1] - 4.0 * abc[0] * abc[2];

    if (discriminant < 0)
        return (create_no_hit());  // Sem intersecção

    // Usa a fórmula de Bhaskara para encontrar t
    t = solve_quadratic(abc[0], abc[1], discriminant);

    if (t < 0 || t >= closest)
        return (create_no_hit());

    // Preenche informações do hit
    hit.hit = 1;
    hit.t = t;
    hit.point = ray_at(ray, t);  // P = O + t*D

    // Normal: aponta do centro para fora
    hit.normal = vec_normalize(vec_subtract(hit.point, sp.center));
    hit.color = sp.color;

    return (hit);
}
```

### 3.3.2 Raio-Plano: Mais Simples que Parece

**O Problema**: Onde um raio cruza um plano infinito?

```
Equação do plano: (P - P₀) · N = 0
  P₀ = um ponto no plano
  N = vetor normal do plano
  P = qualquer ponto no plano

Substituindo a equação do raio P(t) = O + t·D:
(O + t·D - P₀) · N = 0
(O - P₀)·N + t·(D·N) = 0

Resolvendo para t:
t = ((P₀ - O) · N) / (D · N)

Condições:
- Se D·N ≈ 0: Raio paralelo ao plano (não intersecta)
- Se t < 0: Intersecção atrás da câmera (ignorar)
```

**Implementação**:
```c
t_hit intersect_plane(t_ray ray, t_plane pl, double closest)
{
    double      denom;     // D · N (denominador)
    double      t;
    t_vector    diff;      // P₀ - O
    t_hit       hit;

    // Verifica se o raio é paralelo ao plano
    denom = vec_dot(ray.direction, pl.normal);
    if (fabs(denom) < EPSILON)
        return (create_no_hit());  // Paralelo ou quase

    // t = (P₀ - O) · N / (D · N)
    diff = vec_subtract(pl.point, ray.origin);
    t = vec_dot(diff, pl.normal) / denom;

    if (t < EPSILON || t >= closest)
        return (create_no_hit());

    hit.hit = 1;
    hit.t = t;
    hit.point = ray_at(ray, t);
    hit.normal = pl.normal;

    // Garante que a normal aponta para o lado correto
    if (vec_dot(hit.normal, ray.direction) > 0)
        hit.normal = vec_negate(hit.normal);

    hit.color = pl.color;
    return (hit);
}
```

### 3.3.3 Raio-Cilindro: O Desafio Maior

Um cilindro no miniRT tem três partes para verificar:
1. **Corpo** (superfície lateral curva)
2. **Tampa inferior** (disco)
3. **Tampa superior** (disco)

```
             ┌───────────┐  ← Tampa superior
            /│           │\
           / │           │ \
          /  │   CORPO   │  \
         │   │   (tubo)  │   │
         │   │           │   │
          \  │           │  /
           \ │           │ /
            \│           │/
             └───────────┘  ← Tampa inferior
                  ▲
                  │
                AXIS (eixo do cilindro)
```

**Intersecção com o corpo (mais complexa)**:

```
Ideia: Projetamos tudo no plano perpendicular ao eixo do cilindro,
transformando o problema 3D em um problema 2D (círculo)!

Seja A = eixo do cilindro (normalizado)
O raio projetado no plano perpendicular: D' = D - (D·A)A
O ponto projetado: OC' = OC - (OC·A)A

Agora temos uma equação quadrática 2D:
|OC' + t·D'|² = r²
```

**Verificação de altura**: Após encontrar t, verificamos se o ponto está dentro da altura do cilindro:

```c
int check_body_hit(t_ray ray, t_cylinder cy, double t, double cl)
{
    t_vector    hit_point;
    t_vector    to_hit;
    double      h;

    if (t < EPSILON || t >= cl)
        return (0);

    hit_point = ray_at(ray, t);
    to_hit = vec_subtract(hit_point, cy.center);

    // Projeta no eixo para verificar altura
    h = vec_dot(to_hit, cy.axis);

    // Deve estar entre 0 e altura do cilindro
    return (h >= 0.0 && h <= cy.height);
}
```

---

# 📄 PARTE 4: Sistema de Parsing

## 4.1 Formato do Arquivo .rt

O arquivo de cena segue um formato específico:

```rt
# Comentários começam com #

# ELEMENTOS ÚNICOS (só pode haver 1 de cada)
A 0.2 255,255,255                           # Ambient
C -50.0,0,20 0,0,1 70                        # Camera

# FONTES DE LUZ (pode haver múltiplas)
L -40.0,50.0,0.0 0.6 255,255,255

# OBJETOS (pode haver múltiplos)
sp 0.0,0.0,20.6 12.6 10,0,255               # Esfera
pl 0.0,0.0,-10.0 0.0,1.0,0.0 0,0,225        # Plano
cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255  # Cilindro
```

## 4.2 Anatomia de Cada Elemento

### Ambient Light (A)
```
A    0.2             255,255,255
│    │               │
│    │               └─── Cor RGB (0-255 cada)
│    └─── Intensidade (0.0 a 1.0)
└─── Identificador
```

### Camera (C)
```
C    -50.0,0,20      0,0,1           70
│    │               │               │
│    │               │               └─── FOV em graus (0-180)
│    │               └─── Direção (DEVE ser normalizada!)
│    └─── Posição (x,y,z)
└─── Identificador
```

### Light (L)
```
L    -40.0,50.0,0.0  0.6            255,255,255
│    │               │               │
│    │               │               └─── Cor RGB
│    │               └─── Brilho (0.0 a 1.0)
│    └─── Posição (x,y,z)
└─── Identificador
```

### Sphere (sp)
```
sp   0.0,0.0,20.6    12.6           10,0,255
│    │               │               │
│    │               │               └─── Cor RGB
│    │               └─── DIÂMETRO (não raio!)
│    └─── Centro (x,y,z)
└─── Identificador
```

### Plane (pl)
```
pl   0.0,0.0,-10.0   0.0,1.0,0.0    0,0,225
│    │               │               │
│    │               │               └─── Cor RGB
│    │               └─── Normal (DEVE ser normalizada!)
│    └─── Ponto no plano (x,y,z)
└─── Identificador
```

### Cylinder (cy)
```
cy   50.0,0.0,20.6   0.0,0.0,1.0    14.2    21.42   10,0,255
│    │               │               │       │       │
│    │               │               │       │       └─── Cor RGB
│    │               │               │       └─── Altura
│    │               │               └─── DIÂMETRO (não raio!)
│    │               └─── Eixo (DEVE ser normalizado!)
│    └─── Centro da base (x,y,z)
└─── Identificador
```

## 4.3 Fluxo do Parser

```
┌─────────────────────────────────────────────────────────────────────┐
│                      FLUXO DO PARSER                                │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  Arquivo .rt                                                        │
│      │                                                              │
│      ▼                                                              │
│  ┌────────────────┐                                                │
│  │  parse_scene   │◄─── Controla o loop de leitura                 │
│  └───────┬────────┘                                                │
│          │                                                          │
│          ▼ Para cada linha                                          │
│  ┌────────────────┐                                                │
│  │   trim_line    │◄─── Remove espaços, ignora # e vazias          │
│  └───────┬────────┘                                                │
│          │                                                          │
│          ▼                                                          │
│  ┌────────────────┐                                                │
│  │   parse_line   │◄─── Identifica tipo (A, C, L, sp, pl, cy)      │
│  └───────┬────────┘                                                │
│          │                                                          │
│          ▼                                                          │
│    ┌─────┴─────┬──────┬──────┬──────┬──────┐                       │
│    ▼           ▼      ▼      ▼      ▼      ▼                       │
│ parse_     parse_  parse_  parse_  parse_  parse_                  │
│ ambient    camera  light   sphere  plane   cylinder                │
│    │           │      │      │      │      │                       │
│    └─────┬─────┴──────┴──────┴──────┴──────┘                       │
│          │                                                          │
│          ▼ Cada função usa:                                         │
│  ┌────────────────────────────────────────┐                        │
│  │ parse_vector() - converte "x,y,z"      │                        │
│  │ parse_color()  - converte "r,g,b"      │                        │
│  │ parse_double() - converte string→double │                        │
│  │ validate_*()   - valida ranges         │                        │
│  └────────────────────────────────────────┘                        │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

## 4.4 Validações Importantes

```c
// Validação de vetor normalizado
int validate_normalized(t_vector vec)
{
    double magnitude = vec_magnitude(vec);
    // Tolerância de 1% para erro de arredondamento
    return (fabs(magnitude - 1.0) < 0.01);
}

// Validação de range para doubles
int validate_range_double(double val, double min, double max)
{
    return (val >= min && val <= max);
}

// Validação de range para inteiros (cores)
int validate_range_int(int val, int min, int max)
{
    return (val >= min && val <= max);
}
```

---

# 💥 PARTE 5: Intersecções com Objetos - O Coração do Ray Tracing

## 5.1 Estrutura de Hit

Quando um raio atinge um objeto, precisamos guardar várias informações:

```c
typedef struct s_hit
{
    int         hit;      // 1 se atingiu algo, 0 caso contrário
    double      t;        // "Distância" ao longo do raio
    t_vector    point;    // Ponto exato de intersecção
    t_vector    normal;   // Normal da superfície no ponto
    t_color     color;    // Cor do objeto atingido
}   t_hit;
```

## 5.2 Encontrando a Intersecção Mais Próxima

```c
t_hit find_closest_hit(t_scene *scene, t_ray ray)
{
    t_hit   closest;

    closest.hit = 0;
    closest.t = INFINITY;  // Começa com "infinitamente longe"

    // Verifica todos os objetos
    check_spheres(scene, ray, &closest);
    check_planes(scene, ray, &closest);
    check_cylinders(scene, ray, &closest);

    return (closest);
}
```

**Por que precisamos da intersecção mais PRÓXIMA?**

```
         Camera                    Esfera A        Esfera B
            ○─────────────────────────●──────────────●
                                     t=5            t=15
                                      │
                                      └─── Esta é a visível!

    Se não verificássemos a distância, poderíamos pintar o pixel
    com a cor da esfera B, que está ATRÁS da esfera A!
```

## 5.3 O Parâmetro `closest`

Passamos o `closest.t` atual para as funções de intersecção como otimização:

```c
// Se já encontramos algo em t=5, não precisamos considerar
// intersecções com t > 5
t_hit intersect_sphere(t_ray ray, t_sphere sp, double closest)
{
    // ...
    t = solve_quadratic(abc[0], abc[1], discriminant);

    // Ignora se for mais longe que o mais próximo atual
    if (t < 0 || t >= closest)
        return (create_no_hit());
    // ...
}
```

## 5.4 A Normal: Por Que Ela é Tão Importante?

A **normal** é o vetor perpendicular à superfície em um ponto. Ela é crucial para:

1. **Iluminação**: Determina quanto a superfície está voltada para a luz
2. **Reflexões** (bônus): Direção do raio refletido
3. **Sombras**: Offset para evitar auto-sombra

```
                    Normal (N)
                        ▲
                        │
        Luz ☀️ ─────────●──────────▶
                       /│\
                      / │ \
                     /  │  \
                    ────●────
                    Superfície

    dot(N, luz_dir) = cos(θ)

    Se θ pequeno: superfície bem iluminada
    Se θ grande: superfície escura
    Se θ > 90°: superfície de costas para a luz
```

---

# 💡 PARTE 6: Sistema de Iluminação

## 6.1 Modelo de Iluminação do miniRT

O miniRT usa uma versão simplificada do modelo de Phong:

```
Cor_final = Ambiente + Difusa

(O bônus adiciona: Especular)
```

### 6.1.1 Luz Ambiente

A luz ambiente simula a luz indireta que vem de todas as direções. Garante que objetos nunca fiquem completamente pretos.

```c
static t_color calc_ambient(t_color obj_color, t_ambient ambient)
{
    t_color result;

    // Multiplica a cor do objeto pela intensidade ambiente
    result.r = (int)(obj_color.r * ambient.ratio);
    result.g = (int)(obj_color.g * ambient.ratio);
    result.b = (int)(obj_color.b * ambient.ratio);

    return (result);
}
```

**Visualização**:
```
    ┌─────────────────────────────────────────────────────────────┐
    │                       SEM Luz Ambiente                      │
    │                                                             │
    │            ☀️                                                │
    │             \                                               │
    │              \   Lado iluminado                            │
    │               ◐ ◀─┐                                        │
    │                   └─ Lado completamente PRETO              │
    │                      (irrealista!)                         │
    └─────────────────────────────────────────────────────────────┘

    ┌─────────────────────────────────────────────────────────────┐
    │                       COM Luz Ambiente                      │
    │                                                             │
    │            ☀️                                                │
    │             \                                               │
    │              \   Lado iluminado (mais brilhante)           │
    │               ◐ ◀─┐                                        │
    │                   └─ Lado escuro mas não preto             │
    │                      (mais realista!)                      │
    └─────────────────────────────────────────────────────────────┘
```

### 6.1.2 Luz Difusa (Lambertian)

A luz difusa simula superfícies foscas que espalham a luz igualmente em todas as direções.

```c
static t_color calc_diffuse(t_color obj, t_light light, t_vector n, t_vector l)
{
    double  diffuse_factor;
    t_color result;

    // Quanto a superfície está voltada para a luz
    // n = normal da superfície
    // l = direção para a luz
    diffuse_factor = fmax(0.0, vec_dot(n, l));  // Clampa negativos para 0
    diffuse_factor *= light.brightness;

    result.r = (int)(obj.r * diffuse_factor);
    result.g = (int)(obj.g * diffuse_factor);
    result.b = (int)(obj.b * diffuse_factor);

    return (result);
}
```

**A Lei de Lambert**:
```
    Intensidade = max(0, N · L) × brightness

                    N (normal)
                    ▲
                    │  θ = ângulo entre N e L
        L (luz) ────┼────▶
                    │
              ──────●──────
              superfície

    Se θ = 0° → cos(0) = 1   → máximo brilho
    Se θ = 45° → cos(45) ≈ 0.7 → brilho médio
    Se θ = 90° → cos(90) = 0   → sem luz difusa
    Se θ > 90° → cos(θ) < 0    → usamos 0 (luz atrás da superfície)
```

## 6.2 Sombras Duras

Sombras são calculadas traçando um "shadow ray" do ponto de intersecção até cada fonte de luz:

```c
int check_shadow(t_scene *scene, t_vector point, t_light light)
{
    t_ray       shadow_ray;
    t_vector    to_light;
    double      light_distance;
    t_hit       hit;

    // Vetor do ponto até a luz
    to_light = vec_subtract(light.position, point);
    light_distance = vec_magnitude(to_light);

    // Cria raio de sombra
    shadow_ray.origin = point;
    shadow_ray.direction = vec_normalize(to_light);

    // Verifica se há algo bloqueando
    hit = find_closest_hit(scene, shadow_ray);

    // Se atingiu algo ANTES de chegar na luz, está em sombra
    if (hit.hit && hit.t < light_distance)
        return (1);  // Em sombra

    return (0);  // Iluminado
}
```

**Visualização**:
```
    ┌─────────────────────────────────────────────────────────────┐
    │                      CÁLCULO DE SOMBRA                      │
    ├─────────────────────────────────────────────────────────────┤
    │                                                             │
    │                         💡 Luz                              │
    │                        /                                    │
    │                       / Shadow Ray                          │
    │                      /                                      │
    │                     /                                       │
    │        Objeto  ────●───▶ Ponto P                           │
    │        Bloqueador   \                                       │
    │                      \                                      │
    │                       ●                                     │
    │                      Superfície                             │
    │                                                             │
    │  Se o Shadow Ray atinge o "Objeto Bloqueador"              │
    │  ANTES de chegar na luz → SOMBRA!                          │
    │                                                             │
    └─────────────────────────────────────────────────────────────┘
```

### 6.2.1 Shadow Acne: O Problema do Auto-Sombreamento

Um problema comum: o ponto pode "sombrear a si mesmo" devido a erros de ponto flutuante.

```
    O problema:
    ──────────●──────────  ← Superfície
              │ P (ponto calculado)
              │
              │ O raio de sombra pode começar DENTRO do objeto
              ▼ devido a erros de precisão!

    A solução:
    ──────────●──────────  ← Superfície
              │
              ● P' = P + ε×N  ← Offset na direção da normal
              │
              ▼ Agora começa FORA do objeto!
```

**No código atual**:
```c
// Em apply_lights():
offset_point = vec_add(hit.point, vec_scale(hit.normal, EPSILON));
```

## 6.3 Juntando Tudo

```c
t_color calculate_lighting(t_scene *scene, t_hit hit, t_ray ray)
{
    t_color ambient;
    t_color diffuse;
    t_color final;

    (void)ray;  // Não usado na parte obrigatória (será usado no especular)

    // Componente ambiente (sempre presente)
    ambient = calc_ambient(hit.color, scene->ambient);

    // Componente difusa (de todas as luzes não bloqueadas)
    diffuse = apply_lights(scene, hit);

    // Soma as componentes
    final = add_colors(ambient, diffuse);

    // Garante que não excede 255
    final = clamp_color(final);

    return (final);
}
```

---

# 📷 PARTE 7: Câmera e Projeção

## 7.1 O Sistema de Coordenadas da Câmera

A câmera tem seu próprio sistema de coordenadas:

```
                        Up
                         ▲
                         │
                         │
              ───────────●───────────▶ Right
                        /│\
                       / │ \
                      /  │  \
                     /   │   \
                    /    │    \
                   /     │     \
                  /      │      \
                 /       │       \
                ─────────┼─────────
                        ▼
                   Orientation
                   (direção que
                    a câmera olha)
```

## 7.2 Calculando os Vetores da Câmera

```c
void init_camera(t_camera *cam)
{
    t_vector    world_up;
    double      fov_rad;

    // "Up" do mundo (geralmente Y positivo)
    world_up = vec_create(0, 1, 0);

    // Caso especial: câmera olhando direto para cima ou para baixo
    if (fabs(cam->orientation.y) > 0.999)
        world_up = vec_create(0, 0, -1);

    // Right = Orientation × WorldUp
    // (perpendicular à direção da câmera e ao "cima" do mundo)
    cam->right = vec_normalize(vec_cross(cam->orientation, world_up));

    // Up = Right × Orientation
    // (perpendicular à direção da câmera e ao "direita")
    cam->up = vec_normalize(vec_cross(cam->right, cam->orientation));

    // Calcula dimensões do viewport baseado no FOV
    fov_rad = cam->fov * M_PI / 180.0;  // Graus → Radianos
    cam->viewport_height = 2.0 * tan(fov_rad / 2.0);
    cam->aspect_ratio = (double)WIDTH / (double)HEIGHT;
    cam->viewport_width = cam->viewport_height * cam->aspect_ratio;
}
```

## 7.3 Gerando Raios para Cada Pixel

```c
t_ray generate_ray(t_scene *scene, int x, int y)
{
    double      uv[2];
    t_vector    pixel_center;
    t_ray       ray;

    // Converte coordenadas de pixel para coordenadas do viewport
    // uv[0] vai de -viewport_width/2 até +viewport_width/2
    // uv[1] vai de +viewport_height/2 até -viewport_height/2 (invertido!)
    uv[0] = (2.0 * ((double)x / (scene->width - 1)) - 1.0)
        * scene->camera.viewport_width / 2.0;
    uv[1] = (1.0 - 2.0 * ((double)y / (scene->height - 1)))
        * scene->camera.viewport_height / 2.0;

    // Ponto no plano de projeção
    pixel_center = scene->camera.orientation;
    pixel_center = vec_add(pixel_center,
            vec_scale(scene->camera.right, uv[0]));
    pixel_center = vec_add(pixel_center,
            vec_scale(scene->camera.up, uv[1]));

    // Raio da câmera até o ponto no viewport
    ray.origin = scene->camera.position;
    ray.direction = vec_normalize(pixel_center);

    return (ray);
}
```

**Visualização**:
```
    ┌─────────────────────────────────────────────────────────────┐
    │                     GERAÇÃO DE RAIOS                        │
    ├─────────────────────────────────────────────────────────────┤
    │                                                             │
    │            ┌──────────────────┐                            │
    │            │    TELA (pixels) │                            │
    │            │  (0,0)     (W,0) │                            │
    │            │    ●─────────●   │                            │
    │            │    │         │   │                            │
    │            │    │ viewport│   │                            │
    │            │    │         │   │                            │
    │            │    ●─────────●   │                            │
    │            │  (0,H)     (W,H) │                            │
    │            └──────────────────┘                            │
    │                    │                                        │
    │                    │ Mapeia para                            │
    │                    ▼                                        │
    │            ┌──────────────────┐                            │
    │            │   VIEWPORT 3D    │                            │
    │      (-w/2,+h/2)     (+w/2,+h/2)                           │
    │            ●─────────●                                     │
    │            │         │                                     │
    │            │    ●────┼─── Câmera                           │
    │            │    │    │                                      │
    │            ●─────────●                                     │
    │      (-w/2,-h/2)     (+w/2,-h/2)                           │
    │                                                             │
    │   Cada pixel gera um raio da câmera passando pelo          │
    │   ponto correspondente no viewport.                        │
    │                                                             │
    └─────────────────────────────────────────────────────────────┘
```

## 7.4 FOV (Field of View)

O FOV controla "quanto" da cena a câmera consegue ver:

```
    FOV pequeno (30°):                    FOV grande (120°):
    Visão "telescópica"                   Visão "olho de peixe"

         │                                    /\
         │                                   /  \
         │                                  /    \
    ─────●─────                        ────●────
         │                                  \    /
         │                                   \  /
         │                                    \/

    Objetos parecem                     Objetos parecem
    maiores/mais próximos               menores/mais distantes
    Campo de visão estreito             Campo de visão amplo
```

---

# 🔍 PARTE 8: Análise do Projeto Atual

## 8.1 Status de Conformidade com os Requisitos

### ✅ Requisitos Implementados Corretamente

| Requisito | Status | Observações |
|-----------|--------|-------------|
| Esfera (sp) | ✅ | Intersecção funcionando corretamente |
| Plano (pl) | ✅ | Com flip de normal para visibilidade dos dois lados |
| Cilindro (cy) | ✅ | Corpo + tampas implementados |
| Luz ambiente (A) | ✅ | Aplicada corretamente |
| Câmera (C) | ✅ | FOV e orientação funcionais |
| Luz pontual (L) | ✅ | Com suporte a múltiplas luzes |
| Sombras duras | ✅ | Shadow rays implementados |
| Iluminação difusa | ✅ | Lei de Lambert aplicada |
| ESC fecha programa | ✅ | Hook implementado |
| Botão X fecha programa | ✅ | Close hook implementado |
| Parsing de .rt | ✅ | Parser robusto com validações |
| Tratamento de erros | ✅ | Mensagens "Error\n" + descrição |

### ⚠️ Problemas Identificados

#### 1. Validação de FOV Zero

**Arquivo**: [src/parser/parse_elements.c](src/parser/parse_elements.c#L46-L48)

```c
// ATUAL:
if (error || !validate_range_double(fov, 0.0, 180.0))
    return (error_exit("Camera FOV must be in [0, 180]"), 0);

// PROBLEMA: Aceita FOV = 0, que é matematicamente inválido
// (não gera nenhuma projeção visível)
```

**Solução**:
```c
// CORRIGIDO:
if (error || fov <= 0.0 || fov > 180.0)
    return (error_exit("Camera FOV must be in (0, 180]"), 0);
```

#### 2. Parâmetros Extras Ignorados

**Problema**: O parser ignora parâmetros extras em vez de reportar erro.

```rt
A 0.2 255,255,255 extra_param  # Deveria dar erro, mas é ignorado
```

**Arquivo**: [src/parser/parse_elements.c](src/parser/parse_elements.c)

**Solução**: Adicionar verificação de contagem de parâmetros:
```c
int parse_ambient(char **parts, t_scene *scene)
{
    // Contar parâmetros
    int count = 0;
    while (parts[count])
        count++;

    if (count != 3)  // A, ratio, color
        return (error_exit("Ambient: wrong number of parameters"), 0);

    // ... resto do código
}
```

#### 3. Testes com Vetores Não Normalizados

**Problema**: Vários arquivos de teste contêm vetores de direção não normalizados.

**Arquivos afetados** (conforme relatório de erros):
- tests/level1_basic/02_single_plane.rt
- tests/level1_basic/05_simple_shadows.rt
- tests/level2_intermediate/08_multiple_shadows.rt
- E mais 7 arquivos...

**Observação**: Isso é um problema nos **arquivos de teste**, não no código. O parser corretamente rejeita vetores não normalizados.

## 8.2 Análise de Qualidade do Código

### Pontos Fortes 💪

1. **Modularização**: Código bem separado por funcionalidade
2. **Headers organizados**: Estruturas bem definidas
3. **Operações vetoriais**: Biblioteca matemática sólida
4. **Tratamento de borda**: Uso de EPSILON para evitar divisão por zero

### Áreas para Melhoria 🔧

1. **Otimização de renderização**: Pode ser paralelizado
2. **Estruturas de aceleração**: Falta BVH ou octree para cenas complexas
3. **Tratamento de interior de objetos**: Pode melhorar para casos edge

---

# 🚀 PARTE 9: Melhorias Propostas

## 9.1 Correções de Bugs

### Correção 1: Validação de FOV

```c
// Em src/parser/parse_elements.c, função parse_camera():

// ANTES:
if (error || !validate_range_double(fov, 0.0, 180.0))

// DEPOIS:
if (error || fov <= 0.0 || fov > 180.0)
```

### Correção 2: Validação de Parâmetros Extras

Adicione esta função auxiliar:

```c
static int count_parts(char **parts)
{
    int count = 0;
    while (parts[count])
        count++;
    return count;
}

// Use em cada função de parse:
int parse_ambient(char **parts, t_scene *scene)
{
    if (count_parts(parts) != 3)  // A, ratio, color
        return (error_exit("Ambient: wrong number of parameters"), 0);
    // ...
}

int parse_camera(char **parts, t_scene *scene)
{
    if (count_parts(parts) != 4)  // C, pos, orientation, fov
        return (error_exit("Camera: wrong number of parameters"), 0);
    // ...
}
```

## 9.2 Melhorias de Performance

### 9.2.1 Renderização Multi-threaded

O loop de renderização é "embarassingly parallel" - cada pixel é independente!

```c
#include <pthread.h>

typedef struct s_thread_data
{
    t_scene *scene;
    int     start_y;
    int     end_y;
}   t_thread_data;

void *render_section(void *arg)
{
    t_thread_data *data = (t_thread_data *)arg;
    int x, y;
    t_color color;

    y = data->start_y;
    while (y < data->end_y)
    {
        x = 0;
        while (x < data->scene->width)
        {
            color = trace_ray(data->scene, x, y);
            put_pixel(data->scene->image, x, y, color);
            x++;
        }
        y++;
    }
    return NULL;
}

void render_scene_parallel(t_scene *scene)
{
    pthread_t threads[NUM_THREADS];
    t_thread_data data[NUM_THREADS];
    int rows_per_thread = scene->height / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        data[i].scene = scene;
        data[i].start_y = i * rows_per_thread;
        data[i].end_y = (i == NUM_THREADS - 1) ?
                        scene->height : (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, render_section, &data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);
}
```

### 9.2.2 Barra de Progresso

```c
void render_scene_with_progress(t_scene *scene)
{
    int x, y;
    t_color color;
    int last_percent = -1;
    int current_percent;

    y = 0;
    while (y < scene->height)
    {
        x = 0;
        while (x < scene->width)
        {
            color = trace_ray(scene, x, y);
            put_pixel(scene->image, x, y, color);
            x++;
        }

        // Atualiza progresso
        current_percent = (y * 100) / scene->height;
        if (current_percent != last_percent)
        {
            printf("\rRendering: %d%%", current_percent);
            fflush(stdout);
            last_percent = current_percent;
        }
        y++;
    }
    printf("\rRendering: 100%% - Done!\n");
}
```

## 9.3 Melhorias de Qualidade de Imagem

### 9.3.1 Anti-aliasing (Supersampling)

```c
t_color trace_ray_aa(t_scene *scene, int x, int y, int samples)
{
    t_color total = {0, 0, 0};
    double offset;
    int i, j;

    for (i = 0; i < samples; i++)
    {
        for (j = 0; j < samples; j++)
        {
            // Offset dentro do pixel
            double sub_x = x + (i + 0.5) / samples;
            double sub_y = y + (j + 0.5) / samples;

            t_ray ray = generate_ray_subpixel(scene, sub_x, sub_y);
            t_hit hit = find_closest_hit(scene, ray);

            if (hit.hit)
                total = add_colors(total, calculate_lighting(scene, hit, ray));
        }
    }

    // Média das amostras
    int total_samples = samples * samples;
    total.r /= total_samples;
    total.g /= total_samples;
    total.b /= total_samples;

    return total;
}
```

---

# 🏆 PARTE 10: Guia de Implementação do Bônus

## 10.1 Bônus 1: Reflexão Especular (Phong)

### Conceito

A reflexão especular simula o brilho em superfícies polidas. Quando a luz atinge uma superfície lisa, ela reflete de forma previsível:

```
                    N (normal)
                     ▲
                     │
    L (luz) ─────────●─────────▶ R (reflexão)
              θ      │      θ
                     │
               ──────●──────
                superfície

    Ângulo de incidência = Ângulo de reflexão

    V = direção para o observador (câmera)

    Se R e V estiverem alinhados → brilho máximo (specular highlight)
```

### Fórmula

```
Specular = (R · V)^n × ks × light_intensity × light_color

Onde:
  R = vetor de reflexão da luz
  V = vetor para o observador
  n = expoente de brilho (shininess): maior = brilho mais concentrado
  ks = coeficiente especular (0 a 1)
```

### Implementação

```c
// Adicione ao t_sphere/t_plane/t_cylinder:
typedef struct s_sphere
{
    // ... campos existentes ...
    double  specular;    // Coeficiente especular (0-1)
    int     shininess;   // Expoente (8-256 típico)
}   t_sphere;

// Função para calcular vetor de reflexão
t_vector vec_reflect(t_vector incident, t_vector normal)
{
    // R = I - 2(I·N)N
    double dot = vec_dot(incident, normal);
    return vec_subtract(incident, vec_scale(normal, 2.0 * dot));
}

// Componente especular
static t_color calc_specular(t_vector view, t_vector light_dir,
                             t_vector normal, t_light light,
                             double specular, int shininess)
{
    t_vector reflect;
    double spec_factor;
    t_color result;

    // Direção da luz refletida
    reflect = vec_reflect(vec_negate(light_dir), normal);

    // Quanto a reflexão está alinhada com a câmera
    spec_factor = fmax(0.0, vec_dot(reflect, view));
    spec_factor = pow(spec_factor, shininess);
    spec_factor *= specular * light.brightness;

    result.r = (int)(light.color.r * spec_factor);
    result.g = (int)(light.color.g * spec_factor);
    result.b = (int)(light.color.b * spec_factor);

    return result;
}

// Atualizar calculate_lighting():
t_color calculate_lighting(t_scene *scene, t_hit hit, t_ray ray)
{
    t_color ambient;
    t_color diffuse;
    t_color specular;
    t_color final;
    t_vector view;

    // Direção para a câmera
    view = vec_normalize(vec_negate(ray.direction));

    ambient = calc_ambient(hit.color, scene->ambient);
    diffuse = apply_lights_diffuse(scene, hit);
    specular = apply_lights_specular(scene, hit, view);

    final = add_colors(ambient, diffuse);
    final = add_colors(final, specular);
    final = clamp_color(final);

    return final;
}
```

### Efeito Visual

```
    Sem Specular:              Com Specular:

        ○                          ○
       ╱│╲                        ╱│●╲  ← Highlight!
      ╱ │ ╲                      ╱ │ ╲
     ╱  │  ╲                    ╱  │  ╲
    ╱   │   ╲                  ╱   │   ╲
   ─────┼─────                ─────┼─────

    Superfície fosca          Superfície brilhante
```

## 10.2 Bônus 2: Padrão de Xadrez (Checker)

### Conceito

Aplica um padrão de xadrez alternando entre duas cores baseado nas coordenadas do ponto.

### Implementação

```c
t_color apply_checker(t_color c1, t_color c2, t_vector point, double scale)
{
    int x = (int)floor(point.x / scale);
    int y = (int)floor(point.y / scale);
    int z = (int)floor(point.z / scale);

    // XOR para criar padrão alternado
    if ((x + y + z) % 2 == 0)
        return c1;
    return c2;
}

// Uso em calculate_lighting():
if (hit.has_checker)
{
    hit.color = apply_checker(hit.color, hit.checker_color,
                              hit.point, hit.checker_scale);
}
```

### Modificação do Parser

```rt
# Novo formato para esfera com checker:
sp 0,0,5 2 255,0,0 checker:255,255,255 scale:0.5
#                  ^^^^^^^^^^^^^^^^^^ ^^^^^^^^^^
#                  Cor alternativa     Escala
```

## 10.3 Bônus 3: Luzes Coloridas e Multi-spot

### Conceito

Na versão básica, a cor da luz é ignorada. No bônus, a cor da luz afeta a iluminação.

### Implementação

```c
static t_color calc_diffuse_colored(t_color obj, t_light light,
                                    t_vector n, t_vector l)
{
    double diffuse_factor;
    t_color result;

    diffuse_factor = fmax(0.0, vec_dot(n, l));
    diffuse_factor *= light.brightness;

    // Mistura cor do objeto com cor da luz
    result.r = (int)(obj.r * (light.color.r / 255.0) * diffuse_factor);
    result.g = (int)(obj.g * (light.color.g / 255.0) * diffuse_factor);
    result.b = (int)(obj.b * (light.color.b / 255.0) * diffuse_factor);

    return result;
}
```

### Efeito Visual

```
    Luz Branca:              Luz Vermelha:           Luz Azul:

        ☀️ (255,255,255)         🔴 (255,0,0)            🔵 (0,0,255)
         │                        │                       │
         ▼                        ▼                       ▼
        ┌─┐                      ┌─┐                     ┌─┐
        │█│ Esfera branca        │█│ Tons avermelhados  │█│ Tons azulados
        └─┘                      └─┘                     └─┘
```

## 10.4 Bônus 4: Cone

### Matemática do Cone

Um cone infinito pode ser definido por:

```
(P - apex) · axis = |P - apex| × cos(θ)

Onde:
  apex = ponta do cone
  axis = direção do eixo do cone (normalizado)
  θ = ângulo de abertura do cone
```

Elevando ao quadrado:

```
((P - apex) · axis)² = |P - apex|² × cos²(θ)
```

### Implementação

```c
typedef struct s_cone
{
    t_vector    apex;       // Ponta do cone
    t_vector    axis;       // Direção (normalizado)
    double      angle;      // Ângulo em radianos
    double      height;     // Altura (para cone finito)
    t_color     color;
}   t_cone;

t_hit intersect_cone(t_ray ray, t_cone cone, double closest)
{
    // Variáveis auxiliares
    t_vector    oc = vec_subtract(ray.origin, cone.apex);
    double      cos2 = cos(cone.angle) * cos(cone.angle);
    double      sin2 = sin(cone.angle) * sin(cone.angle);

    double      d_dot_v = vec_dot(ray.direction, cone.axis);
    double      oc_dot_v = vec_dot(oc, cone.axis);

    // Coeficientes da equação quadrática
    double a = cos2 * vec_dot(ray.direction, ray.direction)
               - d_dot_v * d_dot_v;
    double b = 2.0 * (cos2 * vec_dot(ray.direction, oc)
               - d_dot_v * oc_dot_v);
    double c = cos2 * vec_dot(oc, oc) - oc_dot_v * oc_dot_v;

    double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0)
        return create_no_hit();

    // ... resolver quadrática e verificar altura ...
}
```

### Normal do Cone

```c
t_vector cone_normal(t_cone cone, t_vector hit_point)
{
    t_vector to_hit = vec_subtract(hit_point, cone.apex);
    double proj = vec_dot(to_hit, cone.axis);

    // Ponto no eixo mais próximo
    t_vector axis_point = vec_add(cone.apex, vec_scale(cone.axis, proj));

    // Normal aponta para fora, perpendicular ao eixo
    t_vector radial = vec_subtract(hit_point, axis_point);

    // Ajusta para a inclinação do cone
    return vec_normalize(
        vec_subtract(radial,
                     vec_scale(cone.axis, vec_magnitude(radial) * tan(cone.angle)))
    );
}
```

## 10.5 Bônus 5: Bump Mapping

### Conceito

Bump mapping perturba as normais da superfície para criar aparência de textura/relevo sem modificar a geometria.

```
    Sem Bump Mapping:         Com Bump Mapping:

    Normal →  → → → →         Normal → ↗ → ↘ → ↗
              ─────────                ≈≈≈≈≈≈≈≈≈
    Superfície lisa           Aparência de rugosidade
```

### Implementação Simples (Procedural)

```c
t_vector apply_bump(t_vector normal, t_vector point, double scale, double intensity)
{
    // Gera "ruído" baseado na posição
    double noise_x = sin(point.x * scale) * cos(point.z * scale);
    double noise_y = sin(point.y * scale) * cos(point.x * scale);
    double noise_z = sin(point.z * scale) * cos(point.y * scale);

    // Perturba a normal
    t_vector perturbation = vec_create(noise_x, noise_y, noise_z);
    perturbation = vec_scale(perturbation, intensity);

    // Nova normal = normal original + perturbação
    t_vector bumped = vec_add(normal, perturbation);

    return vec_normalize(bumped);
}
```

### Bump Map de Textura

Para usar uma imagem como bump map:

```c
// Carrega bump map (escala de cinza)
unsigned char *bump_map;
int bm_width, bm_height;

t_vector sample_bump_map(t_vector point, t_vector normal)
{
    // Converte coordenadas 3D para UV
    double u = // ... mapeamento depende do tipo de objeto
    double v = // ...

    // Amostra pixels vizinhos para calcular gradiente
    int x = (int)(u * bm_width) % bm_width;
    int y = (int)(v * bm_height) % bm_height;

    double center = bump_map[y * bm_width + x] / 255.0;
    double right = bump_map[y * bm_width + ((x + 1) % bm_width)] / 255.0;
    double up = bump_map[((y + 1) % bm_height) * bm_width + x] / 255.0;

    // Gradiente
    double dx = right - center;
    double dy = up - center;

    // Perturba normal com base no gradiente
    // ...
}
```

---

# 🌟 PARTE 11: Bônus do Bônus - Indo Além

## 11.1 Reflexões Reais (Espelhos)

Crie superfícies que refletem a cena!

```c
t_color trace_ray_recursive(t_scene *scene, t_ray ray, int depth)
{
    if (depth <= 0)
        return (t_color){0, 0, 0};

    t_hit hit = find_closest_hit(scene, ray);

    if (!hit.hit)
        return (t_color){0, 0, 0};

    t_color direct = calculate_lighting(scene, hit, ray);

    // Se o objeto é reflexivo
    if (hit.reflectivity > 0)
    {
        t_vector reflect_dir = vec_reflect(ray.direction, hit.normal);
        t_ray reflect_ray;
        reflect_ray.origin = vec_add(hit.point,
                                     vec_scale(hit.normal, EPSILON));
        reflect_ray.direction = reflect_dir;

        t_color reflected = trace_ray_recursive(scene, reflect_ray, depth - 1);

        // Mistura cor direta com reflexão
        direct = blend_colors(direct, reflected, hit.reflectivity);
    }

    return direct;
}
```

## 11.2 Refração (Transparência)

Simule objetos transparentes como vidro e água!

```c
t_vector vec_refract(t_vector incident, t_vector normal, double eta)
{
    double cos_i = -vec_dot(incident, normal);
    double sin2_t = eta * eta * (1.0 - cos_i * cos_i);

    if (sin2_t > 1.0)
        return vec_reflect(incident, normal);  // Reflexão total interna

    double cos_t = sqrt(1.0 - sin2_t);

    return vec_add(
        vec_scale(incident, eta),
        vec_scale(normal, eta * cos_i - cos_t)
    );
}
```

## 11.3 Profundidade de Campo (Depth of Field)

Simule o foco de uma câmera real!

```c
t_color trace_ray_dof(t_scene *scene, int x, int y)
{
    t_color total = {0, 0, 0};

    for (int sample = 0; sample < DOF_SAMPLES; sample++)
    {
        // Ponto focal na cena
        t_ray focus_ray = generate_ray(scene, x, y);
        t_vector focal_point = ray_at(focus_ray, scene->camera.focal_distance);

        // Origem ligeiramente deslocada na "lente"
        double lens_u = random_double(-1, 1) * scene->camera.aperture;
        double lens_v = random_double(-1, 1) * scene->camera.aperture;

        t_vector lens_offset = vec_add(
            vec_scale(scene->camera.right, lens_u),
            vec_scale(scene->camera.up, lens_v)
        );

        t_ray dof_ray;
        dof_ray.origin = vec_add(scene->camera.position, lens_offset);
        dof_ray.direction = vec_normalize(
            vec_subtract(focal_point, dof_ray.origin)
        );

        total = add_colors(total, trace_ray_single(scene, dof_ray));
    }

    // Média
    total.r /= DOF_SAMPLES;
    total.g /= DOF_SAMPLES;
    total.b /= DOF_SAMPLES;

    return total;
}
```

## 11.4 Ambient Occlusion

Simule sombras suaves em cantos e fendas!

```c
double calculate_ao(t_scene *scene, t_hit hit)
{
    double occlusion = 0.0;

    for (int i = 0; i < AO_SAMPLES; i++)
    {
        // Gera direção aleatória no hemisfério da normal
        t_vector random_dir = random_hemisphere_direction(hit.normal);

        t_ray ao_ray;
        ao_ray.origin = vec_add(hit.point, vec_scale(hit.normal, EPSILON));
        ao_ray.direction = random_dir;

        t_hit ao_hit = find_closest_hit(scene, ao_ray);

        // Se atingiu algo próximo, está "ocludido"
        if (ao_hit.hit && ao_hit.t < AO_RADIUS)
            occlusion += 1.0 - (ao_hit.t / AO_RADIUS);
    }

    return 1.0 - (occlusion / AO_SAMPLES);
}
```

## 11.5 Motion Blur

Simule objetos em movimento!

```c
typedef struct s_animated_sphere
{
    t_sphere    sphere;
    t_vector    velocity;  // Movimento por frame
}   t_animated_sphere;

t_vector sphere_position_at_time(t_animated_sphere *asph, double time)
{
    return vec_add(asph->sphere.center, vec_scale(asph->velocity, time));
}

t_color trace_ray_motion_blur(t_scene *scene, int x, int y)
{
    t_color total = {0, 0, 0};

    for (int sample = 0; sample < MOTION_SAMPLES; sample++)
    {
        // Tempo aleatório durante o "frame"
        double time = random_double(0, 1);

        // Atualiza posições dos objetos para este tempo
        update_scene_for_time(scene, time);

        // Traça raio
        t_color sample_color = trace_ray(scene, x, y);
        total = add_colors(total, sample_color);
    }

    // Média
    total.r /= MOTION_SAMPLES;
    total.g /= MOTION_SAMPLES;
    total.b /= MOTION_SAMPLES;

    return total;
}
```

## 11.6 Volumetric Lighting (God Rays)

Simule raios de luz atravessando poeira/névoa!

```c
t_color ray_march_volume(t_scene *scene, t_ray ray, double max_t)
{
    t_color accumulated = {0, 0, 0};
    double step = 0.1;  // Tamanho do passo
    double t = 0;

    while (t < max_t)
    {
        t_vector sample_point = ray_at(ray, t);

        // Para cada luz
        for (int i = 0; i < scene->light_count; i++)
        {
            // Verifica se este ponto está iluminado
            if (!check_shadow(scene, sample_point, scene->lights[i]))
            {
                // Adiciona um pouco de luz
                double distance = vec_distance(sample_point, scene->lights[i].position);
                double intensity = scene->lights[i].brightness / (distance * distance);

                accumulated.r += (int)(intensity * scene->lights[i].color.r * VOLUME_DENSITY);
                accumulated.g += (int)(intensity * scene->lights[i].color.g * VOLUME_DENSITY);
                accumulated.b += (int)(intensity * scene->lights[i].color.b * VOLUME_DENSITY);
            }
        }

        t += step;
    }

    return clamp_color(accumulated);
}
```

---

# 📋 PARTE 12: Dicas para a Defesa

## 12.1 Perguntas Comuns e Respostas

### "Por que você usa EPSILON?"

**Resposta**: EPSILON (geralmente 1e-6) é usado para evitar problemas de precisão de ponto flutuante. Quando comparamos se um número é "zero", na verdade verificamos se é "muito próximo de zero" porque operações de ponto flutuante podem ter pequenos erros.

**Exemplo prático**: Ao verificar shadow acne, o ponto de intersecção pode estar ligeiramente "dentro" do objeto devido a erros de arredondamento.

### "Explique a fórmula de intersecção da esfera"

**Resposta**: Use as visualizações desta enciclopédia! Mostre como substituímos a equação paramétrica do raio na equação da esfera e chegamos a uma equação quadrática. O discriminante nos diz quantas soluções existem (0, 1 ou 2 intersecções).

### "O que acontece se o vetor de direção não estiver normalizado?"

**Resposta**: Se o vetor não estiver normalizado:
1. O parâmetro `t` não corresponderá diretamente à distância
2. Cálculos de iluminação darão resultados errados (dot product assume vetores unitários)
3. O FOV será calculado incorretamente

### "Como funcionam as sombras?"

**Resposta**: Para cada ponto visível, lançamos um "shadow ray" em direção a cada fonte de luz. Se esse raio atingir algum objeto antes de chegar na luz, o ponto está em sombra para aquela luz específica.

## 12.2 Cenas de Demonstração Sugeridas

### Cena para mostrar sombras
```rt
A 0.1 255,255,255
C 0,5,-15 0,-0.2,1 70
L 5,10,-5 0.8 255,255,255

# Chão
pl 0,-2,0 0,1,0 200,200,200

# Esfera que faz sombra
sp 0,0,0 4 255,0,0

# Esfera atrás (para mostrar sombra)
sp 0,-1.5,3 1 0,255,0
```

### Cena para mostrar cilindro com tampas
```rt
A 0.2 255,255,255
C 0,8,-12 0,-0.5,1 70
L -5,10,-5 0.8 255,255,255

# Cilindro deitado
cy 0,0,0 1,0,0 2 8 0,100,255

# Cilindro em pé
cy 5,0,0 0,1,0 2 4 255,100,0
```

### Cena para mostrar múltiplas luzes
```rt
A 0.1 255,255,255
C 0,5,-12 0,-0.3,1 70
L -5,8,-2 0.5 255,100,100
L 5,8,-2 0.5 100,100,255
L 0,8,-8 0.5 100,255,100

pl 0,-2,0 0,1,0 180,180,180
sp 0,0,0 3 255,255,255
```

## 12.3 Modificações Rápidas Durante a Defesa

Esteja preparado para fazer estas mudanças ao vivo:

1. **Mudar cor de um objeto**: Editar o valor RGB no arquivo .rt
2. **Adicionar/remover luz**: Comentar/descomentar linhas com `#`
3. **Mudar FOV**: Alterar o último valor da linha C
4. **Mover a câmera**: Alterar as coordenadas de posição

---

# 📚 PARTE 13: Referências e Recursos

## 13.1 Livros Recomendados

1. **"Ray Tracing in One Weekend"** - Peter Shirley
   - Gratuito online
   - Excelente introdução passo a passo
   - https://raytracing.github.io/

2. **"Physically Based Rendering: From Theory to Implementation"** - Pharr, Jakob, Humphreys
   - Referência completa sobre renderização
   - Mais avançado

3. **"Real-Time Rendering"** - Akenine-Möller, Haines, Hoffman
   - Foco em performance e técnicas modernas

## 13.2 Recursos Online

- **Scratchapixel**: https://www.scratchapixel.com/
  - Tutoriais detalhados sobre ray tracing

- **The Graphics Codex**: http://graphicscodex.com/
  - Referência rápida de fórmulas e algoritmos

- **Inigo Quilez**: https://iquilezles.org/
  - Técnicas avançadas de renderização e SDFs

## 13.3 Ferramentas Úteis

- **Shadertoy**: https://www.shadertoy.com/
  - Experimente ray tracing em GLSL no navegador

- **GeoGebra 3D**: https://www.geogebra.org/3d
  - Visualize vetores e geometria 3D

- **Desmos**: https://www.desmos.com/calculator
  - Visualize equações e funções

## 13.4 Fórmulas de Referência Rápida

### Equações de Intersecção

```
ESFERA:
  Raio: P(t) = O + tD
  Esfera: |P - C|² = r²
  Quadrática: at² + bt + c = 0
    a = D·D
    b = 2(O-C)·D
    c = (O-C)·(O-C) - r²

PLANO:
  t = ((P₀ - O) · N) / (D · N)
  Condição: |D · N| > ε

CILINDRO (corpo):
  Projete D e OC perpendicular ao eixo A
  D' = D - (D·A)A
  OC' = OC - (OC·A)A
  Resolva |OC' + tD'|² = r²
```

### Iluminação

```
LAMBERT (Difusa):
  I_d = max(0, N·L) × k_d × I_light

PHONG (Especular):
  R = 2(N·L)N - L
  I_s = (R·V)^n × k_s × I_light

REFLEXÃO:
  R = I - 2(I·N)N

REFRAÇÃO (Lei de Snell):
  n₁ sin(θ₁) = n₂ sin(θ₂)
```

---

# 🎯 Conclusão

O miniRT é muito mais do que um projeto de programação - é uma jornada pelo fascinante mundo da computação gráfica. Ao completá-lo, você terá:

1. **Dominado matemática vetorial** aplicada a problemas reais
2. **Entendido como imagens são geradas** pixel a pixel
3. **Implementado um ray tracer funcional** do zero
4. **Aprendido a parsear arquivos** de configuração
5. **Praticado gestão de memória** em C

Os bônus não são apenas "extras" - são oportunidades de explorar tópicos mais avançados que formam a base de toda a indústria de jogos, cinema e design 3D.

**Lembre-se**: O mais importante não é apenas fazer o código funcionar, mas entender **por que** ele funciona. Essa compreensão profunda é o que diferencia um programador mediano de um excelente.

Boa sorte na sua jornada! 🚀

---

*"A arte é a mentira que nos permite perceber a verdade."* - Pablo Picasso

*No ray tracing, simulamos mentiras (simplificações da física real) que nos permitem ver mundos virtuais que parecem reais.*

---

**Versão**: 1.0
**Última atualização**: Janeiro 2026
**Autor**: Enciclopédia gerada para o projeto miniRT da Ecole 42
