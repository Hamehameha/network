#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int
main()
{
	int sock1, sock2;
	struct sockaddr_in addr1, addr2;
	fd_set fds, readfds;
	char buf[2048];
	socklen_t sin_size;


	/* �t�@�C���f�B�X�N���v�^�̍ő�l���v�Z���܂��Bselect�Ŏg���܂� */
	int maxfd;

	/* ��M�\�P�b�g��2���܂� */
	sock1 = socket(AF_INET, SOCK_DGRAM, 0);
	sock2 = socket(AF_INET, SOCK_DGRAM, 0);

	addr1.sin_family = AF_INET;
	addr2.sin_family = AF_INET;

	addr1.sin_addr.s_addr = INADDR_ANY;
	addr2.sin_addr.s_addr = INADDR_ANY;

	/* 2�̕ʁX�̃|�[�g�ő҂��߂ɕʂ̃|�[�g�ԍ������ꂼ��ݒ肵�܂� */
	addr1.sin_port = htons(11111);
	addr2.sin_port = htons(22222);

	/* 2�̕ʁX�̃|�[�g�ő҂悤��bind���܂� */
	bind(sock1, (struct sockaddr *)&addr1, sizeof(addr1));
	bind(sock2, (struct sockaddr *)&addr2, sizeof(addr2));

	/* fd_set�̏��������܂� */
	 FD_ZERO(&readfds);

	/* select�ő҂ǂݍ��݃\�P�b�g�Ƃ���sock1��o�^���܂� */
	FD_SET(sock1, &readfds);
	/* select�ő҂ǂݍ��݃\�P�b�g�Ƃ���sock2��o�^���܂� */
	FD_SET(sock2, &readfds);

	/* select�ŊĎ�����t�@�C���f�B�X�N���v�^�̍ő�l���v�Z���܂� */
	if (sock1 > sock2) {
		maxfd = sock1;
	} else {
		maxfd = sock2;
	}

	sin_size = sizeof(struct sockaddr_in); 

	/*
	�������[�v�ł�
	���̃T���v���ł́A���̖������[�v�𔲂��܂���
	*/
	
	while (1) {
		/* �ǂݍ��ݗpfd_set�̏����� */
		/* select��������e���㏑�����Ă��܂��̂ŁA���񏉊������܂� */
		memcpy(&fds, &readfds, sizeof(fd_set));

		/* fds�ɐݒ肳�ꂽ�\�P�b�g���ǂݍ��݉\�ɂȂ�܂ő҂��܂� */
		/* ��ڂ̈����̓t�@�C���f�B�X�N���v�^�̍ő�l�{�P�ɂ��܂� */
		select(maxfd+1, &fds, NULL, NULL, NULL);

		/* sock1�ɓǂݍ��݉\�f�[�^������ꍇ */
		if (FD_ISSET(sock1, &fds)) {
			/* sock1����f�[�^����M���ĕ\�����܂� */
			memset(buf, 0, sizeof(buf));
			//recv(sock1, buf, sizeof(buf), 0);
			recvfrom(sock1, buf, sizeof(buf), 0, (struct sockaddr *)&addr1, &sin_size);

			printf("%s\n", buf);
		}

		/* sock2�ɓǂݍ��݉\�f�[�^������ꍇ */
		if (FD_ISSET(sock2, &fds)) {
			/* sock2����f�[�^����M���ĕ\�����܂� */
			memset(buf, 0, sizeof(buf));
			//recv(sock2, buf, sizeof(buf), 0);
			recvfrom(sock2, buf, sizeof(buf), 0, (struct sockaddr *)&addr2, &sin_size);

			printf("%s\n", buf);
		}
	}

	/* ���̃T���v���ł́A�����ւ͓��B���܂��� */
	close(sock1);
	close(sock2);

	return 0;
}

