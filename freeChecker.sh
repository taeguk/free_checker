#!/bin/bash

c_file=$(dirname $0)"/fc.c"
so_file=$(dirname $0)"/fc.so"
#if [ ! -f "$so_file" ];then
	echo "Making fc.so..."
	if [ ! -f "$c_file" ];then
		echo "$c_file 가 존재하지 않습니다.."
		exit
	fi
	gcc -fPIC -shared -o $so_file $c_file -ldl
	echo "Make Complete!"
	echo
#fi

echo "----------free check start----------"

if [ "$#" -eq "0" ]; then
	echo "[Usage] freeChecker.sh target_program target_program_parameters"
else
	cmd="LD_PRELOAD=$so_file $@"
	eval $cmd
fi
