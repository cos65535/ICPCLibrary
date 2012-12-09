#!/bin/sh
./test.out $1 > temp.txt
./a.out < temp.txt > a.txt
./sub.out < temp.txt > b.txt
cmp a.txt b.txt
