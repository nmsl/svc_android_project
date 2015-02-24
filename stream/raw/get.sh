# Download videos
wget http://nsl.cs.sfu.ca/video/library/CBC/doc-reality.avi
wget http://nsl.cs.sfu.ca/video/library/CBC/jeux.avi
wget http://nsl.cs.sfu.ca/video/library/CBC/soap.avi
wget http://nsl.cs.sfu.ca/video/library/CBC/sport.avi
wget http://nsl.cs.sfu.ca/video/library/CBC/talking_head.avi


# Downsampling and encoding to multiple resolutions
# doc-reality
ffmpeg -i doc-reality.avi -s 160x90     -pix_fmt yuv420p doc-reality-layer0.yuv &
ffmpeg -i doc-reality.avi -s 320x180    -pix_fmt yuv420p doc-reality-layer1.yuv &
ffmpeg -i doc-reality.avi -s 640x360    -pix_fmt yuv420p doc-reality-layer2.yuv &
ffmpeg -i doc-reality.avi -s 1280x720   -pix_fmt yuv420p doc-reality-layer3.yuv &

# jeux version 1
ffmpeg -i jeux.avi -s 160x90    -pix_fmt yuv420p jeux-layer0.yuv &
ffmpeg -i jeux.avi -s 320x180   -pix_fmt yuv420p jeux-layer1.yuv &
ffmpeg -i jeux.avi -s 640x360   -pix_fmt yuv420p jeux-layer2.yuv &
ffmpeg -i jeux.avi -s 1280x720  -pix_fmt yuv420p jeux-layer3.yuv &

# soap version 1
ffmpeg -i soap.avi -s 160x90    -pix_fmt yuv420p soap-layer0.yuv &
ffmpeg -i soap.avi -s 320x180   -pix_fmt yuv420p soap-layer1.yuv &
ffmpeg -i soap.avi -s 640x360   -pix_fmt yuv420p soap-layer2.yuv &
ffmpeg -i soap.avi -s 1280x720  -pix_fmt yuv420p soap-layer3.yuv &

# sport version 1
ffmpeg -i sport.avi -s 160x90   -pix_fmt yuv420p sport-layer0.yuv &
ffmpeg -i sport.avi -s 320x180  -pix_fmt yuv420p sport-layer1.yuv &
ffmpeg -i sport.avi -s 640x360  -pix_fmt yuv420p sport-layer2.yuv &
ffmpeg -i sport.avi -s 1280x720 -pix_fmt yuv420p sport-layer3.yuv &

# talking_head version 1
ffmpeg -i talking_head.avi -s 160x90    -pix_fmt yuv420p talking_head-layer0.yuv &
ffmpeg -i talking_head.avi -s 320x180   -pix_fmt yuv420p talking_head-layer1.yuv &
ffmpeg -i talking_head.avi -s 640x360   -pix_fmt yuv420p talking_head-layer2.yuv &
ffmpeg -i talking_head.avi -s 1280x720  -pix_fmt yuv420p talking_head-layer3.yuv &