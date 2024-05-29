#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include<time.h>

const char logFile[] = "log.txt";
const char signalFile[] = "signal.txt";

void clearBuffer(char buffer[], int len);
void print_to_file(char inputBuffer[], int len);
void init_files();


int main(void) {
    const char fifoName[] = "myfifo";
    const int inputBufferSize = 200, outputBufferSize = 300;
    char inputBuffer[inputBufferSize], outputBuffer[outputBufferSize];
    int fd, bytes, count;

    printf("Waiting for writer...\n");

    //mknod(fifoName, __S_IFIFO | 0666, 0);

    fd = open(fifoName, O_RDONLY);
    if( -1 == fd){
        perror("File not opened");
        return 1;
    } else {
        printf("FIFO %s opened\n", fifoName);
    }

    init_files();

    do {
        
        bytes = read(fd, inputBuffer, inputBufferSize);
        if (bytes == -1){
            perror("read");
            return 1;
        } else {
            printf("reader: read %d bytes: %s", bytes, inputBuffer);
            print_to_file(inputBuffer, strlen(inputBuffer));
        }
        
        clearBuffer(inputBuffer,inputBufferSize);
  
    } while (bytes > 0);


    close(fd);
    return 0;

}


void clearBuffer(char buffer[], int len){
    for( int i = 0; i < len; i++){
        buffer[i] = '\0';
    }
}


void print_to_file(char inputBuffer[], int len){
    FILE* fd;

    if(inputBuffer[0] == 'D'){
        printf("Data! >> writing to %s file\n", logFile);

        fd = fopen(logFile, "a");
        fprintf(fd, "%s", inputBuffer);
        fclose(fd);
        
    } else if (inputBuffer[0] == 'S'){
        printf("Signal! >> writing to %s file\n", signalFile);

        fd = fopen(signalFile, "a");
        fprintf(fd, "%s", inputBuffer);
        fclose(fd);
    }

}


void init_files(){
    FILE* fd;
    time_t t;

    time(&t);

    fd = fopen(logFile, "a");
    fprintf(fd, "%s", ctime(&t));
    fclose(fd);

    fd = fopen(signalFile, "a");
    fprintf(fd, ">> START >> %s", ctime(&t));
    fclose(fd);
}
