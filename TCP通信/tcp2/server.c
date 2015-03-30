#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

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

	if (data == NULL) {
		return (void *)-1;
	}

	/* acceptで生成した通信用ソケットを使用 */
	sock = cdata->sock;

	/* クライアントからの送信データを読み出し */
	n = recv(sock, buf, sizeof(buf), 0);
	if (n < 0) {
		perror("recv(server)");
		goto err;
	}

	/* クライアントからの送信データをそのまま返信 */
	n =send(sock, buf, n, 0);
	if (n < 0) {
		perror("send(server)");
		goto err;
	}

	/* １回ごとに切断 */
	/* TCPセッションの終了 */
	if (close(sock) != 0) {
		perror("close");
		goto err;
	}

	free(data);

	return NULL;

err:
	free(data);
	return (void *)-1;
}


int main()
{
	int sock0;
	struct sockaddr_in addr;
	socklen_t len;
	pthread_t th;
	struct clientdata *cdata;

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
		if (pthread_create(&th, NULL, threadfunc, cdata) != 0) {
			perror("pthread_create");
			return 1;
		}

		/* スレッドが終了したらメモリを開放する */
		if (pthread_detach(th) != 0) {
			perror("pthread_detach");
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
