### miniRT

A minimal ray tracer written in C — a 42 school project.

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-blue" alt="C" />
  <img src="https://img.shields.io/badge/Graphics-MLX42-orange" alt="MLX42" />
  <img src="https://img.shields.io/badge/Norm-passing-success" alt="Norminette" />
</p>

---

## About

**miniRT** is a ray tracer built from scratch in C. It generates 3D images by simulating how light interacts with objects in a scene — casting rays from a virtual camera, computing intersections with geometric shapes, and determining the color of each pixel based on lighting.

The project is part of the 42 curriculum and focuses on understanding the mathematical foundations behind 3D rendering: vector algebra, ray-object intersection, and the Phong reflection model — all without relying on any high-level graphics engine.

---

## Features

- **Ray casting** from a configurable virtual camera with adjustable FOV
- **Ambient, diffuse, and shadow lighting** (Phong reflection model)
- **Hard shadows** — objects block light realistically
- **Three geometric primitives**: sphere, plane, and cylinder (with caps)
- **Scene description files** (`.rt`) — fully configurable via text
- **Real-time display** using MLX42 + GLFW/OpenGL
- **Robust error handling** with descriptive messages
- **Memory-leak free** (validated with Valgrind + suppression file for GPU drivers)

---

## Supported Objects

| Object   | ID     | Properties                            |
| -------- | ------ | ------------------------------------- |
| Sphere   | `sp` | center, diameter, color               |
| Plane    | `pl` | point, normal vector, color           |
| Cylinder | `cy` | center, axis, diameter, height, color |

---

## Scene File Format (`.rt`)

Each line describes a scene element — camera, light, or object:

```
A  0.2           255,255,255        # Ambient light: ratio + color
C  0,0,-8  0,0,1  60               # Camera: position, direction, FOV
L  0,4,0   0.8  255,255,255        # Light: position, brightness, color

pl  0,-5,0   0,1,0   200,200,200            # Plane (floor)
sp  -2,-3,5  3        255,0,0               # Red sphere
cy  2,-5,6   0,1,0   2  4  200,180,100      # Cylinder
```

### Element Rules

| Element | ID                   | Occurrences |
| ------- | -------------------- | ----------- |
| Ambient | `A`                | Exactly 1   |
| Camera  | `C`                | Exactly 1   |
| Light   | `L`                | Exactly 1   |
| Objects | `sp` `pl` `cy` | Unlimited   |

---

## Getting Started

### Prerequisites

- **GCC** or **CC** compiler
- **CMake** (for building MLX42)
- **GLFW** (`libglfw3-dev`)
- **Make**

### Build

```bash
make
```

### Run

```bash
./miniRT scenes/simple.rt
```

Press **ESC** or close the window to exit.

### Run with a specific scene

```bash
make run SCENE=scenes/cornell_box.rt
```

---

## Makefile Rules

| Rule                                   | Description                                       |
| -------------------------------------- | ------------------------------------------------- |
| `make`                               | Build the project                                 |
| `make clean`                         | Remove object files                               |
| `make fclean`                        | Remove objects + binary                           |
| `make re`                            | Full rebuild                                      |
| `make norm`                          | Run norminette (excludes MLX42)                   |
| `make run SCENE=<file>`              | Run a specific `.rt` scene                      |
| `make valgrind SCENE=<file>`         | Run with Valgrind + MLX suppression file          |
| `make test`                          | Run all tests                                     |
| `make test1` / `test2` / `test3` | Run test levels (basic / intermediate / advanced) |
| `make test-error`                    | Run error handling tests                          |
| `make test-leak`                     | Run Valgrind leak tests on all scenes             |
| `make demo`                          | Quick demo with a single sphere                   |

---

## Project Structure

```
miniRT/
├── includes/          # Header files (structs, prototypes)
├── lib/
│   ├── libft/         # Custom C standard library
│   └── MLX42/         # Graphics library (GLFW/OpenGL backend)
├── src/
│   ├── core/          # main, cleanup, error handling
│   ├── math/          # Vector operations (add, sub, dot, cross, normalize)
│   ├── mlx/           # Window init, events, pixel rendering
│   ├── parser/        # Scene file parsing and validation
│   ├── render/        # Ray tracing, lighting, shadows
│   ├── scene/         # Camera setup, scene initialization
│   ├── shapes/        # Ray-object intersection (sphere, plane, cylinder)
│   └── utils/         # Color utilities
├── scenes/            # Example .rt scene files
├── tests/             # Test scenes organized by difficulty
├── mlx.supp           # Valgrind suppression file for MLX42/Mesa/LLVM
└── Makefile
```

---

## Evaluation Scenes

| Scene                                | What it tests                           |
| ------------------------------------ | --------------------------------------- |
| `eval_01_sphere.rt`                | Sphere at origin                        |
| `eval_02_plane.rt`                 | Plane at z=0                            |
| `eval_03_cylinder.rt`              | Cylinder on Y axis                      |
| `eval_04_translation.rt`           | Two spheres (Z translation)             |
| `eval_05_rotation.rt`              | Cylinder rotated 90° (Y→X)            |
| `eval_06_multi_intersect.rt`       | Sphere + cylinder intersecting          |
| `eval_07_multi_same.rt`            | 2 cylinders + 2 spheres + plane         |
| `eval_08_cam_x.rt`                 | Camera on X axis                        |
| `eval_09_cam_y.rt`                 | Camera on Y axis (top view)             |
| `eval_10_cam_z.rt`                 | Camera on Z axis                        |
| `eval_11_cam_random.rt`            | Camera at random position               |
| `eval_12_brightness_lateral.rt`    | Lateral brightness — sphere at origin  |
| `eval_13_brightness_translated.rt` | Lateral brightness — translated sphere |
| `eval_14_shadow.rt`                | Sphere shadow on plane                  |
| `eval_15_complex.rt`               | Complex scene with multiple shadows     |

---

## Authors

- **lpaula-n** — [42 São Paulo](https://www.42sp.org.br/)
- **rpaulo-c** — [42 São Paulo](https://www.42sp.org.br/)

---

## License

This project is developed as part of the 42 school curriculum.
