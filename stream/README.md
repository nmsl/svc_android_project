Used OS is Ubuntu 12.04 64-bit

This folder contains the encoding scripts and some video streams. 

We assume the JSVM library is compiled under ~/JSVM and ffmpeg is installed

We have removed all the videos files, which can be downloaded from nsl's website. 

*NOTE: The storage space at least 75 GB*

The four scripts are: 
	raw/get.sh :
		to download the CBC avi files, and downsample them to yuvs in multiple resolutions

	jsvm/encode.sh :
		to encode the yuv files into 264 files

	jsvm/extract.sh :
		to truncate the 264 files into various <s,t>, where s is the resolution and t is the temporal layer

	layered/chopping.sh
		to chop the source video into segments