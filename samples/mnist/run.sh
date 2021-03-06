#!/bin/bash

set -e

# Download the files if necessary
if [ ! -f train-images-idx3-ubyte ]; then
	wget http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz

	gunzip train-images-idx3-ubyte.gz
fi

if [ ! -f train-labels-idx1-ubyte ]; then
	wget http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz

	gunzip train-labels-idx1-ubyte.gz
fi

if [ ! -f t10k-images-idx3-ubyte ]; then
	wget http://yann.lecun.com/exdb/mnist/t10k-images-idx3-ubyte.gz

	gunzip t10k-images-idx3-ubyte.gz
fi

if [ ! -f t10k-labels-idx1-ubyte ]; then
	wget http://yann.lecun.com/exdb/mnist/t10k-labels-idx1-ubyte.gz

	gunzip t10k-labels-idx1-ubyte.gz
fi

# Compile and run/debug
if [ "$1" == "vgrind" ]; then
	g++-8 -I ../../engine main.cpp ../../bin/libzhp.a -pthread -g -o mnist
elif [ "$1" == "gdb" ]; then
	g++-8 -I ../../engine main.cpp ../../bin/libzhp.a -pthread -g -o mnist
else
	g++-8 -I ../../engine main.cpp ../../bin/libzhp.a -pthread -O3 -o mnist
fi

mkdir -p bin

mv mnist bin/

if [ "$1" == "gdb" ]; then
	gdb ./bin/mnist
elif [ "$1" == "vgrind" ]; then
	valgrind --leak-check=full --track-origins=yes ./bin/mnist
else
	./bin/mnist
fi
