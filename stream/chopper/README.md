How to use chopping script
==================
This chopper provides chopping video into segments for **old version decoder** and paring SVC video format (the sequence of NAL headers).

## Compile source code
````
make clear; make
````

## Chopping video into segments
NOTE: The GOP (group of picture) for each segment is 8.
````
Usage: ./chopping.sh -f video -m mode_number [seg_no] [-o output_folder] [-p prefix_name] -l layer_id
````
There are six parameters.
- **-f video**: indicate the location of video source
- **-m mode_number**: there are three modes
  - **mode 0**: chopping all segments from video.
  - **mode 1**: chopping the [seg_no]th segment from video.
  - **mode 3**: only show the NAL header message.
- **-o output_folder**: the location for output files (default is current folder).
- **-p prefix_name**: this parameter is used for output segment name and output sub-folder (default is 'tmp').
- **-l layer_id**: the range of layer_id is from 0 to 2. 0 meas based layer only. 2 means based layer + two enhancement layers.

## Examples
## Example 1:
Getting all segment from video1.264 with based layer.

The segments are located under `outputs/video1/` folder. Each name of sgmenet start with `video1`
````
./chopping -f video1.264 -m 0 -o outputs -p video1 -l 0
````

## Example 2:
Getting the 5th segment from video2.264 with based layer and 1 enhancement layer.

The segment is stored under `./test/` and name of segment is `test_5_1.264`.
````
./chopping -f video2.264 -m 1 5 -p test -l 1
````

## Example 3:
Parsing SVC format video to get the NAL header type. Using exmaple 2 result as this example input.

Getting the sequence of NAL headers from `test_5_1.264` and stored at `./msg/msg_parsing.log`.
````
./chopping -f ./test/test_5_1.264 -m 2 -p msg
````
