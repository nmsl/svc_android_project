#!/bin/bash

argv=("$@")
argc=$#

# Vars
video=""
mode=0
seg_number=0
output="."
prefix="tmp"
layerID=2
outFolder="./tmp"

if [ $argc == "0" ]; then
    echo "Usage: ./chopping.sh -f video.264 -m mode_number [seg_no] [-o output_folder] [-p prefix_name] -l layer_id"
    echo "      mode_number     :   0 -> chopping all segments"
    echo "                          1 -> chopping <seg_no>th segment"
    echo "                          2 -> show NAL message"
    echo "      output_folder   :   Default is current location"
    echo "      prefix_name     :   Used for output video name and output sub-folder name. Default is tmp."
    echo "      layer_id        :   Range from 0 to 2. Default is 2."
    exit
fi

function show_vars () {
    echo "video      = $video"
    echo "mode       = $mode"
    echo "seg_number = $seg_number"
    echo "output     = $output"
    echo "prefix     = $prefix"
    echo "layerID    = $layerID"
}

function check_bound() {
    if [ $1 -le $2 ]; then
        echo "Error: missing input variable after $3."
        exit
    fi
}

function check_video() {
    if [ ! -f $1 ]; then
        echo "Error: File $1 not found."
        exit
    fi
}

function check_is_numeric() {
    if [ ! "$(echo $1 | grep "^[ [:digit:] ]*$")" ]; then
        echo "Error: seg_no=$1 is not numeric."
        exit
    fi
}

function check_range() {
    if [ "$1" -lt 0 -o "$1" -gt 2 ]; then
        echo "Error: $2 range is incorrect, shoud from 0 to 2."
        exit
    fi
}

function main () {
    for (( i=0; i<$argc; i++ )) do
        var=${argv[$i]}
        case $var in
            "-f")
                check_bound $argc $((i+1)) $var
                video=${argv[$((++i))]}
                check_video $video
                ;;
            "-m")
                check_bound $argc $((i+1)) $var
                mode=${argv[$((++i))]}
                check_range $mode "mode"
                if [ $mode == "1" ]; then
                    seg_number=${argv[$((++i))]}
                    check_is_numeric $seg_number "seg_no"
                fi
                ;;
            "-o")
                check_bound $argc $((i+1)) $var
                output=${argv[$((++i))]}
                ;;
            "-p")
                check_bound $argc $((i+1)) $var
                prefix=${argv[$((++i))]}
                ;;
            "-l")
                check_bound $argc $((i+1)) $var
                layerID=${argv[$((++i))]}
                check_is_numeric $layerID "layerID"
                check_range $layerID "layerID"
                ;;
            *)  
                echo "unknow input : $var"
                ;;
        esac
    done

    # show_vars
    outFolder="$output/$prefix"
    mkdir -p "$outFolder"

    case $mode in
        "0")
            echo "Start chopping all segments of $video"
            # echo "./main 0 $video $outFolder $prefix $layerID"
            ./main 0 $video $outFolder $prefix $layerID
            ;;
        "1")
            echo "Start chopping $seg_number segment of $video"
            # echo "./main 1 $video $outFolder $prefix $layerID $seg_number"
            ./main 1 $video $outFolder $prefix $layerID $seg_number
            ;;
        "2")
            echo "Start parsing $video"
            # echo "./main_g 2 $video > $outFolder/${prefix}_layer${layerID}_$(date +%s).log"
            ./main_g 2 $video > $outFolder/${prefix}_parsing.log
            ;;
        *)
            ;;
    esac
}

main