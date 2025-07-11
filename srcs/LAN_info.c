#include "ft_malcolm.h"

/* verbose output of LAN info */

void	display_LAN_info(int iface_count, struct iface_info *infos, char *ipv_type)
{
	for (int i = 0; i < iface_count; ++i)
	{
		if (infos[i].is_up == false && infos[i].has_ipv6 == true)
			continue ;
		fprintf(stdout, "   Interface %-16s ", infos[i].name);
		if (infos[i].is_up == false)
			fprintf(stdout, "%-34s", "(IP):   N/A - status: [DOWN]");
		else if (ft_strncmp(ipv_type, "IPv4", 4) == 0 && infos[i].has_ipv4)
			fprintf(stdout, "(IPv4): %-26s", infos[i].ipv4);
		else if (ft_strncmp(ipv_type, "IPv6", 4) == 0 && infos[i].has_ipv6)
			fprintf(stdout, "(IPv6): %-26s", infos[i].ipv6);
		print_MAC_addr(" :: MAC: ", ':', infos[i].mac);
		fprintf(stdout, "\n");
	}
}
