#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include <chrono>
using namespace std;

#define SERVER_PATH "/tmp/server"
#define BUFFER_LENGTH 1024
#define FALSE 0

void checkNewData();

const double timerMaxAllow = 850; 
const unsigned int sleepMicroSecondBetweenCommand = 50000U;
double timerElapsed = 0;
double testTimerElapsed = 0;

int turns = 0;
//IStrategy strategy;
//Board board;

// client socket client running
int sd = -1, rc, bytesReceived;
char buffer[BUFFER_LENGTH];
struct sockaddr_un serveraddr;

// command variables
char output[1024], *token;
const char *data[10];
int id;
bool isFailed;

int main(int argc, char *argv[]) {
    decltype(chrono::steady_clock::now()) timerStart = chrono::steady_clock::now();
    decltype(chrono::steady_clock::now()) testTimerStart = chrono::steady_clock::now();
    while (true) {
        timerElapsed = chrono::duration_cast<chrono::duration<double>>(chrono::steady_clock::now() - timerStart).count();
        testTimerElapsed = chrono::duration_cast<chrono::duration<double>>(chrono::steady_clock::now() - testTimerStart).count();
        if (testTimerElapsed > 2) {
            checkNewData();
            testTimerStart = chrono::steady_clock::now();
        }
    
    }
    return 0;
}

// check the new data from socket server, process, and send the result.
void checkNewData() {
    do {
        sd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sd < 0) {
            perror("socket() failed");
            break;
        }
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sun_family = AF_UNIX;
        strcpy(serveraddr.sun_path, SERVER_PATH);

        rc = connect(sd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
        if (rc < 0) {
            perror("connect() failed");
            break;
        }

        memset(buffer, 'a', 20);
        rc = send(sd, buffer, sizeof(buffer), 0);
        if (rc < 0) {
            perror("send() failed");
            break;
        }
        //bytesReceived = 0;
        rc = recv(sd, &buffer[0], BUFFER_LENGTH, 0);
        printf("receive %d bytes, %s\n", rc, buffer);
        if (rc < 0) {
            perror("recv() failed");
            break;
        } else if (rc == 0) {
            printf("The server closed the connection\n");
            break;
        }

        // deal with the problem
        token = strtok(buffer, " ");
        sscanf(token, "%d", &id);
        // get command name
        token = strtok(NULL, " ");
        // get command data
        int i = 0;
        while ((token = strtok(NULL, " ")) != NULL) {
            data[i++] = token;
        }

        printf("get cmdId %d\n", id);


        // send the result of command.
        sprintf(output, "=%d\n", id);
        rc = send(sd, output, sizeof(output), 0);
        if (rc < 0) {
            perror("send() failed");
            break;
        }
        //bytesReceived = 0;
        rc = recv(sd, &buffer[0], BUFFER_LENGTH, 0);
        printf("receive %d bytes, %s\n", rc, buffer);
        if (rc < 0) {
            perror("recv() failed");
            break;
        } else if (rc == 0) {
            printf("The server closed the connection\n");
            break;
        }

        // sleep and wait for next command (if there is)
        usleep(sleepMicroSecondBetweenCommand);
    } while (true);

    // close socket
    if (sd != -1)
        close(sd);
}