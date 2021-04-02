#include "ping.h"

char	*set_inetaddr(struct sockaddr *sa, socklen_t salen)
{
	struct sockaddr_in	*sin;

	sin = (struct sockaddr_in *)sa;
	if (!inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)))
		return (ft_strdup("CONVERTION_FAIL"));
	return (ft_strdup(str));
}
