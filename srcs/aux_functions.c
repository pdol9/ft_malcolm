#include "libft.h"
#include "ft_malcolm.h"

/* print MAC address to the standard output */

inline
void	print_MAC_addr(const char *msg, char ch, uint8_t arr[MAC_LENGTH])
{
	fprintf(stdout, "%s %02x%c%02x%c%02x%c%02x%c%02x%c%02x",
			msg, arr[0],ch, arr[1],ch, arr[2],ch, arr[3],ch, arr[4],ch, arr[5]);
}

/* convert MAC address from text to binary form */

static
int	mac_pton(const char *str, uint8_t mac[6])
{
	for (int idx = 0, len = 0; idx < 6; ++idx, ++len)
	{
		if (str[len] >='0' && str[len] <= '9')
			mac[idx] = (str[len] - '0') * 16;
		else if (str[len] >= 'a' && str[len] <= 'f')
			mac[idx] = (str[len] - 'a' + 10) * 16;
		else
			return (ERROR);
		len++;
		if (str[len] >='0' && str[len] <= '9')
			mac[idx] += (str[len]) - '0';
		else if (str[len] >= 'a' && str[len] <= 'f')
			mac[idx] += (str[len] - 'a' + 10) % 16;
		else
			return (ERROR);
		len++;
		if (str[len] == ':' || len == 17)
			continue;
		else
			return (ERROR);
	}
	return (0);
}

int validate_input(char **argv, int argc, t_addr *data)
{
	if (argc != 5)
	{
		fprintf(stderr, "Invalid number of parameters. Provide following"
				" arguments:\n <source IP> <source MAC address>"
				" <target IP> <target MAC address>\n");
		return (ERROR);
	}

#if DEBUG == 1
	fprintf(stdout, "** Debug mode enabled. **\n");
#endif

#if VERBOSE == 1
	fprintf(stdout, "** Verbose mode enabled. **\n");
#endif
	fprintf(stdout, "Start of the program!\nValidating input parameters ...\n");

	//TODO: check error testing
	/* validate MAC format */
	if (ft_strlen(argv[2]) != 17 || ft_strlen(argv[4]) != 17)
	{
		fprintf(stderr, "Invalid MAC address. Use the following format:"
				" aa:bb:cc:dd:ee:ff\n");
		return (ERROR);
	}

	/* validate MAC address */
	if ((mac_pton(argv[2], data->mac_addr_src) == ERROR)
		|| (mac_pton(argv[4], data->mac_addr_target)) == ERROR)
	{
		fprintf(stderr, "Invalid MAC address. Use valid format: "
				"aa:bb:cc:dd:ee:ff\n");
		return (ERROR);
	}

	/* validate IPv4 address */
	int s;

	for (int i = 1; i < 4; i += 2)
	{
		// set source / target address
		int j = i / 3;
		s = inet_pton(AF_INET, argv[i], data->ipv4_addr[j]);
		if (s <= 0)
		{
			fprintf(stderr, "unknown host or invalid IP address: (%s)\n", argv[i]);
			return (ERROR);
		}
		if (inet_ntop(AF_INET, data->ipv4_addr[j], data->ipv4_name[j], INET_ADDRSTRLEN) == NULL)
		{
			fprintf(stderr, "unknown host or invalid IP address: (%s)\n", argv[i]);
			return (ERROR);
		}
	}
	return (0);
}
