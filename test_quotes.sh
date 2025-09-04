#!/bin/bash

# Test script for quote handling in minishell
# Tests progress from simple to complex cases

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

test_count=0
pass_count=0

test_case() {
    local test_name="$1"
    local command="$2"
    
    ((test_count++))
    echo -e "\n${YELLOW}Test $test_count: $test_name${NC}"
    echo "Command: $command"
    
    # Get expected output from bash
    echo "Expected (bash):"
    bash_output=$(echo "$command" | bash 2>&1)
    echo "$bash_output"
    
    # Get actual output from minishell
    echo "Actual (minishell):"
    minishell_output=$(echo "$command" | ./minishell 2>&1 | sed '/minishell\$/d' | sed '/^$/d')
    echo "$minishell_output"
    
    # Compare outputs
    if [ "$bash_output" = "$minishell_output" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        ((pass_count++))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "Difference detected!"
    fi
}

echo "=== COMPREHENSIVE QUOTE TESTING ==="
echo "Testing quote handling from simple to complex cases"

# LEVEL 1: Basic quotes (no nesting)
echo -e "\n${YELLOW}=== LEVEL 1: BASIC QUOTES ===${NC}"

test_case "Simple word without quotes" "echo hello"
test_case "Single word in double quotes" "echo \"hello\""
test_case "Single word in single quotes" "echo 'hello'"
test_case "Multiple words in double quotes" "echo \"hello world\""
test_case "Multiple words in single quotes" "echo 'hello world'"

# LEVEL 2: Single quote characters
echo -e "\n${YELLOW}=== LEVEL 2: SINGLE QUOTE CHARACTERS ===${NC}"

test_case "Single quote in double quotes" "echo \"'\""
test_case "Double quote in single quotes" "echo '\"'"
test_case "Two single quotes in double quotes" "echo \"''\""
test_case "Two double quotes in single quotes" "echo '\"\"'"

# LEVEL 3: Triple quotes
echo -e "\n${YELLOW}=== LEVEL 3: TRIPLE QUOTES ===${NC}"

test_case "Three single quotes in double quotes" "echo \"'''\""
test_case "Three double quotes in single quotes" "echo '\"\"\"'"
test_case "Four single quotes in double quotes" "echo \"''''\""
test_case "Four double quotes in single quotes" "echo '\"\"\"\"'"

# LEVEL 4: Multiple arguments with quotes
echo -e "\n${YELLOW}=== LEVEL 4: MULTIPLE ARGUMENTS ===${NC}"

test_case "Multiple args with mixed quotes" "echo \"'\" '\"' \"hello\""
test_case "Arguments with quote sequences" "echo \"'''\" '\"\"\"' normal"
test_case "Complex argument sequence" "echo first \"'\" second '\"' third"

# LEVEL 5: Edge cases
echo -e "\n${YELLOW}=== LEVEL 5: EDGE CASES ===${NC}"

test_case "Empty double quotes" "echo \"\""
test_case "Empty single quotes" "echo ''"
test_case "Just spaces in quotes" "echo \"   \""
test_case "Mixed content in quotes" "echo \"hello'world\""
test_case "Mixed content in single quotes" "echo 'hello\"world'"

# LEVEL 6: Special sequences
echo -e "\n${YELLOW}=== LEVEL 6: SPECIAL SEQUENCES ===${NC}"

test_case "Five single quotes in double quotes" "echo \"'''''\""
test_case "Six single quotes in double quotes" "echo \"''''''\""
test_case "Five double quotes in single quotes" "echo '\"\"\"\"\"'"
test_case "Six double quotes in single quotes" "echo '\"\"\"\"\"\"'"

# LEVEL 7: Real-world scenarios
echo -e "\n${YELLOW}=== LEVEL 7: REAL-WORLD SCENARIOS ===${NC}"

test_case "Apostrophe in message" "echo \"Don't worry\""
test_case "Quote in sentence" "echo 'He said \"hello\"'"
test_case "Code-like string" "echo \"function() { return 'value'; }\""
test_case "JSON-like string" "echo '{\"name\": \"John\", \"age\": 30}'"

# LEVEL 8: Known edge cases (may have differences from bash)
echo -e "\n${YELLOW}=== LEVEL 8: EDGE CASES (INFORMATIONAL) ===${NC}"
echo "Note: These cases may behave differently from bash due to tokenization approach"

test_case "Mixed quote content" "echo \"'a'\""

# Summary
echo -e "\n${YELLOW}=== TEST SUMMARY ===${NC}"
echo "Total tests: $test_count"
echo "Passed: $pass_count"
echo "Failed: $((test_count - pass_count))"

if [ $pass_count -eq $test_count ]; then
    echo -e "${GREEN}All tests passed! 🎉${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed. 😞${NC}"
    exit 1
fi