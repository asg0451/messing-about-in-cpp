# use eg (cd .. && make build/tree) from inside subproj

all: build/hello build/tree

clean:
	rm -rf build/*
	make build/obj

build/obj:
	mkdir build/obj

# added pthread & benchmark from benchmark dir. did make install, couldnt figure out otherwise
# added src as include. see comment in tree.h
build/%: %/main.cpp build/obj/%.o
	clang++ -g -std=c++17 $^ -o $@ -I$(<D)/lib -I$(<D)/src -Icommon/lib -pthread -lbenchmark

build/obj/%.o: %/src/*.cpp
	clang++ -g -std=c++17 $^ -o $@ -I`dirname $(<D)`/lib -I$(<D) -Icommon/lib -c
