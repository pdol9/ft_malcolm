#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	/* @parameters:
	 * 
	 ◦ source ip
	 ◦ source mac address
	 ◦ target ip
	 ◦ target mac address
	 * */
	if (argc != 4) {
		fprintf(stderr, "Invalid number of parameters.\nProvide following:\n◦ source ip\n◦ source mac address\n◦ target ip\n◦ target mac address\n");
	}
	return 0;
}
