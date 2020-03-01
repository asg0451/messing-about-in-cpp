this_dir := /home/miles/cpp


all: build/hello build/tree

clean:
	rm build/*

build/%: $(this_dir)/%/src/*.cpp
	clang++ -g -std=c++17 $^ -o $(this_dir)/$@ -I`dirname $(<D)`/lib
