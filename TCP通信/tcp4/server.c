//
//  $ sh4-linux-gcc -o server server.c -lpthread
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

struct clientdata {
	int sock;
	struct sockaddr_in saddr;
};

void *threadfunc(void *data)
{
	int sock;
	struct clientdata *cdata = data;
	char buf[1024];
	int n;
	
	fd_set fds, readfds;
	
	struct timeval tv;
	
	printf("Start Server thread\n");
	
	// 引数が設定されてない場合
	if (data == NULL) {
		return(0);
	}
	
	/* acceptで生成した通信用ソケットを使用 */
	sock = cdata->sock;	
	
	/* fd_setの初期化します */
	FD_ZERO(&readfds);

	/* selectで待つ読み込みソケットとしてsockを登録します */
	FD_SET(sock, &readfds);
	
		
	while(1){
		// タイムアウト5秒
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		
		/* 読み込み用fd_setの初期化 */
		/* selectが毎回内容を上書きしてしまうので、毎回初期化します */
		memcpy(&fds, &readfds, sizeof(fd_set));
		
		/* 一つ目の引数はファイルディスクリプタの最大値＋１にします */
		select(sock+1, &fds, NULL, NULL, &tv);
		
		/* sockに読み込み可能データがある場合 */
		if (FD_ISSET(sock, &fds)) {
			/* sockからデータを受信して表示します */
			memset(buf, 0, sizeof(buf));
			
			/* クライアントからの送信データを読み出し */
			// 注意：クライアントから閉じると0バイとの受信が発生
			n = recv(sock, buf, sizeof(buf), 0);
			
			if (n <= 0) {
				perror("recv(server)");
				goto err;
			}
		
			/* クライアントからの送信データをそのまま返信 */
			n =send(sock, buf, n, 0);
			if (n <= 0) {
				perror("send(server)");
				goto err;
			}
		
		}else{	// タイムアウトの場合（５秒間）
			printf("recv timeout(5 S) !!\n");
			fflush(stdout);
			sleep(1);
			break;
		}
		
	}

err:
	/* TCPセッションの終了 */
	printf("server thread: close socket\n");
	fflush(stdout);

	close(sock);
	free(data);

	return(0);
}


int main()
{
	int sock0;
	struct sockaddr_in addr;
	socklen_t len;
	pthread_t th;
	struct clientdata *cdata;
	
	//スレッドの終了でリソースを開放させるためデタッチスレッドとする。
	pthread_attr_t attr;	// pthread属性用
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED );	// デタッチスレッド

	/* 受付準備 */
	/* 待ち受け用ソケットの作成 */
	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	/* 待ち受け用ソケットの設定 */
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = INADDR_ANY;

	bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

	/* TCPクライアントからの接続要求を待てる状態にする */
	listen(sock0, 5);
	/*  受付準備  終わり */

	/* サーバの動作 */
	for (;;) {
		cdata = malloc(sizeof(struct clientdata));
		if (cdata == NULL) {
			perror("malloc");
			return 1;
		}

		/* TCPクライアントからの接続要求を受け付ける */
		len = sizeof(cdata->saddr);
		cdata->sock = accept(sock0, (struct sockaddr *)&cdata->saddr, &len);

		/* スレッド生成 */
		if (pthread_create(&th, NULL , threadfunc, cdata) != 0) {
			perror("pthread_create");
			return 1;
		}

	}

	/* ここに来る予定なし */
	/* listen するsocketの終了 */
	if (close(sock0) != 0) {
		perror("close");
		return 1;
	}

	return 0;
}
