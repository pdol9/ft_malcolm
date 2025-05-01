#include "ft_malcolm.h"

int	broadcast(t_addr *addr_data)
{
	/* Retrieve a raw socket descriptor */
	addr_data->arp_soc = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (addr_data->arp_soc == -1) {
		perror("socket(): ");
		return (ERROR);
	}

#if VERBOSE == 1
	printf("Success: Socket opened -> %d\n", addr_data->arp_soc);
#endif

	ssize_t ret = sendto(addr_data->arp_soc, addr_data->buffer, 42, 0, \
				(struct sockaddr *) &addr_data->socket_address,\
				sizeof(addr_data->socket_address));
	if (ret == -1) {
		perror("Failed to broadcast.\nsendto(): ");
		return (ERROR);
	}
	printf("-> ARP request has been broadcast.\n"
			"Now sending an ARP reply to the target address with "
			"spoofed source, please wait ...\n");
	ft_memset(addr_data->buffer, 0x0, sizeof(addr_data->buffer));

	return (0);
}

static int	recv_arp_repl(t_addr *addr_data)
{
	ssize_t len = recvfrom(addr_data->arp_soc, addr_data->buffer, \
				BUF_SIZE, 0, NULL, NULL);
	if (len == -1) {
		perror("recvfrom(): ");
		return (ERROR);
	}
	if ((htons(addr_data->eth_header->h_proto)) != PROTO_ARP)
	{

#if VERBOSE == 1
		fprintf(stderr, "Not an ARP packet.\nType: %u -- ", \
				ntohs(addr_data->eth_header->h_proto));

		printf("\n DEBUG_INFO\n");
		verbose_header_info(addr_data);
		printf("\n DEBUG_INFO\n");
#endif

		return (2);
	}

	return (0);
}

int	arp_reply(t_addr *addr_data)
{
	while (1)
	{
		//printf("Waiting for ARP reply ...\n");
		int ret = recv_arp_repl(addr_data);
		if (ret == 0) {
			printf("Success: ARP reply has been received!\n");
			break;
		} else if (ret == -1) {
			close(addr_data->arp_soc);
			return (ERROR);
		}
	}

#if VERBOSE == 1
	verbose_header_info(addr_data);
#endif

	close(addr_data->arp_soc);
	printf("You may now check the ARP table on the target.\nExiting program ...\n");

	return (0);
}
