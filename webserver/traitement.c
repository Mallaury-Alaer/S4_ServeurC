#include "lib.h"

char *fgets_or_exit(char *buffer, int size, FILE *stream)
{
  if(fgets(buffer, size, stream) == NULL)
    exit(0);
  return buffer;
}

int parse_http_request(char *request_line, http_request *request)
{
  char *lastpart = NULL;
  printf("%s", request_line);
  //Verifie le nombre de mots et qu'il n'y a pas deux espaces consécutifs
  if(espaces(request_line) == 1 || totalEspaces(request_line) !=2)
    return 0;

  //Verifie la methode HTTP
  char *token = strtok(request_line, " ");
  if(strcmp(token, "GET") == 0)
    request->method = HTTP_GET;
  else
    request->method = HTTP_UNSUPPORTED;

  //Récupère et assigne la cible
  char *targ = rewrite_target(strtok(NULL, " "));
  request->target = targ;

  //Verifie la version HTTP
  lastpart = strtok(NULL, " ");
  request->major_version = 1;
  if(strncmp(lastpart, "HTTP/1.1", 8) == 0)
    request->minor_version = 1;
  else if(strncmp(lastpart, "HTTP/1.0",8) == 0)
    request->minor_version = 0;
  else
  {
    request->minor_version = 505;    
    return 0;
  }
  return 1;

}

char *rewrite_target(char *target)
{
  if(strcmp(target, "/") == 0)
    target = "/index.html\0";

  char *chr = strchr(target, '?');
  if(chr != NULL)
    *chr = '\0';    

  return target; 
}

int espaces(char *request)
{
  int i = 0;
  while((unsigned int)(i+1)<strlen(request))
  {
    if(request[i] == ' ' && request[i+1] == ' ')
      return 1;
    i++;
  }
  return 0;  
}

int totalEspaces(char *request)
{
  int i = 0;
  int cpt = 0;
  while(request[i+1] != '\0')
  {
    if(request[i] == ' ')
      cpt ++;
    i++;
  }
  return cpt;
}