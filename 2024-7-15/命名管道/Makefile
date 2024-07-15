.PHONY all:
all:clientFIFO serverFIFO

clientFIFO:clientFIFO.cpp
	g++ -o $@ $^

serverFIFO:serverFIFO.cpp
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -rf clientFIFO serverFIFO .fifo