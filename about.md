# miniRT

miniRT é um ray tracer escrito em C, desenvolvido como projeto da escola 42. Seu objetivo é gerar imagens 3D realistas a partir de cenas descritas em arquivos de texto (`.rt`), simulando o comportamento físico da luz.

---

## O que ele faz

O programa lança raios a partir de uma câmera virtual em direção à cena. Para cada pixel da tela, calcula se o raio atinge algum objeto, e em caso afirmativo, determina a cor desse pixel com base em:

- **Iluminação ambiente** — luz base que evita sombras totalmente negras
- **Iluminação difusa** — variação de brilho conforme o ângulo da luz sobre a superfície
- **Sombras duras** — áreas bloqueadas da fonte de luz
- **Ponto de luz** — fonte pontual com intensidade configurável

---

## Objetos suportados

| Objeto   | Identificador | Propriedades                        |
| -------- | ------------- | ----------------------------------- |
| Esfera   | `sp`          | centro, diâmetro, cor               |
| Plano    | `pl`          | ponto, normal, cor                  |
| Cilindro | `cy`          | centro, eixo, diâmetro, altura, cor |

---

## Formato de cena (.rt)

A cena é definida num arquivo de texto. Cada linha descreve um elemento — câmera, luz ou objeto:

```
A 0.2 255,255,255          # luz ambiente: intensidade + cor
C 0,0,-8 0,0,1 60          # câmera: posição, direção, FOV
L 0,4,0 0.8 255,255,255    # luz: posição, intensidade, cor

pl 0,-5,0 0,1,0 200,200,200   # plano (chão)
sp -2,-3,5 3 255,0,0           # esfera vermelha
cy 2,-5,6 0,1,0 2 4 200,180,100  # cilindro
```

---

## Como usar

```bash
make
./miniRT scenes/cornell_box.rt
```

- **ESC** ou fechar a janela encerra o programa.
- O arquivo `.rt` deve ser passado como único argumento.
- Erros de configuração na cena exibem `Error\n` seguido de mensagem descritiva.

---

## Objetivo do projeto

Implementar do zero os fundamentos de ray tracing — matemática vetorial, interseção raio-objeto, modelo de iluminação — sem bibliotecas gráficas além da MiniLibX. O foco é entender como imagens 3D são geradas computacionalmente, pixel a pixel.

| Arquivo                          | Seção                                  |
| -------------------------------- | -------------------------------------- |
| eval_01_sphere.rt                | Elementos técnicos + Esfera em {0,0,0} |
| eval_02_plane.rt                 | Plano com z=0                          |
| eval_03_cylinder.rt              | Cilindro no eixo Y                     |
| eval_04_translation.rt           | Duas esferas (translação em Z)         |
| eval_05_rotation.rt              | Cilindro rotacionado 90° (Y→X)         |
| eval_06_multi_intersect.rt       | Esfera + cilindro se intersectando     |
| eval_07_multi_same.rt            | 2 cilindros + 2 esferas + plano        |
| eval_08_cam_x.rt                 | Câmera no eixo X                       |
| eval_09_cam_y.rt                 | Câmera no eixo Y (de cima)             |
| eval_10_cam_z.rt                 | Câmera no eixo Z                       |
| eval_11_cam_random.rt            | Câmera em posição aleatória            |
| eval_12_brightness_lateral.rt    | Brilho lateral — esfera {0,0,0}        |
| eval_13_brightness_translated.rt | Brilho lateral — esfera transladada    |
| eval_14_shadow.rt                | Sombra da esfera no plano              |
| eval_15_complex.rt               | Cena complexa com sombras múltiplas    |
