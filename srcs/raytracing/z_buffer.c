#include "entities.h"

static double	distance(t_vec3d a, t_vec3d b)
{
	return (vec3d_length2(vec3d_sub(a, b)));
}


static t_vec3d	calculate_position(t_entity entity, t_camera camera)
{
	t_vec3d dir;
	t_vec3d fr;
	t_vec3d up;
	t_vec3d rg;
	t_vec3d pos;

	dir = vec3d_sub(camera.origin, entity.pos);
	fr = camera.direction;
	up = camera.up;
	rg = camera.right;
	pos = (t_vec3d){(int)(dir.x * (up.y * rg.z - rg.y * up.z)
					+ dir.y * (rg.x * up.z - up.x * rg.z)
					+ dir.z * (up.x * rg.y - rg.x * up.y)),
					(int)(dir.x * (rg.y * fr.z - rg.z * fr.y)
					+ dir.y * (fr.x * rg.z - fr.z * rg.x)
					+ dir.z * (rg.x * fr.y - rg.y * fr.x)), 0};
	return (pos);
}

void		create_entity(t_entity *entities, t_vec3d pos, SDL_Surface *texture)
{
	t_entity *entity;

	if (!(entity = (t_entity*)ft_memalloc(sizeof(t_entity))))
		return ;
	entity->next = NULL;
	entity->pos = pos;
	entity->texture = texture;
	entity->id = 0;
	if (!entities)
		entities = entity;
	else
	{
		entity->id = entities->id + 1;
		entity->next = entities;
		entities = entity;
	}
}

int 		init_zbuf(t_zbuf *zbuf)
{
	if (zbuf->zdist)
		ft_memset(zbuf->zdist, HEIGHT * WIDTH * sizeof(double), -1);
	if (!zbuf->zdist)
		if (!(zbuf->zdist = ft_memalloc(HEIGHT * WIDTH * sizeof(double))))	
			return (0);
	if (!zbuf->zcolor)
		if (!(zbuf->zcolor = ft_memalloc(HEIGHT * WIDTH * sizeof(int))))
			return (0);
	if (zbuf->zcolor)
		ft_memset(zbuf->zcolor, HEIGHT * WIDTH * sizeof(int), 0);
	return (1);
}

static void	place_in_zbuf(t_entity entities, t_player player, t_zbuf *zbuf)
{
	double	dist;
	t_vec3d pos;
	int 	text_height;
	int		text_width;
	int 	x;
	int 	y;
	int		*img;

	img = (int*)entities.texture->pixels;
	dist = distance(player.position, entities.pos);
	text_height = entities.texture->h;
	text_width = entities.texture->w;
	pos = calculate_position(entities, player.camera);
	x = pos.x - text_width / 2;
	while (x < pos.x + text_width / 2)
	{
		y = pos.y - text_height / 2;
		while(y < pos.y + text_height / 2)
		{
			if (x <= WIDTH && x >= 0 && y <= HEIGHT && y >= 0 && dist < zbuf->zdist[x + y * WIDTH])
			{
				zbuf->zdist[x + y * WIDTH] = dist,
				zbuf->zcolor[x + y * WIDTH] = img[(int)((x - pos.x + text_width / 2) + (y - pos.y + text_height / 2) * text_width)]; 
			}
		}
	}
}

void		z_buffer(t_entity *entities, t_player player, t_zbuf *zbuf)
{
	t_entity	*ptr;

	ptr = entities;
	while (ptr)
	{
		place_in_zbuf(*ptr, player, zbuf);
		ptr = ptr->next;
	}
}
