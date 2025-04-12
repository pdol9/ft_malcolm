#include "ft_malcolm.h"

inline
void	print_MAC_addr(const char *str, char ch, uint8_t arr[MAC_LENGTH])
{
	printf("%s", str);
	int i = 0;
	for (; i < MAC_LENGTH - 1; ++i)
		printf("%02x%c", (arr[i]), ch);
	printf("%02x", (arr[i]));
}

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
	if (argc != 5) {
		fprintf(stderr, "Invalid number of parameters. Provide following"
				" arguments:\n <source ip> <source mac address>"
				" <target ip> <target mac address>\n");
		return (ERROR);
	}
	printf("Start of the program!\nValidating input parameters ...\n");

	/* validate MAC input */
	if (ft_strlen(argv[2]) != 17 || ft_strlen(argv[4]) != 17) {
		fprintf(stderr, "Invalid MAC address. Use the following format:"
				" aa:bb:cc:dd:ee:ff\n");
		return (ERROR);
	}

	/* validate MAC address */
	if ((mac_pton(argv[2], data->mac_addr_src) == ERROR)
		|| (mac_pton(argv[4], data->mac_addr_target)) == ERROR) {
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
			if (!s) fprintf(stderr, "IP address not in presentation format\n");
			else perror("IP address - inet_pton: ");
			return (ERROR);
		}
		if (inet_ntop(AF_INET, data->ipv4_addr[j], data->ipv4_name[j], INET_ADDRSTRLEN) == NULL) {
			perror("IP address - inet_pton: ");
			return (ERROR);
		}
	}
	return (0);
}
