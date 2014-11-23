#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
void error_handling(char* message);
void* send_message(void* socket);
void* receive_message(void* socket);

int main(int argc, char *argv[])
{
    int sock, sthread_ret;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;
    pthread_t sthread, rthread;


    if(argc!=3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock=socket(PF_INET, SOCK_STREAM, 0);   
    if(sock==-1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("connect() error!");
    else
        puts("Connected...........");

    pthread_create(&sthread, NULL, send_message, (void*)&sock);
    pthread_create(&rthread, NULL, receive_message, (void*)&sock);
        
    sthread_ret = pthread_join(sthread, NULL);

    //if(sthread_ret==0){
    //     pthread_cancel(rthread);
    //}
    pthread_join(rthread, NULL);

    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}


void* send_message(void* socket){
    int sock = *((int*)socket);    
    char message[BUF_SIZE];

    while(1) 
    {
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message,"q\n") || !strcmp(message,"Q\n")){
            break;
        }

        write(sock, message, strlen(message));
    }
   exit(0);

}

void* receive_message(void* socket){
    int sock = *((int*)socket);
    char message[BUF_SIZE];
    int str_len;
    while(1)
    {
        str_len = read(sock, message, BUF_SIZE);
        message[str_len]=0;
       
        fputs(message, stdout);
    }
    return NULL;
}

