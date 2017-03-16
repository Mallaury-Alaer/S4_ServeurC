#include "lib.h"


int creer_serveur(int port)
{
	int socket_serveur = socket(AF_INET, SOCK_STREAM,0);
	if(socket_serveur == -1)
	{
		perror("socket_serveur");
		return -1;
	}
	
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	int sock_opt = 1;
	if(setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof(int)) == -1)
	{
		perror("Can not set SO_REUSEADDR option");
		return -1;
	}

	if(bind(socket_serveur, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("bind");
		return -1;
	}	
	
	if(listen(socket_serveur, 10) == -1)
	{
		perror("listen");
		return -1;
	}	

	return socket_serveur; 
}