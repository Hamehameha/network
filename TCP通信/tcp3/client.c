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
	struct timeval tv;		// タイムアウト用
	
	int n;
	int i;

	/* ソケットの作成 */
	sock = socket(AF_INET, SOCK_STREAM, 0);

	/* 接続先指定用構造体の準備 */
	server.sin_family = AF_INET;
	server.sin_port = htons(12345);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");

	/* サーバに接続 */
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0){
		perror("connect(client)");
		return 1;
	}
	
	/* fd_setの初期化します */
	 FD_ZERO(&readfds);

	/* selectで待つ読み込みソケットとしてsockを登録します */
	FD_SET(sock, &readfds);

	/* 100mS でタイムアウトするようにします */
	tv.tv_sec = 0;
	tv.tv_usec = 100000;



	// サーバとの送受信を複数回おこないます。		
	for (i=0; i<10; i++){
	
		// 受信できるまで送信をリトライします
		while (1) {
			
			/* データを返信 */
			sprintf(buf,": %d: AAAAAAAA", i);
			printf("%s --> ",buf);
			n = send(sock, buf, strlen(buf), 0);
			
			if (n <= 0) {
				perror("send(client)");
				return 1;
			}

			/* 読み込み用fd_setの初期化 */
			/* selectが毎回内容を上書きしてしまうので、毎回初期化します */
			memcpy(&fds, &readfds, sizeof(fd_set));

			/* fdsに設定されたソケットが読み込み可能になるか、タイムアウトになるまで待ちます */
			/* 一つ目の引数はファイルディスクリプタの最大値＋１にします */
			select(sock+1, &fds, NULL, NULL, &tv);

			/* sockに読み込み可能データがある場合 */
			if (FD_ISSET(sock, &fds)) 
				break;
			
			// 受信なし
			printf(".");
			
			sleep(2);	// 再送までの時間待ち
		}

		/* サーバからデータを受信 */
		memset(buf, 0, sizeof(buf));
		n = recv(sock, buf, sizeof(buf), 0);

		printf(" %s\n", buf);
		
		//sleep(i);	// 送信待ち（４回目で送信エラー）
		
	}

	/* socketの終了 */
	close(sock);

	return 0;
}

