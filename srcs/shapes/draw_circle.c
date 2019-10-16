/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roduquen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 16:39:37 by roduquen          #+#    #+#             */
/*   Updated: 2019/10/16 16:41:21 by roduquen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "graphic_lib.h"
#include "vec3.h"

static inline void	putpixel(unsigned int *image, int a, int b, int c)
{
	if (a + b * WIDTH >= 0 && a + b * WIDTH < WIDTH * HEIGHT)
		image[a + b * WIDTH] = c;
}

static inline void	drawCircle(unsigned int *image, t_vec3l circle, int x
	, int y)
{
	rand();
    putpixel(image, circle.x + x, circle.y + y, circle.z);
    putpixel(image, circle.x - x, circle.y + y, circle.z);
    putpixel(image, circle.x + x, circle.y - y, circle.z);
    putpixel(image, circle.x - x, circle.y - y, circle.z);
    putpixel(image, circle.x + y, circle.y + x, circle.z);
    putpixel(image, circle.x - y, circle.y + x, circle.z);
    putpixel(image, circle.x + y, circle.y - x, circle.z);
    putpixel(image, circle.x - y, circle.y - x, circle.z);
}

static inline void	circleBres(unsigned int *image, t_vec3l circle, int color)
{
    int x;
    int	y;
    int d;

    x = 0;
    y = circle.z;
    d = 3 - 2 * circle.z;
    circle.z = color;
    drawCircle(image, circle, x, y);
    while (y >= x)
    {
        x++;
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;
        drawCircle(image, circle, x, y);
    }
}

void				start_drawing_circle(t_doom *data, int i, unsigned int *image)
{
	t_vec3l		circle;

	circle.x = data->tab[i].pos % WIDTH;
	circle.y = data->tab[i].pos / WIDTH;
	circle.z = data->tab[i].size;
	circleBres(image, circle, data->tab[i].color);
	circle.z = data->tab[i].size / (i % 3 + 3);
	if (data->tab[i].pos / WIDTH > 445)
		circle.y -= data->tab[i].size / 4;
	else
		circle.y += data->tab[i].size / 4;
	if (data->tab[i].pos % WIDTH > 1174)
		circle.x -= data->tab[i].size / 4;
	else
		circle.x += data->tab[i].size / 4;
	circleBres(image, circle, data->tab[i].color
		+ ((data->tab[i].color >> 17) << 8) + (data->tab[i].color >> 17));
	data->tab[i].pos += (data->tab[i].speed * WIDTH);
}
