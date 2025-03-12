#include "ft_malcolm.h"

	/* @parameters:
	 * 
		<source ip> <source mac address> <target ip> <target mac address>
	 *
	 */

int main(int argc, char *argv[]) {

	t_addr addr_data;
	printf("Validating input parameters ...\n");
	if (validate_input(argv, argc, &addr_data) == -1)
		return (EXIT_ERROR);
	printf("Entering initialiation phase ...\n");

	int soc = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (soc == -1) {
		perror("socket(): ");
		return (EXIT_ERROR);
	}
	printf("Success: socket opened -> %i\n", soc);

	struct ifreq ifr;
    ft_memset(&ifr, 0, sizeof(struct ifreq));

	int if_index; 
	if (retrieve_interface(&ifr, &if_index) == -1)
		return (EXIT_ERROR);

	if (if_index == -1)
		return (EXIT_ERROR);
	
	// unsigned char buffer[BUF_SIZE];
	// ft_memset(buffer, 0x0, BUF_SIZE);

	// struct ethhdr *send_req = (struct ethhdr *)buffer;
	// struct ethhdr *rcv_resp = (struct ethhdr *)buffer;
	// struct arp_header *arp_req = (struct arp_header *)(buffer+ETH2_HEADER_LEN);
	// struct arp_header *arp_resp = (struct arp_header *)(buffer+ETH2_HEADER_LEN);
	// struct sockaddr_ll socket_address;
	// int index,ret,length = 0;

	
	///// -----------------------------------------------------------------
	close(soc);
	
	return 0;
}
