# Minishell

Una implementación completa de un intérprete de comandos (shell) en C que replica las funcionalidades básicas de bash, desarrollado como proyecto de la escuela 42.

## 📖 Descripción

Minishell es un shell funcional que maneja la interpretación y ejecución de comandos, gestión de variables de entorno, redirecciones, pipes y comandos built-in. El proyecto está diseñado para comprender profundamente el funcionamiento interno de un shell Unix.

## ✨ Características

### Funcionalidades Principales
- **Prompt interactivo** con historial de comandos (usando readline)
- **Búsqueda de ejecutables** en PATH y rutas absolutas/relativas
- **Gestión de señales** (Ctrl+C, Ctrl+D, Ctrl+\)
- **Variables de entorno** con expansión automática
- **Gestión de códigos de salida** (`$?`)

### Redirecciones
- `<` Redirección de entrada
- `>` Redirección de salida
- `>>` Redirección de salida en modo append
- `<<` Here documents (heredoc)

### Pipes
- Soporte completo para pipes (`|`) entre comandos
- Manejo de pipelines complejos

### Built-ins Implementados
- `echo` con opción `-n`
- `cd` con rutas relativas y absolutas
- `pwd` - directorio actual
- `export` - establecer variables de entorno
- `unset` - eliminar variables de entorno
- `env` - mostrar variables de entorno
- `exit` - salir del shell

### Expansión de Variables
- Variables de entorno (`$VAR`)
- Código de salida del último comando (`$?`)
- PID del shell (`$$`)
- Manejo de comillas simples y dobles

## 🏗️ Arquitectura

El proyecto está organizado en módulos bien definidos:

```
minishell/
├── execution/          # Sistema de ejecución
│   ├── core/          # Núcleo del shell
│   ├── builtins/      # Comandos built-in
│   ├── exec_system/   # Sistema de ejecución de comandos
│   ├── pipe_system/   # Manejo de pipes
│   └── redirection/   # Sistema de redirecciones
├── parsing/           # Análisis sintáctico
│   ├── lexer/        # Tokenización
│   └── tokens/       # Procesamiento de tokens
├── libft/            # Biblioteca de funciones auxiliares
└── Makefile
```

## 🚀 Compilación y Uso

### Requisitos
- GCC o Clang
- Make
- Biblioteca readline (`apt-get install libreadline-dev` en Ubuntu/Debian)

### Compilación
```bash
make
```

### Ejecución
```bash
./minishell
```

### Comandos adicionales
```bash
make clean      # Limpiar archivos objeto
make fclean     # Limpieza completa
make re         # Recompilar desde cero
make run        # Compilar y ejecutar con animación
make start      # Alias para run
```

## 💡 Ejemplos de Uso

### Comandos básicos
```bash
minishell$ echo "Hola mundo"
Hola mundo

minishell$ pwd
/home/usuario/minishell

minishell$ ls -la | grep minishell
-rwxr-xr-x 1 user user 125432 Sep 21 10:30 minishell
```

### Redirecciones
```bash
minishell$ echo "contenido" > archivo.txt
minishell$ cat < archivo.txt
contenido

minishell$ echo "más contenido" >> archivo.txt
minishell$ cat archivo.txt
contenido
más contenido
```

### Here documents
```bash
minishell$ cat << EOF
> Primera línea
> Segunda línea
> EOF
Primera línea
Segunda línea
```

### Variables de entorno
```bash
minishell$ export MI_VAR="valor"
minishell$ echo $MI_VAR
valor

minishell$ echo $?
0
```

### Pipes complejos
```bash
minishell$ ls | grep .c | wc -l
42
```

## 🛠️ Implementación Técnica

### Procesamiento de Comandos
1. **Lexer**: Tokenización de la entrada
2. **Parser**: Análisis sintáctico y construcción del AST
3. **Expansor**: Expansión de variables y wildcards
4. **Ejecutor**: Ejecución de comandos y manejo de pipes

### Gestión de Procesos
- Fork y exec para comandos externos
- Manejo de pipes con `pipe()` y `dup2()`
- Gestión de señales con `signal()` y `sigaction()`
- Control de procesos hijo con `wait()` y `waitpid()`

### Gestión de Memoria
- Liberación automática de memoria
- Manejo de errores y limpieza en caso de fallo
- Uso de valgrind para verificar memory leaks

## 🔍 Características Avanzadas

### Manejo de Comillas
```bash
minishell$ echo 'texto con $VAR'    # No expansión
texto con $VAR

minishell$ echo "texto con $VAR"    # Expansión
texto con valor
```

### Heredocs con Expansión
```bash
minishell$ cat << "EOF"             # Sin expansión
> $HOME
> EOF
$HOME

minishell$ cat << EOF               # Con expansión
> $HOME
> EOF
/home/usuario
```

### Códigos de Error
```bash
minishell$ false
minishell$ echo $?
1

minishell$ comando_inexistente
comando_inexistente: command not found
minishell$ echo $?
127
```

## 🧪 Testing

### Casos de Prueba
- Comandos simples y complejos
- Pipes múltiples
- Redirecciones combinadas
- Variables de entorno
- Manejo de errores
- Señales

### Herramientas de Testing
```bash
# Verificar memory leaks
valgrind --leak-check=full ./minishell

# Comparar con bash
bash -c "comando" > bash_output.txt
./minishell -c "comando" > mini_output.txt
diff bash_output.txt mini_output.txt
```

## 📚 Referencias

- [Manual de Bash](https://www.gnu.org/software/bash/manual/)
- [Advanced Programming in the UNIX Environment](https://www.apue.org/)
- [The Linux Programming Interface](http://man7.org/tlpi/)


## 👥 Autores

- **rmakende**
- **ruortiz-**

---
