#include "doom.h"
#include "libft.h"
#include "menus.h"

#include <stdio.h>

static int		check_input(t_doom *data, SDL_Keycode input)
{
	int i;

	i = -1;
	while (++i < NB_MODIF_INPUTS)
		if ((unsigned int)input == data->tabinputs.keycode[i])
			return (1);
	if (input == SDLK_ESCAPE || input == SDLK_RETURN)
		return (1);
	return (0);
}

int				state_get_input(t_doom *data)
{
	int		ret;

	ret = 0;
	while (SDL_PollEvent(&data->lib.event))
	{
		if (data->lib.event.type == SDL_KEYDOWN)
		{
			if (check_input(data, data->lib.event.key.keysym.sym) == 1)
				printf("invalid input\n");
			else
				data->tabinputs.keycode[(data->button > 8 ?
					data->button - 1 : data->button)] =
					data->lib.event.key.keysym.sym;
			switch_state(data, GET_INPUT, SETTINGS);
		}
	}
	return (0);
}