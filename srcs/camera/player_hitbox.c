/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_hitbox.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roduquen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 13:31:51 by roduquen          #+#    #+#             */
/*   Updated: 2019/12/04 20:16:53 by roduquen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "player.h"

void		check_if_inside_map(t_doom *data, t_vec3d *position)
{
	if (position->x > 63.8)
	{
		data->player.acceleration.x = 0;
		position->x = 63.8;
	}
	else if (position->x < 0.2)
	{
		data->player.acceleration.x = 0;
		position->x = 0.2;
	}
	if (position->y > 63.8)
	{
		data->player.acceleration.y = 0;
		position->y = 63.8;
	}
	else if (position->y < 1.5)
	{
		data->player.acceleration.y = 0;
		position->y = 1.5;
	}
	if (position->z > 63.8)
	{
		data->player.acceleration.z = 0;
		position->z = 63.8;
	}
	else if (position->z < 0.2)
	{
		data->player.acceleration.z = 0;
		position->z = 0.2;
	}
}

void			create_hitbox_offset(t_hitbox hitbox[12], t_vec3d origin)
{
	hitbox[0].vertex = vec3d_add(origin, vec3d(-0.2, 0.2, -0.2));
	hitbox[1].vertex = vec3d_add(origin, vec3d(-0.2, 0.2, 0.2));
	hitbox[2].vertex = vec3d_add(origin, vec3d(-0.2, -0.65, -0.2));
	hitbox[3].vertex = vec3d_add(origin, vec3d(-0.2, -0.65, 0.2));
	hitbox[4].vertex = vec3d_add(origin, vec3d(-0.2, -1.5, -0.2));
	hitbox[5].vertex = vec3d_add(origin, vec3d(-0.2, -1.5, 0.2));
	hitbox[6].vertex = vec3d_add(origin, vec3d(0.2, -1.5, 0.2));
	hitbox[7].vertex = vec3d_add(origin, vec3d(0.2, -1.5, -0.2));
	hitbox[8].vertex = vec3d_add(origin, vec3d(0.2, -0.65, 0.2));
	hitbox[9].vertex = vec3d_add(origin, vec3d(0.2, -0.65, -0.2));
	hitbox[10].vertex = vec3d_add(origin, vec3d(0.2, 0.2, 0.2));
	hitbox[11].vertex = vec3d_add(origin, vec3d(0.2, 0.2, -0.2));
}

void			add_clipping_for_each_point(t_doom *data, t_player *player)
{
	int		i;
	t_vec3d	new_pos;
	t_vec3d	new_acceleration;
	t_vec3d	new_offset;
	t_hitbox	hit[12];

	i = 0;
	camera_event_translate(data);
//	printf("start = data->player.camera.origin = [%.2f, %.2f, %.2f]\n", data->player.camera.origin.x, data->player.camera.origin.y, data->player.camera.origin.z);
	new_pos = vec3d_add(data->player.camera.origin, data->player.acceleration);
//	printf("i = %d || new_pos = [%.2f, %.2f, %.2f]\n", i, new_pos.x, new_pos.y, new_pos.z);
	check_if_inside_map(data, &new_pos);
//	printf("end = new_pos = [%.2f, %.2f, %.2f]\n", new_pos.x, new_pos.y, new_pos.z);
	new_acceleration = player->acceleration;
	create_hitbox_offset(player->hitbox, new_pos);
	create_hitbox_offset(hit, data->player.camera.origin);
	while (i < 6)
	{
		new_offset = player->hitbox[i].vertex;
		if (add_vertex_clipping_x_min(&new_acceleration, &new_offset, data->map_to_save
			, hit[i].vertex))
		{
			new_pos.x = data->player.camera.origin.x;
			create_hitbox_offset(player->hitbox, new_pos);
		}
		if (i % 2 == 1)
		{
			new_offset = player->hitbox[i].vertex;
			if (add_vertex_clipping_z_max(&new_acceleration, &new_offset, data->map_to_save
						, hit[i].vertex))
			{
				new_pos.z = data->player.camera.origin.z;
				create_hitbox_offset(player->hitbox, new_pos);
			}
		}
		else
		{
			new_offset = player->hitbox[i].vertex;
			if (add_vertex_clipping_z_min(&new_acceleration, &new_offset, data->map_to_save
						, hit[i].vertex))
			{
				new_pos.z = data->player.camera.origin.z;
				create_hitbox_offset(player->hitbox, new_pos);
			}
		}
		i++;
	}
	while (i < 12)
	{
		new_offset = player->hitbox[i].vertex;
		if (add_vertex_clipping_x_max(&new_acceleration, &new_offset, data->map_to_save
					, hit[i].vertex))
		{
			new_pos.x = data->player.camera.origin.x;
			create_hitbox_offset(player->hitbox, new_pos);
		}
		if (i % 2 == 0)
		{
			new_offset = player->hitbox[i].vertex;
			if (add_vertex_clipping_z_max(&new_acceleration, &new_offset, data->map_to_save
						, hit[i].vertex))
			{
				new_pos.z = data->player.camera.origin.z;
				create_hitbox_offset(player->hitbox, new_pos);
			}
		}
		else
		{
			new_offset = player->hitbox[i].vertex;
			if (add_vertex_clipping_z_min(&new_acceleration, &new_offset, data->map_to_save
						, hit[i].vertex))
			{
				new_pos.z = data->player.camera.origin.z;
				create_hitbox_offset(player->hitbox, new_pos);
			}
		}
		i++;
	}
	i = 4;
	while (i < 8)
	{
		new_offset = player->hitbox[i].vertex;
		if (add_vertex_clipping_y_min(&new_acceleration, &new_offset, data->map_to_save
					, hit[i].vertex))
		{
			new_pos.y = data->player.camera.origin.y;
			create_hitbox_offset(player->hitbox, new_pos);
		}
		i++;
	}

	data->player.physics.acceleration = new_acceleration;
	data->player.physics.origin = new_pos;
}
