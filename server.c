#include "clientAPI.h"

int server_fd;

int main(int argc, char *argv[])
{
	struct sockaddr_in addr;
	int cfd;
	int opt = 1;

	// Ensure proper usage
	if (argc < 3 || strcmp(argv[1], "--help") == 0)
	{
		printf("Usage: %s <IP_ADDRESS> <PORT>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Create the socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("SERVER: socket creation failed (%s)\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	// set socket options
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	// bind the socket to the provided IP and port
	addr.sin_family = AF_INET;
	if (inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0)
	{
		printf("SERVER: Invalid IP address (%s)\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	addr.sin_port = htons(atoi(argv[2]));
	if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		printf("SERVER: bind failed (%s)\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	// listen for incoming connections
	if (listen(server_fd, BACKLOG) == -1)
	{
		printf("SERVER: listen failed (%s)\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("SERVER: Listening on %s:%s...\n", argv[1], argv[2]);

	for (;;)
	{
		// accept a connection
		cfd = accept(server_fd, NULL, NULL);
		if (cfd == -1)
		{
			printf("SERVER: accept failed (%s)\n", strerror(errno));
			continue;
		}

		printf("SERVER: Connection accepted\n");

		// handle the client
		handle_client(cfd);
	}

	return 0;
}
