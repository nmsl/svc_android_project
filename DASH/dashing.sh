argv=("$@")
argc=$#

video=""
length="5120"

if [ $argc == "0" ]; then
    echo "Usage: ./command.sh -f video_source [-l segment_length_in_ms]"
    echo "      -l segment_length_in_ms : the length for each segment. default is 51200"
    exit
fi

show_vars() {
    echo "video = $video"   
    echo "length = $length"   
    echo "with_rap = $with_rap"   
}

check_bound() {
    if [ $1 -le $2 ]; then
        echo "Error: missing input variable after $3"
        exit
    fi
}

check_video() {
    if [ ! -f "$video" ]; then
        echo "File $video not found."
        exit
    fi
}

check_is_numeric() {
    if [ ! "$(echo $length | grep "^[ [:digit:] ]*$")" ]; then
        echo "segment_length_in_ms=$length is not numeric"
        exit
    fi
}

for (( i=0; i<$argc; i++ )) do
    var=${argv[$i]}
    case $var in
        "-f")
            check_bound $argc $((i+1)) $var
            video=${argv[$((++i))]}
            check_video
            ;;
        "-l")
            check_bound $argc $((i+1)) $var
            length=${argv[$((++i))]}
            check_is_numeric
            ;;
        "-rap")
            with_rap=$var
            ;;
        *)
            echo "unknown input : $var"
            ;;
    esac
done

mkdir -p dash_output
cd dash_output
rm -f *.m4s *.mpd *.mp4

# Import an Scalable Video Coding (SVC) bit-stream into a ISO Base Media file
#
# Usage: MP4Box -add inputfile:svcmode=MODE output.mp4
#     MODE : merged|splitbase|splitall
importing() {
    echo "Importing svc bitstream into ISOBMF"
    MP4Box -add "${video}:svcmode=splitall" svc_splitall.mp4
    echo "Importing Done."
    echo ""
}

# extract each track into decodable bit-stream
# Usage: MP4Box -raw trackID inputFile
extracting() {
    echo "Start extract layers"
    MP4Box -raw 1 svc_splitall.mp4
    MP4Box -raw 2 svc_splitall.mp4
    MP4Box -raw 3 svc_splitall.mp4
    MP4Box -raw 4 svc_splitall.mp4
}

# Generating MPD file and dashed segments with $length ms
dashing() {
    MP4Box -dash ${length} -segment-name SVC svc_splitall.mp4
}

importing
dashing