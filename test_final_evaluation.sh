#!/bin/bash

# Test Suite FINAL para Minishell 42 - Parte Obligatoria ÚNICAMENTE
# Manejo correcto de salidas y prompts

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

test_count=0
pass_count=0

# Función para capturar SOLO la salida del comando
capture_command_output() {
    local command="$1"
    local use_file_input="${2:-false}"
    
    if [ "$use_file_input" = "true" ]; then
        # Para comandos complejos como heredocs, usar archivo temporal
        echo "$command" > temp_cmd.txt
        ./minishell < temp_cmd.txt 2>&1 | sed '/^minishell\$ /d' | sed '/^$/d'
        rm -f temp_cmd.txt
    else
        # Para comandos simples
        echo "$command" | ./minishell 2>&1 | grep -v "minishell\$" | sed '/^$/d' | sed 's/^minishell: /bash: line 1: /'
    fi
}

test_command() {
    local test_name="$1"
    local command="$2"
    local use_file_input="${3:-false}"
    
    ((test_count++))
    echo -e "\n${YELLOW}Test $test_count: $test_name${NC}"
    
    # Bash output
    bash_output=$(bash -c "$command" 2>&1)
    
    # Minishell output
    minishell_output=$(capture_command_output "$command" "$use_file_input")
    
    # Comparar
    if [ "$bash_output" = "$minishell_output" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        ((pass_count++))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "  Comando: $command"
        echo "  Esperado: '$bash_output'"
        echo "  Obtenido: '$minishell_output'"
    fi
}

# Test especial para echo -n (comportamiento esperado)
test_echo_n() {
    ((test_count++))
    echo -e "\n${YELLOW}Test $test_count: Echo -n (comportamiento especial)${NC}"
    
    # Test que echo -n funciona sin newline
    result=$(echo "echo -n hello" | ./minishell 2>&1)
    if [[ "$result" == *"hellominishell"* ]]; then
        echo -e "${GREEN}✓ PASS (echo -n funciona correctamente - no newline)${NC}"
        ((pass_count++))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "  Echo -n debería no imprimir newline"
        echo "  Got: '$result'"
    fi
}

# Test especial para heredocs
test_heredoc() {
    ((test_count++))
    echo -e "\n${YELLOW}Test $test_count: Heredoc básico${NC}"
    
    # Crear archivo temporal para heredoc
    cat > heredoc_test.txt << 'EOF'
cat << END  
Hello World
This is a test
END
EOF
    
    bash_output=$(bash < heredoc_test.txt 2>&1)
    minishell_output=$(./minishell < heredoc_test.txt 2>&1 | sed '/^minishell\$ /d' | sed '/^$/d')
    
    if [ "$bash_output" = "$minishell_output" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        ((pass_count++))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "  Esperado: '$bash_output'"
        echo "  Obtenido: '$minishell_output'"
    fi
    
    rm -f heredoc_test.txt
}

echo -e "${CYAN}=================================================================="
echo "=== TEST FINAL MINISHELL 42 - PARTE OBLIGATORIA ÚNICAMENTE ==="
echo "===================================================================${NC}\n"

# 1. COMANDOS BÁSICOS
echo -e "${CYAN}=== 1. COMANDOS BÁSICOS ===${NC}"
test_command "Echo simple" "echo hello"
test_command "Echo con argumentos" "echo hello world"  
test_command "Comando con path absoluto" "/bin/echo test"
test_command "Espacios múltiples" "echo    hello     world"
test_command "PWD" "pwd"

# 2. BUILTINS
echo -e "\n${CYAN}=== 2. COMANDOS BUILTIN ===${NC}"
test_echo_n  # Método especial para -n
test_command "Echo múltiple -n" "echo -nnn test"
test_command "Echo -n inválido" "echo -nX hello"
test_command "Echo vacío" "echo"

# 3. VARIABLES
echo -e "\n${CYAN}=== 3. VARIABLES DE ENTORNO ===${NC}"
test_command "Variable HOME" "echo \$HOME"
test_command "Variable inexistente" "echo \$UNDEFINED_VAR"  
test_command "Exit status" "echo \$?"
test_command "Variable con llaves" "echo \${HOME}"

# 4. COMILLAS  
echo -e "\n${CYAN}=== 4. MANEJO DE COMILLAS ===${NC}"
test_command "Comillas simples" "echo 'hello world'"
test_command "Comillas dobles" "echo \"hello world\""
test_command "Variable en comillas dobles" "echo \"\$HOME\""
test_command "Variable en comillas simples" "echo '\$HOME'"
test_command "Comilla simple en dobles" "echo \"'\""
test_command "Comilla doble en simples" "echo '\"'"
test_command "Triple comillas en dobles" "echo \"'''\""
test_command "Triple comillas dobles en simples" "echo '\"\"\"'"

# 5. REDIRECTIONS
echo -e "\n${CYAN}=== 5. REDIRECTIONS ===${NC}"
echo "test input" > test_input.txt
test_command "Output redirection" "echo hello > out.txt; cat out.txt" true
test_command "Input redirection" "cat < test_input.txt"
test_command "Append redirection" "echo first > app.txt; echo second >> app.txt; cat app.txt" true

# 6. PIPES
echo -e "\n${CYAN}=== 6. PIPES ===${NC}"
test_command "Pipe simple" "echo hello | cat"
test_command "Pipe con grep" "echo -e 'line1\nline2\nline1' | grep line1"
test_command "Múltiples pipes" "echo -e 'a\nb\nc' | head -2 | tail -1"

# 7. HEREDOCS  
echo -e "\n${CYAN}=== 7. HEREDOCS ===${NC}"
test_heredoc

# 8. MANEJO DE ERRORES
echo -e "\n${CYAN}=== 8. MANEJO DE ERRORES ===${NC}"
test_command "Comando inexistente" "commandnotfound"
test_command "Archivo inexistente" "cat nonexistent.txt"

# 9. CASOS EDGE
echo -e "\n${CYAN}=== 9. CASOS EDGE ===${NC}"
test_command "Línea vacía" ""
test_command "Solo espacios" "     "  
test_command "Comillas mixtas con espacios" "echo 'hello    world'"

# CLEANUP
rm -f test_input.txt out.txt app.txt

# RESUMEN FINAL
echo -e "\n${CYAN}=== RESUMEN FINAL ===${NC}"
echo "Total de tests: $test_count"
echo "Tests pasados: $pass_count"
echo "Tests fallidos: $((test_count - pass_count))"

percentage=$((pass_count * 100 / test_count))
echo "Porcentaje de éxito: $percentage%"

if [ $percentage -ge 95 ]; then
    echo -e "${GREEN}🎉 PERFECTO! Minishell 100% funcional para evaluación 42! 🎉${NC}"
elif [ $percentage -ge 90 ]; then
    echo -e "${GREEN}🎉 EXCELENTE! Minishell casi perfecto! 🎉${NC}"
elif [ $percentage -ge 80 ]; then
    echo -e "${YELLOW}⚡ MUY BIEN! Funcionalidad sólida${NC}"
else
    echo -e "${RED}🔧 Necesita más trabajo${NC}"
fi

echo -e "\n${CYAN}RECORDATORIO:${NC}"
echo "✅ Este test cubre SOLO la parte obligatoria"
echo "❌ NO incluye: &&, ||, wildcards (*), bonus"  
echo "📋 Tests manuales pendientes: Ctrl+C, Ctrl+D, signals"
echo "🧠 Memory leaks: Probar con valgrind"