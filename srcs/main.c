#include "malcolm.h"

	/* @parameters:
	 * 
	 ◦ source ip
	 ◦ source mac address
	 ◦ target ip
	 ◦ target mac address
	 *
	 */

int main(int argc, char *argv[]) {

	if (validate_input(argv, argc) == 1)
		return (EXIT_FAILURE);

	// TASKS:
	// wait for the ARP request (from target) requesting the source IP
	// send a single ARP reply
	//

	// TODO:
	// * go thru available interfaces
	// * listen for broadcasted (ARP) requests
	//
	return 0;
}
