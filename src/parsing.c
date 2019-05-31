/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydonse <ydonse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 12:58:00 by ydonse            #+#    #+#             */
/*   Updated: 2019/05/16 16:40:06 by ydonse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	check_player(t_main *s, char *file)
{
	int		fd;
	char	player;

	player = 0;
	if ((fd = open(file, O_RDONLY)) < 1)
		handle_error(s, FILE_ERROR);
	s->parsing_line = NULL;
	while (get_next_line(fd, &(s->parsing_line)) > 0)
	{
		if (ft_strchr(s->parsing_line, 'j'))
			player = 1;
		ft_strdel(&(s->parsing_line));
	}
	ft_strdel(&(s->parsing_line));
	close(fd);
	if (!player)
		handle_error(s, PLAYER_ERROR);
}

int		fill_map(t_main *s, char **tab, int i, int k)
{
	static int		player = 0;

	if (!(s->map[i] = (t_case*)malloc(sizeof(t_case) * s->width)))
	{
		ft_free_tab_str(tab);
		handle_error(s, MALLOC_ERROR);
	}
	while (tab[k])
	{
		s->map[i][k].type = tab[k][0];
		s->map[i][k].block = tab[k][0] == 'j' || tab[k][0] == '.'
			|| tab[k][0] == 't' ? 0 : 1;
		if (s->map[i][k].type == 'j')
		{
			s->start_position.x = k;
			s->start_position.y = i;
			player++;
		}
		s->map[i][k].zone = tab[k][2] - '0';
		s->map[i][k++].valid = 0;
	}
	ft_free_tab_str(tab);
	if (++i == s->height && player > 1)
		handle_error(s, PLAYER_ERROR);
	return (i);
}

void	check_valid_line(t_main *s, char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		if (!ft_strchr(OBJ, tab[i][0]) || tab[i][1] != ',' || tab[i][2] < '0'
			|| tab[i][2] >= MAX_AREA + '0' || tab[i][3] != '\0')
		{
			ft_free_tab_str(tab);
			handle_error(s, SYNTAX_ERROR);
		}
		i++;
	}
	ft_free_tab_str(tab);
	if (i >= 50 || (i != s->width && s->width != 0))
		handle_error(s, SYNTAX_ERROR);
	s->width = i;
}

void	check_file(t_main *s, int fd, char *file)
{
	int		i;

	i = 0;
	s->parsing_line = NULL;
	while (get_next_line(fd, &(s->parsing_line)) > 0)
	{
		i++;
		if (!ft_strchr(OBJ, s->parsing_line[0]))
			handle_error(s, SYNTAX_ERROR);
		ft_strdel(&(s->parsing_line));
	}
	ft_strdel(&(s->parsing_line));
	if (i < MIN_HEIGHT || i > 50)
		handle_error(s, SIZE_ERROR);
	s->height = i;
	close(fd);
	if ((fd = open(file, O_RDONLY)) < 1)
		handle_error(s, FILE_ERROR);
	while (get_next_line(fd, &(s->parsing_line)) > 0)
	{
		check_valid_line(s, ft_strsplit(s->parsing_line, ' '));
		ft_strdel(&(s->parsing_line));
	}
	ft_strdel(&(s->parsing_line));
	close(fd);
}

int		parse_map(t_main *s, char *file)
{
	int fd;
	int i;

	i = 0;
	if ((fd = open(file, O_RDONLY)) < 1)
		handle_error(s, FILE_ERROR);
	if (read(fd, NULL, 0) == -1)
		handle_error(s, FILE_ERROR);
	check_file(s, fd, file);
	check_player(s, file);
	if ((fd = open(file, O_RDONLY)) < 1)
		handle_error(s, FILE_ERROR);
	if (!(s->map = (t_case**)malloc(sizeof(t_case*) * s->height)))
		handle_error(s, MALLOC_ERROR);
	s->parsing_line = NULL;
	while (get_next_line(fd, &(s->parsing_line)) > 0)
	{
		i = fill_map(s, ft_strsplit(s->parsing_line, ' '), i, 0);
		ft_strdel(&(s->parsing_line));
	}
	close(fd);
	if (!check_walls(s, s->start_position.x, s->start_position.y))
		handle_error(s, WALL_ERROR);
	return (1);
}
