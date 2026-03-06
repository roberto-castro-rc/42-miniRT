#!/bin/bash

# =============================================================================
# MiniRT Test Suite
# =============================================================================
# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color
BOLD='\033[1m'

# Counters
PASS=0
FAIL=0
TOTAL=0

# Test configuration
TIMEOUT=5
MINIRT="./miniRT"

# =============================================================================
# Helper Functions
# =============================================================================

print_header() {
    echo -e "\n${BOLD}${BLUE}╔══════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BOLD}${BLUE}║${NC}  ${BOLD}$1${NC}"
    echo -e "${BOLD}${BLUE}╚══════════════════════════════════════════════════════════════╝${NC}\n"
}

print_subheader() {
    echo -e "\n${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${CYAN}  $1${NC}"
    echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}\n"
}

print_result() {
    if [ $1 -eq 0 ]; then
        echo -e "  ${GREEN}✓${NC} $2"
        ((PASS++))
    else
        echo -e "  ${RED}✗${NC} $2 ${RED}(FAILED)${NC}"
        ((FAIL++))
    fi
    ((TOTAL++))
}

print_summary() {
    echo -e "\n${BOLD}${BLUE}╔══════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BOLD}${BLUE}║${NC}  ${BOLD}TEST SUMMARY${NC}"
    echo -e "${BOLD}${BLUE}╚══════════════════════════════════════════════════════════════╝${NC}"
    echo -e "  ${GREEN}Passed:${NC} $PASS"
    echo -e "  ${RED}Failed:${NC} $FAIL"
    echo -e "  ${YELLOW}Total:${NC}  $TOTAL"
    if [ $FAIL -eq 0 ]; then
        echo -e "\n  ${GREEN}${BOLD}All tests passed! ✓${NC}\n"
        return 0
    else
        echo -e "\n  ${RED}${BOLD}Some tests failed! ✗${NC}\n"
        return 1
    fi
}

# =============================================================================
# Test Functions
# =============================================================================

test_valid_scene() {
    local file=$1
    local name=$(basename "$file" .rt)

    if [ ! -f "$file" ]; then
        print_result 1 "$name - File not found"
        return
    fi

    timeout $TIMEOUT $MINIRT "$file" &
    local pid=$!
    sleep 1

    if kill -0 $pid 2>/dev/null; then
        kill $pid 2>/dev/null
        wait $pid 2>/dev/null
        print_result 0 "$name"
    else
        wait $pid 2>/dev/null
        local exit_code=$?
        if [ $exit_code -eq 124 ]; then
            print_result 0 "$name (timeout - expected)"
        else
            print_result 1 "$name (crashed with code $exit_code)"
        fi
    fi
}

test_error_scene() {
    local file=$1
    local name=$(basename "$file" .rt)

    if [ ! -f "$file" ]; then
        if [[ "$file" == *"nonexistent"* ]]; then
            $MINIRT "$file" 2>&1 | grep -qi "error"
            if [ $? -eq 0 ]; then
                print_result 0 "$name - Error detected correctly"
            else
                print_result 1 "$name - Should have produced error"
            fi
        else
            print_result 1 "$name - Test file not found"
        fi
        return
    fi

    output=$($MINIRT "$file" 2>&1)
    exit_code=$?

    if echo "$output" | grep -qi "error"; then
        print_result 0 "$name"
    elif [ $exit_code -ne 0 ]; then
        print_result 0 "$name (exit code: $exit_code)"
    else
        print_result 1 "$name - Should have produced error"
    fi
}

test_missing_file() {
    local output
    output=$($MINIRT "nonexistent_file_xyz.rt" 2>&1)
    if echo "$output" | grep -qi "error"; then
        print_result 0 "Missing file detection"
    else
        print_result 1 "Missing file detection"
    fi
}

test_wrong_extension() {
    local output
    output=$($MINIRT "tests/error_tests/02_wrong_extension.txt" 2>&1)
    if echo "$output" | grep -qi "error"; then
        print_result 0 "Wrong extension detection"
    else
        print_result 1 "Wrong extension detection"
    fi
}

test_no_args() {
    local output
    output=$($MINIRT 2>&1)
    if echo "$output" | grep -qi "error\|usage"; then
        print_result 0 "No arguments handling"
    else
        print_result 1 "No arguments handling"
    fi
}

# =============================================================================
# Main Test Suites
# =============================================================================

run_level1_tests() {
    print_header "Level 1 Tests - Basic Rendering"
    for file in tests/level1_basic/*.rt; do
        if [ -f "$file" ]; then
            test_valid_scene "$file"
        fi
    done
}

run_level2_tests() {
    print_header "Level 2 Tests - Intermediate"
    for file in tests/level2_intermediate/*.rt; do
        if [ -f "$file" ]; then
            test_valid_scene "$file"
        fi
    done
}

run_level3_tests() {
    print_header "Level 3 Tests - Advanced"
    for file in tests/level3_advanced/*.rt; do
        if [ -f "$file" ]; then
            test_valid_scene "$file"
        fi
    done
}

run_error_tests() {
    print_header "Error Handling Tests"
    print_subheader "File Handling"
    test_no_args
    test_missing_file
    test_wrong_extension
    print_subheader "Parsing Errors"
    for file in tests/error_tests/*.rt; do
        if [ -f "$file" ]; then
            test_error_scene "$file"
        fi
    done
}

run_bonus_tests() {
    print_header "Bonus Tests - Multiple Lights"
    for file in tests/bonus/*.rt; do
        if [ -f "$file" ]; then
            test_valid_scene "$file"
        fi
    done
}

run_edge_case_tests() {
    print_header "Edge Case Tests"
    for file in tests/edge_cases/*.rt; do
        if [ -f "$file" ]; then
            test_valid_scene "$file"
        fi
    done
}

run_leak_tests() {
    print_header "Leak Detection Tests (valgrind)"

    if ! command -v valgrind &>/dev/null; then
        echo -e "  ${RED}valgrind not found. Install with: sudo apt install valgrind${NC}"
        return 1
    fi

    # Flags explicadas:
    #   --leak-check=full         mostra cada bloco perdido individualmente
    #   --show-leak-kinds=all     inclui still-reachable, possible, indirect
    #   --track-origins=yes       rastreia origem de valores nao inicializados
    #   --error-exitcode=42       retorna 42 se houver qualquer erro/leak
    #   --errors-for-leak-kinds=all  trata todos os tipos de leak como erro
    #   --num-callers=20          mais frames no stack trace
    # Nota: --track-fds=yes removido pois WSL herda fd /dev/ptmx (falso positivo)
    local VG="valgrind --leak-check=full --show-leak-kinds=all \
              --track-origins=yes \
              --error-exitcode=42 --errors-for-leak-kinds=all \
              --num-callers=20"
    local VG_LOG="/tmp/vg_minirt.log"

    check_leak() {
        local label="$1"
        # </dev/null isola stdin - impede que valgrind/minirt consuma input do menu
        $VG $MINIRT "${@:2}" </dev/null >"$VG_LOG" 2>&1
        local code=$?
        if [ $code -eq 42 ]; then
            print_result 1 "LEAK: $label"
            # Mostra stack trace completo + resumo de bytes
            grep -E 'by 0x|at 0x|definitely|indirectly|still reachable|FILE DESC|bytes in' \
                "$VG_LOG" 2>/dev/null | head -15 | sed 's/^/    /'
        else
            print_result 0 "$label"
        fi
    }

    print_subheader "Invalid args / file paths"
    check_leak "no arguments"
    check_leak "missing file"                   "nonexistent_xyz.rt"
    check_leak "wrong extension"                "tests/error_tests/02_wrong_extension.txt"

    print_subheader "Parser error paths (all error_tests)"
    for file in tests/error_tests/*.rt; do
        [ -f "$file" ] && check_leak "$(basename "$file" .rt)" "$file"
    done

    print_subheader "Valid scenes: parser + cleanup (DISPLAY invalido = sem janela)"
    # DISPLAY=:99 forca MLX a falhar na abertura da janela e sair rapidamente.
    # Isso permite checar se free() de spheres/planes/cylinders/lights ocorre
    # em todos os caminhos de saida, incluindo falha do MLX.
    local valid_scenes=(
        "tests/level1_basic/02_single_plane.rt"
        "tests/level1_basic/03_single_cylinder.rt"
        "tests/level2_intermediate/03_mixed_objects.rt"
        "tests/level2_intermediate/08_multiple_shadows.rt"
        "tests/level3_advanced/01_complex_scene.rt"
        "tests/edge_cases/09_zero_ambient.rt"
        "tests/edge_cases/11_thin_cylinder.rt"
        "tests/edge_cases/13_negative_coords.rt"
    )
    for scene in "${valid_scenes[@]}"; do
        if [ -f "$scene" ]; then
            local name=$(basename "$scene" .rt)
            DISPLAY=:99 timeout 5 $VG $MINIRT "$scene" </dev/null >"$VG_LOG" 2>&1
            local code=$?
            if [ $code -eq 42 ]; then
                print_result 1 "LEAK: $name"
                grep -A2 'definitely lost\|indirectly lost\|Invalid\|FILE DESCRIPTORS' \
                    "$VG_LOG" 2>/dev/null | head -12 | sed 's/^/    /'
            else
                print_result 0 "$name"
            fi
        fi
    done

    rm -f "$VG_LOG"
}

run_quick_visual_test() {
    print_header "Quick Visual Test"
    echo -e "${YELLOW}This will open scenes for visual inspection.${NC}"
    echo -e "${YELLOW}Press ESC to close each window.${NC}"
    local scenes=(
        "tests/level1_basic/01_single_sphere.rt"
        "tests/level2_intermediate/03_mixed_objects.rt"
        "tests/level3_advanced/02_snowman.rt"
    )
    for scene in "${scenes[@]}"; do
        if [ -f "$scene" ]; then
            echo -e "\n${CYAN}Opening: $(basename $scene)${NC}"
            $MINIRT "$scene"
        fi
    done
}

# =============================================================================
# Menu
# =============================================================================

show_menu() {
    echo -e "\n${BOLD}${BLUE}╔══════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BOLD}${BLUE}║${NC}        ${BOLD}MiniRT Test Suite${NC}                                     ${BOLD}${BLUE}║${NC}"
    echo -e "${BOLD}${BLUE}╚══════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    echo "  1) Run All Tests"
    echo "  2) Level 1 - Basic"
    echo "  3) Level 2 - Intermediate"
    echo "  4) Level 3 - Advanced"
    echo "  5) Error Tests"
    echo "  6) Bonus Tests"
    echo "  7) Edge Case Tests"
    echo "  8) Quick Visual Test"
    echo "  9) Leak Detection (valgrind)"
    echo "  0) Exit"
    echo ""
    echo -n "  Select option: "
}

# =============================================================================
# Main
# =============================================================================

if [ ! -f "$MINIRT" ]; then
    echo -e "${RED}Error: miniRT executable not found!${NC}"
    echo -e "Please run 'make' first."
    exit 1
fi

case "$1" in
    "all")
        run_level1_tests
        run_level2_tests
        run_level3_tests
        run_edge_case_tests
        run_error_tests
        run_bonus_tests
        print_summary
        ;;
    "level1")
        run_level1_tests
        print_summary
        ;;
    "level2")
        run_level2_tests
        print_summary
        ;;
    "level3")
        run_level3_tests
        print_summary
        ;;
    "error"|"errors")
        run_error_tests
        print_summary
        ;;
    "bonus")
        run_bonus_tests
        print_summary
        ;;
    "edge")
        run_edge_case_tests
        print_summary
        ;;
    "visual")
        run_quick_visual_test
        ;;
    "leak"|"leaks")
        run_leak_tests
        print_summary
        ;;
    "")
        while true; do
            show_menu
            read choice
            case $choice in
                1)
                    PASS=0; FAIL=0; TOTAL=0
                    run_level1_tests
                    run_level2_tests
                    run_level3_tests
                    run_edge_case_tests
                    run_error_tests
                    run_bonus_tests
                    print_summary
                    ;;
                2)
                    PASS=0; FAIL=0; TOTAL=0
                    run_level1_tests
                    print_summary
                    ;;
                3)
                    PASS=0; FAIL=0; TOTAL=0
                    run_level2_tests
                    print_summary
                    ;;
                4)
                    PASS=0; FAIL=0; TOTAL=0
                    run_level3_tests
                    print_summary
                    ;;
                5)
                    PASS=0; FAIL=0; TOTAL=0
                    run_error_tests
                    print_summary
                    ;;
                6)
                    PASS=0; FAIL=0; TOTAL=0
                    run_bonus_tests
                    print_summary
                    ;;
                7)
                    PASS=0; FAIL=0; TOTAL=0
                    run_edge_case_tests
                    print_summary
                    ;;
                8)
                    run_quick_visual_test
                    ;;
                9)
                    PASS=0; FAIL=0; TOTAL=0
                    run_leak_tests
                    print_summary
                    ;;
                0)
                    echo -e "\n${GREEN}Goodbye!${NC}\n"
                    exit 0
                    ;;
                *)
                    echo -e "${RED}Invalid option${NC}"
                    ;;
            esac
        done
        ;;
    *)
        echo "Usage: $0 [all|level1|level2|level3|error|bonus|edge|visual|leak]"
         exit 1
        ;;
esac
