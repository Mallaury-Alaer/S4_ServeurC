#include "lib.h"

void action(FILE *client, char * directory, int socket_client)
{	
	char buffer [256];
	http_request request;
	int bad_request;

	fgets_or_exit(buffer, sizeof(buffer), client);
	bad_request = parse_http_request(buffer, &request);
	skip_headers(client);

	if(bad_request == 0 && request.minor_version==505 && request.method == HTTP_UNSUPPORTED)
		send_response(client, 400, "Bad Request", "Bad Request \r\n");
	else if (bad_request == 0 && request.minor_version==505)
		send_response(client, 505, "HTTP Version Not Supported", "HTTP Version Not Supported\r\n");
	else if(request.method == HTTP_UNSUPPORTED)
		send_response(client, 405, "Method Not Allowed", "Method Not Allowed\r\n");
	else
	{
		int fd = check_and_open(request.target, directory);
		if(fd == -1)
			send_response(client, 404, "Not Found", "Not Found\r\n");
		else if(fd == -2)
			send_response(client, 403, "Forbidden", "Access Denied\r\n");
		else
			send_file(socket_client, client, fd, 200, "OK", request.target);
	}
}

void skip_headers(FILE *client)
{
	char buffer[256];
	while(1)
	{
		fgets_or_exit(buffer, sizeof(buffer), client);

  		if(strcmp(buffer, "\n") == 0 || strcmp(buffer, "\r\n") == 0)
  			break;
	}
}

void send_status(FILE * client, int code, const char * reason_phrase)
{
	fprintf(client, "HTTP/1.1 %d %s\r\n", code, reason_phrase);
}

void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body)
{
	send_status(client, code, reason_phrase);
	fprintf(client, "Connection: close\r\nContent-Length: %d\r\n \r\n", (int)strlen(message_body));
	fprintf(client, "%s", message_body);
	fflush(client);
}

void send_file(int socket_client, FILE *client, int fd, int code, const char *reason_phrase, char *target)
{
	send_status(client, code, reason_phrase);
	fprintf(client, "Content-Length: %d\r\nContent-Type: %s\r\n\r\n", get_file_size(fd), type_mime(target));
	fflush(client);
	copy(fd, socket_client);
}

char *type_mime(char *file_name)
{
	char *ext = strchr(file_name, '.');

	if(strcmp(ext, ".html") == 0)
		return "text/html";
	else if (strcmp(ext, ".css") == 0)
		return "text/css";
	else if(strcmp(ext, ".jpg") == 0)
		return "image/jpeg";
	else if(strcmp(ext, ".png") == 0)
		return "image/png";
	else if(strcmp(ext, ".icon")==0)
		return "image/x-icon";
	else if(strcmp(ext, ".js") == 0)
		return "application/javascript";
	else if(strcmp(ext, ".json")==0)
		return "application/json";
	return "null";
}
