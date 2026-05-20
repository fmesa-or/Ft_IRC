INTERNET RELAY CHAT

./ircserv <port> <password>

Name:		ircserv
Arguments:	port:		The listening port.
						The port number on which your IRC server will be listening for incoming IRC connections.
			password:	The connection password.
						The connection password. It will be needed by any IRC client that tries to connect to your server.

Allowed Fx:	Everything in C++ 98.
			socket				close
			setsockopt			getsockname
			getprotobyname		gethostbyname
			getaddrinfo			freeaddrinfo
			bind				connect
			listen				accept
			htons				htonl
			ntohs				ntohl
			inet_addr			inet_ntoa
			inet_ntop			send
			recv				signal
			sigaction			sigemptyset
			sigfillset			sigaddset
			sigdelset			sigismember
			lseek				fstat
			fcntl				poll (or equivalent*)
									*select(), kqueue() or epoll().

You must not develop an IRC client.
You must not implement server-to-server communication.

Must use poll()*
Must allow multiple clients simultaneously
Fork is prohibited!

