cd doc-reality
ffmpeg -f h264 -i doc_reality_avc.264 -vcodec copy doc_reality_avc.mp4
cd ../
cd jeux
ffmpeg -f h264 -i jeux_avc.264 -vcodec copy jeux_avc.mp4
cd ../
cd soap
ffmpeg -f h264 -i soap_avc.264 -vcodec copy soap_avc.mp4
cd ../
cd sport
ffmpeg -f h264 -i sport_avc.264 -vcodec copy sport_avc.mp4
cd ../
cd talking_head
ffmpeg -f h264 -i talking_head_avc.264 -vcodec copy talking_head_avc.mp4
cd ../

