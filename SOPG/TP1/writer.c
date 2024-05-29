#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <features.h> 
#include <signal.h>
#include <stdlib.h>

// ps -elf | grep writer
// kill -[signum] pis >> ex. kill -2 31772

// Visual studio code *sigaciton error*: Extensions>C/C++>IntelliSense > C_Cpp › Default: C Standard > change to gnu99

const char fifoName[] = "myfifo";
int fd, bytes, pid;

void clearBuffer(char buffer[], int len);
void handler(int sig);


int main(void) {
    //const char fifoName[] = "myfifo";
    const int inputBufferSize = 100, outputBufferSize = 200;
    char inputBuffer[inputBufferSize], outputBuffer[outputBufferSize];
    //int fd, bytes, pid;

    /* int sigaction(int signum, const struct sigaction *_Nullable restrict act,
        struct sigaction *_Nullable restrict oldact);

        >> signum >specifies the signal and can be any valid signal except SIGKILL and SIGSTOP
        >> act >is non-NULL, the new action for signal signum is installed from act.  
        >> oldact >If oldact is non-NULL, the previous action is saved in oldact.
    */
    struct sigaction sa;        
    sa.sa_handler = handler; // SIG_IGN, SIG_DFL or my defined handler function
    sa.sa_flags = SA_RESTART; 
    /* SA_RESETHAND (reset to default), SA_RESTART (when using a handler, to restart it) 
       If a blocked call to one of the following interfaces is interrupted by a signal handler, then the call is automatically
       restarted after the signal handler returns if the SA_RESTART flag was used. 
    */
    sigemptyset(&sa.sa_mask);    
    sigaction(SIGUSR1, &sa, NULL); // install handler for SIGUSR1
    sigaction(SIGUSR2, &sa, NULL); // install handler for SIGUSR2
    /* SIGUSR1      P1990      Term    User-defined signal 1 >> int 10
       SIGUSR2      P1990      Term    User-defined signal 2 >> int 12
       SIGINT       P1990      Term    Interrupt from keyboard
       SIGCONT      P1990      Cont    Continue if stopped
       SIGSTOP      P1990      Stop    Stop process
    */

    // get process PID
    pid = getpid();
    printf("Writer PID: %d\n\n", pid);

    // Create fifo file 
    if ( -1 == mkfifo(fifoName, 0777) ){ // wrx permissions
        perror("FIFO not created");
        if(errno != EEXIST){
            printf("Closing process\n");
            return 1;
        }
    } else {
        printf("FIFO %s created\n", fifoName);
    }    

    printf("Waiting for readers...\n");

    // Open fifo file to write
    fd = open(fifoName, O_WRONLY);
    if( -1 == fd){
        perror("File not opened");
        return 1;
    } else {
        printf("FIFO %s created\n", fifoName);
    }

    printf("Active reader OK\n");

    // enter infinite loop and get keyboard input
    while (1) {
        // get char string
        printf("> ");
        fgets(inputBuffer, inputBufferSize, stdin);
        sprintf(outputBuffer, "DATA: %s", inputBuffer);

        // write modified input to fifo
        bytes = write(fd, outputBuffer, strlen(outputBuffer));
        if ( bytes == -1 ){
            perror("write");
            return 1;
        } else {
            printf("DATA: wrote %d bytes\n", bytes);
        }
        // clear buffers so it doesn't print old chars in next loop
        clearBuffer(inputBuffer,inputBufferSize);
        clearBuffer(outputBuffer,outputBufferSize);
        
    }
    
    close(fd);
    printf("FIFO %s closed\n", fifoName);
    return 0;
}

// clear char array
void clearBuffer(char buffer[], int len){
    for( int i = 0; i < len; i++){
        buffer[i] = '\0';
    }
}


void handler(int sig) {    
    const int bufferSize = 100;
    char buffer[bufferSize];
    int altSign;

    // print to terminal signal number 1 or 2
    if( SIGUSR1 == sig){
        altSign = 1;    
    } else if (SIGUSR2 == sig){
        altSign = 2; 
    }
    printf("SIGN: %d\n", altSign);

    // write modified input to fifo
    sprintf(buffer, "SIGN: %d\n", altSign);
    bytes = write(fd, buffer, strlen(buffer));
    if ( bytes == -1 ){
        perror("write");
    } else {
        printf("SIGNAL: wrote %d bytes\n", bytes);
    }

}
