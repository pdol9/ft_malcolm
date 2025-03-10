#include "ft_malcolm.h"

	/* @parameters:
	 * 
		<source ip> <source mac address> <target ip> <target mac address>
	 *
	 */

int main(int argc, char *argv[]) {

	t_addr addr_data;
	printf("Validating input parameters ...\n");
	if (validate_input(argv, argc, &addr_data) == 1)
		return (EXIT_ERROR);

	printf("Entering initialiation phase ...\n");

	unsigned char buffer[BUF_SIZE];
	struct ifreq ifr;

	ft_memset(buffer,0x0,60);
	int soc = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (soc == -1) {
		perror("socket( ):");
		return (EXIT_ERROR);
	}
	printf("Success: socket opened -> %i\n", soc);

	// TODO: retrieve interface name -> remove hardcoded line below
	char interface[16] = {0};
	ft_strlcpy(interface,"wlp2s0",7);
	ft_strlcpy(ifr.ifr_name, interface,7);
	
	close(soc);


	return 0;
}
