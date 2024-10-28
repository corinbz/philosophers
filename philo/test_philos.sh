#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

TOTAL_TESTS=0
PASSED_TESTS=0

run_test() {
    local test_name=$1
    local cmd=$2
    local expected_result=$3
    local timeout_seconds=$4
    local check_meals=$5

    echo -e "\n${YELLOW}Running Test: ${test_name}${NC}"
    echo "Command: $cmd"

    local output_file=$(mktemp)
    local error_file=$(mktemp)

    timeout $timeout_seconds sh -c "$cmd" > "$output_file" 2> "$error_file" &
    local pid=$!
    wait $pid
    local exit_code=$?

    TOTAL_TESTS=$((TOTAL_TESTS + 1))

    if [ $expected_result -eq 0 ]; then
        if grep -q "died" "$output_file"; then
            echo -e "${RED}❌ Failed: Philosopher died when they shouldn't have${NC}"
            echo "Output:"
            cat "$output_file"
        elif [ $exit_code -eq 124 ]; then
            echo -e "${GREEN}✅ Passed: No philosophers died (timeout as expected)${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            if [ "$check_meals" = "true" ] && grep -q "have eaten enough times" "$output_file"; then
                echo -e "${GREEN}✅ Passed: All philosophers ate enough times${NC}"
                PASSED_TESTS=$((PASSED_TESTS + 1))
            elif [ "$check_meals" = "true" ]; then
                echo -e "${RED}❌ Failed: Philosophers should have finished their meals${NC}"
            else
                echo -e "${GREEN}✅ Passed: Program completed successfully${NC}"
                PASSED_TESTS=$((PASSED_TESTS + 1))
            fi
        fi
    else
        if grep -q "died" "$output_file"; then
            echo -e "${GREEN}✅ Passed: Philosopher died as expected${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}❌ Failed: Philosopher should have died${NC}"
        fi
    fi

    rm "$output_file" "$error_file"
}

check_memory_leaks() {
    echo -e "\n${YELLOW}Checking for memory leaks...${NC}"
    if ! command -v valgrind &> /dev/null; then
        echo "Valgrind not installed. Skipping memory leak check."
        return
    fi

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    valgrind --tool=memcheck --leak-check=full --error-exitcode=1 ./philo 4 410 200 200 5 > /dev/null 2>&1

    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ Passed: No memory leaks detected${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ Failed: Memory leaks detected${NC}"
    fi
}

check_race_conditions() {
    echo -e "\n${YELLOW}Checking for race conditions...${NC}"
    if ! command -v valgrind &> /dev/null; then
        echo "Valgrind not installed. Skipping race condition check."
        return
    fi

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    valgrind --tool=helgrind --error-exitcode=1 ./philo 4 410 200 200 5 > /dev/null 2>&1

    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ Passed: No race conditions detected${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ Failed: Race conditions detected${NC}"
    fi
}

echo -e "${YELLOW}Starting Philosophers Tests${NC}"

make re

# Basic cases
run_test "Single Philosopher" "./philo 1 800 200 200" 1 3
run_test "Four Philosophers" "./philo 4 410 200 200" 0 3
run_test "Five Philosophers" "./philo 5 800 200 200" 0 3
run_test "Large Number" "./philo 200 800 200 200" 0 5

# Time edge cases
run_test "Tight Timing Death" "./philo 2 310 300 300" 1 3
run_test "Tight Timing Survival" "./philo 2 400 200 200" 0 3
run_test "Very Short Times" "./philo 4 200 200 200" 1 3
run_test "Long Time" "./philo 4 2147483647 200 200" 0 3

# Meals counter cases
run_test "Two Philosophers Seven Meals" "./philo 2 800 200 200 7" 0 5 true
run_test "Four Philosophers Ten Meals" "./philo 4 410 200 200 10" 0 5 true
run_test "Single Meal" "./philo 4 410 200 200 1" 0 3 true

# Invalid input cases
run_test "Negative Time" "./philo 4 -410 200 200" 1 1
run_test "Zero Philosophers" "./philo 0 410 200 200" 1 1
run_test "No Arguments" "./philo" 1 1

# Special cases
run_test "Even Number" "./philo 2 800 200 200" 0 3
run_test "Odd Number" "./philo 3 800 200 200" 0 3
run_test "Equal Eat/Sleep" "./philo 4 410 200 200" 0 3
run_test "Eat Less Than Sleep" "./philo 4 410 100 200" 0 3
run_test "Sleep Less Than Eat" "./philo 4 410 200 100" 0 3

# Memory and thread safety checks
check_memory_leaks
check_race_conditions

echo -e "\n${YELLOW}Test Summary:${NC}"
echo -e "Total Tests: $TOTAL_TESTS"
echo -e "Passed Tests: $PASSED_TESTS"
echo -e "Failed Tests: $((TOTAL_TESTS - PASSED_TESTS))"

if [ $TOTAL_TESTS -eq $PASSED_TESTS ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi
