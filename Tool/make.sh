#!/bin/sh
g++ -Wall -O2 -o a.out $1cpp
g++ -Wall -O2 -o test.out $1test.cpp
g++ -Wall -O2 -o sub.out $1sub.cpp
