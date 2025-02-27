#include "malcolm.h"

static uint8_t *mac_pton(const char *str) {

	static uint8_t res[6] = {0};

	for (int idx = 0, len = 0; idx < 6; ++idx, ++len) {

		if (str[len] >='0' && str[len] <= '9')
			res[idx] = str[len] - '0' * 16;
		else if (str[len] >= 'a' && str[len] <= 'f')
			res[idx] = str[len] - 'a' * 16;
		else
			return NULL;
		len++;
		if (str[len] >='0' && str[len] <= '9')
			res[idx] += str[len] - '0';
		else if (str[len] >= 'a' && str[len] <= 'f')
			res[idx] += str[len] - 'a';
		else
			return NULL;
		len++;
		if (str[len] == ':' || len == 17)
			continue;
		else
			return NULL;
	}
	return res;
}

int validate_input(char **argv, int argc) {

	if (argc != 5) {
		fprintf(stderr, "Invalid number of parameters.\nProvide following:\n◦ source ip\n◦ source mac address\n◦ target ip\n◦ target mac address\n");
		return (EXIT_FAILURE);
	}

	// validate MAC addr
	if (ft_strlen(argv[2]) != 17 || ft_strlen(argv[4]) != 17) {
		fprintf(stderr, "Invalid MAC address. Use the following format: aa:bb:cc:dd:ee:ff\n");
	}

	static uint8_t *mac_addr_src, *mac_addr_target;
	if ((mac_addr_src = mac_pton(argv[2])) == NULL)
		fprintf(stderr, "3 - Invalid MAC address. Use valid format: aa:bb:cc:dd:ee:ff\nInvalid input: %s\n", argv[2]);


	// validate IPv4 addr
	int s, domain;
	char str[INET6_ADDRSTRLEN];
	unsigned char buf[sizeof(struct in6_addr)];

	for (int i = 1; i < 4; i += 2) {
		// IPv4
		domain = AF_INET;
		s = inet_pton(domain, argv[i], buf);
		if (s <= 0) {
			if (!s)
				fprintf(stderr, "Not in presentation format\n");
			else
				perror("inet_pton");
			exit(EXIT_FAILURE);
		}
		if (inet_ntop(AF_INET, buf, str, INET6_ADDRSTRLEN) == NULL) {
			perror("inet_ntop\n");
			exit(EXIT_FAILURE);
		}
		printf("IPv4: %s\n", str);
	}
}
