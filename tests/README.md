# MiniRT Test Suite

## Estrutura de Testes

```
tests/
├── level1_basic/          # Testes básicos (8 cenas)
├── level2_intermediate/   # Testes intermediários (10 cenas)
├── level3_advanced/       # Testes avançados (10 cenas)
├── error_tests/           # Testes de erro (25 testes)
├── bonus/                 # Testes de bonus (10 cenas)
└── run_tests.sh           # Script de execução de testes
```

## Comandos do Makefile

### Testes Automáticos

| Comando | Descrição |
|---------|-----------|
| `make test` | Executa todos os testes |
| `make test1` | Testes Level 1 (básico) |
| `make test2` | Testes Level 2 (intermediário) |
| `make test3` | Testes Level 3 (avançado) |
| `make test-error` | Testes de tratamento de erros |
| `make test-bonus` | Testes de bonus (múltiplas luzes) |
| `make test-menu` | Menu interativo de testes |
| `make test-visual` | Teste visual rápido |

### Execução Individual

| Comando | Descrição |
|---------|-----------|
| `make run SCENE=path/scene.rt` | Executa uma cena específica |
| `make demo` | Demo básica (esfera) |
| `make demo2` | Demo intermediária (objetos mistos) |
| `make demo3` | Demo avançada (boneco de neve) |
| `make valgrind` | Verifica vazamentos de memória |
| `make valgrind SCENE=path/scene.rt` | Valgrind com cena específica |

## Níveis de Teste

### Level 1 - Básico
Testes fundamentais de renderização:
- 01: Esfera única
- 02: Plano único
- 03: Cilindro único
- 04: Apenas luz ambiente
- 05: Sombras simples
- 06: FOV estreito (30°)
- 07: FOV amplo (120°)
- 08: Esfera branca

### Level 2 - Intermediário
Testes de complexidade média:
- 01: Múltiplas esferas
- 02: Sala com planos
- 03: Objetos mistos
- 04: Cilindro inclinado
- 05: Esferas sobrepostas
- 06: Esfera atrás da câmera
- 07: Câmera dentro do cilindro
- 08: Múltiplas sombras
- 09: Plano inclinado
- 10: Luz lateral

### Level 3 - Avançado
Testes complexos:
- 01: Cena complexa
- 02: Boneco de neve
- 03: Floresta de cilindros
- 04: Grid de esferas
- 05: Vista de cima
- 06: Tampas de cilindro
- 07: Cascata de sombras
- 08: Cilindro horizontal
- 09: Teste de profundidade
- 10: Cena de mesa

### Testes de Erro
Validação de tratamento de erros:
- Arquivo inexistente
- Extensão errada
- Arquivo vazio
- Ambient/Camera faltando
- Ambient/Camera duplicados
- Valores fora do range
- Cores inválidas
- FOV inválido
- Vetores não normalizados
- Identificadores inválidos
- Parâmetros faltando/extras
- Valores não numéricos
- Vetores malformados

### Testes Bonus
Funcionalidades bonus:
- 01: Múltiplas luzes
- 02: Luzes coloridas
- 03: Muitas luzes (stress test)
- 04: Efeito spotlight
- 05: Sombras com múltiplas luzes
- 06: Cor do ambient
- 07: Teste de cone
- 08: Iluminação de estúdio
- 09: Cena de galeria
- 10: Iluminação dramática

## Critérios de Avaliação

### Mandatory Part
- ✅ Renderização de esferas
- ✅ Renderização de planos
- ✅ Renderização de cilindros
- ✅ Iluminação ambiente
- ✅ Luz difusa
- ✅ Sombras
- ✅ Tratamento de erros
- ✅ Parser de arquivos .rt

### Bonus Part
- ✅ Múltiplas luzes
- ✅ Luzes coloridas
- ⬜ Specular reflection (se implementado)
- ⬜ Colored/Disrupted surfaces (se implementado)
- ⬜ Cone object (se implementado)
- ⬜ Bump mapping (se implementado)

## Uso do Script de Testes

### Modo Interativo
```bash
./tests/run_tests.sh
```

### Modo Linha de Comando
```bash
./tests/run_tests.sh all      # Todos os testes
./tests/run_tests.sh level1   # Apenas level 1
./tests/run_tests.sh level2   # Apenas level 2
./tests/run_tests.sh level3   # Apenas level 3
./tests/run_tests.sh error    # Testes de erro
./tests/run_tests.sh bonus    # Testes bonus
./tests/run_tests.sh visual   # Teste visual rápido
```

## Dicas para Avaliação

1. **Antes da avaliação:**
   - Execute `make test` para verificar todos os testes
   - Execute `make valgrind` para verificar memory leaks
   - Teste manualmente as cenas level3 para garantir visualização correta

2. **Durante a avaliação:**
   - Use `make demo`, `make demo2`, `make demo3` para demonstrações rápidas
   - Use `make run SCENE=scene.rt` para testar cenas específicas
   - Mostre o tratamento de erros com `make test-error`

3. **Para bonus:**
   - Execute `make test-bonus` para demonstrar múltiplas luzes
   - Use cenas como `09_gallery_scene.rt` para impressionar

## Troubleshooting

Se algum teste falhar:
1. Verifique se o programa compila sem erros: `make re`
2. Execute o teste manualmente: `./miniRT tests/path/to/scene.rt`
3. Use valgrind para detectar problemas de memória
4. Verifique a saída de erro do programa
