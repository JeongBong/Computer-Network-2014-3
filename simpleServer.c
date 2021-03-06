#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char**argv)
{
  pthread_t tid;
  int listenfd,connfd,n,i;
  struct sockaddr_in servaddr,cliaddr;
  socklen_t clilen;
  char mesg[4096];
  char header[] = "HTTP/1.0 200 OK\r\nDate: Wed, 12 Mar 2014 00:14:10 GMT\r\n\r\n";
  char buf[BUFSIZ];

  listenfd=socket(AF_INET,SOCK_STREAM,0);

  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  servaddr.sin_port=htons(8888);
  bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

  listen(listenfd,1024);
  
  for(i = 0; i < 100;i++) {
    clilen=sizeof(cliaddr);
    connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
    printf("connected (%d)\n", connfd);

    if (connfd > 0) {
      n = recv(connfd,mesg,4096,0);
      mesg[n] = 0;
      printf("=====\n%s=====\n", mesg);
     
      strtok(mesg, "/");
      char* file = strtok(NULL, " ");
      printf("%s\n",file);
         
      send(connfd,header,strlen(header),0);
      int fd = open(file, O_RDONLY);
            
      if(fd ==-1)
          fd = open("404error.html", O_RDONLY);
               
      while ((n = read(fd, buf, BUFSIZ)) > 0) {
        send(connfd,buf,n,0);
      }
      close(fd);
      close(connfd);
    }
  }
}
