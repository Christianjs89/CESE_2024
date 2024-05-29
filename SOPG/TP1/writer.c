#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <features.h> 
#include <signal.h>
#include <stdlib.h>

const char fifoName[] = "myfifo";
int fd, bytes, pid;

void clearBuffer(char buffer[], int len);
void handler(int sig);
void pipe_handler(int sig);

int main(void) {
    const int inputBufferSize = 100, outputBufferSize = 200;
    char inputBuffer[inputBufferSize], outputBuffer[outputBufferSize];

    struct sigaction sa;        
    sa.sa_handler = handler; // SIG_IGN, SIG_DFL or my defined handler function
    sa.sa_flags = SA_RESTART; 

    sigemptyset(&sa.sa_mask);    
    sigaction(SIGUSR1, &sa, NULL); // install handler for SIGUSR1
    sigaction(SIGUSR2, &sa, NULL); // install handler for SIGUSR2


    struct sigaction sa_pipe;        
    sa_pipe.sa_handler = pipe_handler; 
    sigaction(SIGPIPE, &sa_pipe, NULL); // install handler for SIGPIPE > pipe closed

    pid = getpid();
    printf("Writer PID: %d\n\n", pid);

    if ( -1 == mkfifo(fifoName, 0777) ){
        perror("FIFO not created");
        if(errno != EEXIST){
            printf("Closing process\n");
            return 1;
        }
    } else {
        printf("FIFO %s created\n", fifoName);
    }    

    printf("Waiting for readers...\n");

    fd = open(fifoName, O_WRONLY);
    if( -1 == fd){
        perror("File not opened");
        return 1;
    } else {
        printf("FIFO %s created\n", fifoName);
    }

    printf("Active reader OK\n");

    while (1) {
        clearBuffer(inputBuffer,inputBufferSize);
        clearBuffer(outputBuffer,outputBufferSize);

        printf("> ");
        if( NULL == fgets(inputBuffer, inputBufferSize, stdin)){
            perror("fgets");
            return 1;
        }
        sprintf(outputBuffer, "DATA: %s", inputBuffer);

        bytes = write(fd, outputBuffer, strlen(outputBuffer));
        if ( bytes == -1 ){
            perror("write");
            return 1;
        } else {
            printf("DATA: wrote %d bytes\n", bytes);
        }
        
    }
    
    close(fd);
    printf("FIFO %s closed\n", fifoName);
    return 0;
}


void clearBuffer(char buffer[], int len){
    for( int i = 0; i < len; i++){
        buffer[i] = '\0';
    }
}


void handler(int sig) {    
    const int bufferSize = 100;
    char buffer[bufferSize];
    int altSign;

    if( SIGUSR1 == sig){
        altSign = 1;    
    } else if (SIGUSR2 == sig){
        altSign = 2; 
    }
    printf("SIGN: %d\n", altSign);

    sprintf(buffer, "SIGN: %d\n", altSign);
    bytes = write(fd, buffer, strlen(buffer));
    if ( bytes == -1 ){
        perror("write");
    } else {
        printf("SIGNAL: wrote %d bytes\n", bytes);
    }
}

void pipe_handler(int sig){
    printf("SIGPIPE signal: %d received!\n", sig);
}