#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

/* ARP predefined macros */

//# define PACKET_BROADCAST	0x01			/* To all	*/
//# define ARPOP_REPLY		0x02
//# define ARPHRD_ETHER		0x01
//# define ETH_P_ARP		0x0806
//# define ETH_P_IP		0x0800
//# define ETH_P_IPV6		0x86DD
//# define ETH_ALEN		6			/* Octets/length in one ethernet addr	 */
//# define ETH_HLEN		14			/* Total octets in header.	 */

/* personal macros */

# define HW_TYPE		ARPHRD_ETHER
# define MAC_LENGTH		ETH_ALEN
# define IPV4_LENGTH		sizeof (struct in_addr)
# define BUF_SIZE		60
# define ARP_PACKET_LEN		42
# define MAX_IFACES		32

# define ERROR			0x01

# include <stdio.h>
# include <stdint.h>
# include <stdbool.h>

# include <arpa/inet.h>	
# include <net/if.h>
# include <netdb.h>
# include <ifaddrs.h>
# include <linux/if_link.h>
# include <linux/if_packet.h>
# include <linux/if_ether.h>
# include <linux/if_arp.h>

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

struct iface_info
{
	char		ifc_name[IFNAMSIZ];
	char		ipv4[INET_ADDRSTRLEN];
	char		ipv6[INET6_ADDRSTRLEN];
	uint8_t		mac[MAC_LENGTH];
	bool		has_ipv4;
	bool		has_ipv6;
	bool		is_up;
};

typedef struct s_addr
{
	struct arp_header	*arp_header;
	struct ethhdr		*eth_header;
	struct sockaddr_ll	socket_address;
	uint8_t			buffer[BUF_SIZE];
	uint8_t			mac_addr_src[MAC_LENGTH];
	uint8_t			mac_addr_target[MAC_LENGTH];
	uint8_t			ipv4_addr[2][sizeof(struct in_addr)];
	char			ipv4_name[2][INET_ADDRSTRLEN];
	int			arp_soc;
	uint8_t			idx_active_ifc;
}	t_addr;

// common functions

void	print_MAC_addr(const char *msg, const char ch, const uint8_t arr[MAC_LENGTH]);
void	verbose_header_info(t_addr *addr_data);

#endif
