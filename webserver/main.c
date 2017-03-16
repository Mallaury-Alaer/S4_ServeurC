#include "lib.h"

int main (int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Nombre d'arguments incorrect!\n");
		return 1;
	}

	if(access(argv[1], X_OK) == -1)
	{
		printf("Vous n'avez pas la permission sur ce repertoire!\n");
		return 1;
	}

	struct stat sb;
	if(stat(argv[1], &sb) == -1)
	{
		perror("stat");
		return 1;
	}

	if(!S_ISDIR(sb.st_mode))
	{
		printf("Ce n'est pas un répertoire!");
		return 1;
	}


	int socket_s = creer_serveur(8080);
	if(socket_s == -1)
	{
		perror("creer_serveur");
		return -1;
	}

	initialiser_signaux();

	while(1)
	{
		int acc = accept(socket_s, NULL, NULL);
		if(acc == -1)
		{
			perror("accept");
			return -1;
		}

		if(fork() == 0)
		{
			FILE *client;
			client = fdopen(acc, "w+");

			if(client == NULL)
				perror("fdopen");

			action(client, argv[1], acc);
			fclose(client);
		}else
			close(acc);
	}
	return 0;
}
