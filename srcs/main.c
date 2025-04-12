#include "ft_malcolm.h"

/* @parameters:										*/
/* 											*/
/*	<source ip> <source mac address> <target ip> <target mac address>		*/
/*											*/

int main(int argc, char *argv[]) {

	t_addr addr_data;

	if (validate_input(argv, argc, &addr_data) == ERROR)
		return (EXIT_FAILURE);

	if (init_data(&addr_data) == ERROR)
		return (EXIT_FAILURE);

	if (broadcast(&addr_data) == ERROR)
		return (EXIT_FAILURE);

	if (arp_reply(&addr_data) == ERROR)
		return (EXIT_FAILURE);

	return 0;
}
