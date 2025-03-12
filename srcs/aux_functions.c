#include "ft_malcolm.h"

static int	mac_pton(const char *str, uint8_t mac[6])
{
	for (int idx = 0, len = 0; idx < 6; ++idx, ++len) {

		if (str[len] >='0' && str[len] <= '9')
			mac[idx] = (str[len] - '0') * 16;
		else if (str[len] >= 'a' && str[len] <= 'f')
			mac[idx] = (str[len] - 'a' + 10) * 16;
		else
			return (-1);
		len++;
		if (str[len] >='0' && str[len] <= '9')
			mac[idx] += (str[len]) - '0';
		else if (str[len] >= 'a' && str[len] <= 'f')
			mac[idx] += (str[len] - 'a' + 10) % 16;
		else
			return (-1);
		len++;
		if (str[len] == ':' || len == 17)
			continue;
		else
			return (-1);
	}
	return (0);
}

int validate_input(char **argv, int argc, t_addr *data)
{
	if (argc != 5) {
		fprintf(stderr, "Invalid number of parameters. Provide following arguments:\n <source ip> <source mac address> <target ip> <target mac address>\n");
		return (-1);
	}

	// validate MAC input
	if (ft_strlen(argv[2]) != 17 || ft_strlen(argv[4]) != 17) {
		fprintf(stderr, "Invalid MAC address. Use the following format: aa:bb:cc:dd:ee:ff\n");
		return (-1);
	}

	// validate MAC addr
	if ((mac_pton(argv[2], data->mac_addr_src)) == EXIT_ERROR) {
		fprintf(stderr, "Invalid MAC address. Use valid format: aa:bb:cc:dd:ee:ff\nInvalid input: %s\n", argv[2]);
		return (-1);
	} else if ((mac_pton(argv[4], data->mac_addr_target)) == EXIT_ERROR) {
		fprintf(stderr, "Invalid MAC address. Use valid format: aa:bb:cc:dd:ee:ff\nInvalid input: %s\n", argv[4]);
		return (-1);
	}

	// validate IPv4 addr
	int s;

	for (int i = 1; i < 4; i += 2) {
		int j = i / 3;				// set source or target (addr)
		s = inet_pton(AF_INET, argv[i], data->ipv4_addr[j]);
		if (s <= 0) {
			if (!s)
				fprintf(stderr, "Not in presentation format\n");
			else
				perror("inet_pton");
			return (-1);
		}
		if (inet_ntop(AF_INET, data->ipv4_addr[j], data->ipv4_name[j], INET_ADDRSTRLEN) == NULL) {
			perror("inet_ntop\n");
			return (-1);
		}
		printf("IPv4: %s\n", data->ipv4_name[j]);
	}
	return (0);
}