How to use dashing script
===============
This scripte is using MP4Box for importing to MP4 format and chopping into segments.
````
Usage: ./command.sh -f video_source [-l segment_length_in_ms]
````
Parameters:
- **-f video_source**: The video souce for importing
- **-l segment_length_in_ms**: The segment lengt (in term of ms). Default value is 51200.

After dashing, you can get the segments under `./dash_output`.

Note that, if video has four layers, and video length is 5.12s. Then you can find four segments inside the output folder.
It meas video will be chopped according to video length and layer number.
