
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>

/// #define DEBUG 1

/// 服务器端的端口号
#define  PORT 	3333
 
/// 连接请求等待的最大长度
#define  LIST 	33

/// 发送消息的最大长度
#define MAX_LEN 333	

/// 用户信息的结构体
struct user {

        /// 用户的名字
        char username[32] ;

        /// 用户的密码
        char password[32] ;

        /// 标志用户是否在线，在线state = 1, 不在线state = 0
        int  state ;

        /// 保存用户创建TCP时的套接字
        int  socket ;
} ;

/// 发送消息的结构体
struct msg {

        /// 发送者的名字
        char from[32] ;

        /// 接收者的名字
        char to[32] ;

        /// 用户的命令
        int command ;

        /// 用户发送的内容
        char content[MAX_LEN] ;

        struct msg *next ;
} ;

///记录在线用户的结构体
struct  line {

        /// 在线用户的名字
        char username[32] ;

        /// 在线用户的套接字
        int socket ;

        struct line *next ;
} ;

struct line *head ;

///自定义的错误处理函数（显示错误并记录到错误日志中）
void my_err(char *string, int line);

///自定义的错误记录函数（记录错误函数的原因及时间）
void record_err(char *string);

///记录错误名字的函数
void record_wr(int fd, char *string);

///记录错误时间的函数
void record_time(int fd);

///创建记录在线用户的链表
struct line *creatline();

///
void process(int sock_fd);

///聊天函数
void *thread_chat(void *member);

///删除不在线的用户函数
void delete_line(char *string);

///显示在线用户的函数
void list_line(char *string);

///记录聊天信息的函数
void record_chat(char *string);

void record_wr ( int fd , char *string )
{
        /// 将出错的原因写入错误日志
        if ( write ( fd , string , strlen (string) ) != strlen (string) ) {
                perror ( "write" ) ;
                exit (1) ;
        }
}

void record_time ( int fd )
{
        time_t  now ;

        /// 获取出错的时间
        time ( &now ) ;

        if ( write ( fd , ctime ( &now ) , strlen ( ctime (&now) ) ) != strlen ( ctime (&now) ) ) {
                perror ( "write" ) ;
                exit (1) ;
        }
}


void my_err ( char *string , int line )
{
        /// 在屏幕上打印出错误的行号和函数名
        fprintf ( stderr , "line: %d.\n" , line ) ;
        perror ( string ) ;

        /// 记录错误的原因及时间于错误日志中
        record_err ( string ) ;

        /// 异常退出
        exit (1) ;
}

void record_err ( char *string )
{
        /// 错误日志的文件描述符
        int     fd ;

        /// 打开错误日志文件
        if ( ( fd = open ( "./error" , O_RDWR | O_CREAT | O_APPEND , 436 ) ) == -1 ) {
                perror ( "open" ) ;
                exit (1) ;
        }

        /// 将出错的函数写入错误日志
        record_wr ( fd , string ) ;

        /// 将出错的时间写入错入日志
        if ( write ( fd , " error at " , 10 ) != 10 ) {
                perror ( "write" ) ;
                exit (1) ;
        }
        record_time ( fd ) ;

        /// 关闭错误日志文件
        close ( fd ) ;
}

struct line *creatlink ()
{
	struct line 	*head , *p ;
	
	head = p = ( struct line * ) malloc ( sizeof ( struct line ) ) ;
	
	p->next = NULL ;
	
	return head ;
}


/// delete the user that will quit
void delete_link ( char *string )
{
	struct line *p , *q ;
	
	p = head->next ;
	
	for ( q = head ; p ; q = p , p = p->next ) {
		
		/// 找到下线用户并删除
		if ( strcmp (p->username , string ) == 0 ) {
			q->next = p->next ;
			free (p) ;
			break ;
		}
	}
}

void list_line ( char *string )
{
	struct line *p = head->next ;
	
	strcpy ( string , "\n\n\t\t在线用户: \n" ) ;
	
	
	while ( p ) {
		
		/// 将用户名信息写入
		strcat ( string , "\t\t\t  " ) ;
		strcat ( string , p->username ) ;
		strcat ( string , " \n" ) ;
		
		/// 遍历所有结点
		p = p->next ;
	}
}

void record_chat ( char *string )
{
	int 	fd ;
	
	if ( ( fd = open ( "./chat" ,  O_RDWR | O_CREAT | O_APPEND , 436 ) ) == -1 ) {
		my_err ( "open" , __LINE__ ) ;	
	}
	
	record_wr ( fd , string ) ;
	record_time ( fd ) ;
	
}

void *thread_chat ( void * member )
{
	struct user 	user = *(struct user *)member ; 
	int flag = 1 ;
	struct msg 	info , sen ;
	int 		send_fd , recv_fd ;
	char 		string[500] ;
	struct line 	*p = head->next ;
	
	while (1) {

		/// 初始化结构体
		memset ( &info , 0 , sizeof (struct msg) ) ;
		
		/// 接收客户端信息
		flag = recv ( user.socket , &info , sizeof (struct msg) , 0 ) ;
		if ( flag < 0 ) {
			my_err ( "recv" , __LINE__ ) ;
		}
		else if ( flag == 0 ) {
			///接受为空时表示断开连接
			printf( "\n\n\t\t客户端断开连接！\n" ) ;

			p = head->next ;
			while ( p != NULL ) {
				/// 找到断开的客户端的套接字
				if ( p->socket == user.socket) {
					break ;
				}
				p = p->next ;
			}
			
			/// 将该客户端在在线链表中删除
			delete_link ( p->username ) ;
			if ( close( user.socket ) < 0 )
				perror( "close" ) ;

			return ;
		}
		
		printf ( "%s to %s : %s.\n" , info.from , info.to , info.content ) ;
		
		/// 记录聊天的信息
		strcpy ( string , info.from ) ;
		strcat ( string , " to " ) ;
		strcat ( string , info.to ) ;
		strcat ( string , " : " ) ;
		strcat ( string , info.content ) ;
		strcat ( string , " \0" ) ;
		
		record_chat ( string ) ;
		
		/// 要聊天的对象是服务器
		if ( strcmp ( info.to , "server" ) == 0 ) {
			 ///判断内容	
			if ( strcmp ( info.content , "list" ) == 0 ) {
				
				list_line ( sen.content ) ;
			} else if ( strcmp ( info.content , "quit" ) == 0 ) {
				
				strcpy ( sen.content , "the user is deleted.\n\0" ) ;

				 /// 将该用户在在线链表中删除
				delete_link ( info.from ) ;
				
				if ( close ( user.socket ) < 0 )
					perror( "close" ) ;
				
				return ;
			} else {
				/// 说明发送信息有误
                                strcpy(sen.content, "抱歉， 您的指令有误!");	
			}

			strcpy ( sen.to , info.from ) ;
			strcpy ( sen.from , "server" ) ;
			if ( send ( user.socket , &sen , sizeof ( struct msg ) , 0 ) < 0 ) {
				my_err ( "send" , __LINE__ ) ;
			}
			
		}
		else if ( strcmp ( info.to , "list" ) == 0 ) {

			
			list_line ( sen.content ) ;
			
			strcpy ( sen.to , info.from ) ;
			strcpy ( sen.from , "server" ) ;

			if ( send ( user.socket , &sen , sizeof ( struct msg ) , 0 ) < 0 ) {
				my_err ( "send" , __LINE__ ) ;
			}
		}
		else if ( strcmp ( info.to , "group" ) == 0 ) {

			p = head->next ;
			memset ( &sen , 0 , sizeof (struct msg) ) ; 

			 /// 将内容显示为[group]info.content
			strcpy ( sen.from , info.from ) ;
			strcpy ( sen.to , info.to ) ;
			strcpy ( sen.content , "[" ) ;
			strcat ( sen.content , sen.to) ;
			strcat ( sen.content , "] " ) ;
			strcat ( sen.content , info.content ) ;

			while ( p != NULL ) {
				if(strcmp(p->username, sen.from) != 0) {	
					if ( send ( p->socket, &sen, sizeof ( struct msg ), 0 ) < 0 ) {
						my_err ( "send" , __LINE__ ) ;
					}
				}
				p = p->next ;
			}
		}
		else{
			
			p = head->next ;

			/// 寻找在线用户中是否存在要聊天的对象
			while ( p != NULL ) {
				if ( strcmp ( p->username , info.to ) == 0 ) {
					send_fd = p->socket ;
					break ;
				}
				p = p->next ;
			}
			
			if ( p == NULL ) {
				strcpy ( sen.content , "您输入的用户不在线或者不存在．\n" ) ;
				
				record_chat ( sen.content ) ;
		
				strcpy ( sen.to , info.from ) ;
				strcpy ( sen.from , "server" ) ;

				if ( send ( user.socket , &sen , sizeof ( struct msg ) , 0 ) < 0 ) {
					my_err ( "send" , __LINE__ ) ;
				}
				
			} 
			else {
				if ( send ( send_fd , &info , sizeof ( struct msg ) , 0 ) < 0 ) {
					my_err ( "send" , __LINE__ ) ;
				}
			}
			
		}
	}
	
}

int main (int argc , char *argv[])
{
 	int 			sock_fd ;
	int 			optval ;

	struct sockaddr_in 	cli_addr , serv_addr ;

	/// 创建一个TCP套接字
	sock_fd = socket ( AF_INET , SOCK_STREAM , 0 ) ;
	if ( sock_fd == -1 ) { 
		my_err ( "socket" , __LINE__ ) ;
	}


	/// 设置套接字，使其可以重新绑定端口
	optval = 1 ;
	if ( setsockopt ( sock_fd , SOL_SOCKET , SO_REUSEADDR , ( void * ) &optval , sizeof ( int ) ) == -1 ) {
		my_err ( "setsockopt" , __LINE__ ) ;
	} 

	/// 初始化服务器端的地址
	memset ( &serv_addr , 0 , sizeof ( struct sockaddr_in ) ) ;
	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_port = htons ( PORT ) ; 
	serv_addr.sin_addr.s_addr = htonl ( INADDR_ANY ) ; 


	/// 绑定端口
	if ( bind ( sock_fd , ( struct sockaddr * ) &serv_addr , sizeof ( struct sockaddr_in ) ) == -1 ) {
		my_err ( "bind" , __LINE__ ) ;
	}
	
	 /// 将套接字转化为监听套接字
	if ( listen ( sock_fd, LIST ) == -1 ) {
		my_err ( "listen" , __LINE__ ) ;
	}

	/// 创建记录在线用户的链表
	head = creatlink () ;

	///getchar () ;

	///system ( "clear" ) ;
	
	///printf ( "\t\t\t   ==~==TCP SERVER IS OK==~==\n" ) ;
	
	/// enter the chat between the server and client
	while ( 1 ) {
		process ( sock_fd ) ;
	}

}


void process ( int sock_fd )
{
	int 			cli_len ; 	
	pthread_t 		tid ; 		
	struct sockaddr_in 	cli_addr ; 	
	
	int 			socket , ret ;
	
	struct user 		member ;
	struct line 		*p = head , *q ;
	
	/// 接收客户端的请求
	cli_len = sizeof ( struct sockaddr_in ) ;
	
	if ( (member.socket = accept ( sock_fd , ( struct sockaddr * )&cli_addr , &cli_len ) ) == -1 )	 {
		my_err ( "accept" , __LINE__ ) ;
	}
	
	printf ( " \n\t\t A new client %s is connected.\n" , inet_ntoa (cli_addr.sin_addr) ) ;
	
	/// 申请一个结点
	q = ( struct line * ) malloc ( sizeof ( struct line ) ) ;
	
	/// 接收客户端发来的名字
	memset ( member.username , 0 , 32 ) ;

	if ( ( ret = recv ( member.socket, member.username, 32, 0 ) ) < 0 ) {
		my_err ( "recv" , __LINE__ ) ;
	}
	
	strcpy ( q->username , member.username ) ;
	q->socket = member.socket ;
	
	printf ( "\n\t\t---------------------------------------------------" ) ;
	printf ( "\n\t\t\t   username：%s\t socket: %d" , member.username , member.socket ) ;
	printf ( "\n\t\t---------------------------------------------------\n" ) ;
	
	
	/// 把新登录的客户连接到链表尾
	while ( p->next != NULL ) {
		p = p->next ;
	}
	
	q->next = p->next ; 	
	
	p = p->next = q ;
	
	/// 创建子线程处理客户端与服务端的通信
	pthread_create ( &tid , NULL , (void *)thread_chat , (void*)&member ) ;
}

