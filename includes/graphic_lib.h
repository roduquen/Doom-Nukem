#ifndef GRAPHIC_LIB_H
# define GRAPHIC_LIB_H

/*
** ====-* INCLUDES *-====
*/

# include <SDL.h>
# include <SDL_ttf.h>

/*
** ====-* DEFINES *-====
*/

# define WIDTH					1920
# define HEIGHT					1080
# define SIZE_MAP				64
# define SIZE_TEXTURE			64
# define BLOCK_SIZE_EDITOR		((HEIGHT - 54) / 64)

# define NBR_TEXT_MENU			5
# define NBR_FONTS				6

# define WHITE					(SDL_Color){255, 255, 255, 0}
# define BLACK					(SDL_Color){0, 0, 0, 0}
# define RED					(SDL_Color){255, 0, 0, 0}
# define BLUE					(SDL_Color){0, 0, 255, 0}
# define GREEN					(SDL_Color){0, 255, 0, 0}
# define YELLOW					(SDL_Color){255, 255, 0, 0}

/*
** ====-* TYPEDEFS *-====
*/

typedef struct s_graphic_lib	t_graphic_lib;
typedef struct s_doom			t_doom;
typedef struct s_format			t_format;
typedef struct s_text			t_text;
typedef struct s_label			t_label;
typedef struct s_point			t_point;

/*
** ====-* STRUCTURES *-====
*/

struct							s_label
{
	char						*str;
	SDL_Color					color;
};

struct							s_format
{
	int							height;
	int							width;
};

struct							s_text
{
	Uint32						format;
	int							access;
	int							w;
	int							h;
};

struct							s_graphic_lib
{
	SDL_Surface					*character;
	SDL_Surface					*menu_texture[NBR_TEXT_MENU];
	SDL_Surface					*skybox[6];
	SDL_Texture					*skybox_t[6];
	SDL_Surface					*editor_texture;
	SDL_Surface					*picked_texture;
	SDL_Surface					*hud_texture;
	SDL_Texture					*texture;
	SDL_Surface					*start_bg;
	SDL_Window					*window;
	SDL_Renderer				*renderer;
	SDL_Surface					*surface;
	SDL_Event					event;
	t_format					format;
	t_text						text;
	TTF_Font					*ptrfont[NBR_FONTS];
	unsigned int				*image;
	unsigned int				cam_keys;
};

/*
** ====-* PROTOTYPES *-====
*/

t_label		label(char *str, SDL_Color color);

void		load_textures(t_doom *data);

void		put_string_on_renderer(t_doom *data, t_point pos, t_label label, TTF_Font *font);

int			anim_main_menu(t_doom *data, int total_frame, int frame);

#endif
