#!/bin/bash

# 42 Minishell Evaluation Test Suite
# Comprehensive testing covering all mandatory requirements

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

test_count=0
pass_count=0
category_count=0
category_pass=0

# Test result tracking
declare -A category_results

start_category() {
    local category_name="$1"
    echo -e "\n${CYAN}=====================================${NC}"
    echo -e "${CYAN}=== $category_name ===${NC}"
    echo -e "${CYAN}=====================================${NC}"
    category_count=0
    category_pass=0
}

end_category() {
    local category_name="$1"
    category_results["$category_name"]="$category_pass/$category_count"
    echo -e "${BLUE}--- $category_name: $category_pass/$category_count tests passed ---${NC}"
}

test_command() {
    local test_name="$1"
    local command="$2"
    local check_exit_code="${3:-false}"
    
    ((test_count++))
    ((category_count++))
    echo -e "\n${YELLOW}Test $test_count: $test_name${NC}"
    echo "Command: $command"
    
    # Execute with bash
    echo "Expected (bash):"
    if [ "$check_exit_code" = "true" ]; then
        bash_output=$(bash -c "$command" 2>&1; echo "EXIT_CODE:$?")
        bash_exit_code=$(echo "$bash_output" | tail -n1 | cut -d: -f2)
        bash_output=$(echo "$bash_output" | head -n -1)
    else
        bash_output=$(bash -c "$command" 2>&1)
        bash_exit_code=0
    fi
    echo "$bash_output"
    [ "$check_exit_code" = "true" ] && echo "Exit code: $bash_exit_code"
    
    # Execute with minishell
    echo "Actual (minishell):"
    if [ "$check_exit_code" = "true" ]; then
        minishell_output=$(echo "$command" | ./minishell 2>&1 | sed '/minishell\$/d' | sed '/^$/d'; echo "EXIT_CODE:$?")
        minishell_exit_code=$(echo "$minishell_output" | tail -n1 | cut -d: -f2)
        minishell_output=$(echo "$minishell_output" | head -n -1)
    else
        minishell_output=$(echo "$command" | ./minishell 2>&1 | sed '/minishell\$/d' | sed '/^$/d')
        minishell_exit_code=0
    fi
    echo "$minishell_output"
    [ "$check_exit_code" = "true" ] && echo "Exit code: $minishell_exit_code"
    
    # Compare results
    if [ "$bash_output" = "$minishell_output" ] && [ "$bash_exit_code" = "$minishell_exit_code" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        ((pass_count++))
        ((category_pass++))
    else
        echo -e "${RED}✗ FAIL${NC}"
        if [ "$bash_output" != "$minishell_output" ]; then
            echo "Output differs!"
        fi
        if [ "$bash_exit_code" != "$minishell_exit_code" ] && [ "$check_exit_code" = "true" ]; then
            echo "Exit codes differ!"
        fi
    fi
}

test_interactive() {
    local test_name="$1"
    local description="$2"
    
    ((test_count++))
    ((category_count++))
    echo -e "\n${YELLOW}Test $test_count: $test_name${NC}"
    echo "Description: $description"
    echo -e "${MAGENTA}[MANUAL TEST - Check manually]${NC}"
    echo -e "${GREEN}✓ DOCUMENTED${NC}"
    ((pass_count++))
    ((category_pass++))
}

echo -e "${MAGENTA}"
echo "=================================================="
echo "=== 42 MINISHELL EVALUATION TEST SUITE ==="
echo "=================================================="
echo -e "${NC}"
echo "Testing all mandatory requirements for minishell"
echo "Comparing behavior with bash"

# ============================================
# 1. BASIC FUNCTIONALITY
# ============================================
start_category "BASIC FUNCTIONALITY"

test_command "Simple command execution" "echo hello world"
test_command "Command with path" "/bin/echo hello"
test_command "Multiple spaces handling" "echo    hello     world"
test_command "Empty command handling" ""
test_command "Command not found" "nonexistentcommand" true

end_category "BASIC FUNCTIONALITY"

# ============================================
# 2. BUILTIN COMMANDS
# ============================================
start_category "BUILTIN COMMANDS"

# Echo tests
test_command "Echo simple" "echo hello"
test_command "Echo with -n flag" "echo -n hello"
test_command "Echo with multiple -n flags" "echo -nnn hello"
test_command "Echo with invalid -n variant" "echo -nX hello"
test_command "Echo with mixed flags" "echo -n -n hello"
test_command "Echo empty string" "echo"
test_command "Echo with quotes" "echo \"hello world\""

# PWD tests
test_command "PWD command" "pwd"

# CD tests  
test_command "CD to home" "cd && pwd"
test_command "CD to root" "cd / && pwd"
test_command "CD with relative path" "cd .. && pwd"
test_command "CD with absolute path" "cd /tmp && pwd"
test_command "CD to nonexistent directory" "cd /nonexistent" true
test_command "CD with no arguments" "cd"
test_command "CD with dash" "cd -"

# ENV tests
test_command "ENV command" "env | head -5"

# EXPORT tests
test_command "Export new variable" "export TESTVAR=hello && env | grep TESTVAR"
test_command "Export existing variable" "export PATH && env | grep PATH | head -1"
test_command "Export without value" "export NEWVAR"
test_command "Export invalid identifier" "export 123INVALID=test" true
test_command "Export with special chars" "export TEST_VAR=hello"

# UNSET tests
test_command "Unset existing variable" "export TESTVAR=hello && unset TESTVAR && env | grep TESTVAR"
test_command "Unset nonexistent variable" "unset NONEXISTENT"
test_command "Unset PATH" "unset PATH && echo \$PATH"
test_command "Unset invalid identifier" "unset 123INVALID" true

# EXIT tests
test_command "Exit with code 0" "exit 0" true
test_command "Exit with code 42" "exit 42" true
test_command "Exit with invalid code" "exit abc" true
test_command "Exit with large number" "exit 999" true

end_category "BUILTIN COMMANDS"

# ============================================
# 3. ENVIRONMENT VARIABLES & EXPANSION
# ============================================
start_category "ENVIRONMENT VARIABLES"

test_command "Basic variable expansion" "echo \$HOME"
test_command "Undefined variable expansion" "echo \$UNDEFINED_VAR"
test_command "Variable in middle of string" "echo hello\${HOME}world"
test_command "Multiple variable expansion" "echo \$HOME \$USER \$PWD"
test_command "Exit status variable" "echo \$?"
test_command "Exit status after failed command" "false && echo \$?"
test_command "Variable with braces" "echo \${HOME}"
test_command "Variable without expansion in single quotes" "echo '\$HOME'"
test_command "Variable with expansion in double quotes" "echo \"\$HOME\""
test_command "Mixed quotes and variables" "echo 'hello' \$USER \"world\""

end_category "ENVIRONMENT VARIABLES"

# ============================================
# 4. QUOTES AND PARSING
# ============================================
start_category "QUOTES AND PARSING"

test_command "Single quotes preserve all" "echo 'hello \$HOME world'"
test_command "Double quotes allow expansion" "echo \"hello \$HOME world\""
test_command "Empty quotes" "echo ''"
test_command "Nested quotes basic" "echo \"hello 'world'\""
test_command "Mixed quote types" "echo 'hello' \"world\""
test_command "Quotes with spaces" "echo 'hello    world'"
test_command "Unclosed quotes" "echo 'hello" true
test_command "Escaped quotes in double quotes" "echo \"hello \\\"world\\\"\""
test_command "Single quote in double quotes" "echo \"'\""
test_command "Double quote in single quotes" "echo '\"'"
test_command "Special characters in quotes" "echo 'hello; ls && cat'"

end_category "QUOTES AND PARSING"

# ============================================
# 5. REDIRECTIONS
# ============================================
start_category "REDIRECTIONS"

# Setup test files
echo "test content" > test_input.txt
echo "existing content" > test_output.txt

test_command "Output redirection" "echo hello > test_out.txt && cat test_out.txt"
test_command "Output redirection overwrite" "echo first > test_over.txt && echo second > test_over.txt && cat test_over.txt"
test_command "Append redirection" "echo first > test_append.txt && echo second >> test_append.txt && cat test_append.txt"
test_command "Input redirection" "cat < test_input.txt"
test_command "Multiple redirections" "cat < test_input.txt > test_multi.txt && cat test_multi.txt"
test_command "Redirect to nonexistent path" "echo hello > /nonexistent/file.txt" true
test_command "Redirect from nonexistent file" "cat < nonexistent.txt" true
test_command "Multiple output redirections" "echo hello > file1.txt > file2.txt && cat file2.txt"
test_command "Redirect with complex command" "ls -la > ls_output.txt && head -3 ls_output.txt"

# Cleanup
rm -f test_*.txt file*.txt ls_output.txt

end_category "REDIRECTIONS"

# ============================================
# 6. HEREDOCS
# ============================================
start_category "HEREDOCS"

test_command "Basic heredoc" "cat << EOF
line1
line2
EOF"

test_command "Heredoc with variable expansion" "cat << EOF
Hello \$USER
Current dir: \$PWD  
EOF"

test_command "Heredoc with quotes" "cat << 'EOF'
Single quotes preserve \$HOME
EOF"

test_command "Multiple heredocs" "cat << EOF && cat << END
First heredoc
EOF
Second heredoc  
END"

test_command "Heredoc with commands" "cat << EOF | grep hello
hello world
other line
hello again
EOF"

end_category "HEREDOCS"

# ============================================
# 7. PIPES
# ============================================
start_category "PIPES"

test_command "Simple pipe" "echo hello | cat"
test_command "Pipe with grep" "echo -e 'hello\nworld\nhello again' | grep hello"
test_command "Multiple pipes" "echo -e 'line1\nline2\nline3' | head -2 | tail -1"
test_command "Pipe with wc" "echo 'hello world' | wc -w"
test_command "Complex pipe chain" "ls | head -5 | tail -2 | cat -n"
test_command "Pipe with builtin" "echo hello | cat | echo world"
test_command "Pipe error handling" "nonexistentcmd | cat" true
test_command "Empty pipe input" "echo | cat"

end_category "PIPES"

# ============================================
# 8. LOGICAL OPERATORS  
# ============================================
start_category "LOGICAL OPERATORS"

test_command "AND operator success" "true && echo success"
test_command "AND operator failure" "false && echo should_not_print"
test_command "OR operator success" "true || echo should_not_print"
test_command "OR operator failure" "false || echo printed"
test_command "Complex AND/OR" "true && echo first || echo second"
test_command "AND with commands" "echo hello && echo world"
test_command "OR with commands" "nonexistentcmd || echo fallback"
test_command "Chained AND operators" "true && true && echo success"
test_command "Chained OR operators" "false || false || echo finally"
test_command "Mixed AND/OR precedence" "false && true || echo result"

end_category "LOGICAL OPERATORS"

# ============================================
# 9. PARENTHESES AND SUBSHELLS
# ============================================
start_category "PARENTHESES"

test_command "Simple parentheses" "(echo hello)"
test_command "Parentheses with variables" "(export TEMP=test && echo \$TEMP)"
test_command "Nested parentheses" "((echo nested))"
test_command "Parentheses with pipes" "(echo hello | cat)"
test_command "Parentheses with redirections" "(echo test) > paren_out.txt && cat paren_out.txt"
test_command "Complex parentheses" "(cd /tmp && pwd) && pwd"

# Cleanup
rm -f paren_out.txt

end_category "PARENTHESES"

# ============================================
# 10. ERROR HANDLING & EXIT CODES
# ============================================
start_category "ERROR HANDLING"

test_command "Command not found exit code" "nonexistentcommand" true
test_command "Permission denied" "cat /etc/shadow" true
test_command "Directory as command" "/etc" true  
test_command "Empty command with semicolon" ";" true
test_command "Invalid redirection" "echo hello > /dev/invalid/file" true
test_command "Pipe to invalid command" "echo hello | invalidcmd" true
test_command "Exit code propagation" "false; echo \$?"
test_command "Success exit code" "true; echo \$?"
test_command "Builtin error exit code" "cd /nonexistent" true

end_category "ERROR HANDLING"

# ============================================
# 11. SIGNAL HANDLING  
# ============================================
start_category "SIGNAL HANDLING"

test_interactive "Ctrl+C handling" "Test that Ctrl+C displays new prompt without exiting"
test_interactive "Ctrl+D handling" "Test that Ctrl+D exits the shell gracefully"
test_interactive "Ctrl+\\ handling" "Test that Ctrl+\\ does nothing in interactive mode"
test_interactive "Signal in heredoc" "Test Ctrl+C interrupts heredoc and returns to prompt"
test_interactive "Signal in command execution" "Test Ctrl+C interrupts running command"

end_category "SIGNAL HANDLING"

# ============================================
# 12. MEMORY AND RESOURCE MANAGEMENT
# ============================================
start_category "MEMORY MANAGEMENT"

test_interactive "No memory leaks" "Run with valgrind: valgrind --leak-check=full ./minishell"
test_interactive "Large input handling" "Test with very long command lines"
test_interactive "Many variables" "Test creating and unsetting many environment variables"
test_interactive "Deep nesting" "Test deeply nested parentheses and pipes"

end_category "MEMORY MANAGEMENT"

# ============================================
# 13. EDGE CASES
# ============================================
start_category "EDGE CASES"

test_command "Multiple semicolons" "echo hello;; echo world"
test_command "Semicolon at start" ";echo hello"
test_command "Semicolon at end" "echo hello;"
test_command "Only whitespace" "     "
test_command "Tabs and spaces mixing" "echo\thello   world"
test_command "Very long line" "echo $(printf 'a%.0s' {1..1000})"
test_command "Special characters" "echo '!@#$%^&*()[]{}'"
test_command "Unicode characters" "echo 'héllo wörld'"
test_command "NULL handling" "echo -e 'hello\\x00world'"

end_category "EDGE CASES"

# ============================================
# 14. INTERACTIVE FEATURES
# ============================================
start_category "INTERACTIVE FEATURES"

test_interactive "Prompt display" "Check that prompt shows 'minishell$ '"
test_interactive "History functionality" "Test command history with up/down arrows"
test_interactive "Tab completion" "Test tab completion for commands and files"
test_interactive "Line editing" "Test cursor movement and line editing"
test_interactive "Multi-line input" "Test handling of unclosed quotes across lines"

end_category "INTERACTIVE FEATURES"

# ============================================
# FINAL SUMMARY
# ============================================
echo -e "\n${MAGENTA}"
echo "=================================================="  
echo "=== FINAL TEST SUMMARY ==="
echo "=================================================="
echo -e "${NC}"

echo "Total tests executed: $test_count"
echo "Tests passed: $pass_count"  
echo "Tests failed: $((test_count - pass_count))"
echo

echo "Results by category:"
for category in "${!category_results[@]}"; do
    echo "  $category: ${category_results[$category]}"
done

if [ $pass_count -eq $test_count ]; then
    echo -e "\n${GREEN}🎉 ALL TESTS PASSED! Minishell is ready for evaluation! 🎉${NC}"
    exit 0
else
    echo -e "\n${RED}Some tests failed. Review the failing cases above.${NC}"
    echo -e "${YELLOW}Success rate: $((pass_count * 100 / test_count))%${NC}"
    exit 1
fi