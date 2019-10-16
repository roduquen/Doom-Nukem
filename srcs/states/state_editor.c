/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roduquen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 14:12:25 by roduquen          #+#    #+#             */
/*   Updated: 2019/10/16 16:55:32 by roduquen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>
#include "vec3.h"

static inline void	color_rectangle(t_doom *data, t_vec3l rectangle, int step)
{
	int			i;
	int			j;

	i = 0;
	while (i < BLOCK_SIZE_EDITOR)
	{
		j = 0;
		while (j < BLOCK_SIZE_EDITOR)
		{
			data->lib.image[(i + rectangle.x) * WIDTH + rectangle.y + j++] =
				rectangle.z;
		}
		i++;
	}
	if (rectangle.z)
	{
		data->map_to_save[step][rectangle.x / BLOCK_SIZE_EDITOR]
			[rectangle.y / BLOCK_SIZE_EDITOR] = 0x61616161;
	}
	else
	{
		data->map_to_save[step][rectangle.x / BLOCK_SIZE_EDITOR]
			[rectangle.y / BLOCK_SIZE_EDITOR] = 0;
	}
}

static inline void	set_quadrillage(t_doom *data, int step)
{
	int			i;
	int			j;

	i = 0;
	while (i < SIZE_MAP)
	{
		j = 0;
		while (j < SIZE_MAP)
		{
			if (data->map_to_save[step][i][j])
			{
				color_rectangle(data, (t_vec3l){i * BLOCK_SIZE_EDITOR
					, j * BLOCK_SIZE_EDITOR, 0xFFFFFF}, step);
			}
			else
			{
				color_rectangle(data, (t_vec3l){i * BLOCK_SIZE_EDITOR
					, j * BLOCK_SIZE_EDITOR, 0}, step);
			}
			j++;
		}
		i++;
	}
}

static inline void	mouse_editor_commands(t_doom *data, int *ok, int step)
{
	if (data->lib.event.button.x >= 1024)
	{
	}
	else if (data->lib.event.button.x >= 0 && data->lib.event.button.y < 1024
		&& data->lib.event.button.y >= 0)
	{
		*ok = 1;
		color_rectangle(data, (t_vec3l){data->lib.event.button.y
			- data->lib.event.button.y % BLOCK_SIZE_EDITOR
			, data->lib.event.button.x - data->lib.event.button.x
			% BLOCK_SIZE_EDITOR, 0xFFFFFF}, step);
	}
}

static inline void	save_map_to_file(t_doom *data, char *str)
{
	int			fd;

	if ((fd = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0777)) != -1)
	{
		write(fd, data->map_to_save, SIZE_MAP * 4 * SIZE_MAP * SIZE_MAP);
		close(fd);
	}
}

static inline int	parse_file(t_doom *data, char *str, int step)
{
	int		fd;
	int		ret;
	char	str_to_map[SIZE_MAP * SIZE_MAP * 4 * SIZE_MAP];

	if ((fd = open(str, O_RDONLY)) != -1)
	{
		ret = read(fd, str_to_map, SIZE_MAP * SIZE_MAP * SIZE_MAP * 4);
		if (ret != SIZE_MAP * SIZE_MAP * SIZE_MAP * 4)
			return (1);
		ft_memcpy(data->map_to_save, str_to_map, SIZE_MAP * SIZE_MAP * SIZE_MAP
			* 4);
		set_quadrillage(data, step);
		close(fd);
	}
	else
	{
		ft_memset(data->lib.image, 0, HEIGHT * WIDTH * 4);
		ft_memset(data->map_to_save, 0, SIZE_MAP * SIZE_MAP * SIZE_MAP * 4);
	}
	return (0);
}

static inline void	keydown_editor_commands(t_doom *data, char *str, int *map)
{
	if (data->lib.event.key.keysym.sym == SDLK_UP)
	{
		if (*map < SIZE_MAP - 1)
		{
			save_map_to_file(data, str);
			(*map)++;
		}
	}
	if (data->lib.event.key.keysym.sym == SDLK_DOWN)
	{
		if (*map > 0)
		{
			save_map_to_file(data, str);
			(*map)--;
		}
	}
}

static inline void	editor_commands(t_doom *data, char str[50], int *map)
{
	static int		ok = 0;

	if (data->lib.event.type == SDL_MOUSEBUTTONDOWN)
		mouse_editor_commands(data, &ok, *map);
	if (data->lib.event.type == SDL_MOUSEBUTTONUP)
		ok = 0;
	if (ok == 1)
	{
		if (data->lib.event.button.y < 1024 && data->lib.event.button.x < 1024
			&& data->lib.event.button.y >= 0 && data->lib.event.button.x >= 0)
			color_rectangle(data, (t_vec3l){data->lib.event.button.y
				- data->lib.event.button.y % BLOCK_SIZE_EDITOR
				, data->lib.event.button.x - data->lib.event.button.x
				% BLOCK_SIZE_EDITOR, 0xFFFFFF}, *map);
	}
	else if (data->lib.event.type == SDL_KEYDOWN)
		keydown_editor_commands(data, str, map);
}

int					state_editor(t_doom *data)
{
	static int		first = 0;
	static int		map = 0;
	static char		*str = "MAP_CREATOR";

	if (!first)
	{
		ft_memset(data->lib.image, 0, HEIGHT * WIDTH * 4);
		parse_file(data, str, map);
		first++;
	}
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_ShowCursor(SDL_TRUE);
	set_quadrillage(data, map);
	while (SDL_PollEvent(&data->lib.event))
		editor_commands(data, str, &map);
	SDL_RenderCopy(data->lib.renderer, data->lib.texture, NULL, NULL);
	SDL_RenderPresent(data->lib.renderer);
	return (0);
}
