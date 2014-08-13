///服务器端
#include "head.h"
#include "err.h"

///创建链表存储在线用户的信息
struct line
{	
	///在线用户的用户名
	char online_name[LEN];
	///在线用户的套接字描述符
	int  online_fd;
	struct line *next;
};

#define N sizeof(struct line)
struct line *head, *online, *conn;

///登录验证函数
struct line *login_test(int conn_fd)
{
	FILE *fp;
	char username[LEN], passwd[LEN];
	int sock_fd;

	///打开保存有用户登录信息的文件
        if((fp = fopen("install", "r")) == NULL) {
                printf("抱歉，系统出现故障!\n");
                my_err("fopen", __LINE__);
        }
	
	///若用户信息输入不成功，循环输入
	while(1) {

		memset(recv_buf, 0, sizeof(recv_buf));		

		///接收输入的用户名
       		if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
                	my_err("recv", __LINE__);
	 	strcpy(username, recv_buf);
		memset(recv_buf, 0, sizeof(recv_buf));

		///接收输入的密码
		if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
                	my_err("recv", __LINE__);
        	strcpy(passwd, recv_buf);
		memset(recv_buf, 0, sizeof(recv_buf));

		///从文件中读取用户信息并比较
        	while(fscanf(fp, "%s\n", user_msg.username) != EOF) {
			fscanf(fp, "%s\n", user_msg.passwd);
			if((strcmp(username, user_msg.username) == 0) && (strcmp(passwd, user_msg.passwd) == 0)) {
				if(send(conn_fd, "y", 1, 0) < 0) 
					my_err("send", __LINE__);
				if(recv(conn_fd, &sock_fd, sizeof(int), 0) < 0)
					my_err("recv", __LINE__);
				online = (struct line *)malloc(N);
				strcpy(online->online_name, username);
				online->online_fd = sock_fd;
				conn->next = online;
				conn = online;
				conn->next = NULL;
				return ;
			}
				
		}
		if(send(conn_fd, "n", 1, 0) < 0)
			my_err("send", __LINE__);
		
	}
}

///服务器作为中转站实现客户端与客户端交流的函数
int s_communicate(int conn_fd)
{

	memset(recv_buf, 0, sizeof(recv_buf));
	if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
		my_err("recv", __LINE__);
	printf("chatname: %s\n", recv_buf);
	conn = head->next;
	while(conn != NULL) {
		if(strcmp(conn->online_name, recv_buf) == 0) {
			printf("%d\n", conn->online_fd);
			return conn->online_fd;
		}
		conn = conn->next;
	}
	return -1;
}

///c/s交流函数
 void *c_connect_c(int conn_fd)
 {	
	///要交流对象的套接字描述符
	int chat_fd;
	char chatmsg[BUFSIZE];

        login_test(conn_fd);
	if((chat_fd = s_communicate(conn_fd)) != -1) {
		if(send(conn_fd, "y", 1, 0) < 0)
			my_err("send", __LINE__);		
		while(1) {
			memset(chatmsg, 0, sizeof(chatmsg));
			if(recv(conn_fd, chatmsg, sizeof(chatmsg), 0) < 0)
				my_err("recv", __LINE__);
			printf("chatmsg: %s\n", chatmsg);
			if(send(chat_fd, chatmsg, chatmsstrlen(chatmsg), 0) < 0)
				my_err("send", __LINE__);
			printf("enen!\n");
		}
	}else{
		if(send(conn_fd, "n", 1, 0) < 0)
			my_err("send", __LINE__);
	}
 }

int main()
{  
        int sock_fd, conn_fd;
        int optval;
        int ret;
        pthread_t tid;
        socklen_t cli_len;
        struct sockaddr_in cli_addr, serv_addr;

        ///创建一个tcp套接字
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(sock_fd < 0)
                my_err("socket", __LINE__);

        ///设置该套接字使之可以重新绑定端口
        optval = 1;
        if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int)) < 0)
   		my_err("setsockopt", __LINE__);

        ///初始化服务器端地址结构
        memset(&serv_addr, 0, sizeof(struct sockaddr_in));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(SERV_PORT);
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        ///将套接字绑定到本地端口
        if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
                my_err("bind", __LINE__);

        ///将套接字转化为监听套接字
        if(listen(sock_fd, LISTENQ) < 0)
                my_err("listen", __LINE__);

	cli_len = sizeof(struct sockaddr_in);	
	///创建链表，记录在线用户的信息
        head = conn = (struct line *)malloc(N);

	while (1) {
                ///通过accept接收客户端的连接请求，并返回连接套接字用于收发数据
                conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
                if(conn_fd < 0)
                        my_err("accept", __LINE__);
		///客户端服务端分别提示连接成功
                printf("accept a new client, ip: %s\n", inet_ntoa(cli_addr.sin_addr));

                ///创建一个子线程处理刚刚接收的连接请求
		if(pthread_create(&tid, NULL, c_connect_c, conn_fd) != 0) {
			my_err("pthread_create", __LINE__);
			close(conn_fd);
		}
	}
}
