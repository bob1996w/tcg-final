#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <chrono>

#include "BobAI.hpp"

using namespace std;

#ifdef SERVER2
    #define SERVER_PATH "/tmp/server2"
#else
    #define SERVER_PATH "/tmp/server"
#endif
#define BUFFER_LENGTH 1024
#define FALSE 0

void checkNewData();

const double timerMaxAllow = 850; 
const unsigned int sleepMicroSecondBetweenCommand = 50000U;
double timerElapsed = 0;
double testTimerElapsed = 0;
bool quitGame = false;

int turns = 0;
BobAI bobai;
//IStrategy strategy;
//Board board;

// client socket client running
int sd = -1, rc, bytesReceived;
char buffer[BUFFER_LENGTH];
struct sockaddr_un serveraddr;

// command variables
char resultMessage[1024], output[1024], *token;
const char *data[10];
int id;
bool isFailed;

// function pointer array
static bool (BobAI::*functions[])(const char* [], char*) = {
    &BobAI::protocol_version,
    &BobAI::name,
    &BobAI::version,
    &BobAI::known_command,
    &BobAI::list_commands,
    &BobAI::quit,
    &BobAI::boardsize,
    &BobAI::reset_board,
    &BobAI::num_repetition,
    &BobAI::num_moves_to_draw,
    &BobAI::move,
    &BobAI::flip,
    &BobAI::genmove,
    &BobAI::game_over,
    &BobAI::ready,
    &BobAI::time_settings,
    &BobAI::time_left,
    &BobAI::showboard
};

int main(int argc, char *argv[]) {
    decltype(chrono::steady_clock::now()) timerStart = chrono::steady_clock::now();
    decltype(chrono::steady_clock::now()) testTimerStart = chrono::steady_clock::now();
    while (true) {
        timerElapsed = chrono::duration_cast<chrono::duration<double>>(chrono::steady_clock::now() - timerStart).count();
        testTimerElapsed = chrono::duration_cast<chrono::duration<double>>(chrono::steady_clock::now() - testTimerStart).count();
        //if (testTimerElapsed > 2) {
            checkNewData();
            testTimerStart = chrono::steady_clock::now();
        //}
        if (quitGame) {
            fprintf(stderr, "quit sent\n");
            fflush(stderr);
            break;
        }
        if (getppid() == 1) {
            break;
        }
        usleep(sleepMicroSecondBetweenCommand);
    }
    fflush(stdout);
    fflush(stderr);
    return 0;
}

// check the new data from socket server, process, and send the result.
void checkNewData() {
    do {
        //printf("#\n");
        sd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sd < 0) {
            // perror("socket() failed");
            break;
        }
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sun_family = AF_UNIX;
        strcpy(serveraddr.sun_path, SERVER_PATH);

        rc = connect(sd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
        if (rc < 0) {
            // perror("connect() failed");
            break;
        }

        memset(buffer, 'a', 20);
        rc = send(sd, buffer, sizeof(buffer), 0);
        if (rc < 0) {
            // perror("send() failed");
            break;
        }
        //bytesReceived = 0;
        rc = recv(sd, &buffer[0], BUFFER_LENGTH, 0);
        //printf("receive %d bytes, %s\n", rc, buffer);
        if (rc < 0) {
            // perror("recv() failed");
            break;
        } else if (rc == 0) {
            // fprintf(stderr, "The server closed the connection\n");
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

        fprintf(stderr, "get cmdId %d\n", id);
        resultMessage[0] = '\0';

        isFailed = (bobai.*functions[id])(data, resultMessage);

        // flush all stdout and stderr messages before sending resultMessage
        // to prevent clashing with main_agent
        fflush(stdout);
        fflush(stderr);

        output[0] = '\0';
        fprintf(stderr, "resultMessage: %s\n", resultMessage);
        if(strlen(resultMessage) > 0){
            if(isFailed){
                sprintf(output, "?%d %s\n", id, resultMessage);
            }else{
                sprintf(output, "=%d %s\n", id, resultMessage);
            }
            }else{
            if(isFailed){
                sprintf(output, "?%d\n", id);
            }else{
                sprintf(output, "=%d\n", id);
            }
        }

        // send the result of command.
        rc = send(sd, output, sizeof(output), 0);
        if (rc < 0) {
            // perror("send() failed");
            break;
        }
        //bytesReceived = 0;
        rc = recv(sd, &buffer[0], BUFFER_LENGTH, 0);
        // printf("receive %d bytes, %s\n", rc, buffer);
        if (rc < 0) {
            // perror("recv() failed");
            break;
        } else if (rc == 0) {
            // fprintf(stderr, "The server closed the connection\n");
            break;
        }

        // sleep and wait for next command (if there is)
        usleep(sleepMicroSecondBetweenCommand);
    } while (true);

    // close socket
    if (sd != -1)
        close(sd);
}