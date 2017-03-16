#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <fcntl.h>

#ifndef __SOCKET_H__
#define __SOCKET_H__
int creer_serveur(int port);

#endif
void initialiser_signaux(void);
void traitement_signal(int sig);
enum http_method{
	HTTP_GET,
	HTTP_UNSUPPORTED,
};

typedef struct 
{
	enum http_method method ;
	int major_version ;
	int minor_version ;
	char * target ;
}http_request;

int traitement(char *buffer);
char *fgets_or_exit(char *buffer, int size, FILE *stream);
int parse_http_request(char *request_line, http_request *request);
char *rewrite_target(char *target);
int espaces(char *request);
int totalEspaces(char *request);
void action(FILE *client, char * directory, int socket_client);
void skip_headers(FILE *client);
void send_status(FILE * client, int code, const char * reason_phrase);
void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body);
void send_file(int socket_client, FILE *client, int fd, int code, const char *reason_phrase, char *target);
int check_and_open(const char *target, const char *document_root);
int get_file_size(int fd);
int copy(int in, int out);
char *type_mime(char *file_name);
