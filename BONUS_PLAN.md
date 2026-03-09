# Plano de Implementação — Bônus miniRT

## TL;DR

Os 5 bônus do subject (reflexão especular Phong, checkerboard, luzes coloridas
multi-spot, cone, bump mapping) serão implementados incrementalmente na branch
`bonus/implementation`. A arquitetura segue **separação total**: todos os arquivos
da build bonus são cópias independentes com sufixo `_bonus.c` / `_bonus.h`, mesmo
os que permanecem iguais à mandatory. Isso garante que `make` e `make bonus` sejam
builds 100% isoladas, como exigido pelo subject da 42. A ordem é:
multi-light → especular → cone → checkerboard → bump map — cada passo constrói
sobre a infraestrutura do anterior.

---

## Justificativa da Ordem

| Ordem | Bônus                    | Porquê primeiro |
|-------|--------------------------|-----------------|
| 1     | **Multi-light colorida** | É o fix mais simples — a infraestrutura já existe (`t_light.color` parseado, `apply_lights` itera `light_count`). Só falta desbloquear o parser e multiplicar `light.color` no cálculo. Validável imediatamente com cenas coloridas. |
| 2     | **Especular Phong**      | Completa o modelo Phong (ambient + diffuse + specular). Depende de multi-light para testar com várias luzes. Adiciona `vec_reflect` reutilizável para futura reflexão recursiva. |
| 3     | **Cone**                 | Segue o padrão exato do cilindro (struct, parse, intersect corpo+caps, cleanup). Bônus mais "mecânico" — copia-e-adapta. Feito após iluminação estar completa para que cone renderize com Phong. |
| 4     | **Checkerboard**         | Precisa de UV mapping por forma. Feito após cone existir para que cone também possa ter checkerboard. Introduz coordenadas de textura (base do bump map). |
| 5     | **Bump mapping**         | O mais complexo — leitura de textura, UV mapping (já do checkerboard), perturbação de normais. Por último, todo o resto já está estável. |

---

## Arquitetura: Separação Total

O subject exige: *"Bonuses must be in `_bonus.{c/h}` files"*. A build bonus usa
**exclusivamente** arquivos `_bonus.c` e `minirt_bonus.h`. Nenhum `.c` é
compartilhado entre `make` e `make bonus`.

### Headers

```
includes/
  minirt.h          ← INALTERADO (build mandatory)
  vectors.h         ← INALTERADO (build mandatory)
  minirt_bonus.h    ← NOVO: cópia de minirt.h + structs extras + protótipos bonus
  vectors_bonus.h   ← NOVO: cópia de vectors.h (incluso por minirt_bonus.h)
```

### Arquivos Fonte — Cópias sem mudança (renomeados `_bonus.c`)

Estes são cópias 1:1 do mandatory, apenas com `#include "minirt_bonus.h"` no topo:

```
src/math/vec_basic_bonus.c            ← cópia de vec_basic.c
src/math/vec_products_bonus.c         ← cópia de vec_products.c
src/math/vec_utils_bonus.c            ← cópia de vec_utils.c
src/mlx/mlx_init_bonus.c             ← cópia de mlx_init.c
src/mlx/mlx_events_bonus.c           ← cópia de mlx_events.c
src/mlx/mlx_utils_bonus.c            ← cópia de mlx_utils.c
src/scene/camera_bonus.c             ← cópia de camera.c
src/scene/init_scene_bonus.c         ← cópia de init_scene.c
src/parser/parse_utils_bonus.c       ← cópia de parse_utils.c
src/parser/parse_elements_bonus.c    ← cópia de parse_elements.c
src/parser/parse_objects_bonus.c     ← cópia de parse_objects.c
src/parser/parse_sphere_bonus.c      ← cópia de parse_sphere.c (*)
src/parser/ft_atod_bonus.c           ← cópia de ft_atod.c
src/parser/validate_bonus.c          ← cópia de validate.c
src/shapes/intersect_sphere_bonus.c  ← cópia de intersect_sphere.c
src/shapes/intersect_plane_bonus.c   ← cópia de intersect_plane.c
src/shapes/intersect_cylinder_bonus.c ← cópia de intersect_cylinder.c
src/shapes/cylinder_utils_bonus.c    ← cópia de cylinder_utils.c
src/core/error_bonus.c               ← cópia de error.c
src/render/render_bonus.c            ← cópia de render.c
src/render/shadows_bonus.c           ← cópia de shadows.c
```

(*) `parse_sphere_bonus.c` será modificado no Step 4 para aceitar campo `checker`/`bump`

### Arquivos Fonte — Modificados na build bonus

```
src/core/main_bonus.c                ← cópia de main.c, include minirt_bonus.h
src/core/cleanup_bonus.c             ← cópia de cleanup.c + free(cones) + free texturas
src/parser/parse_scene_bonus.c       ← cópia de parse_scene.c + multi-light + cone
src/render/lighting_bonus.c          ← cópia de lighting.c + Phong + luz colorida
src/render/ray_trace_bonus.c         ← cópia de ray_trace.c + check_cones
src/utils/color_bonus.c              ← cópia de color.c + multiply_colors
```

### Arquivos Fonte — Novos (sem equivalente mandatory)

```
src/math/vec_reflect_bonus.c         ← vec_reflect() para especular
src/shapes/intersect_cone_bonus.c    ← interseção corpo + caps do cone
src/shapes/cone_utils_bonus.c        ← helpers do cone
src/parser/parse_cone_bonus.c        ← parse "cn" identifier
src/render/specular_bonus.c          ← calc_specular()
src/render/checkerboard_bonus.c      ← pattern xadrez por forma geométrica
src/render/bump_map_bonus.c          ← perturbação de normais via textura
src/utils/texture_bonus.c            ← leitura de PPM + sampling
```

### Total: ~30 arquivos `_bonus.c` + 2 headers `_bonus.h`

---

## Makefile

```makefile
# Variáveis existentes para mandatory (SRCS, OBJS) permanecem iguais.
# Nova variável BONUS_SRCS lista todos os _bonus.c:
BONUS_SRCS = src/core/main_bonus.c \
             src/core/cleanup_bonus.c \
             src/core/error_bonus.c \
             src/scene/init_scene_bonus.c \
             src/scene/camera_bonus.c \
             src/parser/parse_scene_bonus.c \
             src/parser/parse_elements_bonus.c \
             src/parser/parse_objects_bonus.c \
             src/parser/parse_sphere_bonus.c \
             src/parser/parse_cone_bonus.c \
             src/parser/parse_utils_bonus.c \
             src/parser/ft_atod_bonus.c \
             src/parser/validate_bonus.c \
             src/math/vec_basic_bonus.c \
             src/math/vec_products_bonus.c \
             src/math/vec_utils_bonus.c \
             src/math/vec_reflect_bonus.c \
             src/render/render_bonus.c \
             src/render/ray_trace_bonus.c \
             src/render/lighting_bonus.c \
             src/render/specular_bonus.c \
             src/render/shadows_bonus.c \
             src/render/checkerboard_bonus.c \
             src/render/bump_map_bonus.c \
             src/shapes/intersect_sphere_bonus.c \
             src/shapes/intersect_plane_bonus.c \
             src/shapes/intersect_cylinder_bonus.c \
             src/shapes/cylinder_utils_bonus.c \
             src/shapes/intersect_cone_bonus.c \
             src/shapes/cone_utils_bonus.c \
             src/utils/color_bonus.c \
             src/utils/texture_bonus.c \
             src/mlx/mlx_init_bonus.c \
             src/mlx/mlx_events_bonus.c \
             src/mlx/mlx_utils_bonus.c

BONUS_OBJS = $(BONUS_SRCS:%.c=$(OBJ_DIR)/%.o)

bonus: $(LIBFT) $(MLX) $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) $(LDFLAGS) -o $(NAME)

.PHONY: ... bonus
```

- `make` compila a mandatory normalmente
- `make bonus` compila apenas `_bonus.c` → mesmo binário `miniRT`
- `make fclean` limpa tudo (obj de ambas)
- **Sem conflito**: os `.o` ficam em paths diferentes (`obj/src/.../file.o` vs `obj/src/.../file_bonus.o`)

---

## Steps Detalhados

### Step 0 — Preparação da infraestrutura

**0.1 — Criar `includes/minirt_bonus.h`:**
- Cópia de `minirt.h` com as seguintes adições:
  - `#include "vectors_bonus.h"` no lugar de `vectors.h`
  - Struct `t_cone`:
    ```c
    typedef struct s_cone
    {
        t_vector    center;
        t_vector    axis;
        double      half_angle;
        double      height;
        t_color     color;
        int         material;
    }   t_cone;
    ```
  - Struct `t_texture`:
    ```c
    typedef struct s_texture
    {
        int     width;
        int     height;
        t_color *pixels;
    }   t_texture;
    ```
  - Campo `int material` adicionado a `t_sphere`, `t_plane`, `t_cylinder`
    (0=solid, 1=checkerboard, 2=bump)
  - Campo `char *bump_file` adicionado a `t_sphere`, `t_plane`, `t_cylinder`, `t_cone`
  - Campos `t_cone *cones; int cone_count;` em `t_scene`
  - Campo `t_texture *textures; int texture_count;` em `t_scene`
  - Campo `int material` em `t_hit` (para saber o tipo de pattern)
  - `#define SHININESS 32`
  - `#define CHECKER_SCALE 10.0`
  - Protótipos novos:
    - `t_vector vec_reflect(t_vector incident, t_vector normal);`
    - `t_color calc_specular(t_hit hit, t_light light, t_vector view, t_vector ldir);`
    - `t_hit intersect_cone(t_ray ray, t_cone cn, double closest);`
    - `t_hit create_cone_body_hit(t_ray ray, t_cone cn, double t);`
    - `int check_cone_body(t_ray ray, t_cone cn, double t, double cl);`
    - `t_hit check_cone_cap(t_ray ray, t_cone cn, t_vector cap, double cl);`
    - `int parse_cone(char *line, t_scene *scene);`
    - `t_color multiply_colors(t_color c1, t_color c2);`
    - `void apply_checkerboard(t_hit *hit, t_scene *scene);`
    - `void apply_bump_map(t_hit *hit, t_texture *tex);`
    - `t_texture *load_ppm(char *filename);`
    - `void free_texture(t_texture *tex);`
    - `t_color sample_texture(t_texture *tex, double u, double v);`

**0.2 — Criar `includes/vectors_bonus.h`:**
- Cópia exata de `vectors.h` com header guard `VECTORS_BONUS_H`

**0.3 — Copiar todos os .c mandatory → _bonus.c:**
- Script automatizado: para cada .c em SRCS, copiar para `_bonus.c`
- Em cada cópia, trocar `#include "minirt.h"` por `#include "minirt_bonus.h"`
- Validar que `make bonus` compila sem erros antes de qualquer mudança funcional

**0.4 — Atualizar Makefile:**
- Adicionar `BONUS_SRCS`, `BONUS_OBJS`, regra `bonus`
- Adicionar `bonus` ao `.PHONY`

**0.5 — Checkpoint:** `make` e `make bonus` devem compilar e produzir binários idênticos.

---

### Step 1 — Multi-light colorida (~30 linhas de mudança)

**1.1 — Editar `src/parser/parse_scene_bonus.c`:**
- Na função `parse_line`: remover a condição `!(*flags & 4)` da linha do `"L"`
- Permitir múltiplas luzes sem restrição de flag
- Adicionar `else if (ft_strncmp(parts[0], "cn", 3) == 0)` (placeholder para Step 3)

**1.2 — Editar `src/utils/color_bonus.c`:**
- Adicionar função `multiply_colors`:
  ```c
  t_color multiply_colors(t_color c1, t_color c2)
  {
      t_color result;
      result.r = c1.r * c2.r / 255;
      result.g = c1.g * c2.g / 255;
      result.b = c1.b * c2.b / 255;
      return (result);
  }
  ```

**1.3 — Editar `src/render/lighting_bonus.c`:**
- `calc_ambient`: multiplicar por `ambient.color`:
  `result = multiply_colors(scale_color(obj_color, ratio), ambient.color)`
- `calc_diffuse`: multiplicar resultado por `light.color`:
  `result = multiply_colors(result, light.color)` após o cálculo difuso

**1.4 — Verificação:**
- `make bonus`
- Rodar: `./miniRT tests/bonus/01_multi_lights.rt`
- Esperado: 3 luzes com cores R, G, B distintas iluminando a cena

---

### Step 2 — Reflexão Especular Phong (~60 linhas novas)

**2.1 — Criar `src/math/vec_reflect_bonus.c`:**
```c
#include "minirt_bonus.h"

t_vector    vec_reflect(t_vector incident, t_vector normal)
{
    double      dot;

    dot = vec_dot(incident, normal);
    return (vec_subtract(incident, vec_scale(normal, 2.0 * dot)));
}
```

**2.2 — Criar `src/render/specular_bonus.c`:**
```c
#include "minirt_bonus.h"

t_color calc_specular(t_hit hit, t_light light,
        t_vector view_dir, t_vector light_dir)
{
    t_vector    reflect_dir;
    double      spec_dot;
    double      spec_factor;

    reflect_dir = vec_reflect(vec_negate(light_dir), hit.normal);
    spec_dot = fmax(0.0, vec_dot(reflect_dir, vec_normalize(view_dir)));
    spec_factor = pow(spec_dot, SHININESS) * light.brightness;
    return (scale_color(light.color, spec_factor));
}
```

**2.3 — Editar `src/render/lighting_bonus.c`:**
- Em `apply_lights`: calcular `specular = calc_specular(hit, light, ray.direction, light_dir)`
  e somar ao total junto com diffuse
- Em `calculate_lighting`: remover `(void)ray`, passar `ray` para `apply_lights`
- Signature de `apply_lights` muda: `static t_color apply_lights(t_scene *scene, t_hit hit, t_ray ray)`

**2.4 — Verificação:**
- Cena com esfera e spot lateral → halo branco brilhante visível
- Mover a posição da luz na cena → highlight acompanha

---

### Step 3 — Cone (~180 linhas novas)

**3.1 — Criar `src/parser/parse_cone_bonus.c`:**
- Formato: `cn x,y,z axis_x,y,z half_angle height R,G,B [checker|bump path]`
- Segue padrão de `parse_sphere_bonus.c`:
  `ft_split → validar nº de campos → parse_vector → parse_double → add to array`
- `half_angle` em graus, converter para radianos: `cone.half_angle = angle * M_PI / 180.0`

**3.2 — Ativar em `src/parser/parse_scene_bonus.c`:**
- O `else if` do `"cn"` agora chama `parse_cone(line, scene)`

**3.3 — Criar `src/shapes/intersect_cone_bonus.c`:**
- Equação quadrática do cone infinito com eixo arbitrário:
  - $a = (D \cdot V)^2 - \cos^2\theta$, onde $D$ = ray dir, $V$ = eixo cone
  - $b = 2[(D \cdot V)(OC \cdot V) - (D \cdot OC)\cos^2\theta]$
  - $c = (OC \cdot V)^2 - (OC \cdot OC)\cos^2\theta$
  - $OC$ = ray.origin - cone.center (apex)
- Clipping por altura: `0 ≤ h ≤ cone.height` (projeção no eixo)
- Cap: disco na base (raio = `height * tan(half_angle)`)

**3.4 — Criar `src/shapes/cone_utils_bonus.c`:**
- `create_cone_body_hit`: calcula normal no ponto
  - Normal = `normalize(P - C_proj) - tan(θ) * V_normalized` (perpendicular à superfície)
- `check_cone_body`: valida t > EPSILON && t < closest && height in range
- `check_cone_cap`: interseção raio-plano na base + distância ao centro ≤ raio_base

**3.5 — Editar `src/render/ray_trace_bonus.c`:**
- Adicionar `check_cones(scene, ray, &closest)` em `find_closest_hit`, após `check_cylinders`
- Função `check_cones`: padrão idêntico — loop while, `intersect_cone`, update closest

**3.6 — Editar `src/core/cleanup_bonus.c`:**
- Adicionar `free(scene->cones)` antes do mlx cleanup

**3.7 — Verificação:**
- Cena: `cn 0,0,5 0,1,0 30 4 255,128,0` (cone laranja, eixo Y, 30° abertura)
- Deve renderizar sem glitch, com sombra correta, iluminação Phong

---

### Step 4 — Checkerboard (~80 linhas novas)

**4.1 — Criar `src/render/checkerboard_bonus.c`:**
- `apply_checkerboard(t_hit *hit, t_scene *scene)`: modifica `hit->color` baseado em UV
- Funções UV por forma:
  - `uv_sphere`: `u = 0.5 + atan2(z, x) / (2π)`, `v = 0.5 - asin(y) / π`
  - `uv_plane`: coordenadas locais usando eixos tangente/bitangente do plano
  - `uv_cylinder`: `u = 0.5 + atan2(local.z, local.x) / (2π)`, `v = h / height`
  - `uv_cone`: similar ao cilindro
- Pattern: `if (((int)floor(u * SCALE) + (int)floor(v * SCALE)) % 2 == 0)`
  → cor original; senão → cor complementar (255 - r, 255 - g, 255 - b)

**4.2 — Editar parsers bonus para aceitar campo `checker`:**
- `parse_sphere_bonus.c`, `parse_plane_bonus.c` (dentro de `parse_objects_bonus.c`),
  `parse_cylinder_bonus.c` (idem), `parse_cone_bonus.c`
- Campo opcional no final da linha: `sp 0,0,5 2 255,0,0 checker`
- Se presente: `obj.material = 1`

**4.3 — Editar `src/render/lighting_bonus.c` ou `ray_trace_bonus.c`:**
- Após `find_closest_hit`, antes de calcular iluminação:
  `if (hit.material == 1) apply_checkerboard(&hit, scene)`

**4.4 — Verificação:**
- Esfera com checker → padrão xadrez acompanha curvatura
- Plano com checker → grid perfeito
- Cone com checker → pattern ao longo da superfície cônica

---

### Step 5 — Bump Mapping (~120 linhas novas)

**5.1 — Criar `src/utils/texture_bonus.c`:**
- `load_ppm(char *filename)`: lê arquivo PPM P3 (ASCII):
  - Linha 1: `P3`
  - Linha 2: `width height`
  - Linha 3: `max_color` (255)
  - Dados: `r g b` por pixel, row-major
- `free_texture(t_texture *tex)`: `free(tex->pixels); free(tex);`
- `sample_texture(t_texture *tex, double u, double v)`:
  - Clamp u,v para [0,1]
  - Mapear para coordenadas de pixel
  - Retornar `tex->pixels[y * width + x]` (nearest neighbor)

**5.2 — Criar `src/render/bump_map_bonus.c`:**
- `apply_bump_map(t_hit *hit, t_texture *tex)`:
  - Obter UV (reutiliza funções UV do checkerboard)
  - Amostrar textura em `(u, v)`, `(u + delta, v)`, `(u, v + delta)`
  - Gradiente: `du = (sample_right.intensity - sample_center.intensity)`
  - Gradiente: `dv = (sample_up.intensity - sample_center.intensity)`
  - Intensity = `(r + g + b) / 3.0 / 255.0`
  - Calcular tangent space: `T = cross(N, up_approx)`, `B = cross(N, T)`
  - Perturbar: `N' = normalize(N + du * T + dv * B)`
  - Atualizar `hit->normal = N'`

**5.3 — Editar parsers bonus para campo `bump path`:**
- `sp 0,0,5 2 255,0,0 bump textures/stone.ppm`
- Parser: se campo = `"bump"`, próximo campo = path da textura
- Carregar textura via `load_ppm` e armazenar na scene
- `obj.material = 2`

**5.4 — Editar `src/render/lighting_bonus.c` ou `ray_trace_bonus.c`:**
- Após `find_closest_hit`:
  `if (hit.material == 2) apply_bump_map(&hit, texture_for_object)`

**5.5 — Editar `src/core/cleanup_bonus.c`:**
- Liberar todas as texturas: loop `free_texture` + `free(scene->textures)`

**5.6 — Verificação:**
- Esfera com bump map de tijolos → superfície "rugosa"
- Plano com bump de pedra → textura de relevo visível nas sombras

---

## Verificação Final

| Check | Comando | Esperado |
|-------|---------|----------|
| Mandatory compila | `make` | 0 erros, 0 warnings, 0 re-link |
| Bonus compila | `make bonus` | 0 erros, 0 warnings |
| Norminette | `norminette` | 0 erros em todos os `.c` e `.h` |
| Leaks mandatory | `make test-leak` | 0 leaks (todos ✓) |
| Leaks bonus | `valgrind ./miniRT tests/bonus/*.rt` | 0 leaks |
| Cenas de defesa | Ver abaixo | Renderização correta |

### Cenas de demonstração para defesa

1. **Multi-light**: 3 luzes coloridas (R, G, B) em triângulo ao redor dos objetos
2. **Especular**: esfera metálica com highlight brilhante (spot lateral)
3. **Cone**: cone ao lado de cilindro para comparação visual
4. **Checkerboard**: plano xadrez + esfera xadrez + cone xadrez
5. **Bump map**: esfera com textura de pedra ao lado de esfera lisa

---

## Restrições da Norma 42

Cada arquivo `_bonus.c` deve respeitar:
- **25 linhas** máximo por função (corpo entre `{` e `}`)
- **5 parâmetros** máximo por função
- **4 declarações** máximo por função (excluindo parâmetros)
- **80 caracteres** por linha
- **Sem `for`** — apenas `while`
- **Sem `//`** — apenas `/* */`
- **Header guard** obrigatório (`#ifndef MINIRT_BONUS_H`)
- **Funções `static`** para helpers internos de arquivo
- **Sem variável global**

---

## Decisões Técnicas

| Decisão | Escolha | Justificativa |
|---------|---------|---------------|
| Separação total | Todos `_bonus.c` | Subject exige; builds isoladas; sem risco na avaliação |
| Formato do cone | `cn x,y,z axis angle height R,G,B` | Análogo ao cilindro; ângulo ao invés de diâmetro |
| Formato de textura | PPM P3 (ASCII) | Trivial de parsear; sem deps externas; norma-friendly |
| Checkerboard | Opt-in por objeto (`checker`) | Mais flexível; demonstra parser robusto |
| Bump map | Opt-in por objeto (`bump path`) | Permite misturar solid + checker + bump na mesma cena |
| Shininess | `#define SHININESS 32` | Valor padrão razoável; parametrizável como bonus-do-bonus |

---

## Bugs do mandatory a corrigir no bonus

| Bug | Local | Fix |
|-----|-------|-----|
| Parser impede >1 luz | `parse_scene.c:16` `!(*flags & 4)` | Remover condição em `parse_scene_bonus.c` |
| Ambient color ignorada | `lighting.c` `calc_ambient` | Multiplicar por `ambient.color` em `lighting_bonus.c` |
| Light color ignorada | `lighting.c` `calc_diffuse` | Multiplicar por `light.color` em `lighting_bonus.c` |
| `(void)ray` descartado | `lighting.c` `calculate_lighting` | Usar ray para especular em `lighting_bonus.c` |
| Falta `multiply_colors` | `color.c` | Criar em `color_bonus.c` |
| Falta `vec_reflect` | não existe | Criar em `vec_reflect_bonus.c` |

---

## Bônus do Bônus (sugestões extras)

Features opcionais para impressionar na defesa, após os 5 bônus oficiais:

| Feature | Dificuldade | Impacto | Descrição |
|---------|-------------|---------|-----------|
| **Antialiasing (SSAA)** | Fácil | Visual alto | 4-16 raios/pixel, média das cores. Remove serrilhado. ~10 linhas no render. |
| **Multithreading (pthread)** | Média | Performance | Divide imagem em faixas, cada thread renderiza uma. Speedup ~4x. |
| **Reflexão recursiva** | Média | Visual muito alto | Objetos espelhados refletem a cena. Recursão depth 3-5. Usa `vec_reflect`. |
| **Transparência + Refração** | Alta | Visual muito alto | Lei de Snell, Fresnel. Objetos de vidro. |
| **Soft shadows** | Fácil | Visual alto | Luz como área → múltiplos shadow rays → sombras borradas. |
| **Skybox** | Fácil | Visual alto | Raios sem hit → cor do céu (gradiente ou textura). Substitui fundo preto. |
| **Progress bar** | Trivial | UX | `printf("\rRendering: %d%%")` no render. Útil na defesa. |
| **Shininess parametrizável** | Trivial | Flexibilidade | Campo extra no .rt: `sp 0,0,5 2 255,0,0 shiny 64` |

**Recomendação**: Antialiasing + Skybox + Progress bar (fáceis, impacto enorme)
e Multithreading (demonstra domínio de concorrência). ~80 linhas extras no total.

---

## Git Workflow

- Branch: `bonus/implementation` (criada a partir da `main` limpa)
- Commits incrementais por step:
  - `feat(bonus): setup infra — headers, Makefile, copy _bonus files`
  - `feat(bonus): multi-light colored + multiply_colors`
  - `feat(bonus): specular Phong reflection`
  - `feat(bonus): cone intersection + parsing`
  - `feat(bonus): checkerboard pattern + UV mapping`
  - `feat(bonus): bump mapping via PPM textures`
- Merge para `main` apenas quando todos os 5 bônus estiverem validados
- Tags: `v1.0-mandatory` (antes do merge) e `v2.0-bonus` (após)
