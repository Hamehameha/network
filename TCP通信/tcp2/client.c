#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int
main()
{
	struct sockaddr_in server;
	int sock;
	char buf[32];
	int n;
	int i;



	for (i=0; i<10; i++){

		/* �\�P�b�g�̍쐬 */
		sock = socket(AF_INET, SOCK_STREAM, 0);

		/* �ڑ���w��p�\���̂̏��� */
		server.sin_family = AF_INET;
		server.sin_port = htons(12345);
		server.sin_addr.s_addr = inet_addr("127.0.0.1");

		/* �T�[�o�ɐڑ� */
		if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0){
			perror("connect(client)");
			return 1;
		}

		/* �f�[�^��ԐM */
		sprintf(buf,"%d : AAAAAAAA", i);
		printf("%s --> ",buf);
		n = send(sock, buf, strlen(buf), 0);
		if (n < 0) {
			perror("send(client)");
			return 1;
		}
		sleep(1);

		/* �T�[�o����f�[�^����M */
		memset(buf, 0, sizeof(buf));
		n = recv(sock, buf, sizeof(buf), 0);

		printf(" %s\n", buf);

		/* socket�̏I�� */
		close(sock);

	}


	return 0;
}





