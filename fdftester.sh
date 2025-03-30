#!/bin/bash

# Interactive FDF tester with Valgrind
# Usage: ./test_fdf.sh ./your_fdf_executable

# Check for valid arguments
if [ $# -ne 1 ] || [ ! -f "$1" ]; then
    echo "Usage: $0 /path/to/fdf_executable"
    exit 1
fi

# Configuration
TEST_DIR="./test_maps"
FDF_EXEC="$1"
COLOR_GREEN="\033[32m"
COLOR_BLUE="\033[34m"
COLOR_RED="\033[31m"
COLOR_YELLOW="\033[33m"
COLOR_RESET="\033[0m"
VALGRIND_ENABLED=true  # Set to false to disable Valgrind
VALGRIND_OPTS="--leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes -s"

# Check test directory
if [ ! -d "$TEST_DIR" ]; then
    echo "Error: Test directory $TEST_DIR not found!"
    exit 1
fi

# Get test files
TEST_FILES=("$TEST_DIR"/*.fdf)
if [ ${#TEST_FILES[@]} -eq 0 ]; then
    echo "Error: No .fdf files found in $TEST_DIR"
    exit 1
fi

# Test counter
TOTAL_TESTS=${#TEST_FILES[@]}
CURRENT_TEST=1
VALGRIND_FAILS=0

# Run tests
echo "FDF Interactive Test Suite"
echo "--------------------------"
echo "Executable: $FDF_EXEC"
echo "Found $TOTAL_TESTS test files"
echo -e "Controls:\n- Press ESC or close window to advance to next test\n- Ctrl+C to exit tester\n"

for test_file in "${TEST_FILES[@]}"; do
    echo -e "${COLOR_BLUE}[Test $CURRENT_TEST/$TOTAL_TESTS]${COLOR_RESET}"
    echo "Loading: $(basename "$test_file")"
    
    if [ "$VALGRIND_ENABLED" = true ]; then
        echo -e "${COLOR_YELLOW}Running with Valgrind...${COLOR_RESET}"
        # Run with Valgrind in the background
        valgrind $VALGRIND_OPTS "$FDF_EXEC" "$test_file" > valgrind_output_$CURRENT_TEST.txt 2>&1 &
        VALGRIND_PID=$!
        
        # Give some time for the window to open
        sleep 2
        
        # Wait for the FDF program to finish
        wait $VALGRIND_PID
        VALGRIND_RESULT=$?
        
        # Check Valgrind's output for errors
        if grep -q "ERROR SUMMARY: 0 errors" valgrind_output_$CURRENT_TEST.txt; then
            echo -e "${COLOR_GREEN}Valgrind: No errors detected${COLOR_RESET}"
        else
            echo -e "${COLOR_RED}Valgrind: Errors detected!${COLOR_RESET}"
            ((VALGRIND_FAILS++))
            # Show Valgrind summary
            tail -n 10 valgrind_output_$CURRENT_TEST.txt | grep -A 10 "ERROR SUMMARY"
        fi
    else
        echo -e "${COLOR_GREEN}Window should open shortly...${COLOR_RESET}"
        "$FDF_EXEC" "$test_file"
    fi
    
    echo -e "Closed: $(basename "$test_file")\n"
    ((CURRENT_TEST++))
done

# Summary
echo "All tests completed!"
if [ "$VALGRIND_ENABLED" = true ]; then
    echo -e "Valgrind results: $((TOTAL_TESTS - VALGRIND_FAILS))/$TOTAL_TESTS tests clean"
    if [ $VALGRIND_FAILS -ne 0 ]; then
        echo -e "${COLOR_RED}Valgrind detected issues in $VALGRIND_FAILS test(s)${COLOR_RESET}"
        echo "Check valgrind_output_*.txt files for details"
    fi
fi
