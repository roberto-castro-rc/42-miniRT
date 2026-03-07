#!/bin/bash

# =============================================================================
# miniRT Bonus Test Suite
# =============================================================================
# Tests for the 5 bonus features, with mandatory vs bonus comparison.
#
# Usage:
#   ./tests/test_bonus.sh              → menu interativo
#   ./tests/test_bonus.sh all          → roda todos os testes
#   ./tests/test_bonus.sh step1        → testa multi-light
#   ./tests/test_bonus.sh step2        → testa specular
#   ./tests/test_bonus.sh step3        → testa cone
#   ./tests/test_bonus.sh step4        → testa checkerboard
#   ./tests/test_bonus.sh step5        → testa bump map
#   ./tests/test_bonus.sh compare      → comparação visual mandatory vs bonus
#   ./tests/test_bonus.sh progress     → dashboard de progresso
#   ./tests/test_bonus.sh leak         → valgrind em cenas bonus
# =============================================================================

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m'
BOLD='\033[1m'
DIM='\033[2m'

# Counters
PASS=0
FAIL=0
SKIP=0
TOTAL=0

# Config
TIMEOUT=5
MANDATORY="./miniRT_mandatory"
BONUS="./miniRT"
TESTS_DIR="tests/bonus_tests"

# =============================================================================
# Helper Functions
# =============================================================================

print_banner() {
    echo -e "\n${BOLD}${MAGENTA}╔══════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BOLD}${MAGENTA}║${NC}    ${BOLD}miniRT Bonus Test Suite${NC}                                  ${BOLD}${MAGENTA}║${NC}"
    echo -e "${BOLD}${MAGENTA}║${NC}    ${DIM}Teste cada bonus individualmente + compare builds${NC}        ${BOLD}${MAGENTA}║${NC}"
    echo -e "${BOLD}${MAGENTA}╚══════════════════════════════════════════════════════════════╝${NC}\n"
}

print_header() {
    echo -e "\n${BOLD}${BLUE}┌──────────────────────────────────────────────────────────────┐${NC}"
    echo -e "${BOLD}${BLUE}│${NC}  ${BOLD}$1${NC}"
    echo -e "${BOLD}${BLUE}└──────────────────────────────────────────────────────────────┘${NC}\n"
}

print_subheader() {
    echo -e "  ${CYAN}── $1 ──${NC}"
}

print_pass() {
    echo -e "    ${GREEN}✓${NC} $1"
    ((PASS++))
    ((TOTAL++))
}

print_fail() {
    echo -e "    ${RED}✗${NC} $1 ${DIM}($2)${NC}"
    ((FAIL++))
    ((TOTAL++))
}

print_skip() {
    echo -e "    ${YELLOW}○${NC} $1 ${DIM}(skipped: $2)${NC}"
    ((SKIP++))
    ((TOTAL++))
}

print_info() {
    echo -e "    ${DIM}ℹ $1${NC}"
}

reset_counters() {
    PASS=0
    FAIL=0
    SKIP=0
    TOTAL=0
}

print_summary() {
    echo -e "\n${BOLD}${BLUE}┌──────────────────────────────────────────────────────────────┐${NC}"
    echo -e "${BOLD}${BLUE}│${NC}  ${BOLD}RESULTADO${NC}"
    echo -e "${BOLD}${BLUE}└──────────────────────────────────────────────────────────────┘${NC}"
    echo -e "    ${GREEN}Passed:${NC}  $PASS"
    echo -e "    ${RED}Failed:${NC}  $FAIL"
    echo -e "    ${YELLOW}Skipped:${NC} $SKIP"
    echo -e "    Total:   $TOTAL"
    if [ $FAIL -eq 0 ] && [ $TOTAL -gt 0 ]; then
        echo -e "\n    ${GREEN}${BOLD}Todos os testes passaram! ✓${NC}\n"
    elif [ $FAIL -gt 0 ]; then
        echo -e "\n    ${RED}${BOLD}Alguns testes falharam! ✗${NC}\n"
    fi
}

# =============================================================================
# Build Helpers
# =============================================================================

ensure_mandatory() {
    if [ ! -f "$MANDATORY" ]; then
        echo -e "  ${YELLOW}Compilando build mandatory...${NC}"
        make re >/dev/null 2>&1
        if [ $? -eq 0 ]; then
            cp miniRT "$MANDATORY"
            echo -e "  ${GREEN}Build mandatory OK → $MANDATORY${NC}"
        else
            echo -e "  ${RED}Falha ao compilar mandatory!${NC}"
            return 1
        fi
    fi
    return 0
}

ensure_bonus() {
    echo -e "  ${YELLOW}Compilando build bonus...${NC}"
    make bonus >/dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo -e "  ${GREEN}Build bonus OK → ./miniRT${NC}"
        return 0
    else
        echo -e "  ${RED}Falha ao compilar bonus!${NC}"
        return 1
    fi
}

# =============================================================================
# Core Test Functions
# =============================================================================

# Testa se uma cena roda sem crash (exit 0 ou timeout)
test_scene_runs() {
    local binary=$1
    local file=$2
    local name=$3

    if [ ! -f "$file" ]; then
        print_fail "$name" "arquivo nao encontrado"
        return
    fi

    timeout $TIMEOUT "$binary" "$file" </dev/null >/dev/null 2>&1 &
    local pid=$!
    sleep 1

    if kill -0 $pid 2>/dev/null; then
        kill $pid 2>/dev/null
        wait $pid 2>/dev/null
        print_pass "$name"
    else
        wait $pid 2>/dev/null
        local code=$?
        if [ $code -eq 124 ]; then
            print_pass "$name (timeout OK)"
        else
            print_fail "$name" "crash code=$code"
        fi
    fi
}

# Testa se uma cena gera erro (para cenas com "cn" no mandatory)
test_scene_expects_error() {
    local binary=$1
    local file=$2
    local name=$3

    output=$("$binary" "$file" </dev/null 2>&1)
    local code=$?

    if echo "$output" | grep -qi "error"; then
        print_pass "$name (error detected)"
        return 0
    elif [ $code -ne 0 ]; then
        print_pass "$name (exit code=$code)"
        return 0
    else
        print_fail "$name" "expected error but got success"
        return 1
    fi
}

# Testa cena no bonus build (roda) e no mandatory (mesmo comportamento ou erro esperado)
test_bonus_scene() {
    local file=$1
    local name=$(basename "$file" .rt)
    local has_cone=0

    # Detecta se a cena usa "cn" (cone)
    if grep -q "^cn " "$file" 2>/dev/null; then
        has_cone=1
    fi

    # Testa no bonus build
    test_scene_runs "$BONUS" "$file" "BONUS  $name"

    # Se tem cone, mandatory deve dar erro
    if [ $has_cone -eq 1 ]; then
        if [ -f "$MANDATORY" ]; then
            test_scene_expects_error "$MANDATORY" "$file" "MAND   $name (cone→error)"
        else
            print_skip "MAND   $name" "mandatory nao compilado"
        fi
    fi
}

# =============================================================================
# Step Tests
# =============================================================================

run_step1_tests() {
    print_header "Step 1: Multi-Light Colorida"
    echo -e "  ${DIM}Testa: multiplas luzes + cor da luz + cor do ambient${NC}\n"

    print_subheader "Cenas de multi-light"
    for file in $TESTS_DIR/step1_multilight/*.rt; do
        [ -f "$file" ] && test_bonus_scene "$file"
    done

    print_subheader "O que observar visualmente"
    print_info "01: Ambos lados da esfera iluminados (vs apenas 1 lado)"
    print_info "02: Mistura RGB no centro da esfera branca"
    print_info "03: Tom laranja quente nas sombras (ambient colorido)"
    print_info "04: Multiplas sombras em direcoes diferentes"
    print_info "05: Lado esquerdo quente, lado direito frio"
}

run_step2_tests() {
    print_header "Step 2: Specular Phong"
    echo -e "  ${DIM}Testa: reflexao especular em superficies${NC}\n"

    print_subheader "Cenas de specular"
    for file in $TESTS_DIR/step2_specular/*.rt; do
        [ -f "$file" ] && test_bonus_scene "$file"
    done

    print_subheader "O que observar visualmente"
    print_info "01: Hotspot branco brilhante na esfera vermelha"
    print_info "02: Cada esfera tem highlight de cor diferente"
    print_info "03: Streak especular ao longo do corpo do cilindro"
    print_info "04: Spotlight frontal → highlight no centro da esfera"
}

run_step3_tests() {
    print_header "Step 3: Cone"
    echo -e "  ${DIM}Testa: interseção + parse do cone${NC}\n"

    print_subheader "Cenas de cone"
    for file in $TESTS_DIR/step3_cone/*.rt; do
        [ -f "$file" ] && test_bonus_scene "$file"
    done

    print_subheader "O que observar visualmente"
    print_info "01: Cone basico parado no chao"
    print_info "02: Cone lado a lado com cilindro — compare as formas"
    print_info "03: Cone inclinado a 45 graus"
    print_info "04: Tres cones de sorvete com bolas coloridas"
    print_info "05: Cone invertido (funil)"
}

run_step4_tests() {
    print_header "Step 4: Checkerboard"
    echo -e "  ${DIM}Testa: padrao xadrez em superficies${NC}\n"

    print_subheader "Cenas de checkerboard"
    for file in $TESTS_DIR/step4_checkerboard/*.rt; do
        [ -f "$file" ] && test_bonus_scene "$file"
    done

    print_subheader "O que observar visualmente"
    print_info "01: Esfera com xadrez preto/vermelho"
    print_info "02: Chao classico preto/branco"
    print_info "03: Cilindro com padrao xadrez no corpo"
    print_info "04: Cena inteira com xadrez em tudo"
}

run_step5_tests() {
    print_header "Step 5: Bump Mapping"
    echo -e "  ${DIM}Testa: perturbacao de normais via textura${NC}\n"

    print_subheader "Cenas de bump map"
    for file in $TESTS_DIR/step5_bumpmap/*.rt; do
        [ -f "$file" ] && test_bonus_scene "$file"
    done

    print_subheader "O que observar visualmente"
    print_info "01: Esfera com superficie rugosa/irregular"
    print_info "02: Plano com textura de tijolo/ondulacao"
    print_info "03: Cilindro com aspecto de casca de arvore"
}

# =============================================================================
# Comparison Mode
# =============================================================================

run_comparison() {
    print_header "Comparacao Visual: Mandatory vs Bonus"

    echo -e "  ${YELLOW}Este modo compila ambas as builds e abre as mesmas cenas${NC}"
    echo -e "  ${YELLOW}lado a lado para comparacao visual.${NC}"
    echo -e "  ${YELLOW}Pressione ESC para fechar cada janela.${NC}\n"

    ensure_mandatory
    if [ $? -ne 0 ]; then
        echo -e "  ${RED}Nao foi possivel compilar mandatory${NC}"
        return 1
    fi
    ensure_bonus
    if [ $? -ne 0 ]; then
        echo -e "  ${RED}Nao foi possivel compilar bonus${NC}"
        return 1
    fi

    local scenes=(
        "$TESTS_DIR/comparison/01_baseline_single_light.rt"
        "$TESTS_DIR/comparison/02_multilight_diff.rt"
        "$TESTS_DIR/comparison/03_specular_diff.rt"
    )

    for scene in "${scenes[@]}"; do
        if [ ! -f "$scene" ]; then
            echo -e "\n  ${RED}Cena nao encontrada: $scene${NC}"
            continue
        fi

        local name=$(basename "$scene" .rt)
        echo -e "\n  ${BOLD}━━━ $name ━━━${NC}"

        # Mostra descricao da cena
        head -3 "$scene" | grep "^#" | sed 's/^#/    /'

        echo -e "\n    ${CYAN}[1/2] Abrindo MANDATORY...${NC} (ESC para fechar)"
        "$MANDATORY" "$scene"

        echo -e "    ${MAGENTA}[2/2] Abrindo BONUS...${NC} (ESC para fechar)"
        "$BONUS" "$scene"

        echo -e "    ${DIM}Compare as diferencas acima!${NC}"
    done

    echo -e "\n  ${GREEN}Comparacao concluida!${NC}"
}

# =============================================================================
# Progress Dashboard
# =============================================================================

check_feature_status() {
    local step_name=$1
    local test_dir=$2
    local has_cone=$3
    local status="?"

    # Conta testes que passam
    local passed=0
    local total=0

    for file in $test_dir/*.rt; do
        [ -f "$file" ] || continue
        ((total++))

        # Roda silenciosamente
        timeout $TIMEOUT "$BONUS" "$file" </dev/null >/dev/null 2>&1 &
        local pid=$!
        sleep 1

        if kill -0 $pid 2>/dev/null; then
            kill $pid 2>/dev/null
            wait $pid 2>/dev/null
            ((passed++))
        else
            wait $pid 2>/dev/null
            local code=$?
            [ $code -eq 124 ] && ((passed++))
        fi
    done

    if [ $total -eq 0 ]; then
        echo -e "    ${YELLOW}○${NC} $step_name: nenhuma cena encontrada"
    elif [ $passed -eq $total ]; then
        echo -e "    ${GREEN}■${NC} $step_name: ${GREEN}$passed/$total sem crash${NC}"
    elif [ $passed -gt 0 ]; then
        echo -e "    ${YELLOW}◧${NC} $step_name: ${YELLOW}$passed/$total sem crash${NC}"
    else
        echo -e "    ${RED}□${NC} $step_name: ${RED}$passed/$total sem crash${NC}"
    fi
}

run_progress() {
    print_header "Dashboard de Progresso dos Bonus"

    if [ ! -f "$BONUS" ]; then
        echo -e "  ${YELLOW}Compilando bonus...${NC}"
        make bonus >/dev/null 2>&1
        if [ $? -ne 0 ]; then
            echo -e "  ${RED}Bonus nao compila! Corrija os erros primeiro.${NC}"
            return 1
        fi
    fi

    echo -e "  ${DIM}Testando cada step (sem crash = feature funcional)...${NC}\n"

    echo -e "  ${BOLD}Legenda:${NC} ${GREEN}■${NC}=OK  ${YELLOW}◧${NC}=Parcial  ${RED}□${NC}=Nao funciona  ${YELLOW}○${NC}=Sem cenas\n"

    check_feature_status "Step 1: Multi-light colorida " "$TESTS_DIR/step1_multilight"
    check_feature_status "Step 2: Specular Phong       " "$TESTS_DIR/step2_specular"
    check_feature_status "Step 3: Cone                 " "$TESTS_DIR/step3_cone"
    check_feature_status "Step 4: Checkerboard         " "$TESTS_DIR/step4_checkerboard"
    check_feature_status "Step 5: Bump mapping         " "$TESTS_DIR/step5_bumpmap"

    echo ""
    echo -e "  ${DIM}Nota: 'sem crash' indica que a feature foi parsada e renderizada.${NC}"
    echo -e "  ${DIM}Para validar visualmente, use os modos step1..step5 ou compare.${NC}"
}

# =============================================================================
# Leak Tests
# =============================================================================

run_bonus_leaks() {
    print_header "Leak Detection (Bonus Build)"

    if ! command -v valgrind &>/dev/null; then
        echo -e "  ${RED}valgrind nao encontrado. Instale: sudo apt install valgrind${NC}"
        return 1
    fi

    local VG="valgrind --leak-check=full --show-leak-kinds=all \
              --track-origins=yes --error-exitcode=42 \
              --errors-for-leak-kinds=all --num-callers=20"
    local VG_LOG="/tmp/vg_bonus.log"

    check_bonus_leak() {
        local label="$1"
        local file="$2"

        if [ ! -f "$file" ]; then
            print_skip "$label" "arquivo nao encontrado"
            return
        fi

        DISPLAY=:99 timeout 10 $VG "$BONUS" "$file" </dev/null >"$VG_LOG" 2>&1
        local code=$?

        if [ $code -eq 42 ]; then
            print_fail "LEAK: $label" "valgrind detectou leak"
            grep -E 'definitely|indirectly|still reachable|bytes in' \
                "$VG_LOG" 2>/dev/null | head -8 | sed 's/^/      /'
        else
            print_pass "$label"
        fi
    }

    print_subheader "Step 1: Multi-light"
    for file in $TESTS_DIR/step1_multilight/*.rt; do
        [ -f "$file" ] && check_bonus_leak "$(basename "$file" .rt)" "$file"
    done

    print_subheader "Step 2: Specular"
    for file in $TESTS_DIR/step2_specular/*.rt; do
        [ -f "$file" ] && check_bonus_leak "$(basename "$file" .rt)" "$file"
    done

    print_subheader "Step 3: Cone"
    for file in $TESTS_DIR/step3_cone/*.rt; do
        [ -f "$file" ] && check_bonus_leak "$(basename "$file" .rt)" "$file"
    done

    print_subheader "Step 4: Checkerboard"
    for file in $TESTS_DIR/step4_checkerboard/*.rt; do
        [ -f "$file" ] && check_bonus_leak "$(basename "$file" .rt)" "$file"
    done

    print_subheader "Step 5: Bump map"
    for file in $TESTS_DIR/step5_bumpmap/*.rt; do
        [ -f "$file" ] && check_bonus_leak "$(basename "$file" .rt)" "$file"
    done

    rm -f "$VG_LOG"
}

# =============================================================================
# Quick Visual — opens particular scenes for manual inspection
# =============================================================================

run_visual_step() {
    local step=$1
    local dir=$2

    print_header "Visual: $step"
    echo -e "  ${YELLOW}Pressione ESC para fechar cada janela.${NC}\n"

    for file in $dir/*.rt; do
        [ -f "$file" ] || continue
        local name=$(basename "$file" .rt)
        echo -e "    ${CYAN}Abrindo: $name${NC}"
        head -3 "$file" | grep "^#" | sed 's/^#/      /'
        "$BONUS" "$file"
        echo ""
    done
}

# =============================================================================
# Menu
# =============================================================================

show_menu() {
    print_banner
    echo "  ${BOLD}Testes por Step:${NC}"
    echo "    1)  Step 1 — Multi-light colorida"
    echo "    2)  Step 2 — Specular Phong"
    echo "    3)  Step 3 — Cone"
    echo "    4)  Step 4 — Checkerboard"
    echo "    5)  Step 5 — Bump mapping"
    echo ""
    echo "  ${BOLD}Modos Especiais:${NC}"
    echo "    6)  Rodar TODOS os steps"
    echo "    7)  Comparacao visual (mandatory vs bonus)"
    echo "    8)  Dashboard de progresso"
    echo "    9)  Leak detection (valgrind)"
    echo ""
    echo "  ${BOLD}Visual (abre cenas no bonus build):${NC}"
    echo "    v1) Visual Step 1    v2) Visual Step 2"
    echo "    v3) Visual Step 3    v4) Visual Step 4"
    echo "    v5) Visual Step 5"
    echo ""
    echo "    0)  Sair"
    echo ""
    echo -n "  Opcao: "
}

# =============================================================================
# Main
# =============================================================================

# Verifica se pelo menos um binario existe
check_binary() {
    if [ ! -f "$BONUS" ]; then
        echo -e "${YELLOW}Compilando bonus...${NC}"
        make bonus >/dev/null 2>&1
        if [ $? -ne 0 ]; then
            echo -e "${RED}Erro: bonus nao compilou! Rode 'make bonus' para ver erros.${NC}"
            exit 1
        fi
    fi
}

case "$1" in
    "step1")
        check_binary
        run_step1_tests
        print_summary
        ;;
    "step2")
        check_binary
        run_step2_tests
        print_summary
        ;;
    "step3")
        check_binary
        run_step3_tests
        print_summary
        ;;
    "step4")
        check_binary
        run_step4_tests
        print_summary
        ;;
    "step5")
        check_binary
        run_step5_tests
        print_summary
        ;;
    "all")
        check_binary
        run_step1_tests
        run_step2_tests
        run_step3_tests
        run_step4_tests
        run_step5_tests
        print_summary
        ;;
    "compare"|"cmp")
        run_comparison
        ;;
    "progress"|"status")
        check_binary
        run_progress
        ;;
    "leak"|"leaks")
        check_binary
        run_bonus_leaks
        print_summary
        ;;
    "")
        # Menu interativo
        check_binary
        while true; do
            show_menu
            read choice
            case $choice in
                1) reset_counters; run_step1_tests; print_summary ;;
                2) reset_counters; run_step2_tests; print_summary ;;
                3) reset_counters; run_step3_tests; print_summary ;;
                4) reset_counters; run_step4_tests; print_summary ;;
                5) reset_counters; run_step5_tests; print_summary ;;
                6)
                    reset_counters
                    run_step1_tests
                    run_step2_tests
                    run_step3_tests
                    run_step4_tests
                    run_step5_tests
                    print_summary
                    ;;
                7) run_comparison ;;
                8) run_progress ;;
                9) reset_counters; run_bonus_leaks; print_summary ;;
                v1) run_visual_step "Step 1: Multi-light" "$TESTS_DIR/step1_multilight" ;;
                v2) run_visual_step "Step 2: Specular" "$TESTS_DIR/step2_specular" ;;
                v3) run_visual_step "Step 3: Cone" "$TESTS_DIR/step3_cone" ;;
                v4) run_visual_step "Step 4: Checkerboard" "$TESTS_DIR/step4_checkerboard" ;;
                v5) run_visual_step "Step 5: Bump map" "$TESTS_DIR/step5_bumpmap" ;;
                0)
                    echo -e "\n${GREEN}Ate mais!${NC}\n"
                    exit 0
                    ;;
                *)
                    echo -e "${RED}Opcao invalida${NC}"
                    ;;
            esac
        done
        ;;
    *)
        echo "Uso: $0 [step1|step2|step3|step4|step5|all|compare|progress|leak]"
        exit 1
        ;;
esac
