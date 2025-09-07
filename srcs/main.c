#include "ft_malcolm.h"
#define EXIT_FAILURE	0x01

int	validate_input(char **argv, int argc, t_addr *data);
void	init_memory(t_addr *addr_data);
int	retrieve_host_LAN_info(t_addr *addr_data);
int	set_arp_reply(t_addr *addr_data);

/* @parameters:										*/
/* 											*/
/*	<source ip> <source mac address> <target ip> <target mac address>		*/
/*											*/

int	main(int argc, char *argv[])
{
	t_addr addr_data;

	if (validate_input(argv, argc, &addr_data) == ERROR)
		return (EXIT_FAILURE);

	init_memory(&addr_data);

	if (retrieve_host_LAN_info(&addr_data) == ERROR)
		return (EXIT_FAILURE);

	if (set_arp_reply(&addr_data) == ERROR)
		return (EXIT_FAILURE);

	return (0);
}
