.PHONY all:
all:IpcShmCli IpcShmSer

IpcShmCli:ipcShmCli.cpp
	g++ -o $@ $^

IpcShmSer:ipcShmSer.cpp
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -rf IpcShmCli IpcShmSer