#! /usr/bin/sh

mkdir -p $1
rsync -azv *.h *.c Makefile $1 
