CPLUSPLUS = g++ -std=c++11 -O2 -Wall
DEL = rm -f

all: src/main.cpp src/MyAI.cpp
	mkdir -p target
	$(CPLUSPLUS) src/main.cpp src/MyAI.cpp -o target/client

clean:
	$(DEL) -r target