
#include "micro_paint.h"

int	main(int argc, char **argv)
{
	FILE	*file;
	t_win	win;
	t_rect	*rect_arr = NULL;

	win.width = 0;
	win.height = 0;
	win.panel = NULL;
	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	if (!(file = fopen(argv[1], "r")))
	{
		write(1, "Error: Operation file corrupted\n", 31);
		return (1);
	}
	if (parse_input(file, &win, &rect_arr) == 1)
		return (1);
	if (paint_rect(win, rect_arr) == 1)
		return (1);
	//draw_panel(win);
	return (0);
}

int	get_win_info(FILE *file, t_win *win)
{
	char	c;
	int		i = 0;

	if (fscanf(file, "%d %d %c\n", &win->width, &win->height, &c) != 3)
	{
		write(1, "Error: Operation file corrupted\n", 31);
		return (1);
	}
	if ((0 >= win->width || win->width > 300) || (0 >= win->height || win->height > 300))
	{
		write(1, "Error: Operation file corrupted\n", 31);
		return (1);
	}
	win->panel = malloc(win->height * win->width);
	if (!win->panel)
		return (1);
	while (i < (win->height * win->width))
		win->panel[i++] = c;
	return (0);
}

int	parse_input(FILE *file, t_win *win, t_rect **rect_arr)
{
	int		bytes_read;
	t_rect	rect;

	if (get_win_info(file, win))
		return (1);	
	while (bytes_read)
	{
		bytes_read = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.c);
		if (bytes_read == EOF)
			break ;
		if (bytes_read != 6)
		{
			write(1, "Error: Operation file corrupted\n", 31);
			return (1);
		}
		lstadd_rect(rect_arr, lstnew_rect(rect));
	}
	fclose(file);
	return (0);
}

void	draw_panel(t_win win)
{
	int i = -1;

	while (++i < (win.width * win.height))
	{
		if (i != 0 && i % win.width == 0)
			write(1, "\n", 1);
		write(1, &win.panel[i], 1);
	}
	write(1, "\n", 1);
}

int	paint_rect(t_win win, t_rect *rect_arr)
{
	int i = -1;

	while (++i < (win.width * win.height))
	{
		//is inside
		//is border
		//is outside
		check_for_rect(&win, rect_arr, (i%win.width), (i/win.height), i);
		if (i != 0 && i % win.width == 0)
			write(1, "\n", 1);
		write(1, &win.panel[i], 1);
	}
	write(1, "\n", 1);
	return (0);
}

void	check_for_rect(t_win *win, t_rect *rect_arr, int x, int y, int i)
{
	float Xtl, Xa, Xbr, Ytl, Ya, Ybr;
	while (rect_arr)
	{
		Xtl = rect_arr->x;
		Ytl = rect_arr->y;
		Xa = x;
		Ya = y;
		Ybr = rect_arr->y + win->height;
		Xbr = rect_arr->x + win->width;
		if (rect_arr->type != 'R' && rect_arr->type != 'r' || ((rect_arr->width <= 0.00000000) || (rect_arr->height <= 0.00000000)))	
		{
			rect_arr = rect_arr->next;
			continue ;
		}
		if ((((x < rect_arr->x) || (rect_arr->x + rect_arr->width < x)) || (y < rect_arr->y)) || (rect_arr->y + rect_arr->height < y))                                                    {
			rect_arr = rect_arr->next;
			continue ;
		}
       	if (((x - rect_arr->x < 1.00000000) || ((rect_arr->x + rect_arr->width) - x < 1.00000000)) ||
           ((y - rect_arr->y < 1.00000000 || ((rect_arr->y + rect_arr->height) - y < 1.00000000))))
			win->panel[i] = rect_arr->c;
		if (((Xtl <= Xa) && (Xa <= Xbr)) && ((Ytl <= Ya) && (Ya <= Ybr)))
			win->panel[i] = rect_arr->c;
		if (rect_arr->type == 'R')
			win->panel[i] = rect_arr->c;	
		rect_arr = rect_arr->next;
	}
}

void	lstadd_rect(t_rect **rect_arr, t_rect *rect)
{
	if (!*rect_arr)
	{
		*rect_arr = rect;
		return ;
	}
	t_rect *head = *rect_arr;
	while (head->next)
		head = head->next;
	head->next = rect;
}

t_rect	*lstnew_rect(t_rect rect)
{
	t_rect *new;
	new = malloc(sizeof(t_rect));
	if (!new)
		return (NULL);
	new->type = rect.type;
	new->x = rect.x;
	new->y = rect.y;
	new->width = rect.width;
	new->height = rect.height;
	new->c = rect.c;
	new->next = NULL;
}
