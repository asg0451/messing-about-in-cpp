# use eg (cd .. && make build/tree) from inside subproj

CC := clang++
CC_OPTS := -g -Wall -Werror -Wextra -Wpedantic -std=c++17


all: build/hello build/tree

clean:
	rm -rf build/*
	make build/obj

build/obj:
	mkdir build/obj


# added src as include. see comment in tree.h
# added benchmark (local), google-sparse-hash (local), curl (system)
build/%: %/main.cpp # build/obj/%.o
	$(CC) $(CC_OPTS) $^ -o $@ -I$(<D)/lib -Icommon/lib \
	-isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread \
	-Isparsehash/install/include \
	-lcurl

build/obj/%.o: %/src/*.cpp
	clang++ -g -std=c++17 $^ -o $@ -I`dirname $(<D)`/lib -I$(<D) -Icommon/lib -c
