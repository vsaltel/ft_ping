#ifndef FT_PING_H
# define FT_PING_H

# define OPTIONS "hv"

# include "libft.h"

typedef struct		s_ping
{
	int				opt_h;	
	int				opt_v;	
	char			*dest;
}					t_ping;

t_ping		*get_args(int ac, char **av);
int			check_args(int ac, char **av);
void		print_usage(void);

#endif
