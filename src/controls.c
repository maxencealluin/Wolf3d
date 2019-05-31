/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydonse <ydonse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 09:44:06 by ydonse            #+#    #+#             */
/*   Updated: 2019/05/15 14:27:25 by malluin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int		keyboard_controls(t_main *s, int key)
{
	if (key == SDLK_ESCAPE)
		return (0);
	else if (key == SDLK_e)
		open_door(s);
	else if (key == SDLK_m && HEIGHT / SPACE <= s->height && WIDTH / SPACE
		<= s->width)
	{
		s->active_map = !s->active_map;
		draw_interface(s);
	}
	return (1);
}

int		check_win(t_main *s)
{
	t_position	coord;
	double		px;
	double		py;
	int			pl;

	if (!(s->player_pos.x >= 0 && s->player_pos.x <= s->width - 1.0
		&& s->player_pos.y >= 0 && s->player_pos.y <= s->height - 1.0))
		return (0);
	if (s->map[(int)s->player_pos.y][(int)s->player_pos.x].type != 't')
		return (0);
	coord.x = -1;
	while (++(coord.x) < WIDTH)
	{
		coord.y = 0;
		px = (double)coord.x / (double)WIDTH;
		while (coord.y < HEIGHT)
		{
			py = (percent(coord.y++, HEIGHT));
			pl = (int)(py * s->win->h) * s->win->w + (int)(px * s->win->w);
			if (pl >= 0 && pl < s->win->h * s->win->w)
				set_pixel(s->sdl->game, s->win->tex[pl], coord);
		}
	}
	update_image(s, s->sdl->game);
	return (1);
}

void	handle_keys(t_main *s)
{
	const Uint8 *keys;

	keys = SDL_GetKeyboardState(NULL);
	if (keys[LEFT] || keys[RIGHT] || keys[UP] || keys[DOWN])
		move_player(s, keys, keys[SPRINT]);
	if (keys[LEFT_AR] || keys[RIGHT_AR] || keys[UP_AR] || keys[DOWN_AR])
		turn_camera(s, keys, 0);
	if (s->sdl->event.type == SDL_MOUSEMOTION)
		turn_camera(s, keys, 1);
	if (s->active_map && HEIGHT / SPACE <= s->height && WIDTH / SPACE
		<= s->width)
		draw_minimap(s, 0, 0);
	else
	{
		raycast_visualization(s);
		update_image(s, s->sdl->game);
	}
}

void	event_handler(t_main *s)
{
	int			game;

	game = 1;
	SDL_SetRelativeMouseMode(SDL_TRUE);
	draw_interface(s);
	while (game)
	{
		while ((SDL_PollEvent(&(s->sdl->event))) != 0)
		{
			if (s->sdl->event.type == SDL_QUIT)
				game = 0;
			if (s->sdl->event.type == SDL_MOUSEBUTTONDOWN
				&& s->weapon.current == 0)
				shoot(s);
			if (s->sdl->event.type == SDL_KEYDOWN
				&& keyboard_controls(s, s->sdl->event.key.keysym.sym) == 0)
				game = 0;
		}
		handle_keys(s);
		if (check_win(s))
		{
			sleep(3);
			break ;
		}
	}
}
