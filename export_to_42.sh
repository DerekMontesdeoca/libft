#! /usr/bin/sh

SCRIPT_DIR=$(realpath $(dirname $0))

mkdir -p $1
rsync -azv $SCRIPT_DIR/*.h $SCRIPT_DIR/*.c $SCRIPT_DIR/Makefile $1 
