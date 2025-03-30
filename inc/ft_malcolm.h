#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

# include <arpa/inet.h>
# include <ifaddrs.h>
# include <net/if.h>
# include <linux/if_ether.h>
# include <linux/if_link.h>
# include <linux/if_packet.h>
# include <linux/if_arp.h>
# include <sys/types.h>
# include <stdio.h>
# include "libft.h"

# define EXIT_ERROR		0x01

// ARP definitions
# define PROTO_ARP		0x0806
# define ARP_REQUEST	0x01
# define ARP_REPLY		0x02
# define BUF_SIZE		0x40

# define HW_TYPE		0x01
# define ETH_P_NULL		0x0
# define MAC_LENGTH		0x06
# define IPV4_LENGTH	0x04
# define ETH2_HEADER_LEN 0x0e

struct arp_header
{
	uint16_t	hardware_type;
	uint16_t	protocol_type;
	uint8_t		hardware_len;
	uint8_t		protocol_len;
	uint16_t	opcode;
	uint8_t		sender_mac[MAC_LENGTH];
	uint8_t		sender_ip[IPV4_LENGTH];
	uint8_t		target_mac[MAC_LENGTH];
	uint8_t		target_ip[IPV4_LENGTH];
};

//struct ethhdr {
//	unsigned char	h_dest[ETH_ALEN];	/* destination eth addr	*/
//	unsigned char	h_source[ETH_ALEN];	/* source ether addr	*/
//	__be16		h_proto;		/* packet type ID field	*/
//} __attribute__((packed));

typedef struct s_addr {
	struct arp_header	*arp_req;
	struct arp_header	*arp_resp;
	struct ethhdr		*send_req;
	struct ethhdr		*rcv_resp;
	struct sockaddr_ll	socket_address;
	uint8_t				buffer[BUF_SIZE];
	uint8_t				mac_addr_src[MAC_LENGTH];
	uint8_t				mac_addr_target[MAC_LENGTH];
	uint8_t				ifc_mac[MAC_LENGTH];
	uint8_t				ipv4_addr[2][sizeof(struct in_addr)];
	char				ipv4_name[2][INET_ADDRSTRLEN];
}	t_addr;

// function prototype
int		validate_input(char **argv, int argc, t_addr *data);
int		retrieve_interface_info(uint8_t ifc_mac[MAC_LENGTH]);
int		init_data(t_addr *addr_data);
int		recv_arp_repl(int arp_soc, t_addr *addr_data);
void	print_MAC_addr(const char *str, uint8_t arr[MAC_LENGTH]);
void	verbose_eth_package(t_addr *addr_data);
void	verbose_arp_package(t_addr *addr_data);

#endif
