#include "ping.h"

char	*set_inetaddr(struct sockaddr *sa)
{
	struct sockaddr_in	*sin;
	char				str[IP_STR_SIZE];

	sin = (struct sockaddr_in *)sa;
	if (!inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)))
		return (ft_strdup("CONVERTION_FAIL"));
	return (ft_strdup(str));
}
