/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_octree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roduquen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 17:19:56 by roduquen          #+#    #+#             */
/*   Updated: 2019/12/14 16:15:28 by roduquen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "vec3.h"
#include "octree.h"
#include <math.h>
#include "libft.h"

static inline int		verify_inside_node(t_doom *data, t_octree *node)
{
	t_vec3l		tester;
	t_vec3l		count;
	int			nbr_node;
	int			total;
	char		c;

	total = 0;
	nbr_node = 0;
	tester.x = (node->center.x >> 1) - (node->size >> 2);
	tester.y = (node->center.y >> 1) - (node->size >> 2);
	tester.z = (node->center.z >> 1) - (node->size >> 2);
	count.x = -1;
	while (++count.x < node->size >> 1 && (count.y = -1))
	{
		while (++count.y < node->size >> 1 && (count.z = -1))
		{
			while (++count.z < node->size >> 1)
			{
				c = data->fire_model[tester.x + count.x][tester.y + count.y]
					[tester.z + count.z];
				if (c)
					nbr_node++;
				else if (nbr_node)
					return (-1);
				total++;
			}
		}
	}
	if (nbr_node != 0 && total != nbr_node)
		return (-1);
	return (nbr_node);
}

static inline void		verify_and_add_to_queue(t_queue *queue[2]
	, t_octree *actual)
{
	int			i;

	i = 0;
	while (i < 8)
	{
		if (actual->child[i]->leaf == INSIDE)
		{
			queue[1]->next = queue_new(actual->child[i]);
			queue[1]->next->prev = queue[1];
			queue[1] = queue[1]->next;
		}
		i++;
	}
}

static inline int		breadth_first_create_octree(t_doom *data
	, t_octree *actual)
{
	t_queue		*queue[2];

	queue[0] = queue_new(actual);
	queue[1] = queue[0];
	while (queue[0])
	{
		actual = (t_octree*)queue[0]->ptr;
		if (actual->leaf == INSIDE)
		{
			if (actual->size > 2)
			{
				create_child(actual, data);
				verify_and_add_to_queue(queue, actual);
			}
		}
		queue[0] = queue[0]->next;
	}
	return (free_queue(queue));
}

void					check_if_child_is_leaf(t_doom *data, t_octree *node)
{
	int			i;
	int			ret;

	i = 0;
	while (i < 8)
	{
		if ((ret = verify_inside_node(data, node->child[i])) == 0)
			node->child[i]->leaf = EMPTY;
		else if (ret == -1)
			node->child[i]->leaf = INSIDE;
		else
			node->child[i]->leaf = FULL;
		i++;
	}
}

void				aff_octree(t_octree *node, t_doom *data, int oct[3])
{
	int i;
	i = 0;
	while (i < 8)
	{
		if (node->child[i])
			aff_octree(node->child[i], data, oct);
		i++;
	}
	if (node->leaf == INSIDE)
		oct[2]++;
	else if (node->leaf == EMPTY)
		oct[1]++;
	else
		oct[0]++;
}

int						create_octree(t_doom *data)
{
	t_octree	*actual;
	int			size;
	int			ret;
	int			oct[3];

	size = SIZE_MAP;
	actual = create_node(size << 1, vec3l(size, size, size), NULL);
	data->octree = actual;
	ret = verify_inside_node(data, actual);
	if (ret == -1)
		actual->leaf = INSIDE;
	else if (!ret)
		actual->leaf = EMPTY;
	else
		actual->leaf = FULL;
	if (actual->leaf == INSIDE)
		breadth_first_create_octree(data, actual);
	actual = data->octree;
	oct[0] = 0;
	oct[1] = 0;
	oct[2] = 0;
	aff_octree(actual, data, oct);
//	printf("empty = %d, full = %d, inside = %d, total = %d, total bytes used = %d\n"
//			, oct[1], oct[0], oct[2], oct[0] + oct[1] + oct[2], (oct[0] + oct[1] + oct[2]) * 65);
	return (0);
}
