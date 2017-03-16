#include "lib.h"

void initialiser_signaux(void)
{
	struct sigaction sa ;

	sa.sa_handler = traitement_signal ;
	sigemptyset(& sa.sa_mask );
	sa.sa_flags = SA_RESTART ;
	if ( sigaction ( SIGCHLD , & sa , NULL ) == -1)
	{
		perror ( " sigaction ( SIGCHLD ) " );
	}

	if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		perror ("signal");
	}
}

void traitement_signal(int sig)
{
	int status;
	printf("Signal %d recu\n", sig);
	waitpid(-1, &status, WNOHANG);
}