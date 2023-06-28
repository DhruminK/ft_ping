#include "icmp.h"

int	ft_print_arg_error(int ret, char *val)
{
	if (ret == FT_ARG_INV_EQUAL)
		printf("ft_ping: invalid value (`=' near `=')\n");
	else if (ret == FT_ARG_INVALID_ARG)
		printf("ft_ping: invalid option -- '%c'\n"
				"Try 'ping --help' or 'ping --usage' for more information\n", *val);
	else if (ret == FT_ARG_INVALID_ARG_WHOLE)
		printf("ft_ping: invalid option '%s'\n"
				"Try 'ping --help' or 'ping --usage' for more information\n", val);
	else if (ret == FT_ARG_MISSING_HOST)
		printf("ft_ping: missing host operand\n"
				"Try 'ping --help' or 'ping --usage' for more information\n");
	else if (ret == FT_ROOT_PER_REQ)
		printf("ft_ping:: usage error: need to be run as root\n");
	else if (ret == FT_UNKOWN_HOST)
		 printf("ft_ping: unkown host\n");
	return (ret);
}
