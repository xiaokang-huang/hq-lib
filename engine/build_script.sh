#!/bin/sh

CMD=$1

function print_sector() {
	echo "========================================"
	echo $1
	echo "========================================"
}

if [ $1 = "build" ]; then
	print_sector "building_project"
	(cmake ./)&&(make -j4)
elif [ $1 = "clean" ]; then
	print_sector "clean_project"
	make clean
elif [ $1 = "distclean" ]; then
	print_sector "distclean_project"
	make clean
	rm -rf build
	rm -rf cmake-build-debug
	rm CMakeCache.txt
	find ./ -name "*CMakeFiles" | xargs -I {} rm -rf {}
	find ./ -name "*.cmake" | xargs rm
	find ./ -name "*Makefile" | xargs rm
	rm *.cbp
fi