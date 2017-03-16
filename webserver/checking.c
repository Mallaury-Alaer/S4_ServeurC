#include "lib.h"

int check_and_open(const char *target, const char *document_root)
{
	struct stat st;
	// Construction du chemin
	int size = (int)strlen(document_root) + (int)strlen(target) + 1;
	char *tmp = (char *)malloc(sizeof(char) * size);
	snprintf(tmp, size, "%s%s", document_root, target);

	if(access(tmp, F_OK) == -1)
		return -1;
	
	if(access(tmp, R_OK) == -1)
		return -2;

	if(stat(tmp, &st) == -1)
		return -1;

	if(S_ISREG(st.st_mode) == -1)
		return -1;

	int retour = open(tmp, O_RDONLY);
	if(retour == -1)
	{
		perror("open");
		return -1;
	}
	free(tmp);
	return retour;

}

int get_file_size(int fd)
{
	struct stat file;
	fstat(fd, &file);
	return (int)file.st_size;
}

int copy(int in, int out)
{
	char buffer[256];
	int n = 0;
	while((n = read(in, buffer, 256*sizeof(char)))>0)
	{
		write(out, buffer, n);
	}
	return n;
}