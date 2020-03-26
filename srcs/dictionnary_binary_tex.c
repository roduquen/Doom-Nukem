/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dictionnary_binary_tex.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacuvill <dacuvill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 13:41:11 by roduquen          #+#    #+#             */
/*   Updated: 2020/03/01 22:15:44 by roduquen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include <stdio.h>

void			dictionnary_binary_models(t_doom *data)
{
	int			i;

	i = 0;
	while (i < NBR_TEXTURES_OBJECTS)
	{
		sprintf(data->dic_obj[i]
			, "ressources/models/%d.binary", i);
		i++;
	}
}
