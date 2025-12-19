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

# Test that a valid file runs without crashing (visual test)
test_valid_scene() {
    local file=$1
    local name=$(basename "$file" .rt)

    if [ ! -f "$file" ]; then
        print_result 1 "$name - File not found"
        return
    fi

    # Run miniRT with timeout and check if it starts successfully
    # The program should open and render without crashing
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

# Test that an invalid file produces an error
test_error_scene() {
    local file=$1
    local name=$(basename "$file" .rt)

    if [ ! -f "$file" ]; then
        # For missing file test
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

    # Run miniRT and expect it to fail with error message
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

# Test missing file
test_missing_file() {
    local output
    output=$($MINIRT "nonexistent_file_xyz.rt" 2>&1)

    if echo "$output" | grep -qi "error"; then
        print_result 0 "Missing file detection"
    else
        print_result 1 "Missing file detection"
    fi
}

# Test wrong extension
test_wrong_extension() {
    local output
    output=$($MINIRT "tests/error_tests/02_wrong_extension.txt" 2>&1)

    if echo "$output" | grep -qi "error"; then
        print_result 0 "Wrong extension detection"
    else
        print_result 1 "Wrong extension detection"
    fi
}

# Test no arguments
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
    echo "  9) Exit"
    echo ""
    echo -n "  Select option: "
}

# =============================================================================
# Main
# =============================================================================

# Check if miniRT exists
if [ ! -f "$MINIRT" ]; then
    echo -e "${RED}Error: miniRT executable not found!${NC}"
    echo -e "Please run 'make' first."
    exit 1
fi

# Parse command line arguments
case "$1" in
    "all")
        run_level1_tests
        run_level2_tests
        run_level3_tests
        run_edge_case_tests
        run_error_tests
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
    "")
        # Interactive mode
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
        echo "Usage: $0 [all|level1|level2|level3|error|bonus|edge|visual]"
        exit 1
        ;;
esac
