#!/bin/bash

echo "=== DEBUG HEREDOC SIMPLE ==="

# Test manual paso a paso
echo "1. Test con bash:"
bash << 'SCRIPT'
cat << END
line 1
line 2
END
SCRIPT

echo -e "\n2. Test con minishell usando archivo temporal:"
cat > temp_heredoc.txt << 'INPUT'
cat << END
line 1  
line 2
END
INPUT

echo "Contenido del archivo:"
cat temp_heredoc.txt

echo -e "\nEjecutando en minishell:"
./minishell < temp_heredoc.txt

rm temp_heredoc.txt