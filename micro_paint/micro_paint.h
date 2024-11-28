#ifndef MICRO_PAINT
# define MICRO_PAINT

# include<fcntl.h>
# include<stdio.h>
# include<stdlib.h>
# include<math.h>
# include<unistd.h>

typedef struct s_rect
{
	char			type;
	float			x;
	float			y;
	float			width;
	float			height;
	char			c;
	struct s_rect	*next;
	
}	t_rect;

typedef struct s_win
{
	int		width;
	int		height;
	char	*panel;
}	t_win;

int		paint_rect(t_win win, t_rect *rect_arr);
int		parse_input(FILE *ptr, t_win *win, t_rect **rect_arr);
t_rect	*lstnew_rect(t_rect rect);
void	lstadd_rect(t_rect **rect_arr, t_rect *rect);
int		get_win_info(FILE *file, t_win *win);
void	draw_panel(t_win win);
void	check_for_rect(t_win *win, t_rect *rect_arr, int x, int y, int i);

#endif
