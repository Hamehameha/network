#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int
main()
{
	int sock0;
	struct sockaddr_in addr;
	struct sockaddr_in client;
	int len;
	int sock;

	/* �\�P�b�g�̍쐬 */
	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	/* �\�P�b�g�̐ݒ� */
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = INADDR_ANY;
	bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

	/* TCP�N���C�A���g����̐ڑ��v����҂Ă��Ԃɂ��� */
	listen(sock0, 5);

	/* TCP�N���C�A���g����̐ڑ��v�����󂯕t���� */
	len = sizeof(client);
	sock = accept(sock0, (struct sockaddr *)&client, &len);

	/* 5�������M */
	send(sock, "HELLO", 5, 0);

	/* �N���C�A���g����ɕ���̂�҂� */
	sleep(1);
	
	/* TCP�Z�b�V�����̏I�� */
	close(sock);

	/* listen ����socket�̏I�� */
	close(sock0);
	return 0;
}



