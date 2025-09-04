#!/bin/bash

# Test Suite FINAL para Minishell 42 - Solo parte obligatoria
# Manejo correcto de output sin prompts

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

test_count=0
pass_count=0

# Función para extraer solo el output del comando, sin prompts
extract_output() {
    local raw_output="$1"
    # Elimina prompts y líneas vacías, mantiene solo el output real
    echo "$raw_output" | sed '/^minishell\$ /d' | sed '/^minishell\$/d' | sed '/^$/d' | sed 's/^minishell: /bash: line 1: /'
}

test_command() {
    local test_name="$1"
    local command="$2"
    
    ((test_count++))
    echo -e "\n${YELLOW}Test $test_count: $test_name${NC}"
    echo "Comando: $command"
    
    # Bash output
    bash_output=$(bash -c "$command" 2>&1)
    
    # Minishell output
    minishell_raw=$(echo "$command" | timeout 5 ./minishell 2>&1)
    minishell_output=$(extract_output "$minishell_raw")
    
    # Comparar
    if [ "$bash_output" = "$minishell_output" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        ((pass_count++))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "  Esperado: '$bash_output'"
        echo "  Obtenido: '$minishell_output'"
    fi
}

echo -e "${CYAN}=== TEST MINISHELL 42 - PARTE OBLIGATORIA ===${NC}\n"

# COMANDOS BÁSICOS
echo -e "${CYAN}1. COMANDOS BÁSICOS${NC}"
test_command "Echo simple" "echo hello"
test_command "Echo con argumentos" "echo hello world"
test_command "Comando con path" "/bin/echo test"
test_command "Espacios múltiples" "echo    multiple     spaces"

# BUILTINS  
echo -e "\n${CYAN}2. COMANDOS BUILTIN${NC}"
test_command "PWD" "pwd"
test_command "Echo sin salto de línea" "echo -n hello"
test_command "Echo múltiple -n" "echo -nnn test"
test_command "Echo -n inválido" "echo -nX hello"

# VARIABLES
echo -e "\n${CYAN}3. VARIABLES DE ENTORNO${NC}"
test_command "Variable HOME" "echo \$HOME"
test_command "Variable inexistente" "echo \$NONEXISTENT_VAR"
test_command "Exit status" "echo \$?"

# COMILLAS
echo -e "\n${CYAN}4. MANEJO DE COMILLAS${NC}"
test_command "Comillas simples" "echo 'hello world'"
test_command "Comillas dobles" "echo \"hello world\""
test_command "Variable en comillas dobles" "echo \"\$HOME\""
test_command "Variable en comillas simples" "echo '\$HOME'"
test_command "Comilla simple en dobles" "echo \"'\""
test_command "Comilla doble en simples" "echo '\"'"

# REDIRECTIONS (casos simples)
echo -e "\n${CYAN}5. REDIRECTIONS BÁSICAS${NC}"
echo "test content" > test_file.txt
test_command "Input redirection" "cat < test_file.txt"
test_command "Output redirection" "echo hello > out.txt && cat out.txt"

# PIPES (casos simples)
echo -e "\n${CYAN}6. PIPES BÁSICOS${NC}"
test_command "Pipe simple" "echo hello | cat"
test_command "Echo a través de pipe" "echo -e 'line1\nline2' | head -1"

# HEREDOC (test básico)
echo -e "\n${CYAN}7. HEREDOC BÁSICO${NC}"
echo -e "\n${YELLOW}Test heredoc: cat << EOF${NC}"
echo "Verificar manualmente que funciona:"
echo "cat << EOF
Hello
World  
EOF" | ./minishell

# MANEJO DE ERRORES
echo -e "\n${CYAN}8. MANEJO DE ERRORES${NC}"
test_command "Comando inexistente" "nonexistentcommand"
test_command "Archivo inexistente" "cat nonexistent_file.txt"

# CLEANUP
rm -f test_file.txt out.txt

# RESUMEN
echo -e "\n${CYAN}=== RESUMEN ===${NC}"
echo "Total: $test_count tests"
echo "Pasados: $pass_count"
echo "Fallidos: $((test_count - pass_count))"
echo "Éxito: $((pass_count * 100 / test_count))%"

if [ $((pass_count * 100 / test_count)) -ge 80 ]; then
    echo -e "${GREEN}🎉 BIEN! Funcionalidad básica correcta${NC}"
else
    echo -e "${YELLOW}⚠️ Necesita ajustes${NC}"
fi

echo -e "\n${CYAN}RECORDAR:${NC}"
echo "- Este test cubre SOLO parte obligatoria"
echo "- NO incluye: &&, ||, wildcards, bonus"
echo "- Tests manuales: Ctrl+C, Ctrl+D, signals"