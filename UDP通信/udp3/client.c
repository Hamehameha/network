#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main()
{
 int sock;
 struct sockaddr_in addr;
 in_addr_t ipaddr;
 struct ip_mreq mreq;

 sock = socket(AF_INET, SOCK_DGRAM, 0);

 addr.sin_family = AF_INET;
 addr.sin_port = htons(12345);
 addr.sin_addr.s_addr = inet_addr("239.192.168.76");

 ipaddr = inet_addr("192.168.76.200");
 if (setsockopt(sock,
		IPPROTO_IP,
		IP_MULTICAST_IF,
		(char *)&ipaddr, sizeof(ipaddr)) != 0) {
	perror("setsockopt");
	return 1;
 }

// マルチキャストグループに「参加（JOIN）」
 memset(&mreq, 0, sizeof(mreq));
 mreq.imr_interface.s_addr = inet_addr("192.168.74.155");
 mreq.imr_multiaddr.s_addr = inet_addr("239.192.168.76");

 if (setsockopt(sock,
	IPPROTO_IP,
	IP_ADD_MEMBERSHIP,
	(char *)&mreq, sizeof(mreq)) != 0) {
	perror("setsockopt IP_ADD_MEMBERSHIP");
	return 1;
 }

 sendto(sock, "HELLO from SH4", 15, 0, (struct sockaddr *)&addr, sizeof(addr));

 close(sock);

 return 0;
}


