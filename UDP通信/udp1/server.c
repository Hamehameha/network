#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int
main()
{
	int sock;
	struct sockaddr_in addr;
	char buf[2048];
	socklen_t sin_size;

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = INADDR_ANY;

	bind(sock, (struct sockaddr *)&addr, sizeof(addr));

	memset(buf, 0, sizeof(buf));

	// recvf(sock, buf, sizeof(buf), 0);
	sin_size = sizeof(struct sockaddr_in); 
	recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &sin_size);

	printf("%s\n", buf);

	close(sock);
	return 0;
}
