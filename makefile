CPLUSPLUS = g++ -std=c++11 -O2 -Wall
DEL = rm -f

all: src/main_agent.cpp src/main_helper.cpp
	mkdir -p target
	$(CPLUSPLUS) src/main_agent.cpp -o target/agent
	$(CPLUSPLUS) src/main_helper.cpp -o target/helper

example: src/main.cpp src/MyAI.cpp
	mkdir -p target
	$(CPLUSPLUS) src/main.cpp src/MyAI.cpp -o target/client
	# gprof - software profiler
	$(CPLUSPLUS) src/main.cpp src/MyAI.cpp -pg -o target/client_profile

socket: src/socketExample/client.cpp src/socketExample/server.cpp
	mkdir -p target
	$(CPLUSPLUS) src/socketExample/client.cpp -o target/socketClient
	$(CPLUSPLUS) src/socketExample/server.cpp -o target/socketServer
clean:
	$(DEL) -r target