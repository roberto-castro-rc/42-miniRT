# 🎯 Roteiro de Avaliação — miniRT

## Como usar este guia

Leia antes da avaliação. Cada seção cobre uma área que o avaliador pode perguntar.
Tenha cenas prontas para demonstrar cada feature.

---

## 1. Visão Geral do Projeto

### O que é o miniRT?

Um **ray tracer** escrito em C que gera imagens 3D a partir de arquivos `.rt`.
Para cada pixel da janela (1280×720), lançamos um raio da câmera e verificamos
se ele atinge algum objeto. Se sim, calculamos a cor com base na iluminação.

### Pipeline de execução

```
main → parse_scene → init_camera → render_scene → mlx_loop → cleanup_scene
```

1. **parse_scene**: Lê o `.rt` linha por linha, identifica cada elemento pelo identificador
2. **init_camera**: Calcula os vetores right/up e o viewport a partir do FOV
3. **render_scene**: Percorre cada pixel (x,y), chama `trace_ray`
4. **trace_ray**: Gera raio → encontra hit mais próximo → aplica materiais → calcula iluminação
5. **mlx_loop**: Exibe a janela até ESC ou fechar

---

## 2. Formato do Arquivo de Cena (.rt)

### Elementos únicos (maiúsculas — só podem aparecer uma vez)

| Elemento | Formato | Exemplo |
|----------|---------|---------|
| **A** (Ambient) | `A <ratio> <R,G,B>` | `A 0.2 255,255,255` |
| **C** (Camera) | `C <x,y,z> <nx,ny,nz> <fov>` | `C 0,0,0 0,0,1 70` |
| **L** (Light) | `L <x,y,z> <brightness> <R,G,B>` | `L -5,5,0 0.7 255,255,255` |

### Objetos (minúsculas — podem aparecer quantas vezes quiser)

| Objeto | Formato | Exemplo |
|--------|---------|---------|
| **sp** (Sphere) | `sp <center> <diameter> <R,G,B> [material]` | `sp 0,0,5 2 255,0,0 checker` |
| **pl** (Plane) | `pl <point> <normal> <R,G,B> [material]` | `pl 0,-1,0 0,1,0 200,200,200` |
| **cy** (Cylinder) | `cy <center> <axis> <diameter> <height> <R,G,B> [material]` | `cy 0,-1,5 0,1,0 2 4 100,100,255` |
| **cn** (Cone) | `cn <center> <axis> <angle> <height> <R,G,B> [material]` | `cn 0,0,10 0,1,0 30 3 255,200,0 bump` |

### Detalhamento de cada coluna

**Ambient (A)**:
- `ratio`: Intensidade da luz ambiente, de 0.0 (escuro total) a 1.0 (totalmente iluminado). Garante que nenhum objeto fique 100% preto nas sombras.
- `R,G,B`: Cor da luz ambiente (0-255 cada). No bonus, multiplica com a cor do objeto.

**Camera (C)**:
- `x,y,z`: Posição da câmera no espaço 3D (de onde estamos olhando)
- `nx,ny,nz`: Vetor de direção normalizado (para onde estamos olhando). Valores entre -1 e 1. Exemplo: `0,0,1` olha para frente no eixo Z.
- `fov`: Campo de visão horizontal em graus (0-180). Quanto maior, mais "aberto" — 70 é natural, 120 é olho de peixe.

**Light (L)**:
- `x,y,z`: Posição da fonte de luz no espaço 3D
- `brightness`: Intensidade da luz, de 0.0 a 1.0
- `R,G,B`: Cor da luz (bonus). No mandatory a cor é ignorada, usa-se branco.

**Sphere (sp)**:
- `center (x,y,z)`: Centro da esfera no espaço 3D
- `diameter`: Diâmetro da esfera (raio = diameter / 2)
- `R,G,B`: Cor da superfície
- `[material]`: Opcional no bonus — `checker` ou `bump`

**Plane (pl)**:
- `point (x,y,z)`: Um ponto qualquer que pertence ao plano
- `normal (nx,ny,nz)`: Vetor perpendicular ao plano (normalizado). `0,1,0` = plano horizontal (chão)
- `R,G,B`: Cor da superfície

**Cylinder (cy)**:
- `center (x,y,z)`: Centro da base do cilindro
- `axis (nx,ny,nz)`: Direção do eixo (normalizado). `0,1,0` = cilindro em pé
- `diameter`: Diâmetro do cilindro
- `height`: Altura do cilindro ao longo do eixo
- `R,G,B`: Cor da superfície

**Cone (cn)** (bonus):
- `center (x,y,z)`: Vértice (ponta) do cone
- `axis (nx,ny,nz)`: Direção do eixo (normalizado)
- `angle`: Metade do ângulo de abertura em graus (0-90). 30° = cone médio.
- `height`: Até onde o cone se estende ao longo do eixo
- `R,G,B`: Cor da superfície

---

## 3. Conceitos Matemáticos — Como Explicar

### 3.1 Equação do Raio

```
P(t) = origin + t × direction
```

- `origin`: posição da câmera
- `direction`: vetor normalizado apontando para o pixel
- `t`: parâmetro escalar — `t > 0` significa à frente da câmera

**Se o avaliador perguntar**: "O raio é uma reta semi-infinita. Para cada pixel, calculo a direção com base no FOV e na posição do pixel na grade do viewport. Depois testo essa reta contra todos os objetos para achar o `t` mais próximo — ou seja, o primeiro objeto que o raio atinge."

### 3.2 Interseção com Esfera

Substitui P(t) na equação da esfera `|P - C|² = r²`:

```
a = D·D
b = 2 × OC·D
c = OC·OC - r²
discriminante = b² - 4ac
```

- `discriminante < 0`: raio não atinge (nenhum hit)
- `discriminante = 0`: tangente (toca em 1 ponto)
- `discriminante > 0`: atravessa (2 pontos — pegar o menor t > 0)

**Se perguntar "e se a câmera estiver dentro da esfera?"**: Nesse caso `t1 < 0` e `t2 > 0`, pegamos `t2`.

### 3.3 Interseção com Plano

```
t = (pl.point - ray.origin) · normal / (ray.direction · normal)
```

Se `denominador ≈ 0`, o raio é paralelo ao plano (sem interseção).
A normal é invertida se estiver apontando na mesma direção do raio (para iluminar dos dois lados).

### 3.4 Interseção com Cilindro

É a mais complexa. O cilindro tem **corpo + 2 tampas (caps)**.

**Corpo**: Projeta o raio no plano perpendicular ao eixo e resolve uma equação quadrática 2D. Depois verifica se o ponto de hit está dentro da altura (`0 ≤ h ≤ height`).

**Tampas**: Trata como dois planos circulares perpendiculares ao eixo. Calcula a interseção com o plano e verifica se o ponto está dentro do raio.

Resultado final: `best_hit(body, cap_bottom, cap_top)` — retorna o mais próximo.

### 3.5 Interseção com Cone (bonus)

Similar ao cilindro, mas a equação quadrática usa `cos²(θ)` onde θ é o half_angle:

```
a = (D·V)² − cos²θ × (D·D)
b = 2 × [(D·V)(OC·V) − cos²θ × (D·OC)]
c = (OC·V)² − cos²θ × (OC·OC)
```

Clipagem no corpo: `0 ≤ h ≤ height`. Apenas uma tampa na base (o vértice é a ponta).

### 3.6 Produto Escalar (dot product)

```
A·B = |A| × |B| × cos(θ)
```

- Usado em **iluminação difusa**: `max(0, N·L)` — quanto a superfície está virada para a luz
- Usado em **sombra**: determinar se a superfície está "de costas" para a luz
- Usado em **especular**: `max(0, R·V)^n` — brilho do reflexo

**Se perguntar "para que serve o dot product?"**: É a ferramenta central do ray tracing. Mede o ângulo entre dois vetores. Retorna positivo se apontam na mesma direção, zero se perpendiculares, negativo se opostos.

---

## 4. Sistema de Iluminação

### Componentes da cor final

```
cor_pixel = ambient + Σ(diffuse + specular) por cada luz sem sombra
```

**Ambient** = `cor_objeto × cor_ambient × ratio_ambient`
- Garante que nada fique 100% preto. É uma "luz mínima" global.

**Diffuse** (Lambert) = `cor_objeto × max(0, N·L) × brightness`
- Quanto mais a superfície está de frente para a luz, mais ela é iluminada.
- `N` = normal da superfície, `L` = direção para a luz.

**Specular** (Phong, bonus) = `cor_luz × max(0, R·V)^shininess × brightness`
- Cria o brilho pontual (reflexo da luz na superfície).
- `R` = reflexo do vetor de luz sobre a normal, `V` = direção para a câmera.
- `shininess = 32` — quanto maior, menor e mais focado o brilho.

### Sombras

Para cada ponto iluminado, lançamos um **shadow ray** do ponto até a luz.
Se esse raio atinge algum objeto antes de chegar na luz (`hit.t < distância_até_luz`),
o ponto está na sombra dessa luz → ignora a contribuição diffuse+specular dessa luz.

**Offset da sombra** (EPSILON): O ponto de origem do shadow ray é deslocado levemente
na direção da normal (`point + normal × 0.000001`). Sem isso, o shadow ray pode
atingir o próprio objeto de onde saiu (self-shadowing / acne).

---

## 5. Câmera e Viewport

### Como o FOV vira viewport

```c
theta = fov × π / 180
viewport_height = 2 × tan(theta / 2)
viewport_width = viewport_height × aspect_ratio  // 1280/720
```

### Como gerar o raio para cada pixel

```c
u = (2 × x / (width - 1) - 1) × viewport_width / 2
v = (1 - 2 × y / (height - 1)) × viewport_height / 2
direction = normalize(orientation + right × u + up × v)
```

- `x` varia de 0 a 1279 (colunas = horizontal)
- `y` varia de 0 a 719 (linhas = vertical)
- `u` mapeia x para -viewport_width/2 a +viewport_width/2
- `v` mapeia y para +viewport_height/2 a -viewport_height/2 (Y invertido: topo é positivo)

**Os vetores right e up** são calculados com cross product da orientação com `world_up (0,1,0)`.
Caso especial: se a câmera aponta direto para cima/baixo, usa `world_up = (0,0,1)`.

---

## 6. Bônus Implementados

### 6.1 Multi-light colorida (Step 1)

- Suporte a múltiplas luzes na cena (`L` pode aparecer várias vezes)
- Cada luz tem sua própria cor RGB que se multiplica com a cor do objeto
- A cor da ambient também é considerada

**Demonstrar com**: cenas com 2-3 luzes de cores diferentes iluminando os mesmos objetos.

### 6.2 Reflexão Especular — Phong (Step 2)

- `R = 2(N·L)N - L` — reflete o vetor de luz sobre a normal
- `spec = max(0, R·V)^32 × brightness`
- Adiciona um ponto branco/brilhante na superfície onde a luz reflete direto para a câmera

**Demonstrar com**: esferas com brilho visível — um ponto branco nítido.

### 6.3 Cone (Step 3)

- Novo objeto `cn` com equação quadrática baseada em cos²(half_angle)
- Suporte a corpo + tampa circular na base
- Normal do corpo: combinação de cos(θ) e sin(θ) em relação ao eixo

**Demonstrar com**: cenas com cones em diferentes ângulos e alturas, inclusive cones no mandatory dando erro.

### 6.4 Checkerboard (Step 4)

- Padrão xadrez procedural (sem texturas/imagens)
- UV mapping por tipo de objeto:
  - **Esfera**: coordenadas esféricas (atan2, asin)
  - **Plano**: projeção tangente/bitangente
  - **Cilindro/Cone**: ângulo ao redor do eixo + altura
- Fórmula: `if (floor(u × 10) + floor(v × 10)) é ímpar → preto`
- Ativado com keyword `checker` no `.rt`

**Demonstrar com**: chão xadrez, esfera quadriculada.

### 6.5 Bump Mapping (Step 5)

- Perturbação procedural das normais com funções sin/cos
- Não muda a geometria, só altera como a luz incide → aparência de rugosidade
- `perturbation = (sin(x×8)×cos(z×8), sin(y×8)×cos(x×8), sin(z×8)×cos(y×8)) × 0.3`
- Normal final = `normalize(normal_original + perturbation)`
- Ativado com keyword `bump` no `.rt`

**Demonstrar com**: esfera rugosa, plano com ondulações.

---

## 7. Tratamento de Erros

O programa deve imprimir `Error\n` seguido de mensagem descritiva para:

| Erro | Exemplo |
|------|---------|
| Argumento inválido | `./miniRT` (sem arquivo) |
| Extensão errada | `./miniRT scene.txt` |
| Arquivo vazio | `.rt` sem nenhum elemento |
| Elemento faltando | Cena sem A ou sem C |
| Elemento duplicado | Dois A ou dois C |
| Ratio fora de range | `A 1.5 255,255,255` |
| Cor fora de range | `A 0.2 300,0,0` |
| FOV inválido | `C 0,0,0 0,0,1 200` |
| Normal não normalizada | `pl 0,0,0 1,1,0 255,0,0` (magnitude ≠ 1) |
| Vetor zero | `pl 0,0,0 0,0,0 255,0,0` |
| Diâmetro/altura ≤ 0 | `sp 0,0,0 -1 255,0,0` |
| Brightness fora de range | `L 0,0,0 1.5 255,255,255` |
| Keyword desconhecida | `sp 0,0,5 2 255,0,0 mirror` |
| Identificador desconhecido | `triangle 0,0,0 ...` |

**Dica na avaliação**: Tenha as cenas de `tests/error_tests/` prontas para rodar rapidamente.

---

## 8. Perguntas Prováveis da Avaliação

### Parte Obrigatória

1. **"Como funciona o ray tracing?"**
   → Para cada pixel, lanço um raio da câmera, testo contra todos os objetos, pego o hit mais próximo, calculo a iluminação (ambient + diffuse + sombras) e pinto o pixel.

2. **"O que é a equação paramétrica do raio?"**
   → `P(t) = origin + t × direction`. t é o parâmetro que percorre a reta. Só nos interessam t > 0 (frente).

3. **"Como funciona a interseção com a esfera?"**
   → Substituo P(t) na eq da esfera, resulta numa quadrática. Discriminante negativo = sem hit.

4. **"Como as sombras funcionam?"**
   → Shadow ray: do ponto de hit até a luz. Se bate em algo antes, está na sombra.

5. **"O que é EPSILON e por que existe?"**
   → 1e-6. Evita self-intersection numérico — sem ele, o shadow ray pode atingir o próprio objeto por erro de ponto flutuante.

6. **"Como o FOV afeta a imagem?"**
   → FOV menor = zoom in (mais apertado). FOV maior = grande angular (mais aberto). Convertemos para viewport_height via tangente.

7. **"O que acontece se a câmera estiver dentro de um objeto?"**
   → O t1 fica negativo, usamos t2. Renderiza o interior normalmente.

8. **"Faz alguma coisa de diferente se o objeto estiver atrás da câmera?"**
   → Se ambos t1 e t2 são negativos, retornamos no_hit. Objeto atrás da câmera não é renderizado.

### Bônus

9. **"O que é o modelo de iluminação de Phong?"**
   → Phong = ambient + diffuse (Lambert) + specular. O specular cria o brilho do reflexo da luz, calculado com o vetor refletido e a direção da câmera.

10. **"Como funciona o checkerboard sem textura?"**
    → UV mapping procedural. Converto as coordenadas 3D do hit em 2D (u,v), multiplico por uma escala, e alterno preto/cor baseado na paridade de floor(u) + floor(v).

11. **"O que é bump mapping?"**
    → Perturbação da normal com ruído procedural (sin/cos). Não muda a geometria, só como a luz é calculada, dando aparência de rugosidade.

12. **"Como funciona a interseção do cone?"**
    → Equação quadrática similar ao cilindro, mas com cos²(half_angle). Clipamos no height e adicionamos uma tampa circular na base.

---

## 9. Cenas de Demonstração

### Ordem sugerida de apresentação

1. **Cena simples** — `scenes/simple.rt` → esfera vermelha, prova que funciona
2. **Multi-objetos** — `scenes/test_multi.rt` → múltiplos objetos e luzes
3. **Erros** — `tests/error_tests/` → mostrar 3-4 erros sendo detectados
4. **Bonus step 1** — Luzes coloridas → `tests/bonus_tests/step1_multilight/`
5. **Bonus step 2** — Specular → mostrar o brilho branco na esfera
6. **Bonus step 3** — Cone → `tests/bonus_tests/step3_cone/`
7. **Bonus step 4** — Checkerboard → `tests/bonus_tests/step4_checkerboard/`
8. **Bonus step 5** — Bump → `tests/bonus_tests/step5_bumpmap/`

### Rodar todos os testes automaticamente

```bash
bash tests/test_bonus.sh progress    # Dashboard rápido
bash tests/test_bonus.sh all         # Todos os steps
bash tests/test_bonus.sh step4       # Step específico
```

---

## 10. Estrutura do Código

```
src/
  core/           main, cleanup, error
  parser/         parse_scene, parse_elements, parse_objects, parse_sphere,
                  parse_cone, parse_utils, validate, ft_atod
  scene/          init_scene, camera (FOV → viewport, vetores right/up)
  math/           vec_basic (add, sub, scale, negate), vec_products (dot, cross),
                  vec_utils (magnitude, normalize, distance), vec_reflect
  render/         render_scene (loop de pixels), ray_trace (trace_ray, find_closest_hit),
                  lighting (ambient + multi-light), shadows (shadow rays),
                  specular (Phong), checkerboard (UV mapping), bumpmap (noise)
  shapes/         intersect_sphere, intersect_plane, intersect_cylinder + utils,
                  intersect_cone + utils
  utils/          color (add, scale, clamp, multiply, to_int)
  mlx/            mlx_init, mlx_events (ESC, close), mlx_utils (put_pixel)
```

### Fluxo de dados principal

```
main
 └→ parse_scene(filename, &scene)      // preenche t_scene
 └→ init_camera(&scene.camera)         // calcula right, up, viewport
 └→ render_scene(&scene)
      └→ para cada pixel (x, y):
           └→ trace_ray(scene, x, y)
                └→ generate_ray(scene, x, y)     // pixel → raio 3D
                └→ find_closest_hit(scene, ray)   // testa todos os objetos
                └→ apply_checkerboard/bump(&hit)  // se material != 0
                └→ calculate_lighting(scene, hit, ray)
                     └→ calc_ambient(...)
                     └→ para cada luz:
                          └→ check_shadow(...)     // shadow ray
                          └→ calc_diffuse(...)     // Lambert
                          └→ calc_specular(...)    // Phong
                └→ clamp_color()                   // limita a 0-255
```

---

## 11. Dicas Finais

- **Compile na frente do avaliador**: `make re` e `make bonus`
- **ESC e X**: Teste fechar a janela das duas formas
- **Valgrind**: Se pedirem, rode com `valgrind --suppressions=mlx.supp ./miniRT cena.rt`
- **Se pedirem para alterar algo**: Modifique um `.rt` ao vivo (ex: mudar cor, posição, FOV)
- **Norminette**: `norminette src/ includes/` — os únicos erros devem ser INVALID_HEADER
- **Não entre em pânico**: Você sabe como cada parte funciona. O ray tracing é elegante — o raio é uma reta, a esfera é uma quadrática, o dot product faz tudo.
