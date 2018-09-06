/*
Example to ilustrate the heap

~~ r2con 2018 ~~

by morbtih-dqtz
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define NETBUF 200
#define MAXPTRS 100

/* returns the status of the pointers array */
void stat_ptrs (char *ptrs[MAXPTRS], int sock) {
	char buff[NETBUF];
	int i;
	for (i = 1; i <= MAXPTRS - 1; i++ ) {
		snprintf (buff, NETBUF, "idx:%d - (%p)\n", i , *(ptrs + i));
		write (sock, buff, strlen (buff));
	}
}

/* check that idx in the ptrs array is not NULL */
int check_idx (char *ptrs[MAXPTRS], int idx) {
	if (ptrs[idx] != NULL) {
		return 1;
	}
	return 0;
}

/* returns the first free idx in the ptrs array or -1 if is not one*/
int first_free_idx (char *ptrs[MAXPTRS]){
	int i;
	for(i = 1; i <= MAXPTRS - 1; i++) {
		if ( *( ptrs + i ) == NULL ) {
			return i;
		}
	}
	return -1;
}

void welcome_home(int sock) {
	write (sock, "Welcome home\n", 13);
}

void stop_server(int signum){
	printf ("\nCTRL-C : Bye!\n");
	exit(0);
}

int heap_play(int sock) {
	int idx, spc, len = 1;
	char pass[0x80]="SETEC ASTRONOMY";
	char *p;
	char *ptrs[MAXPTRS] = { NULL };
	char attr[NETBUF], buff[NETBUF], choice[NETBUF], tmp[NETBUF];
	sprintf (buff, "pass is at: (%p)\n"
		"ptrs is at: (%p)\n"
		" tmp is at: (%p)\n"
		, pass, ptrs, tmp);

	write (sock, &buff, strlen (buff));
	write (sock, "*.... Heap playground ....*\n", 28);
	while (len > 0) {
		write (sock, "\n1.Alloc chunk\n",     15);
		write (sock, "2.Free chunk\n",        13);
		write (sock, "3.Write into chunk\n",  19);
		write (sock, "4.Print chunk\n",       14);
		write (sock, "5.Print ptrs array\n",  19);
		write (sock, "6.Check password\n",    17);
		write (sock, "7.Write into tmp\n",    17);
		write (sock, "8.Exit\n>",              8);

		len = read (sock, &choice, NETBUF);
		/* 1 . Alloc chunk */
		if (choice[0] == '1') {
			idx = first_free_idx (ptrs);
			if (idx != -1) {
				write (sock, "How many space ?: ", 18);
				len = read (sock, attr, NETBUF);
				if (len) {
					spc = atoi (attr);
					if (spc) {
						p = malloc (spc);
						ptrs[idx] = p;
						sprintf (buff, "idx:%d - (%p)\n", idx, p);
						write (sock, &buff, strlen (buff));
					} else {
						write (sock,"Error, can't use this value\n", 28);
					}
				} else {
					write (sock, "Socket error\n", 13);
				}
			} else {
				write (sock, "There is no free idx to store the pointer\n", 42);
			}
		}

		/* 2. Free chunk */
		else if (choice[0] == '2') {
			write(sock,"Index to free: ",15);
			len = read (sock, attr, NETBUF);
			if (len) {
				idx = atoi(attr);
				if ((idx > 0) && (idx / MAXPTRS < 1)) {
					if (check_idx(ptrs, idx)) {
						free (ptrs[idx]);
						ptrs[idx] = NULL;
						stat_ptrs (ptrs, sock);
					} else {
						write (sock,"Wrong index\n",12);
					}
				} else {
					write (sock, "Wrong index\n", 12);
				}
			} else {
				write (sock, "Socket error\n", 13);
			}
		}

		/* 3. Write into chunk */
		else if (choice[0] == '3') {
			write (sock, "Write index: ", 13);
			len = read (sock, attr, NETBUF);
			if (len) {
				idx = atoi (attr);
				if ((idx > 0) && (idx / MAXPTRS < 1)) {
					if (check_idx (ptrs, idx)) {
						write ( sock, "Write data: ", 12);
						len = read (sock, attr, NETBUF);
						if (len) {
							memcpy(ptrs[idx], (char *)&attr, len);
						} else {
							write (sock, "Socket error\n", 13);
						}
					} else {
						write (sock, "Wrong index\n", 12);
					}
				} else {
					write (sock, "Wrong index\n", 13);
				}
			} else {
				write (sock, "Socket error\n", 13);
			}
		}

		/* 4. Print chunk */
		else if (choice[0] == '4') {
			write (sock, "Print index :", 13);
			len = read (sock, attr, NETBUF);
			if (len) {
				idx = atoi (attr);
				if ((idx > 0) && (idx / MAXPTRS < 1)) {
					if (check_idx (ptrs,idx)) {
						write (sock, *( ptrs + idx), strlen(*( ptrs + idx)));
					} else {
						write (sock, "Wrong index\n", 12);
					}
				} else {
					write (sock, "Wrong index\n", 12);
				}
			} else {
				write (sock, "Socket error\n", 13);
			}
		}

		/* 5. Print pointers array */
		else if (choice[0] == '5') {
			stat_ptrs(ptrs, sock);
			}

		/* 6. Check password */
		else if (choice[0] == '6') {
			write (sock, "Insert Password : ", 18);
			len = read (sock, attr, NETBUF);
			if (len) {
				if(!strncmp(attr,pass,strlen(pass))) {
					welcome_home (sock);
				} else {
					write (sock, "Get out!\n", 10);
					close (sock);
				}
			} else {
					write (sock, "Socket error\n", 13);
			}

		}

		/* 7. Write into tmp */
		else if (choice[0] == '7') {
			write (sock, "Data : \n", 8);
			len = read (sock, attr, NETBUF);
			if (len) {
				strncpy(tmp,attr,strlen(attr));
			}	else {
				write (sock, "Socket error\n", 13);
			}
		}

		/* 8. Exit playground */
		else if (choice[0] == '8') {
			write (sock, "See you soon!\n", 14);
			close(sock);
			return 0;
		}

		else {
			printf("Invalid choice\n");
		}

		if ( len <= 0 ) {
			printf("Error reading socket\n");
			close(sock);
		}
		choice[0] = '\0';
		attr[0] = '\0';
	}
	return 0;
}

void *connection_handler (void *socket_desc) {
	int sock = *(int*)socket_desc;
	heap_play(sock);
	printf("client gone\n");
	close(sock);
	free(socket_desc);
}

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c, rc, *new_sock;
	struct sockaddr_in server , client;
	static sigset_t signal_mask;
	socket_desc = socket (AF_INET , SOCK_STREAM , 0);

	if (socket_desc == -1) {
		printf ("socket miss\n");
		return -1;
	}
	printf ("socket on\n");

	if (setsockopt (socket_desc, SOL_SOCKET, SO_REUSEADDR, &socket_desc, sizeof(socket_desc)) < 0) {
		printf ("Can't set reuse if time_wait\n");
		return -1;
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 1234 );

	if (bind (socket_desc, (struct sockaddr *)&server, sizeof (server)) < 0) {
		printf ("bind error\n");
		return -1;
	}
	printf ("bind on\n");

	listen(socket_desc , 3);
	printf ("waiting connections\n");

	c = sizeof(struct sockaddr_in);
	signal(SIGINT, stop_server);
	sigemptyset (&signal_mask);
	sigaddset (&signal_mask, SIGPIPE);
	rc = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);

	if (rc != 0) {
		printf ("can't block SIG_BLOCK\n");
		return -1;
	}

	while (1) {
		client_sock = accept (socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
		if ( client_sock > 0 ) {
			printf("accepting connection\n");
			pthread_t sniffer_thread;
			new_sock = malloc(1);
			*new_sock = client_sock;

			if(  pthread_create (&sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0) {
				printf("can't create a new server thread\n");
				close(client_sock);
				free(new_sock);
			} else {
				printf("connection manager assigned\n");
			}
		} else {
			printf("connection failed\n");
			close(client_sock);
		}
	}
	return 0;
}
