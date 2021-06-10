#include <libgen.h>	// for basename()
#include <sys/un.h> // for 

int main(int argc, char* argv[])
{
	if (argc <= 2)
	{
		// handle the cmdline parameters
		printf("Usage: %s ip_address port_number\n", basename(argv[0]));
		return 1;
	}
	
	// set address and port
	const char* ip = argv[1];
	int port = atoi(argv[2]);

	struct sockaddr_in address;


	return 0;
}