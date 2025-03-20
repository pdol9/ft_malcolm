#include "ft_malcolm.h"

/* @parameters:															*/
/* 																		*/
/*	<source ip> <source mac address> <target ip> <target mac address>   */
/*																		*/

int main(int argc, char *argv[]) {

	t_addr addr_data;
	struct ifreq ifr;
	int if_index;

	printf("Start of the program!\nValidating input parameters ...\n");
	if (validate_input(argv, argc, &addr_data) == -1)
		return (EXIT_ERROR);
	printf("Entering initialiation phase ...\n");

	ft_memset(&ifr, 0x0, sizeof(struct ifreq));
	if (retrieve_interface_info(&ifr, &if_index, addr_data.ifc_mac) == -1)
		return (EXIT_ERROR);
	if (if_index == -1)
		return (EXIT_ERROR);
	printf("Success: Interface index retrieved: -> %d\n", if_index);

	struct sockaddr_ll sll;

	ft_memset(&sll, 0x0, sizeof(struct sockaddr_ll));
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = if_index;

	struct sockaddr_ll socket_address;

	socket_address.sll_family = AF_PACKET;
	socket_address.sll_protocol = htons(ETH_P_ARP);
	socket_address.sll_ifindex = if_index;
	socket_address.sll_hatype = htons(ARPHRD_ETHER);
	socket_address.sll_pkttype = (PACKET_BROADCAST);
	socket_address.sll_halen = MAC_LENGTH;
	socket_address.sll_addr[6] = 0x0;
	socket_address.sll_addr[7] = 0x0;

	uint8_t buffer[BUF_SIZE];
	ft_memset(buffer, 0x0, sizeof(buffer));

	struct ethhdr *rcv_resp = (struct ethhdr *)buffer;
	struct arp_header *arp_resp = (struct arp_header *)(buffer + ETH2_HEADER_LEN);
	struct ethhdr *send_req = (struct ethhdr *)buffer;
	struct arp_header *arp_req = (struct arp_header *)(buffer + ETH2_HEADER_LEN);

	/* Broadcast request */
	ft_memset(send_req->h_dest, 0xff, MAC_LENGTH);

	/* Set target MAC */
	ft_memset(arp_req->target_mac, 0x0, MAC_LENGTH);

	/* Set provided MAC address as source */
	ft_memcpy(send_req->h_source, addr_data.ifc_mac, MAC_LENGTH);
	ft_memcpy(arp_req->sender_mac, addr_data.ifc_mac, MAC_LENGTH);
	ft_memcpy(socket_address.sll_addr, addr_data.ifc_mac, MAC_LENGTH);

	/* Set protocol of the packet */
	send_req->h_proto = htons(ETH_P_ARP);

	/* ARP request */
	arp_req->hardware_type = htons(HW_TYPE);
	arp_req->protocol_type = htons(ETH_P_IP);
	arp_req->hardware_len = MAC_LENGTH;
	arp_req->protocol_len = IPV4_LENGTH;
	arp_req->opcode = htons(ARP_REQUEST);

	/* Set source and target IP */
	for (int index = 0; index < 5; ++index)
	{
		arp_req->sender_ip[index]=addr_data.ipv4_addr[0][index];
		arp_req->target_ip[index]=addr_data.ipv4_addr[1][index];
	}

	/* Retrieve a raw socket descriptor */
	int arp_soc = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (arp_soc == -1) {
		perror("socket(): ");
		return (EXIT_ERROR);
	}
	printf("Success: Socket opened -> %d\n", arp_soc);
	ssize_t ret = sendto(arp_soc, buffer, 64, 0, (struct sockaddr *) &socket_address, sizeof(socket_address));
	if (ret == -1) {
		perror("sendto(): ");
		return (EXIT_ERROR);
	}
	ft_memset(buffer, 0x0, sizeof(buffer));
	while (1)
	{
		printf("Waiting for ARP reply ...\n");
		int ret = recv_arp_repl(arp_soc, buffer, arp_resp, rcv_resp);
		if (ret == 0) {
			printf("Success: ARP reply has been received!\n");
			break;
		} else if (ret == -1) {
			close(arp_soc);
			return (EXIT_ERROR);
		}
	}
	close(arp_soc);
	printf("Sent an ARP reply packet, you may now check the ARP table "
			"on the target.\nExiting program ...\n");
	return 0;
}

int	recv_arp_repl(int arp_soc,
					uint8_t buffer[BUF_SIZE], 
					struct arp_header *arp_resp,
					struct ethhdr *rcv_resp)
{
	(void)arp_resp;
	ssize_t len = recvfrom(arp_soc, buffer, BUF_SIZE, 0, NULL, NULL);
	if (len == -1) {
		perror("recvfrom(): ");
		return (-1);
	}
	if (htons(rcv_resp->h_proto) != PROTO_ARP) {
		fprintf(stderr, "Not an ARP packet.\n");
		return (1);
	}
	// TODO
	// --verbose
//	printf("Received ARP reply.\n");
//	printf(" Sender IP :");
//	printf("\n Sender MAC :");
//	printf("\n Receiver  IP :");
//	printf("\n Self MAC :");

	return (0);
}
