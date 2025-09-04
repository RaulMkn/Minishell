#!/bin/bash

echo "=== TEST DEBUG HEREDOCS ==="

# Test 1: Heredoc básico
echo -e "\n1. Test heredoc básico:"
echo "Comando: cat << EOF"
echo "Expected (bash):"
cat << EOF
Hello World
This is a test
EOF

echo -e "\nActual (minishell):"
echo "cat << EOF
Hello World  
This is a test
EOF" | ./minishell

# Test 2: Heredoc con variables
echo -e "\n2. Test heredoc con variables:"
echo "Expected (bash):"
cat << EOF
User: $USER
Home: $HOME
EOF

echo -e "\nActual (minishell):"
echo "cat << EOF
User: \$USER
Home: \$HOME
EOF" | ./minishell

# Test 3: Heredoc con comillas
echo -e "\n3. Test heredoc sin expansión:"
echo "Expected (bash):" 
cat << 'EOF'
This should not expand: $HOME
EOF

echo -e "\nActual (minishell):"
echo "cat << 'EOF'
This should not expand: \$HOME
EOF" | ./minishell

# Test 4: Heredoc múltiples
echo -e "\n4. Test heredoc simple interactivo:"
echo "Probemos directamente en minishell:"
echo "Escribe: cat << END"
echo "Luego: hello world"  
echo "Luego: END"