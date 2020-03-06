/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_game.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacuvill <dacuvill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 16:44:31 by dacuvill          #+#    #+#             */
/*   Updated: 2020/03/06 16:44:48 by dacuvill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "player.h"
#include "graphic_lib.h"
#include "libft.h"
#include "octree.h"
#include "mixer.h"
#include "gameplay.h"
#include <SDL.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

static void	add_hud(t_doom *data)
{
	int			i;

	i = 0;
	while (i < WIDTH * HEIGHT)
	{
		if (data->lib.hud_texture[i] != 7929855
			&& data->lib.hud_texture[i] != 0xff00ffff)
			data->lib.image[i] = (data->lib.hud_texture[i] & 0xff000000)
			+ ((data->lib.hud_texture[i] & 0xff) << 16)
			+ (data->lib.hud_texture[i] & 0xff00)
			+ ((data->lib.hud_texture[i] & 0xff0000) >> 16);
		i++;
	}
}

int			state_game(t_doom *data)
{
	struct timeval	time;
	long			wait;
	long			sleep;

	gettimeofday(&time, NULL);
	wait = time.tv_sec * 1000000 + time.tv_usec;
	raytracing(data);
	add_hud(data);
	put_health_bar(data);
	game_sounds(data, &data->player);
	minimap(data->map_to_save, &data->player, &data->lib);
	display_inventory(&data->lib, &data->player, PLAYING);
	move_ennemies(data, &data->player, &data->enemies);
	if (data->photo)
	{
		data->photo = 0;
		convert_to_ppm(data->lib.image);
	}
	SDL_RenderCopy(data->lib.renderer, data->lib.texture, NULL, NULL);
	check_pos_player(data, data->map_to_save, data->player.camera.origin);
	SDL_RenderPresent(data->lib.renderer);
	SDL_RenderClear(data->lib.renderer);
	if (data->player.health <= 0)
		switch_state(data, PLAYING, DEATH);
	gettimeofday(&time, NULL);
	if ((sleep = time.tv_sec * 1000000 + time.tv_usec - wait) < 50000)
		usleep(50000 - sleep);
	return (0);
}
