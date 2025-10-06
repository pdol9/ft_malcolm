#include "libft.h"
#include "ft_malcolm.h"

/* verbose output of host level LAN information */

void	display_LAN_info(int iface_count, struct iface_info *infos, char *ipv_type)
{
	for (int i = 0; i < iface_count; ++i)
	{
		if (infos[i].is_up == false && infos[i].has_ipv6 == true)
			continue ;
		fprintf(stdout, "   Interface %-16s ", infos[i].ifc_name);
		if (infos[i].is_up == false)
			fprintf(stdout, "%-34s", "(IP):   N/A - status: [DOWN]");
		else if (ft_strncmp(ipv_type, "IPv4", 4) == 0 && infos[i].has_ipv4)
			fprintf(stdout, "(IPv4): %-26s", infos[i].ipv4);
		else if (ft_strncmp(ipv_type, "IPv6", 4) == 0 && infos[i].has_ipv6)
			fprintf(stdout, "(IPv6): %-26s", infos[i].ipv6);
		else
			fprintf(stdout, "%-34s", "(IP):   N/A");
		print_MAC_addr(" :: MAC: ", ':', infos[i].mac);
		fprintf(stdout, "\n");
	}
}

/* smaller helper function for verbose output */

static inline
void print_sender_info(t_addr *addr_data, ssize_t len)
{
	    fprintf(stdout, "Packet captured: %ld bytes\n", len);
	    print_MAC_addr("Src MAC: ", ':', addr_data->arp_header->sender_mac);
	    fprintf(stdout, "\n");
	    print_MAC_addr("Dst MAC: ", ':', addr_data->arp_header->target_mac);
	    fprintf(stdout, "\n");
}

/* print debugging info of received/captured ARP packet when compilation option is enabled */

void	verbose_arp_req(t_addr *addr_data, uint16_t proto, ssize_t len)
{
	if (proto >= 0x0600)
	{
            // Ethernet II
	    print_sender_info(addr_data, len);
	    fprintf(stdout, "EtherType: 0x%04x ", proto);

            switch (proto)
	    {
                case ETH_P_ARP:	fprintf(stdout, "(ARP)\n"); 
				fprintf(stdout, "    ARP Opcode: %s\n", (ntohs(addr_data->arp_header->opcode) == ARPOP_REQUEST) ? "Request" : "Reply");
				print_MAC_addr("    ARP Sender MAC:", ':', addr_data->arp_header->sender_mac);
				fprintf(stdout, "\n    ARP Sender IP : %s\n", inet_ntoa(*((struct in_addr *)addr_data->arp_header->sender_ip)));
				print_MAC_addr("    ARP Target MAC:", ':', addr_data->arp_header->target_mac);
				fprintf(stdout, "\n    ARP Target IP : %s\n", inet_ntoa(*((struct in_addr *)addr_data->arp_header->target_ip)));
				break;
                case ETH_P_IP:	fprintf(stdout, "(IPV_4)\n");
				fprintf(stdout, "     IPv4 Src IP: %s\n", inet_ntoa(*((struct in_addr *)addr_data->arp_header->sender_ip)));
				fprintf(stdout, "     IPv4 Dst IP: %s\n", inet_ntoa(*((struct in_addr *)addr_data->arp_header->target_ip)));
				break;
                case ETH_P_IPV6:fprintf(stdout, "(IPV_6)");
				break;
                default:	fprintf(stdout, "(Unknown)"); break;
            }
        }
	// IEEE 802.3 length field
	else
		fprintf(stdout, "IEEE 802.3 frame\nLength Indicator: %u bytes. ", proto);
}
