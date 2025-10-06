#include "libft.h"
#include "ft_malcolm.h"

int	init_arp_reply(t_addr **frame_data);
void	verbose_arp_req(t_addr *addr_data, uint16_t proto, ssize_t len);

/* send ARP reply */

static inline
ssize_t	send_arp_reply(t_addr *addr_data)
{
	ssize_t ret = sendto(addr_data->arp_soc, addr_data->buffer, ARP_PACKET_LEN, 0, \
				(struct sockaddr *) &addr_data->socket_address,\
				sizeof(addr_data->socket_address));
	if (ret == -1)
	{
		fprintf(stderr, "Failed to send a reply. sendto() error.\n");
		return (ERROR);
	}

	return (ret);
}

/* capture ARP packet;
 * return 0 in case of present ARP header
 * */

static
int	recv_arp_req(t_addr *addr_data)
{
	ssize_t len = recvfrom(addr_data->arp_soc, addr_data->buffer, \
				BUF_SIZE, 0, NULL, NULL);
	if (len == -1)
	{
		fprintf(stderr, "recvfrom() error.\n");
		return (ERROR);
	}
	uint16_t frame_header = htons(addr_data->eth_header->h_proto);

#if VERBOSE == 1
	verbose_arp_req(addr_data, frame_header, len);
#endif
	if (frame_header == ETH_P_ARP)
                return (0);
	return (2);
}

static
int	verify_arp_request(t_addr *addr_data)
{
	for (uint8_t i = 0; i < ETH_ALEN; ++i)
	{
		if (addr_data->mac_addr_target[i] != addr_data->arp_header->sender_mac[i])
		{
#if VERBOSE == 1
			char buffer[INET_ADDRSTRLEN];
			fprintf(stdout, "\n * ARP request of another device than target. *\n");
			print_MAC_addr(" sender's MAC ", ':', addr_data->arp_header->sender_mac);
			fprintf(stdout, " - IP address: %s\n", \
				inet_ntop(AF_INET, addr_data->arp_header->sender_ip, buffer, INET_ADDRSTRLEN));
#endif
			return (1);
		}
	}

	return (0);
}

int	set_arp_reply(t_addr *addr_data)
{
	/* Retrieve a socket descriptor */
	addr_data->arp_soc = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (addr_data->arp_soc == -1)
	{
		fprintf(stderr, "socket() error.\n");
		return (ERROR);
	}

#if DEBUG == 1
	fprintf(stdout, "Socket file descriptor retrived -> %d\n", addr_data->arp_soc);
#endif

	while (1)
	{
		fprintf(stdout, "\nWaiting target's ARP request ...\n\n");

		int ret = recv_arp_req(addr_data);
		if (ret == 0 && verify_arp_request(addr_data) == 0) {
			fprintf(stdout, "Success: ARP request has been received!\n");
			break;
		} else if (ret == -1) {
			close(addr_data->arp_soc);
			return (ERROR);
		}
		sleep(1);
	}

	char buffer[INET_ADDRSTRLEN];
	print_MAC_addr("   MAC address of request:", ':', 
				addr_data->arp_header->sender_mac);
	fprintf(stdout, "\n   IP address of request: %s\n", 
				inet_ntop(AF_INET, addr_data->arp_header->sender_ip, buffer, INET_ADDRSTRLEN));

	/* initialize ARP reply */
	if (init_arp_reply(&addr_data) == ERROR)
		return (ERROR);

	/* send ARP reply */
	while (send_arp_reply(addr_data) == ERROR)
		sleep(2);

	fprintf(stdout, "-> ARP reply has been sent. \n" \
			"You may now check the ARP table on the target machine.\n" \
			"Exiting program!\n");

	close(addr_data->arp_soc);

	return (0);
}
