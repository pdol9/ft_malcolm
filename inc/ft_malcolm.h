#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

# include <netdb.h>
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

# define ERROR			0x01
# define MAX_IFACES		32

// ARP definitions
# define PROTO_ARP		0x0806
# define ARP_REQUEST		0x01
# define BUF_SIZE		60

# define HW_TYPE		0x01
# define MAC_LENGTH		6
# define IPV4_LENGTH		4
# define ETH_HEADER_LEN		14

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

struct iface_info {
	char		name[IFNAMSIZ];
	char		ipv4[INET_ADDRSTRLEN];
	char		ipv6[INET6_ADDRSTRLEN];
	uint8_t		mac[6];
	bool		has_ipv4;
	bool		has_ipv6;
	bool		is_up;
};

typedef struct s_addr {
	struct arp_header	*arp_header;
	struct ethhdr		*eth_header;
	struct sockaddr_ll	socket_address;
	uint8_t			buffer[BUF_SIZE];
	uint8_t			mac_addr_src[MAC_LENGTH];
	uint8_t			mac_addr_target[MAC_LENGTH];
	uint8_t			ifc_mac[MAC_LENGTH];
	uint8_t			ipv4_addr[2][sizeof(struct in_addr)];
	char			ipv4_name[2][INET_ADDRSTRLEN];
	int			arp_soc;
}	t_addr;

// function prototype
int	validate_input(char **argv, int argc, t_addr *data);
int	retrieve_LAN_info(t_addr *addr_data);
int	broadcast(t_addr *addr_data);
int	init_data(t_addr *addr_data);
int	arp_reply(t_addr *addr_data);
void	print_MAC_addr(const char *str, char ch, uint8_t arr[MAC_LENGTH]);
void	verbose_header_info(t_addr *addr_data);
void	display_LAN_info(int iface_count, struct iface_info *infos, char *ipv_type);

#endif
