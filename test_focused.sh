#!/bin/bash

# Focused test to identify key issues

RED='\033[0;31m'
GREEN='\033[0;32m'  
YELLOW='\033[1;33m'
NC='\033[0m'

echo "=== FOCUSED TESTING FOR KEY ISSUES ==="

# Test echo -n flag
echo -e "\n${YELLOW}Testing echo -n flag${NC}"
echo "Expected (bash): echo -n hello"
bash -c "echo -n hello; echo"
echo
echo "Actual (minishell): echo -n hello"
echo "echo -n hello" | ./minishell

# Test command not found exit code
echo -e "\n${YELLOW}Testing command not found exit code${NC}"
echo "Expected (bash):"
bash -c "nonexistentcommand; echo Exit code: \$?"
echo "Actual (minishell):"
echo -e "nonexistentcommand\necho Exit code: \$?" | ./minishell

# Test exit codes
echo -e "\n${YELLOW}Testing exit command${NC}"
echo "Expected (bash): exit 42 should exit with code 42"
bash -c "exit 42"; echo "Bash exit code: $?"
echo "Actual (minishell): exit 42"
echo "exit 42" | ./minishell; echo "Minishell exit code: $?"

# Test basic redirection
echo -e "\n${YELLOW}Testing basic redirection${NC}"
echo "Expected (bash): echo hello > test.txt"
bash -c "echo hello > test.txt && cat test.txt"
echo "Actual (minishell):"
echo -e "echo hello > test_mini.txt\ncat test_mini.txt" | ./minishell

# Clean up
rm -f test.txt test_mini.txt

echo -e "\n=== END FOCUSED TESTING ==="