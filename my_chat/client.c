///客户端
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

/// 显示并记录错误于错误日志文件的函数
void my_err ( char *string , int line ) ;

/// 记录错误日志文件的函数
void record_err ( char *string ) ;

/// 记录错误函数于错误日志文件的函数
void record_wr ( int fd , char *string ) ;

/// 记录错误时间于错误日志文件的函数
void record_time ( int fd ) ;

/// 用户注册函数
void regist () ;

/// 登陆系统的函数
void login ( int conn_fd ) ;

/// 主菜单
void menu( int conn_fd );

///记录登录日志的函数
void record_login(char *string);

///退出登录更新用户信息的函数
void exit_change(char *name);

///验证用户登录的函数
void identify(struct user *new, int conn_fd);

///用户登录合法性检验函数
int user_login(int conn_fd,  struct user *new);

///接收信息的函数
void *recv_c(void *arg);

///发送信息的函数
void *send_c(void *arg);

///连接到服务器
int link_server();

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
	time_t 	now ;
	
	/// 获取出错的时间
	time ( &now ) ;
	
	if ( write ( fd , ctime ( &now ) , strlen ( ctime (&now) ) ) != strlen ( ctime (&now) ) ) {
		perror ( "write" ) ;
		exit (1) ;
	}
}

void menu ( int conn_fd )
{
	/// 主菜单分为三个模块： 注册 登录 退出
	int 	choice ;
		
	while (1) {
		/// 清屏
		system ( "clear" ) ;
		
		/// 主菜单选项
		printf ( "\t\t *****************	欢迎使用	*****************\n\n" ) ;
		printf ( "\t\t 		  1: 注册\n\n" ) ;
		printf ( "\t\t 		  2: 登录\n\n" ) ;
		printf ( "\t\t 		  3：退出 \n\n" ) ;
		printf ( "\t\t **********************************************************\n\n" ) ;
		
		printf ( "\t\t 	      请输入您的选择: " ) ;
		
		/// 输入选项
		scanf ( "%d" , &choice ) ;
		
		/// switch结构　客户做出选择
		switch ( choice ) {
			case 1:
				/// 用户注册
				regist () ;
				getchar () ;
				break ;
			case 2:
				/// 用户登录
				login ( conn_fd ) ;
				break ;
			case 3:
				/// 用户退出
				printf ( "\n\n\t\t 您已成功退出!\n " ) ;
				exit (0) ;
			default :
				/// 出错循环
				printf ( "抱歉， 您的选择有误!\n" ) ;
				getchar () ;
		}
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

void record_login(char *string)
{
        /// 登录日志的文件描述符
        int fd;

        /// 打开登录日志文件
        if((fd = open("./login", O_RDWR | O_CREAT | O_APPEND, 436)) == -1) {
                perror("open");
                exit(1);
        }

        if(write(fd, string, strlen(string)) != strlen(string)) {
                my_err("write", __LINE__);
        }

        /// 将登录的时间写入登录日志
        if(write(fd, "     login at ", 14) != 14) {
                my_err("write", __LINE__);
        }

        record_time(fd);

        /// 关闭错误日志文件
        close(fd);

}

void record_err ( char *string )
{
	/// 错误日志的文件描述符
	int 	fd ;
	
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

void regist ()
{
	FILE 		*fp ;
	int 		i , flag ;
	struct user 	new , old[20] ;
	
	while (1) {
		
		///清屏		
		system ( "clear" ) ;
		
		printf ( "\t\t********************	欢迎注册	*******************\n\n\n" ) ;
		printf ("\t\t	         请输入您的用户名:");
                scanf("%s", new.username);
		printf("\n\t\t	         请输入您的密码:");
		system("stty -echo");
                scanf("%s", new.password);
		system("stty echo");
		getchar () ;
		
		/// 打开保存文件信息的文件
		if ( ( fp = fopen ("./info" , "rt" ) ) == NULL ) {
			my_err ( "fopen" , __LINE__ ) ;
		}
		
		/// 检查用户名是否已经存在
		i = 0 ;
		flag = 0 ;
		while ( fscanf ( fp , "%s %s %d %d\n" , old[i].username , old[i].password , &old[i].state , &old[i].socket ) != EOF ) {
			if ( !strcmp ( new.username , old[i].username )  ) {
				printf ( "\n\n\t\t抱歉， 用户名已存在!\n" ) ;
				
				/// 用户名存在flag = 1
				flag = 1 ;
				getchar () ;
				break ;
			}
			i++ ;
		}
		fclose (fp) ;
		
		 /// 用户名不存在
		if ( flag == 0 ) {
			new.state = 0 ;
			new.socket = 0 ;
			
			/// 打开保存文件信息的文件
			if ( ( fp = fopen ("./info" , "at") ) == NULL ) {
				my_err ( "fopen" , __LINE__ ) ;
			}
			
			/// 将新用户的信息写入用户信息文件
			fprintf ( fp , "%s %s %d %d\n" , new.username , new.password , new.state , new.socket ) ;
			
			fclose ( fp ) ;
			
			printf ( "\n\n\n\n\t\t\t       恭喜，您已成功注册!\n" ) ;
			sleep(1);
			break ;
		}
	}
	getchar () ;
}

void exit_change ( char *name )
{
	FILE 		*fp ;
	int 		i , sum ;
	struct user	chater[33] ;
	
	
	if ( ( fp = fopen ( "./info" , "rt" ) ) == NULL ) {
		my_err ( "fopen" , __LINE__ ) ;
	}
	
	i = 0 ;
	while ( fscanf ( fp , "%s %s %d %d\n" , chater[i].username , chater[i].password , &chater[i].state , &chater[i].socket ) != EOF ) {
		if ( strcmp ( chater[i].username , name ) == 0 ) {
			chater[i].state = 0 ;
			chater[i].socket = 0 ;
		}
		i++ ;
	}
	sum = i ;
	fclose (fp) ;
	
	if ( ( fp = fopen ("./info" , "wt") ) == NULL ) {
		my_err ( "fopen" , __LINE__ ) ;
	}
	
	i = 0 ;
	while ( i < sum ) {
		fprintf ( fp , "%s %s %d %d\n" , chater[i].username , chater[i].password , chater[i].state , chater[i].socket ) ;
		i++ ;
	}
	
	fclose (fp) ;
}

void identify (struct user *new , int conn_fd )
{	
	/// 控制输入的次数
	int 	count = 0 ;

	char 	send_msg[32] ; 	
	
	///用户身份验证，有三次机会
	while ( count < 3 ) {
		if ( user_login ( conn_fd , new ) == 0 ) {
			/// 登录成功
			printf ( "\n\n\n\n\t\t                  恭喜， 登录成功!\n" ) ;
			sleep(1);
			break ;
		}
		else {
			/// 登录失败
			count++ ;
		}
		
	}
	
	/// 超高登录次数
	if ( count == 3 ) {
		printf ( "\t\t抱歉， 您已登录三次!\n" ) ;
		sleep(1);
		exit (1) ;
	} 
	
	/// 将用户昵称发送给服务器
	strcpy ( send_msg , new->username ) ;
	if ( send ( conn_fd , send_msg , sizeof (send_msg) , 0 ) < 0 ) {
		my_err ( "send" , __LINE__ ) ;
	}
	
}


void *recv_c ( void *arg )
{
	struct user *user = (struct user *)arg ;
	struct msg  	rec ;
	int 	    	ret ; 
	int 		flag = 1 ;
	
	
	while ( 1 ) {
		memset ( &rec , 0 , sizeof (struct msg) ) ;

		flag = recv ( user->socket , &rec , sizeof (struct msg) , 0 );
		if ( flag < 0 ) {
			my_err ( "recv" , __LINE__ ) ;
		}
		else if ( flag == 0 )
		{
			printf( "\n\n\t\t  与服务器断开连接\n" ) ;
			exit (0) ;
		}
		
		printf ( "\t\t%s : %s\n" , rec.from , rec.content ) ;
	}
}

void *send_c ( void *arg ) 
{
	struct user 	*usr = (struct user *)arg ;
	struct msg 	sen ;
	char 		quit[10];
	
	
	while ( 1 ) {
		memset ( &sen , 0 , sizeof (struct msg) ) ;
		
		strcpy ( sen.from , usr->username) ;
		
		sleep(1);	
		printf ( "\n\t\t**************************************\n" ) ;
		printf ( "\n\t\t     list  : 显示在线用户\n" ) ;
		printf ( "\n\t\t     group : 进入群聊\n" ) ;
		printf ( "\n\t\t     chater: 私聊\n" ) ;
		printf ( "\n\t\t     quit  : 退出聊天\n" ) ;
		printf ( "\n\t\t**************************************\n" ) ;
		printf ( "\n\t\t   请输入您的选择: \n" ) ;
		scanf ( "%s" , sen.to ) ;
		getchar () ;
		if ( strcmp ( sen.to , "list" ) == 0 ) {
			strcpy ( sen.content , "list" ) ;
			strcpy ( sen.to , "list" ) ;
		
			if ( send ( usr->socket , &sen , sizeof ( struct msg ) , 0 ) < 0 ) {
				my_err ( "sen" , __LINE__ ) ;
			}	
		} else if ( strcmp ( sen.to , "quit" ) == 0 ) {
			strcpy ( sen.content , "quit" ) ;
			strcpy ( sen.to , "server" ) ;
						
			if ( send ( usr->socket , &sen , sizeof ( struct msg ) , 0 ) < 0 ) {
				my_err ( "sen" , __LINE__ ) ;
			}
			
			exit_change ( sen.from ) ;
			break ;
		} else {			
			printf ( "\n\t\t *******************************************\n" ) ;
			printf ( "\n\t\t           您将与%s聊天.\n" , sen.to ) ;
			printf ( "\n\t\t           输入quit，退出聊天.\n" ) ;
			printf ( "\n\t\t *******************************************\n" ) ;			
			
			while ( 1 ) {
			
				/// 显示发送者昵称
				usleep ( 10000 ) ;
				printf ( "\t\t%s: " , sen.from ) ;
				gets ( sen.content );							
				if ( strcmp ( sen.content , "quit" ) == 0 ) {
					break ;
				} else {
					
					if ( send ( usr->socket , &sen , sizeof (struct msg) , 0 ) < 0 ) {
						my_err ( "send" , __LINE__ ) ;
					}
				}
			}
			
			printf ( "\n\t\t您已退出与%s的聊天!\n ", sen.to ) ;
			
		}
		
	}
		
}




void login ( int conn_fd )
{
	char 		name[32] ;
	pthread_t 	tid1 ;
	pthread_t 	tid2 ;
	struct user 	new ;
	
	identify (&new , conn_fd) ;
	
	int 	sockopt = 1 ;
	if ( setsockopt ( conn_fd , SOL_SOCKET , SO_KEEPALIVE , &sockopt , sizeof ( sockopt ) ) < 0 ) {
		perror ( "setsockopt" ) ;
	}
	
	pthread_create ( &tid2 , NULL , send_c , (void*)&new ) ;
	pthread_create ( &tid1 , NULL , recv_c , (void*)&new ) ;
	
	
	sleep (10000) ;
}


int link_server ()
{
	int 			conn_fd ; 
	int 			serv_port ;
	char 			ip[32] ;
	struct sockaddr_in 	serv_addr ;
	int 			count = 0 ;
	
	/// 初始化服务器端的地址
	memset ( &serv_addr , 0 , sizeof (struct sockaddr_in) ) ;
	serv_addr.sin_family = AF_INET ;
	
	/// 输入要连接的服务器的ip和端口号
        printf("\n请输入要连接的服务器的ip和端口号：");
	scanf ( "%s %d", ip,  &serv_port ) ;
	
	/// 验证输入的服务器端口号的合法性
	if ( serv_port < 0 || serv_port > 65535 ) {
		printf ( "\n\t\t抱歉，错误的端口号!\n" ) ;
		exit (1) ;
	} else {
		serv_addr.sin_port = htons ( serv_port ) ;
	}
	
	 /// 验证输入的服务器的ip的合法性
	if ( inet_aton ( ip, &serv_addr.sin_addr ) < 0 ) {
		printf ( "invalid ip.\n" ) ;
		exit (1) ;
	}
	
	#ifdef DEBUG
	printf ( "%s\n", ip );
	#endif
	
	/// 创建一个TCP套接字
	conn_fd = socket ( AF_INET , SOCK_STREAM , 0 ) ;
	if ( conn_fd == -1 ) {
		my_err ( "socket" , __LINE__ ) ;
	}
	
	/// 向服务器端发送连接请求
	if ( connect ( conn_fd , ( struct sockaddr * )&serv_addr , sizeof ( struct sockaddr) ) == -1 ) {
		my_err ( "connect" , __LINE__ ) ;
	}
	
	return conn_fd ;
}

int user_login ( int conn_fd , struct user *new )
{
	FILE 		*fp ;
	struct user	chater[33] ;
	char 		recv_msg[MAX_LEN] ;
	int 		flag = 0 ;
	int 		lemon = 0 ;
	int 		i , sum ;
	
	/// 输入用户名和密码，并验证是否存在，如果存在则登录，并修改其信息
	while (1){
		/// 清屏
		system ( "clear" ) ;
		
		/// 登录主菜单            
                printf("\t\t*******************     欢迎登录        ******************\n");
                printf("\n\n\t\t                   请输入用户名:");

                ///输入用户名
                scanf("%s", new->username);
                printf("\n\n\t\t                   请输入密码:");
                getchar();
		system("stty -echo");
                scanf("%s", new->password);
		system("stty echo");
		new->socket = conn_fd ;
		
		/// 打开保存文件信息的文件
		if ( ( fp = fopen ( "./info" , "rt" ) ) == NULL ) {
			my_err ( "fopen" , __LINE__ ) ;
		}
		
		/// 用户身份验证
		i = 0 ;
		while ( !feof ( fp ) ) {
			fscanf ( fp , "%s %s %d %d\n" , chater[i].username , chater[i].password , &chater[i].state , &chater[i].socket ) ;
			
			/// 用户名正确
			if ( strcmp ( new->username , chater[i].username ) == 0 ) {
				
				/// 密码正确
				if ( strcmp ( new->password, chater[i].password ) == 0 ) {
				
					/// 用户未登录
					if ( chater[i].state == 0 ) {
						
						/// 在服务器端写入登录日志
						record_login ( chater[i].username ) ;
						
						/// 登录成功flag = 1
						flag = 1 ;
		
						/// 修改登录用户的信息
						chater[i].state = 1 ;
						chater[i].socket = conn_fd ;
					} else {
						/// 用户已登录
						lemon = 2 ;
					} 
				} else {
					/// 用户密码错误
					lemon = 1 ;
				} 
			} 
			i++ ;
		}
		sum = i ;
		fclose (fp) ;
		
		/// 判断是否登录成功
		if ( !flag ) {
			/// 未成功登录
			switch ( lemon ) {
				case 0: printf("\n\n\t\t抱歉， 您的用户名错误!");
					getchar();
                                        break;

                                case 1: printf("\n\n\t\t抱歉， 您的密码错误!");
					getchar();
                                        break;

                                case 2: printf("\n\n\t\t抱歉， 用户已经登录!");
					getchar();
                                        break;

			}
			getchar () ;
			
			return 1 ;
		} else {
			 /// 成功登录
			if ( ( fp = fopen ( "./info" , "wt" ) ) == NULL ) {
				my_err ( "fopen" , __LINE__ ) ;
			}
			
			i = 0 ;
			
			/// 更新用户信息文件
			while ( i < sum  ) {
				fprintf ( fp , "%s %s %d %d\n" , chater[i].username , chater[i].password , chater[i].state , chater[i].socket ) ;
				i++ ;
			}
			
			fclose (fp) ;
			
			break ;
		}
	}
	
	return 0 ;
}


int main ( int argc , char **argv )
{
	int 	conn_fd ;

	system( "touch info" ) ;
	conn_fd = link_server () ;

	getchar () ;

	menu ( conn_fd ) ;

	return 0 ;
}
