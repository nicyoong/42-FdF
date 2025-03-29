#!/bin/bash

# Test script for FDF project
# Usage: ./test_fdf.sh

# Configuration
TEST_DIR="./test_maps"
FDF_EXEC="./fdf"
TIMEOUT_DURATION=5  # seconds per test
COLOR_GREEN="\033[32m"
COLOR_RED="\033[31m"
COLOR_RESET="\033[0m"

# Check if test directory exists
if [ ! -d "$TEST_DIR" ]; then
    echo "Error: Test directory $TEST_DIR not found!"
    exit 1
fi

# Check if fdf executable exists
if [ ! -f "$FDF_EXEC" ]; then
    echo "Error: FDF executable $FDF_EXEC not found!"
    exit 1
fi

# Get list of test files
TEST_FILES=("$TEST_DIR"/*.fdf)
if [ ${#TEST_FILES[@]} -eq 0 ]; then
    echo "Error: No .fdf files found in $TEST_DIR"
    exit 1
fi

# Counters
TOTAL_TESTS=0
PASSED=0
FAILED=0

# Header
echo "FDF TEST SUITE"
echo "---------------"
echo "Testing executable: $FDF_EXEC"
echo "Test maps directory: $TEST_DIR"
echo "Found ${#TEST_FILES[@]} test files"
echo

# Run tests
for test_file in "${TEST_FILES[@]}"; do
    ((TOTAL_TESTS++))
    printf "Testing %-40s ... " "$(basename "$test_file")"
    
    # Run with timeout
    timeout $TIMEOUT_DURATION $FDF_EXEC "$test_file" >/dev/null 2>&1
    EXIT_STATUS=$?
    
    # Evaluate result
    if [ $EXIT_STATUS -eq 0 ]; then
        printf "${COLOR_GREEN}PASS${COLOR_RESET}\n"
        ((PASSED++))
    elif [ $EXIT_STATUS -eq 124 ]; then
        printf "${COLOR_RED}FAIL (Timeout)${COLOR_RESET}\n"
        ((FAILED++))
    else
        printf "${COLOR_RED}FAIL (Error $EXIT_STATUS)${COLOR_RESET}\n"
        ((FAILED++))
    fi
done

# Summary
echo
echo "TEST SUMMARY"
echo "------------"
echo "Total tests:  $TOTAL_TESTS"
echo "${COLOR_GREEN}Passed:      $PASSED${COLOR_RESET}"
echo "${COLOR_RED}Failed:      $FAILED${COLOR_RESET}"
echo

# Exit code matches failure count
exit $FAILED