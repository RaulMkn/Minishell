/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   example_integration.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/28 22:29:24 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Función mejorada que reemplaza init_new_command()
** Usa el parser unificado para manejo correcto de redirecciones mixtas
*/
static t_command	*init_new_command_improved(t_token **curr_token)
{
	t_command	*new_cmd;

	// Usar el parser unificado en lugar del parser separado
	new_cmd = parse_command_unified(curr_token);
	if (!new_cmd)
		return (NULL);
	
	// Validar redirecciones antes de continuar
	if (!validate_redirections(new_cmd->redir))
	{
		// Limpiar y retornar error
		clear_command_single(new_cmd);
		return (NULL);
	}
	
	return (new_cmd);
}

/*
** Función mejorada de ejecución que incluye validación previa
*/
int	execute_pipeline_improved(t_command *cmd_list, char ***mini_env)
{
	// Validar todas las redirecciones del pipeline antes de ejecutar
	if (!validate_pipeline_redirections(cmd_list))
		return (1);  // Error en redirecciones
	
	// Proceder con ejecución normal
	return (execute_pipeline(cmd_list, mini_env));
}

/*
** Ejemplo de cómo manejar casos específicos que estaban fallando
*/
void	example_test_cases(void)
{
}
