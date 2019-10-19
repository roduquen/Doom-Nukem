#include "doom.h"
#include "graphic_lib.h"
#include "thread.h"
#include <pthread.h>

static int	create_charac_anim(t_doom *data, unsigned int *image, int move)
{
	int			i;
	int			j;

	i = 0;
	while (i < 200)
	{
		j = 0;
		while (j < 200)
		{
			if (((int*)data->lib.character->pixels)
				[(i >> 2) * 500 + move + (j >> 2)])
			{
				image[i * WIDTH + j + 525 * WIDTH + (WIDTH >> 4)] =
				((int*)data->lib.character->pixels)[(i >> 2)
				* 500 + move + (j >> 2)];
			}
			j++;
		}
		i++;
	}
	return (0);
}

static void	choose_animation_part2(t_doom *data, int frame)
{
	int			ret;

	if (frame >= 960)
	{
		if ((ret = (frame >> 5) & 1) == 0)
			create_charac_anim(data, data->lib.image, 350 + 125000);
		else if (ret == 1 || ret == 3)
			create_charac_anim(data, data->lib.image, 400 + 125000);
		else
			create_charac_anim(data, data->lib.image, 450 + 125000);
	}
	else if ((ret = (frame >> 5) & 1) == 0)
		create_charac_anim(data, data->lib.image, 150);
	else if (ret == 1 || ret == 3)
		create_charac_anim(data, data->lib.image, 200);
	else
		create_charac_anim(data, data->lib.image, 250);
}

static void	choose_animation_part1(t_doom *data, int frame)
{
	int			ret;

	if (frame >= 160 && frame <= 256)
	{
		if ((ret = (frame >> 5) & 1) == 0)
			create_charac_anim(data, data->lib.image, 150 + 25000);
		else if (ret == 1 || ret == 3)
			create_charac_anim(data, data->lib.image, 200 + 25000);
		else
			create_charac_anim(data, data->lib.image, 250 + 25000);
	}
	else if (frame >= 448 && frame <= 512)
	{
		if ((ret = (frame >> 5) & 1) == 0)
			create_charac_anim(data, data->lib.image, 100 + 100000);
		else if (ret == 1)
			create_charac_anim(data, data->lib.image, 150 + 100000);
		else if (ret == 2)
			create_charac_anim(data, data->lib.image, 200 + 100000);
		else
			create_charac_anim(data, data->lib.image, 250 + 100000);
	}
	else
		choose_animation_part2(data, frame);
}

void		*thread_main_anim(void *thread)
{
	t_doom		*data;
	int			frame;
	int			total_frame;
	int			i;
	int			j;

	data = ((t_thread*)thread)->data;
	frame = ((t_thread*)thread)->frame;
	total_frame = ((t_thread*)thread)->total_frame;
	i = ((t_thread*)thread)->num;
	total_frame++;
	while (i < WIDTH)
	{
		j = 0;
		while (j < (1 << 7))
		{
			data->lib.image[i + j * WIDTH + (1 << 9) * WIDTH] =
			((int*)data->lib.menu_texture[((i + total_frame) >> 8) & 1]->pixels)[(i + total_frame) % 192 + j * 192];
			if (j < (1 << 6))
			{
				data->lib.image[i + j * (WIDTH - 2) + 640 * WIDTH] =
				((int*)data->lib.menu_texture[((i + total_frame) >> 8) & 1]->pixels)[((i + total_frame) >> 1) % 192 +
				j * 192 * 2] + 18;
			}
			j++;
		}
		i += NBR_THREAD;
	}
	pthread_exit(0);
}

int					anim_main_menu(t_doom *data, int total_frame, int frame)
{
	int				i;
	t_thread		thread[NBR_THREAD];

	i = 0;
	while (i < NBR_THREAD)
	{
		thread[i].data = data;
		thread[i].image = data->lib.image;
		thread[i].num = i;
		thread[i].total_frame = total_frame;
		thread[i].frame = frame;
		if (pthread_create(&thread[i].thread, NULL, (*thread_main_anim), &thread[i]) < 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < NBR_THREAD)
		pthread_join(thread[i++].thread, NULL);
	choose_animation_part1(data, frame);
	return (0);
}
