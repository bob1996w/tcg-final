CPLUSPLUS = g++ -std=c++11  -Wall
LIB = BobAI.o Board.o CustomTypes.o IStrategy.o Piece.o Utility.o TreeNode.o EarlyGameStrategy.o SecondStrategy.o HashMap.o NodePool.o
ALLFILES = src/main_agent.cpp src/main_helper.cpp src/Board.cpp src/BobAI.cpp src/CDCRule.cpp src/CustomTypes.cpp src/ISTrategy.cpp src/Piece.cpp src/Utility.cpp src/TreeNode.cpp src/EarlyGameStrategy.cpp src/SecondStrategy.cpp src/HashMap.cpp src/NodePool.cpp
DEL = rm -f

all:
	mkdir -p target
	$(CPLUSPLUS) -D DEBUG src/*.cpp -c
	$(CPLUSPLUS) main_agent.o -o target/agent
	$(CPLUSPLUS)  main_helper.o $(LIB) -o target/helper
	$(DEL) *.o

noDebug: hash rule
	mkdir -p target
	$(CPLUSPLUS) src/*.cpp -c
	$(CPLUSPLUS) main_agent.o -o target/agent
	$(CPLUSPLUS)  main_helper.o $(LIB) -o target/helper
	$(DEL) *.o

agent2: hash rule
	mkdir -p target
	$(CPLUSPLUS) -D SERVER2 src/*.cpp -c
	$(CPLUSPLUS) -D SERVER2 main_agent.o -o target/agent2
	$(CPLUSPLUS) -D SERVER2 main_helper.o $(LIB) -o target/helper2
	$(DEL) *.o

rule: src/ruleCreator/ruleCreator.cpp
	mkdir -p target
	$(CPLUSPLUS) src/ruleCreator/ruleCreator.cpp -o target/ruleCreator
	target/ruleCreator > src/CDCRule.hpp

hash: src/hashCreator/hashCreator.cpp
	mkdir -p target
	$(CPLUSPLUS) src/hashCreator/hashCreator.cpp -o target/hashCreator
	target/hashCreator > src/HashConstant.hpp

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
	$(DEL) *.o
	$(DEL) src/CDCRule.hpp;