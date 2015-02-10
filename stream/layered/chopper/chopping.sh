#!/bin/bash

# SOURCE="/home/jargo/videos/svc/raw_videos/1280x720/old_encoder"
SOURCE="./"

doChop () {
    mkdir -p $1

    fileName="${1}2.264"

    # ./main 0 ../jsvm/$1/$fileName $1 $1 0
    # ./main 0 ../jsvm/$1/$fileName $1 $1 1
    # ./main 0 ../jsvm/$1/$fileName $1 $1 2
    ./main 0 ${SOURCE}/$1/$fileName $1 $1 0
    ./main 0 ${SOURCE}/$1/$fileName $1 $1 1
    ./main 0 ${SOURCE}/$1/$fileName $1 $1 2
}

# files=("doc-reality" "jeux" "soap" "talking_head")
files=("me")

for (( i=0; i<${#files[@]}; i++ )) do
    doChop ${files[$i]}
done
