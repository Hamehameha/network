//
//  sh4-linux-gcc -o client client.c
//
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
	
	fd_set fds, readfds;
	struct timeval tv;		// �^�C���A�E�g�p
	
	int n;
	int i;

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
	
	/* fd_set�̏��������܂� */
	 FD_ZERO(&readfds);

	/* select�ő҂ǂݍ��݃\�P�b�g�Ƃ���sock��o�^���܂� */
	FD_SET(sock, &readfds);

	/* 100mS �Ń^�C���A�E�g����悤�ɂ��܂� */
	tv.tv_sec = 0;
	tv.tv_usec = 100000;



	// �T�[�o�Ƃ̑���M�𕡐��񂨂��Ȃ��܂��B		
	for (i=0; i<10; i++){
	
		// ��M�ł���܂ő��M�����g���C���܂�
		while (1) {
			
			/* �f�[�^��ԐM */
			sprintf(buf,": %d: AAAAAAAA", i);
			printf("%s --> ",buf);
			n = send(sock, buf, strlen(buf), 0);
			
			if (n <= 0) {
				perror("send(client)");
				return 1;
			}

			/* �ǂݍ��ݗpfd_set�̏����� */
			/* select��������e���㏑�����Ă��܂��̂ŁA���񏉊������܂� */
			memcpy(&fds, &readfds, sizeof(fd_set));

			/* fds�ɐݒ肳�ꂽ�\�P�b�g���ǂݍ��݉\�ɂȂ邩�A�^�C���A�E�g�ɂȂ�܂ő҂��܂� */
			/* ��ڂ̈����̓t�@�C���f�B�X�N���v�^�̍ő�l�{�P�ɂ��܂� */
			select(sock+1, &fds, NULL, NULL, &tv);

			/* sock�ɓǂݍ��݉\�f�[�^������ꍇ */
			if (FD_ISSET(sock, &fds)) 
				break;
			
			// ��M�Ȃ�
			printf(".");
			
			sleep(2);	// �đ��܂ł̎��ԑ҂�
		}

		/* �T�[�o����f�[�^����M */
		memset(buf, 0, sizeof(buf));
		n = recv(sock, buf, sizeof(buf), 0);

		printf(" %s\n", buf);
		
		//sleep(i);	// ���M�҂��i�S��ڂő��M�G���[�j
		
	}

	/* socket�̏I�� */
	close(sock);

	return 0;
}

