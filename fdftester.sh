#!/bin/bash

# Interactive FDF tester
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
COLOR_RESET="\033[0m"

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

# Run tests
echo "FDF Interactive Test Suite"
echo "--------------------------"
echo "Executable: $FDF_EXEC"
echo "Found $TOTAL_TESTS test files"
echo -e "Controls:\n- Press ESC or close window to advance to next test\n- Ctrl+C to exit tester\n"

for test_file in "${TEST_FILES[@]}"; do
    echo -e "${COLOR_BLUE}[Test $CURRENT_TEST/$TOTAL_TESTS]${COLOR_RESET}"
    echo "Loading: $(basename "$test_file")"
    echo -e "${COLOR_GREEN}Window should open shortly...${COLOR_RESET}"
    
    # Run FDF and wait for it to exit
    "$FDF_EXEC" "$test_file"
    
    echo -e "Closed: $(basename "$test_file")\n"
    ((CURRENT_TEST++))
done

echo "All tests completed!"