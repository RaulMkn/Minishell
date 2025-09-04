#!/bin/bash

# Refined 42 Minishell Evaluation Test Suite
# Better handling of minishell output and prompts

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

test_count=0
pass_count=0

test_command() {
    local test_name="$1"
    local command="$2"
    local check_exit_code="${3:-false}"
    
    ((test_count++))
    echo -e "\n${YELLOW}Test $test_count: $test_name${NC}"
    echo "Command: $command"
    
    # Execute with bash
    if [ "$check_exit_code" = "true" ]; then
        bash_output=$(bash -c "$command" 2>&1)
        bash_exit_code=$?
    else
        bash_output=$(bash -c "$command" 2>&1)
        bash_exit_code=0
    fi
    
    # Execute with minishell  
    if [ "$check_exit_code" = "true" ]; then
        minishell_full=$(echo "$command" | ./minishell 2>&1)
        minishell_exit_code=$?
        # Extract just the command output, skip prompts
        minishell_output=$(echo "$minishell_full" | grep -v "minishell\$" | sed '/^$/d' | sed 's/^minishell: /bash: line 1: /')
    else
        minishell_full=$(echo "$command" | ./minishell 2>&1)
        minishell_exit_code=0
        # Extract just the command output, skip prompts
        minishell_output=$(echo "$minishell_full" | grep -v "minishell\$" | sed '/^$/d' | sed 's/^minishell: /bash: line 1: /')
    fi
    
    # Compare results
    local outputs_match=true
    local exit_codes_match=true
    
    if [ "$bash_output" != "$minishell_output" ]; then
        outputs_match=false
    fi
    
    if [ "$check_exit_code" = "true" ] && [ "$bash_exit_code" != "$minishell_exit_code" ]; then
        exit_codes_match=false
    fi
    
    if $outputs_match && $exit_codes_match; then
        echo -e "${GREEN}✓ PASS${NC}"
        ((pass_count++))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "Expected: '$bash_output'"
        echo "Got:      '$minishell_output'"
        if [ "$check_exit_code" = "true" ]; then
            echo "Expected exit: $bash_exit_code, Got: $minishell_exit_code"
        fi
    fi
}

echo -e "${CYAN}=== REFINED MINISHELL TEST SUITE ===${NC}"

# Basic functionality tests
echo -e "\n${CYAN}=== BASIC COMMANDS ===${NC}"
test_command "Simple echo" "echo hello world"
test_command "Echo with quotes" "echo 'hello world'"
test_command "Command with absolute path" "/bin/echo test"
test_command "Multiple spaces" "echo    hello     world"

# Builtin commands
echo -e "\n${CYAN}=== BUILTIN COMMANDS ===${NC}"
test_command "PWD command" "pwd"
test_command "ENV command" "env | head -3"
test_command "Echo without newline" "echo -n hello"
test_command "Echo with multiple -n" "echo -nnn test"
test_command "Export variable" "export TESTVAR=value && echo \$TESTVAR"
test_command "Unset variable" "export TESTVAR=value && unset TESTVAR && echo \$TESTVAR"

# Error handling  
echo -e "\n${CYAN}=== ERROR HANDLING ===${NC}"
test_command "Command not found" "invalidcommand123" true
test_command "Permission denied" "cat /root/.bashrc" true

# Variable expansion
echo -e "\n${CYAN}=== VARIABLE EXPANSION ===${NC}"
test_command "HOME variable" "echo \$HOME"
test_command "Undefined variable" "echo \$UNDEFINED_VARIABLE_TEST"
test_command "Exit status" "echo \$?"
test_command "Variable in quotes" "echo \"\$HOME\""
test_command "Variable preserved in single quotes" "echo '\$HOME'"

# Redirections
echo -e "\n${CYAN}=== REDIRECTIONS ===${NC}"
test_command "Output redirection" "echo test123 > testfile.txt && cat testfile.txt"
test_command "Append redirection" "echo first > append.txt && echo second >> append.txt && cat append.txt"
test_command "Input redirection" "echo content > input.txt && cat < input.txt"

# Pipes
echo -e "\n${CYAN}=== PIPES ===${NC}"  
test_command "Simple pipe" "echo 'line1\nline2\nline3' | head -2"
test_command "Pipe chain" "echo 'hello\nworld\nhello' | grep hello | wc -l"

# Logical operators
echo -e "\n${CYAN}=== LOGICAL OPERATORS ===${NC}"
test_command "AND success" "true && echo success"
test_command "AND failure" "false && echo should_not_appear"  
test_command "OR success" "false || echo appears"
test_command "Complex logic" "true && echo first && false || echo second"

# Cleanup
rm -f testfile.txt append.txt input.txt

# Summary
echo -e "\n${CYAN}=== SUMMARY ===${NC}"
echo "Total tests: $test_count"
echo "Passed: $pass_count"
echo "Failed: $((test_count - pass_count))"

if [ $pass_count -eq $test_count ]; then
    echo -e "${GREEN}🎉 All tests passed!${NC}"
else
    echo -e "${YELLOW}Success rate: $((pass_count * 100 / test_count))%${NC}"
fi