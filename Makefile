.PHONY: all mylib engine test_mylib test_engine clean

all: mylib test_mylib engine test_engine

mylib:
	scons -Q -C mylib -j4
	cp ./mylib/libmylib.so ./

engine:
	scons -Q -C engine -j4
	cp ./engine/libhqengine.so ./

test_mylib:	
	scons -Q -C test_mylib -j4
	cp ./test_mylib/testmylib ./

test_engine:	
	scons -Q -C test_engine -j4
	cp ./test_engine/testengine ./

clean:
	scons -c -C mylib
	scons -c -C engine
	scons -c -C test_mylib
	scons -c -C test_engine