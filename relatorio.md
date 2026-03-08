# Anotações — Caça aos Bugs 🐛

## 1. `parse_double` mascara erro na chamada anterior

**Arquivo:** `src/parser/parse_objects.c` — `parse_cylinder()`

**Problema:** As duas chamadas usam o mesmo `&error`, e `parse_double` sempre reseta `*error = 0` no início. Se a primeira chamada falhar e a segunda passar, o erro é perdido.

```c
cy.diameter = parse_double(parts[3], &error);   // chamada 1 → error = 1
cy.height = parse_double(parts[4], &error);     // chamada 2 → error = 0 (resetou!)
if (error || ...)                                // checa só o último → PASSA ❌
```

**Exemplo:** `cy 0,0,0 0,1,0 2abc 4 255,0,0`

- `"2abc"` → `diameter=2.0`, `error=1` ✅ detectou
- `"4"` → `height=4.0`, `error=0` ← resetou o erro anterior!
- `if (error)` → `0` → cilindro criado com input inválido ❌

**Correção sugerida:** Checar `error` entre cada chamada ou usar variáveis separadas.

---

## 2. Mensagem de erro do FOV inconsistente

**Arquivo:** `src/parser/parse_scene.c` (ou onde valida a câmera)

**Problema:** A mensagem diz `(0, 180)` mas o valor `179` é rejeitado.

```
$ ./miniRT ./scenes/eval/eval_05_rotation.rt
Error
Camera FOV must be in range (0, 180)
```

**Esperado:** FOV `179` deveria ser aceito, pois está dentro de `(0, 180)` — ou seja, `0 < 179 < 180`.

**Verificar:** A validação está usando `<` ou `<=`? Se o range é exclusivo `(0, 180)`, então `179` deveria passar. Se for `[0, 180)` a mensagem está errada.

---

## 3. Cena `eval_05_rotation.rt` com valores errados

**Arquivo:** `scenes/eval/eval_05_rotation.rt`

| Campo           | Valor atual | Valor correto | Motivo                                            |
| --------------- | ----------- | ------------- | ------------------------------------------------- |
| FOV da câmera  | `179`     | `70`        | FOV quase 180 distorce tudo, e o parser rejeita   |
| Diâmetro do cy | `3`       | `2`         | Comentário original diz `2`                    |
| Cor do cy (R)   | `2`       | `255`       | Comentário original diz `255,160,40` (laranja) |

# 📋 Norm Compliance Report — miniRT

**Date**: 2026-03-07
**Branch**: eval
**Norminette**: ✅ All files OK

---

## Summary

| Category                              | Result      |
| ------------------------------------- | ----------- |
| Functions per file (≤5)              | ✅ OK       |
| Lines per function (≤25)             | ✅ OK       |
| Variables per function (≤5)          | ✅ OK       |
| Global variables                      | ✅ None     |
| Forbidden functions                   | ✅ None     |
| Header guards (#ifndef)               | ✅ OK       |
| 42 Header comment                     | ✅ OK       |
| Prototypes in .h only                 | ✅ OK       |
| Static helper functions               | ✅ OK       |
| Makefile rules                        | ✅ OK       |
| **ERRORS (norm violations)**    | **0** |
| **WARNINGS (suspicious/risky)** | **2** |

---

## ⚠️ WARNING 1 — Logic bug in `parse_cylinder` (error masking)

- **File**: `src/parser/parse_objects.c`, lines 83-85
- **Severity**: WARNING (not a norm violation, but a logic bug)

```c
cy.diameter = parse_double(parts[3], &error);  // may set error=1
cy.height = parse_double(parts[4], &error);    // resets error=0 if height is valid
if (error || cy.diameter <= 0 || cy.height <= 0)  // error may be 0 now!
```

**Problem**: `parse_double()` starts with `*error = 0`, so the second call
overwrites the error from the first call. If the diameter is invalid (e.g. `"abc"`)
but the height is valid (e.g. `"4"`), `error` will be `0` and the invalid diameter
won't be caught (it will be `0.0` from `ft_atod`, which is caught by `<= 0` but
a string like `"2abc"` would parse as `2.0` and pass silently).

**Fix**: Use two separate error variables, or check after each call:

```c
cy.diameter = parse_double(parts[3], &error);
if (error)
    return (...);
cy.height = parse_double(parts[4], &error);
if (error || cy.diameter <= 0 || cy.height <= 0)
    return (...);
```

> **Note**: The same pattern exists in `parse_sphere.c` if it uses two
> consecutive `parse_double` calls with the same `&error`. Verify.

---

## ⚠️ WARNING 2 — Files/functions at exact norm limits

Several files and functions are at the **exact maximum** allowed by the norm.
They are currently compliant, but any future addition will cause a violation.

### Files with 5/5 functions:

| File                            | Functions        |
| ------------------------------- | ---------------- |
| `src/parser/parse_objects.c`  | 5 (1 static + 4) |
| `src/parser/parse_utils.c`    | 5                |
| `src/shapes/cylinder_utils.c` | 4                |
| `src/render/lighting.c`       | 5 (3 static + 2) |

### Functions near 25/25 lines:

| Function               | File                                | Lines |
| ---------------------- | ----------------------------------- | ----- |
| `intersect_cylinder` | `src/shapes/intersect_cylinder.c` | ~25   |
| `main`               | `src/core/main.c`                 | ~24   |
| `parse_cylinder`     | `src/parser/parse_objects.c`      | ~24   |

### Functions with 5/5 variables:

| Function                  | File                                |
| ------------------------- | ----------------------------------- |
| `intersect_sphere`      | `src/shapes/intersect_sphere.c`   |
| `intersect_cylinder`    | `src/shapes/intersect_cylinder.c` |
| `apply_lights` (static) | `src/render/lighting.c`           |

---

## ✅ Detailed checks — all passing

### Functions per file

All 28 `.c` files have ≤ 5 functions. ✅

### Lines per function

All functions have ≤ 25 lines in the body. ✅

### Variables per function

All functions have ≤ 5 variable declarations. ✅

### Global variables

No global variables found in any file. ✅

### Forbidden functions

Only allowed functions are used:

- **System**: `open`, `close`, `read`, `write`, `malloc`, `free`, `exit`
- **math.h**: `sqrt`, `fabs`, `fmax`, `tan`, `INFINITY`, `M_PI`
- **libft**: `ft_split`, `ft_strlen`, `ft_strncmp`, `ft_strtrim`, `ft_atoi`, `ft_isdigit`, `ft_putstr_fd`, `get_next_line`, etc.
- **MLX42**: `mlx_init`, `mlx_new_image`, `mlx_image_to_window`, `mlx_terminate`, `mlx_delete_image`, `mlx_key_hook`, `mlx_close_hook`, `mlx_loop`, `mlx_put_pixel`

No `printf`, `fprintf`, `puts`, `getline`, or other forbidden calls found. ✅

### Header guards

- `includes/minirt.h`: `#ifndef MINIRT_H` ✅
- `includes/vectors.h`: `#ifndef VECTORS_H` ✅

### 42 Header comment

All 28 `.c` files and 2 `.h` files have the standard 42 header. ✅

### Function prototypes

No non-static function prototypes found in `.c` files. All prototypes are in `.h` files. ✅

### Static functions

All helper functions used only within a single file are declared `static`. ✅

### Makefile

| Rule                      | Status                                  |
| ------------------------- | --------------------------------------- |
| `NAME = miniRT`         | ✅                                      |
| `all` rule              | ✅                                      |
| `clean` rule            | ✅                                      |
| `fclean` rule           | ✅                                      |
| `re: fclean all`        | ✅                                      |
| `CC = cc`               | ✅                                      |
| `-Wall -Wextra -Werror` | ✅                                      |
| No relink                 | ✅ (`$(NAME)` depends on `$(OBJS)`) |
| `.PHONY` correct        | ✅ (`$(NAME)` not in `.PHONY`)      |

---

## 🎯 Recommended actions

1. **Fix WARNING 1** — The `parse_double` error masking bug in `parse_cylinder`
2. **Keep in mind WARNING 2** — Don't add code to files/functions at the limit
