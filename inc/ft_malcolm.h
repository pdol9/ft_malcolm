#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

# include <sys/ioctl.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <linux/if_packet.h>
# include <linux/if_ether.h>
# include <linux/if_arp.h>
# include <ifaddrs.h>

# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>

# include <stdlib.h>
# include <stdio.h>
# include <stdint.h>
# include "libft.h"

# include <string.h>	// remove

# define EXIT_ERROR		0x01

// ARP definitions
# define HW_TYPE		0x01
# define PROTOCOL_TYPE	0x800
# define PROTO_ARP		0x0806
# define ARP_REQUEST	0x01
# define ARP_REPLY		0x02
# define BUF_SIZE		0x40
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

typedef struct s_addr {
	uint8_t		mac_addr_src[6];
	uint8_t		mac_addr_target[6];
	uint8_t		ipv4_addr[2][sizeof(struct in_addr)];		// [0] -> src; [1] -> target
	char		ipv4_name[2][INET_ADDRSTRLEN];
}	t_addr;

// function prototype
int		validate_input(char **argv, int argc, t_addr *data);

#endif
