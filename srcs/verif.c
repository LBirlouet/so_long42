/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbirloue <lbirloue@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 13:58:41 by lbirloue          #+#    #+#             */
/*   Updated: 2023/12/05 18:59:42 by lbirloue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

int verif_map(t_map *t_map, t_map_element *t_map_element)
{
	char			*line;
	int				line_size;
	int				verif;
	int				line_counter;
	
	t_map_element->exit.exit_count = 0;
	t_map_element->colectible.colectible_count = 0;
	line_counter = 0;
	line = get_next_line(t_map->map_fd);
	verif = verif_first_line(line);
	if (verif == -1)
	{
		free(line);
		return (-1);
	}
	line_size = ft_strlen_before_newline(line);
	while (line)
	{
		free(line);
		line = get_next_line(t_map->map_fd);
		if (!line)
			break ;
		line_counter++;
		verif = verif_line(line, (line_size - 1), t_map_element, line_counter);
		if (verif == -1 || (ft_strlen_before_newline(line) != line_size))
		{
			free(line);
			return (-1);
		}
	}
	close(t_map->map_fd);
	t_map->map_x = line_size;
	t_map->map_y = line_counter;
//	printf("exit_count ==%d|%d\n", t_map_element->exit.exit_count, t_map_element->colectible.colectible_count);
	verif = element_verif(*t_map_element);
	if (verif == -1)
		return (-1);
	verif = verif_last_line(line_counter, t_map);
	if (verif == -1)
		return (-1);
	return (0);
}

int	element_verif(t_map_element t_map_element)
{
	if (t_map_element.exit.exit_count != 1)
		return (-1);
	return (0);
}

int	verif_line(char *map, int line_size, t_map_element *t_map_element, int line_counter)
{
	int	i;

	i = 0;
	if (map[0] != '1' || map[line_size] != '1')
		return (-1);
	while (i < line_size)
	{
		if (map[i] == 'C')
			t_map_element->colectible.colectible_count += 1;
		if (map[i] == 'E')
		{
			t_map_element->exit.exit_count++;
			t_map_element->exit.exit_x = i;
			t_map_element->exit.exit_y = line_counter;
		}
		if (map[i] == 'P')
		{
			t_map_element->player.player_count++;
			t_map_element->player.player_x = i;
			t_map_element->player.player_y = line_counter;
		}
		if (map[i] != '1' && map[i] != '0' && map[i] != 'P' && map[i] != 'C' && map[i] != 'E')
			return (-1);
		i++;
	}
	return (0);
}

int verif_first_line(char *map)
{
	int	i;

	i = 0;
	while (map[i] != '\n')
	{
		if (map[i] != '1')
			return (-1);
		i++;
	}
	return (0);
}

int	verif_last_line(int line_counter, t_map *t_map)
{
	char	*line;
	int		verif;
	int		i;
	
	i = 0;
	verif = 0;
	t_map->map_fd = open(t_map->map_name, O_RDONLY, 0644);
	if (t_map->map_fd == -1)
		return (1);
	while (line_counter > 0)
	{
		line = get_next_line(t_map->map_fd);
		free(line);
		line_counter--;
	}
	line = get_next_line(t_map->map_fd);
	if (ft_strlen_before_newline(line) != t_map->map_x || t_map->map_x == t_map->map_y)
		return (-1);
	while (i != t_map->map_x)
	{
		if (line [i] != '1')
			return (-1);
		i++;
	}
	close(t_map->map_fd);
	return (0);
}
