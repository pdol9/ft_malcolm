#include "malcolm.h"

int main(int argc, char *argv[]) {

	/* @parameters:
	 * 
	 ◦ source ip
	 ◦ source mac address
	 ◦ target ip
	 ◦ target mac address
	 *
	 */
	if (argc != 5) {
		fprintf(stderr, "Invalid number of parameters.\nProvide following:\n◦ source ip\n◦ source mac address\n◦ target ip\n◦ target mac address\n");
		return (EXIT_FAILURE);
	}
	for (int i = 1; i < 5; ++i) {
		fprintf(stdout, "Accepted param: %d -> %s\n", i, argv[i]);
	}
	// TASKS:
	// wait for the ARP request (from target) requesting the source IP
	// send a single ARP reply
	//
	// TODO:
	// include libft as external dependency
	// get string input into valid bytes and reverse
	//
	return 0;
}
