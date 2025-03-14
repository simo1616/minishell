/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:04:51 by jdecarro          #+#    #+#             */
/*   Updated: 2025/02/13 16:04:51 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie si une chaîne est numérique.
 * 
 * Renvoie 1 si la chaîne représente un nombre, 0 sinon.
 * 
 * @param str Chaîne à tester.
 * @return int 1 si numérique, 0 sinon.
 */
int	ft_is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[i])
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Traite le signe dans la chaîne.
 * 
 * Met à jour l'indice et le signe.
 * 
 * @param str Chaîne.
 * @param i Pointeur sur l'indice courant.
 * @param sign Pointeur sur le signe.
 * @return int Toujours 0.
 */
static int	check_sign(const char *str, int *i, int *sign)
{
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			*sign = -1;
		(*i)++;
	}
	return (0);
}

/**
 * @brief Vérifie un débordement.
 * 
 * Compare le résultat partiel et le chiffre courant.
 * 
 * @param result Résultat partiel.
 * @param digit Caractère chiffre.
 * @return int 1 en cas de débordement, 0 sinon.
 */
static int	check_overflow(long result, char digit)
{
	if ((result > LONG_MAX / 10)
		|| (result == LONG_MAX / 10 && (digit - '0') > LONG_MAX % 10))
		return (1);
	return (0);
}

/**
 * @brief Convertit une chaîne en long.
 * 
 * Traite le signe et les chiffres, vérifie le débordement.
 * 
 * @param str Chaîne à convertir.
 * @param error Pointeur sur l'erreur.
 * @return long Valeur convertie.
 */
long	ft_atol(const char *str, int *error)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	if (check_sign(str, &i, &sign) == -1)
	{
		*error = 1;
		return (0);
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]) || check_overflow(result, str[i]))
		{
			*error = 1;
			return (0);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

/**
 * @brief Affiche une erreur pour exit.
 * 
 * Affiche l'argument et le message d'erreur.
 * 
 * @param arg Argument problématique.
 * @param msg Message d'erreur.
 */
void	print_exit_error(char *arg, char *msg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}
