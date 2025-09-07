#include "libft.h"
#include "ft_malcolm.h"

void	print_MAC_addr(const char *msg, char ch, uint8_t arr[MAC_LENGTH]);
int	init_arp_reply(t_addr **frame_data);
void	verbose_header_info(t_addr *addr_data);

/* send ARP reply */

static inline
int	send_arp_reply(t_addr *addr_data)
{
	ssize_t ret = sendto(addr_data->arp_soc, addr_data->buffer, 42, 0, \
				(struct sockaddr *) &addr_data->socket_address,\
				sizeof(addr_data->socket_address));
	if (ret == -1) {
		fprintf(stderr, "Failed to send a reply. sendto() error.\n");
		return (ERROR);
	}
	fprintf(stdout, "-> ARP reply has been sent.\n");

	return (0);
}

/* wait for ARP request */

static
int	recv_arp_reply(t_addr *addr_data)
{
	ssize_t len = recvfrom(addr_data->arp_soc, addr_data->buffer, \
				BUF_SIZE, 0, NULL, NULL);
	if (len == -1) {
		fprintf(stderr, "recvfrom() error.\n");
		return (ERROR);
	}
	if ((htons(addr_data->eth_header->h_proto)) != PROTO_ARP)
	{

#if DEBUG == 1
		int proto_value = ntohs(addr_data->eth_header->h_proto);
		fprintf(stdout, "Not an ARP packet.\nType: 0x%x", proto_value);
		fprintf(stdout, " DEBUG_INFO\n");
		verbose_header_info(addr_data);
		fprintf(stdout, " DEBUG_INFO\n");
#endif

		return (2);
	}

	return (0);
}

static
int	verify_arp_request(t_addr *addr_data)
{
	for (uint8_t i = 0; i < MAC_LENGTH; ++i)
	{
#if DEBUG == 1
		fprintf(stdout, "compare bytes: [%02x] [%02x]\n",addr_data->mac_addr_target[i], addr_data->arp_header->sender_mac[i]);
#endif
		if (addr_data->mac_addr_target[i] != addr_data->arp_header->sender_mac[i])
		{
#if DEBUG == 1
			fprintf(stdout, "\n * ARP request of another device than target. *\n");
			print_MAC_addr(" sender's MAC ", ':', addr_data->arp_header->sender_mac);
			print_MAC_addr(" Target machine - input ", ':', addr_data->mac_addr_target);
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
	if (addr_data->arp_soc == -1) {
		fprintf(stderr, "socket() error.\n");
		return (ERROR);
	}

#if DEBUG == 1
	fprintf(stdout, "Socket file descriptor retrived -> %d\n", addr_data->arp_soc);
#endif

	while (1)
	{
		fprintf(stdout, "Waiting target's ARP request ...\n");
		int ret = recv_arp_reply(addr_data);
		if (ret == 0 && verify_arp_request(addr_data) == 0) {
			fprintf(stdout, "Success: ARP request has been received!\n");
			break;
		} else if (ret == -1) {
			close(addr_data->arp_soc);
			return (ERROR);
		}
		sleep(2);
	}

	// set arp reply
	if (init_arp_reply(&addr_data) == ERROR)
		return (ERROR);
	// send reply in return
	if (send_arp_reply(addr_data) == ERROR)
		return (ERROR);

	close(addr_data->arp_soc);
	fprintf(stdout, "You may now check the ARP table on the target machine.\nExiting program!\n");

	return (0);
}
