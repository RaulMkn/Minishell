/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/06 19:34:39 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Encuentra la última redirección de entrada
** En bash: cat <file1 <file2 <file3 → solo file3 se usa como entrada
*/
static t_redir	*find_last_input_redirection(t_redir *redirs)
{
	t_redir	*last_input;
	t_redir	*current;

	last_input = NULL;
	current = redirs;
	
	while (current)
	{
		if (current->type == REDIR_IN)
			last_input = current;
		current = current->next;
	}
	
	return (last_input);
}

/*
** Encuentra la última redirección de salida (normal o append)
** En bash: echo hi >file1 >file2 >>file3 → solo file3 se usa (append mode)
*/
static t_redir *find_last_output_redirection(t_redir *redirs)
{
    t_redir *first_output;  // 🔧 CAMBIO: Buscar el PRIMERO
    t_redir *current;

    first_output = NULL;    // 🔧 CAMBIO: Nombre actualizado
    current = redirs;
    
    while (current)
    {
        if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
        {
            if (first_output == NULL)  // 🔧 CAMBIO: Solo el PRIMERO
                first_output = current;
        }
        current = current->next;
    }
    
    return (first_output);  // 🔧 CAMBIO: Devolver el primero (que es el último parseado)
}

/*
** Aplica una redirección específica y maneja errores
*/
static int	apply_single_redirection(t_redir *redir)
{
	int	fd;

	if (!redir)
		return (1);

	if (redir->type == REDIR_IN)
	{
		fd = open(redir->file, O_RDONLY);
		if (fd == -1)
		{
			perror(redir->file);
			return (0);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (redir->type == REDIR_OUT)
	{
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror(redir->file);
			return (0);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (redir->type == REDIR_APPEND)
	{
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			perror(redir->file);
			return (0);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	
	return (1);
}

// /* /*
// ** Procesa redirecciones intermedias para verificar errores
// ** En bash: ls >file1 >invalid_permission >file3
// ** - Se intenta abrir file1 (éxito)
// ** - Se intenta abrir invalid_permission (falla → error)
// ** - No se llega a procesar file3
// */
// static int process_intermediate_redirections(t_redir *redirs, t_redir *last_input, t_redir *last_output)
// {
//     t_redir *current;
//     int     fd;

//     current = redirs;
    
//     while (current)
//     {
//         // Saltar las redirecciones que serán aplicadas finalmente
//         if (current == last_input || current == last_output)
//         {
//             current = current->next;
//             continue;
//         }
        
//         // 🔧 SOLO VALIDAR, NO APLICAR
//         if (current->type == REDIR_IN)
//         {
//             // Solo verificar si existe y es legible
//             if (access(current->file, R_OK) != 0)
//             {
//                 perror(current->file);
//                 return (0);
//             }
//         }
//         else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
//         {
//             // Solo verificar si se puede escribir (sin truncar)
//             if (access(current->file, F_OK) == 0)
//             {
//                 // Archivo existe, verificar permisos de escritura
//                 if (access(current->file, W_OK) != 0)
//                 {
//                     perror(current->file);
//                     return (0);
//                 }
//             }
//             else
//             {
//                 // Archivo no existe, verificar si se puede crear
//                 fd = open(current->file, O_WRONLY | O_CREAT | O_EXCL, 0644);
//                 if (fd == -1)
//                 {
//                     perror(current->file);
//                     return (0);
//                 }
//                 close(fd);
//                 unlink(current->file); // Eliminar archivo temporal
//             }
//         }
        
//         current = current->next;
//     }
    
//     return (1);
// } */
// 🔧 NUEVA FUNCIÓN DE VALIDACIÓN SIN TRUNCAR:
static int validate_and_create_all_redirections(t_redir *redirs, t_redir *last_output)
{
    t_redir *current = redirs;
    int fd;
    
    while (current)
    {
        if (current->type == REDIR_IN)
        {
            if (access(current->file, R_OK) != 0)
            {
                perror(current->file);
                return (0);
            }
        }
        else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
        {
            // 🔧 CREAR TODOS LOS ARCHIVOS, no solo el último
            if (current != last_output)
            {
                // Archivo intermedio: crear vacío
                fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1)
                {
                    perror(current->file);
                    return (0);
                }
                close(fd);
            }
            // El último archivo se maneja en apply_single_redirection
        }
        current = current->next;
    }
    
    return (1);
}

int handle_multiple_redirections(t_redir *redirs)
{
    t_redir *last_input;
    t_redir *last_output;

    if (!redirs)
        return (1);

    // Encontrar las redirecciones efectivas
    last_input = find_last_input_redirection(redirs);
    last_output = find_last_output_redirection(redirs);

    // 🔧 CREAR todos los archivos y validar (sin last_input)
    if (!validate_and_create_all_redirections(redirs, last_output))
        return (0);

    // Aplicar solo las redirecciones efectivas
    if (last_input && !apply_single_redirection(last_input))
        return (0);

    if (last_output && !apply_single_redirection(last_output))
        return (0);

    return (1);
}