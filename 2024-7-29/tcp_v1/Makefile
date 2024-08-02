.PHONY:all
all:clientTcp serverTcp

clientTcp:clientTcp.cc
	g++ -o $@ $^ 
serverTcp:serverTcp.cc
	g++ -o $@ $^ -lpthread

.PHONY:clean
clean: 
	rm -rf clientTcp serverTcp