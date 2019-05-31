/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydonse <ydonse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 09:43:56 by ydonse            #+#    #+#             */
/*   Updated: 2019/05/10 13:42:11 by malluin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_dpos	get_direction(t_main *s, const Uint8 *keys, double speed, t_dpos target)
{
	if (keys[UP])
	{
		target.x += cos(to_rad(s->p_angle)) * speed;
		target.y -= sin(to_rad(s->p_angle)) * speed;
	}
	if (keys[DOWN])
	{
		target.x -= cos(to_rad(s->p_angle * -1)) * speed;
		target.y -= sin(to_rad(s->p_angle * -1)) * speed;
	}
	if (keys[LEFT])
	{
		target.x += cos(to_rad(s->p_angle + 90)) * speed;
		target.y -= sin(to_rad(s->p_angle + 90)) * speed;
	}
	if (keys[RIGHT])
	{
		target.x += cos(to_rad(s->p_angle - 90)) * speed;
		target.y -= sin(to_rad(s->p_angle - 90)) * speed;
	}
	return (target);
}

void	move_player(t_main *s, const Uint8 *keys, char sprint)
{
	t_dpos	target;
	double	speed;
	double	tmp;

	speed = s->move_speed + sprint * s->move_speed * 0.5;
	if ((keys[UP] || keys[DOWN]) && (keys[LEFT] || keys[RIGHT]))
		speed /= 2;
	target.x = s->player_pos.x;
	target.y = s->player_pos.y;
	target = get_direction(s, keys, speed, target);
	if (check_collisions(s, target) == 0)
	{
		tmp = target.x;
		target.x = s->player_pos.x;
		if (check_collisions(s, target) == 0)
		{
			target.x = tmp;
			target.y = s->player_pos.y;
			if (check_collisions(s, target) == 0)
				return ;
		}
	}
	s->player_pos.x = target.x > s->width - 1 ? s->width - 1 : target.x;
	s->player_pos.y = target.y > s->height - 1 ? s->height - 1 : target.y;
}

void	open_door(t_main *s)
{
	if (((s->p_angle < 45 && s->p_angle >= 0)
	|| (s->p_angle < 380 && s->p_angle >= 315))
	&& check_door(s->map, (int)s->player_pos.x + 1, (int)s->player_pos.y))
	{
		s->map[(int)s->player_pos.y][(int)s->player_pos.x + 1].block = 0;
		Mix_PlayChannel(2, s->sdl->sounds.door, 0);
	}
	else if ((s->p_angle < 135 && s->p_angle > 45)
	&& check_door(s->map, (int)s->player_pos.x, (int)s->player_pos.y - 1))
	{
		s->map[(int)s->player_pos.y - 1][(int)s->player_pos.x].block = 0;
		Mix_PlayChannel(2, s->sdl->sounds.door, 0);
	}
	else if ((s->p_angle < 225 && s->p_angle > 135)
	&& check_door(s->map, (int)s->player_pos.x - 1, (int)s->player_pos.y))
	{
		s->map[(int)s->player_pos.y][(int)s->player_pos.x - 1].block = 0;
		Mix_PlayChannel(2, s->sdl->sounds.door, 0);
	}
	else if ((s->p_angle < 315 && s->p_angle > 225)
	&& check_door(s->map, (int)s->player_pos.x, (int)s->player_pos.y + 1))
	{
		s->map[(int)s->player_pos.y + 1][(int)s->player_pos.x].block = 0;
		Mix_PlayChannel(2, s->sdl->sounds.door, 0);
	}
}

void	shoot(t_main *s)
{
	time_t		fps;

	fps = clock();
	Mix_PlayChannel(1, s->sdl->sounds.shot, 0);
	while (s->weapon.current < 2)
	{
		s->weapon.current++;
		handle_keys(s);
		while (clock() - fps < 100000)
		{
			if ((SDL_PollEvent(&(s->sdl->event))) != 0)
			{
				if (s->sdl->event.type == SDL_MOUSEMOTION)
				{
					turn_camera(s, 0, 1);
					raycast_visualization(s);
					update_image(s, s->sdl->game);
				}
			}
			handle_keys(s);
		}
		fps = clock();
	}
	s->weapon.current = 0;
}

void	turn_camera(t_main *s, const Uint8 *keys, char command)
{
	if (command)
	{
		s->p_angle -= ft_min_one(s->sdl->event.motion.xrel);
		s->p_angle = (s->p_angle + 360) % 360;
		s->viewline -= (s->sdl->event.motion.yrel / 2);
		s->viewline = (s->viewline < -HEIGHT / 2 ? -HEIGHT / 2 : s->viewline);
		s->viewline = (s->viewline > HEIGHT * 1.5 ? HEIGHT * 1.5 : s->viewline);
		s->sdl->event.type = 0;
	}
	else
	{
		if (keys[LEFT_AR] || keys[RIGHT_AR])
			s->p_angle = (s->p_angle + (keys[LEFT_AR] - keys[RIGHT_AR])
			* ROTATE_SPEED / 10 + 360) % 360;
		if (keys[UP_AR] || keys[DOWN_AR])
		{
			s->viewline = (s->viewline + (keys[UP_AR] - keys[DOWN_AR])
			* ROTATE_SPEED / 2);
			s->viewline = (s->viewline < -HEIGHT / 2 ? -HEIGHT / 2
			: s->viewline);
			s->viewline = (s->viewline > HEIGHT * 1.5 ? HEIGHT * 1.5
			: s->viewline);
		}
	}
}
