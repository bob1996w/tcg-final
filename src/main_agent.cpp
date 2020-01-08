// basic headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for socket server
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

// for forking
#include <cerrno>
#include <csignal>

void signalHandler(int signal); 

// commands enumerate
enum COMMANDS {
    PROTOCOL_VERSION = 0,  // 0
    NAME,                  // 1
    VERSION,               // 2
    KNOWN_COMMAND,         // 3
    LIST_COMMANDS,         // 4
    QUIT,                  // 5
    BOARDSIZE,             // 6
    RESET_BOARD,           // 7
    NUM_REPETITION,        // 8
    NUM_MOVES_TO_DRAW,     // 9
    MOVE,                  // 10
    FLIP,                  // 11
    GENMOVE,               // 12
    GAME_OVER,             // 13
    READY,                 // 14
    TIME_SETTINGS,         // 15
    TIME_LEFT,             // 16
    SHOWBOARD              // 17
};

// socket server constants
#ifdef SERVER2
    #define SERVER_PATH "/tmp/server2"
#else
    #define SERVER_PATH "/tmp/server"
#endif
#define BUFFER_LENGTH 1024
#define FALSE 0

volatile std::sig_atomic_t gSignalStatus;

int main() {
    // always unlink previous session's port to prevent "address already in use"
    unlink(SERVER_PATH);

    // spawn a child process to run the helper program
    pid_t pid = fork();
    if (pid == 0) {
        // is child process
        /*
#ifdef SERVER2
        static char *argv[] = {"helper2", NULL};
#else
        static char *argv[] = {"helper", NULL};
#endif
        */

#ifdef SERVER2
        execlp("/Users/bobwang/Documents/MasterTwo/tcg/final/r07922018/target/helper2", nullptr);
#else
        execlp("/Users/bobwang/Documents/MasterTwo/tcg/final/r07922018/target/helper", nullptr);
#endif
        
        fprintf(stderr, "Error occured when calling helper program: %d\n", errno);
        fprintf(stderr, "%s\n", strerror(errno));
        return 0;
    }
    // is parent process

    // data for java client
    char read[1024], read_cpy[1024], output[1024], *token;
    const char *data[10];
    int id; 
    // bool isFailed;

    fprintf(stderr, "server path = %s\n", SERVER_PATH);
    
    // add signal handler
    signal(SIGTERM, signalHandler);
    signal(SIGABRT, signalHandler);

    do {
        
        // read command
        fgets(read, 1024, stdin);
        strcpy(read_cpy, read);
        fprintf(stderr, "server>%s", read);
        fflush(stderr);
        // remove newline(\n)
        read[strlen(read) - 1] = '\0';
        // get command id
        token = strtok(read_cpy, " ");
        sscanf(token, "%d", &id);

        //TODO: Call AI Function
        // data for socket server with client (AI)
        int sd = -1, sd2 = -1;
        int rc, length;
        char buffer[BUFFER_LENGTH];
        struct sockaddr_un serveraddr;

        // always unlink previous session's port to prevent "address already in use"
        unlink(SERVER_PATH);

        sd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sd < 0) {
            perror("socket() failed");
        }
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sun_family = AF_UNIX;
        strcpy(serveraddr.sun_path, SERVER_PATH);

        rc = bind(sd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
        if (rc < 0) {
            perror("bind() failed");
        }
        // while (true)
        rc = listen(sd, 10);
        if (rc< 0)
        {
            perror("listen() failed");
        }
        // blocking accept() Function
        sd2 = accept(sd, NULL, NULL);
        if (sd2 < 0)
        {
            perror("accept() failed");
        }
        length = BUFFER_LENGTH;

        buffer[0] = '\0';  // empty the char array
        // 1: listen from helper's call        
        rc = recv(sd2, buffer, sizeof(buffer), 0);
        if (rc < 0)
        {
            perror("recv() failed");
            // break;
        } 
        // fprintf(stderr, "received %d bytes, %s\n", rc, buffer);
        if (rc == 0 ||
            rc < sizeof(buffer))
        {
            fprintf(stderr, "The client closed the connection before all of the\n");
            fprintf(stderr, "data was sent\n");
            // break;
        }
        rc = send(sd2, read, sizeof(read), 0);
        if (rc < 0)
        {
            perror("send() failed");
            // break;
        }

        // 2. receive helper's answer
        buffer[0] = '\0';
        rc = recv(sd2, buffer, sizeof(buffer), 0);
        if (rc < 0)
        {
            perror("recv() failed");
            // break;
        } 
        // fprintf(stderr, "received %d bytes, %s\n", rc, buffer);
        if (rc == 0 ||
            rc < sizeof(buffer))
        {
            fprintf(stderr, "The client closed the connection before all of the\n");
            fprintf(stderr, "data was sent\n");
            // break;
        }
        rc = send(sd2, read, sizeof(read), 0);
        if (rc < 0)
        {
            perror("send() failed");
            // break;
        }

        if (sd != -1)
            close(sd);
        if (sd2 != -1)
            close(sd2);
        unlink(SERVER_PATH);
        
        fprintf(stdout, "%s", buffer);
        fprintf(stderr, "%s", buffer);
        // important, do not delete
        fflush(stdout);
        fflush(stderr);

    } while (id != QUIT);
    
    // kill the child, and wait for the child to exit with 0
    kill(pid, SIGTERM);
    waitpid(pid,0,0);
    printf("after wait");
    
    return 0;
}

void signalHandler(int signal) {
    gSignalStatus = signal;
    fprintf(stdout, "SignalValue: %d\n", gSignalStatus);
    fflush(stdout);
    exit(signal);
}