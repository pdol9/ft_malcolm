#include "malcolm.h"

int	mac_pton(const char *str, uint8_t mac[6]) {

	for (int idx = 0, len = 0; idx < 6; ++idx, ++len) {

		if (str[len] >='0' && str[len] <= '9')
			mac[idx] = (str[len] - '0') * 16;
		else if (str[len] >= 'a' && str[len] <= 'f')
			mac[idx] = (str[len] - 'a' + 10) * 16;
		else
			return -1;
		len++;
		if (str[len] >='0' && str[len] <= '9')
			mac[idx] += (str[len]) - '0';
		else if (str[len] >= 'a' && str[len] <= 'f')
			mac[idx] += (str[len] - 'a' + 10) % 16;
		else
			return -1;
		len++;
		if (str[len] == ':' || len == 17)
			continue;
		else
			return -1;
	}

	return 0;
}

int validate_input(char **argv, int argc, t_addr *data) {

	if (argc != 5) {
		fprintf(stderr, "Invalid number of parameters. Provide following arguments:\n <source ip> <source mac address> <target ip> <target mac address>\n");
		return (EXIT_FAILURE);
	}

	// validate MAC addr
	if (ft_strlen(argv[2]) != 17 || ft_strlen(argv[4]) != 17) {
		fprintf(stderr, "Invalid MAC address. Use the following format: aa:bb:cc:dd:ee:ff\n");
		return (EXIT_FAILURE);
	}

	uint8_t mac_addr_src[6];
	uint8_t mac_addr_target[6];

	if ((mac_pton(argv[2], mac_addr_src)) == -1) {
		fprintf(stderr, "Invalid MAC address. Use valid format: aa:bb:cc:dd:ee:ff\nInvalid input: %s\n", argv[2]);
		return (EXIT_FAILURE);
	} else if ((mac_pton(argv[4], mac_addr_target)) == -1) {
		fprintf(stderr, "Invalid MAC address. Use valid format: aa:bb:cc:dd:ee:ff\nInvalid input: %s\n", argv[4]);
		return (EXIT_FAILURE);
	}

	// validate IPv4 addr
	int s;
	char str[INET6_ADDRSTRLEN];
	unsigned char buf[sizeof(struct in6_addr)];

	for (int i = 1; i < 4; i += 2) {
		s = inet_pton(AF_INET, argv[i], buf);
		if (s <= 0) {
			if (!s)
				fprintf(stderr, "Not in presentation format\n");
			else
				perror("inet_pton");
			return (EXIT_FAILURE);
		}
		if (inet_ntop(AF_INET, buf, str, INET6_ADDRSTRLEN) == NULL) {
			perror("inet_ntop\n");
			return (EXIT_FAILURE);
		}
		printf("IPv4: %s\n", str);
	}
	return 0;
}
