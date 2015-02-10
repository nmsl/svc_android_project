#!/bin/bash

doChop() {
	if [ ! -d "$1" ]; then
		mkdir $1
		echo "Create folder $1"
	fi

	./a.out 0 ../jsvm/$1/$1.264 $1 $1 0
	#./a.out 0 ../jsvm/$1/$1.264 $1 $1 1
	#./a.out 0 ../jsvm/$1/$1.264 $1 $1 2
}

files=("doc-reality" "jeux" "soap" "sport" "talking_head")

for (( i=0; i<${#files[@]}; i++ ));
	do
		doChop ${files[$i]}
	done
