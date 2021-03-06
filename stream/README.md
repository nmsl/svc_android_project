Getting source videos and encoding
==============
This folder contains the encoding scripts and some video streams. 

We assume the JSVM library is compiled under **~/JSVM** ([how to complie JSVM](../JSVM)) and **ffmpeg** is installed

We have removed all the videos files, which can be downloaded from nsl's website. 

***NOTE: The storage space at least 75 GB, if five videos are downloaded and encoded***

## There are four scripts for downlaoding and encoding:
###raw/get.sh
To download the CBC avi files, and downsample them to yuvs in multiple resolutions **(using ffmpeg)**.

***NOTE: resolutions must the same as encoding configuration, i.e., *.cfg under jsvm.***

###jsvm/encode.sh
To encode the yuv files into 264 files.

###layered/chopping.sh
To chop the source video into segments (not compatible with MPEG-DASH) and parse the NAL header type ([how to use](chopper)).
		
###jsvm/extract.sh (optional)
To truncate the 264 files into various \<s,t\>, where s is the resolution and t is the temporal layer.
