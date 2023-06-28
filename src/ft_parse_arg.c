#include "icmp.h"

static int	ft_parse_double_dash(char *av, t_icmp_stats *stats)
{
	if (!av || !stats)
		return (-1);
	if (ft_strcmp(av, "--help") == 0)
		stats->flag |= FT_PING_HELP;
	else if (ft_strncmp(av, "--ttl=", 6) == 0)
		stats->ttl = ft_atoi(av + 6);
	else
		return (ft_print_arg_error(FT_ARG_INVALID_ARG_WHOLE, av));
	return (0);
}

static int	ft_parse_normal_arg(char av, t_icmp_stats *stats)
{
	if (!av || !stats)
		return (-1);
	if (av == 'h')
		stats->flag |= FT_PING_HELP;
	else if (av == 'q')
		stats->flag |= FT_PING_QUIET;
	else if (av == 'v')
		stats->flag |= FT_PING_VERBOSE;
	else if (av == '?')
		stats->flag |= FT_PING_HELP;
	else
		return (ft_print_arg_error(FT_ARG_INVALID_ARG, &av));
	return (0);
}

static int	ft_parse_single_arg(char *av, t_icmp_stats *stats)
{
	int	i;
	int	ret;

	if (!av || !stats)
		return (-1);
	if (av[0] != '-' && stats->arg)
		return (0);
	else if (av[0] != '-')
	{
		stats->arg = av;
		return (0);
	}
	i = 0;
	while (av[++i])
	{
		ret = ft_parse_normal_arg(av[i], stats);
		if (ret < 0)
			return (ret);
	}
	return (0);
}

int	ft_parse_args(int ac, char **av, t_icmp_stats *stats)
{
	int	i;
	int	len;
	int	ret;

	if (!stats || !av)
		return (-1);
	ft_memset(stats, 0, sizeof(t_icmp_stats));
	stats->flag = 0;
	stats->arg = 0;
	stats->ttl = FT_IP_TTL;
	i = -1;
	while (++i < ac)
	{
		len = ft_strlen(av[i]);
		if (len < 2)
			return (ft_print_arg_error(FT_ARG_INVALID_ARG, av[i]));
		if (av[i][0] == '-' && av[i][1] == '-')
			ret = ft_parse_double_dash(av[i], stats);
		else
			ret = ft_parse_single_arg(av[i], stats);
		if (ret < 0)
			return (ret);
	}
	if (!(stats->arg) && !(stats->flag & FT_PING_HELP))
		return (ft_print_arg_error(FT_ARG_MISSING_HOST, 0));
	return (0);
}
