/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydonse <ydonse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 10:04:29 by ydonse            #+#    #+#             */
/*   Updated: 2019/05/16 15:37:15 by malluin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H
# include "libft.h"
# include <SDL.h>
# include <SDL_mixer.h>
# include <fcntl.h>
# include <errno.h>
# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include <time.h>

# define WIDTH 800
# define HEIGHT 600
# define SPACE 60
# define MIN_WIDTH 3
# define MIN_HEIGHT 3
# define HALF_X (WIDTH / SPACE) / 2
# define HALF_Y (HEIGHT / SPACE) / 2

# define FILE_ERROR 1
# define SIZE_ERROR 2
# define SYNTAX_ERROR 3
# define MALLOC_ERROR 4
# define PLAYER_ERROR 5
# define WALL_ERROR 6

# define SKY 0x69DBF5FF
# define MIXSW 0x82989DFF
# define WALL  0x9a5444FF
# define GROUND  0xB0B0B0FF
# define DOOR  0xBBBBBBFF

# define MAX_AREA 4
# define OBJ ".mpjt"

# define PLAYER_SIZE 0.25
# define BLOCK_SIZE 64
# define PLAYER_HEIGHT 32
# define DEFAULT_FOV 65
# define ROTATE_SPEED 20

# define PROJ_WIDTH 800
# define PROJ_HEIGHT 600

# define PI 3.14159265

# define LEFT_AR	SDL_SCANCODE_LEFT
# define RIGHT_AR	SDL_SCANCODE_RIGHT
# define UP_AR		SDL_SCANCODE_UP
# define DOWN_AR	SDL_SCANCODE_DOWN

# define LEFT	SDL_SCANCODE_A
# define RIGHT	SDL_SCANCODE_D
# define UP		SDL_SCANCODE_W
# define DOWN	SDL_SCANCODE_S

# define SPRINT	SDL_SCANCODE_LSHIFT

# define PARSE_BUFF_SIZE 64

typedef	struct		s_sounds
{
	Mix_Chunk		*shot;
	Mix_Chunk		*door;
}					t_sounds;

typedef struct		s_position {
	short			x;
	short			y;
}					t_position;

typedef struct		s_dpos {
	double			x;
	double			y;
}					t_dpos;

typedef struct		s_ray {
	double			dist;
	char			type;
	char			orientation;
	t_position		object;
	t_dpos			dpos;
	int				zone;
	short			res;
	double			angle;
}					t_ray;

typedef struct		s_minimap {
	t_dpos			orig;
	t_dpos			dest;
	int				bloc_x;
	int				bloc_y;
	int				bloc_y_cp;
	double			per_pos_x;
	double			per_pos_y;
	int				limit_x;
	int				limit_y;
}					t_minimap;

typedef struct		s_case {
	char			type;
	int				zone;
	char			block;
	t_bool			valid;
}					t_case;

typedef struct		s_texture {
	Uint32			*content;
	SDL_Texture		*texture;
	Uint32			color_tmp;
}					t_texture;

typedef struct		s_image {
	int				bits_color;
	int				bits_alpha;
	Uint32			*tex;
	int				w;
	int				h;
}					t_image;

typedef struct		s_anim {
	int				current;
	t_image			*image[3];
}					t_anim;

typedef struct		s_slice {
	int				bwall;
	int				ewall;
	t_position		pix;
	Uint32			color;
	t_image			*tex;
}					t_slice;

typedef struct		s_area {
	t_image			*wall_e;
	t_image			*wall_w;
	t_image			*wall_n;
	t_image			*wall_s;
}					t_area;

typedef struct		s_sdl {
	SDL_Window		*pwindow;
	SDL_Renderer	*prenderer;
	SDL_Event		event;
	t_texture		*map;
	t_texture		*game;
	int				x_o;
	int				y_o;
	t_sounds		sounds;
	Mix_Music		*musique;
}					t_sdl;

typedef struct		s_main {
	t_sdl			*sdl;
	t_case			**map;
	int				width;
	int				height;
	char			*parsing_line;
	t_position		start_position;
	double			move_speed;
	t_dpos			player_pos;
	short			p_angle;
	char			active_map;
	short			fov;
	int				proj_distance;
	int				viewline;
	t_image			*door;
	t_image			*win;
	t_image			*coupe;
	t_image			*interface;
	t_image			*menu;
	t_image			*skybox;
	t_anim			weapon;
	t_area			areas[MAX_AREA];
	t_minimap		min;
}					t_main;

void				handle_error(t_main *s, int error_nb);
void				free_program(t_main *s);
int					parse_map(t_main *s, char *file);
int					check_next_case(t_main *s, int x, int y);
void				draw_minimap(t_main *s, int i, int j);
void				draw_player(t_sdl *sdl, double bloc_x,
					double bloc_y);
void				draw_rect(t_texture *text, t_dpos orig,
					t_dpos dest);
void				draw_interface (t_main *s);
void				draw_weapon (t_main *s, double perx,
					short orig_x, short orig_y);
void				draw_black(t_main *s);
void				draw_square(t_main *s, t_dpos orig, t_dpos dest,
					t_image *wall);
void				get_case_color(t_main *s, t_dpos orig, t_dpos dest,
					t_case pos);

void				set_pixel(t_texture *text, Uint32 color, t_position coord);
void				update_image(t_main *s, t_texture *texture);
int					check_collisions(t_main *s, t_dpos target);
int					check_walls(t_main *s, int x, int y);

void				ft_print_map(t_main	*s);

void				ft_error_sdl(char *str);

t_main				*initialize_main(void);
void				pre_initialize_sdl(t_main *s);
t_texture			*initialize_texture(t_sdl *sdl, int width, int height);
void				initialize_sdl(t_main *s, t_sdl *sdl);
int					handle_menu(t_main *s);

void				handle_keys(t_main *s);
void				event_handler(t_main *s);
void				turn_camera(t_main *s, const Uint8 *keys, char command);
void				move_player(t_main *s, const Uint8 *keys, char sprint);
int					check_door(t_case **map, int x, int y);
void				open_door(t_main *s);
void				shoot(t_main *s);
void				raycast_visualization(t_main *s);
t_ray				raycast(t_main *s, double r_angle);

t_ray				check_entity_h(t_main *s, t_dpos point, double r_angle,
					t_ray ray);
t_ray				check_entity_v(t_main *s, t_dpos point, double r_angle,
					t_ray ray);
t_ray				get_raycast_dist_v(t_main *s, t_ray ray, double r_a,
					t_dpos fp);
t_ray				get_raycast_dist_h(t_main *s, t_ray ray, double r_angle,
					t_dpos fp);
int					ft_min_one(int a);

void				set_pixel_debug(t_sdl *sdl, t_dpos coord);
void				draw_debug_rect(t_texture *text,
					Uint32 color, t_dpos orig);
void				draw_wall_slice(t_main *s, t_ray ray, double dist, int x);

double				to_rad(double angle);
double				norme(t_dpos player, t_dpos point);
double				percent(double value, double total);
Uint32				darken_color(Uint32 color, double perc);

t_image				*load_tga(char *path, int i, int idx, int ret);
void				load_images(t_main *s);

void				create_sounds(t_sdl *sdl);
#endif
