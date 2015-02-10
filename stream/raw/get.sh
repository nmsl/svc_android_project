wget http://nsl.cs.sfu.ca/video/library/CBC/doc-reality.avi
# doc-reality version 1
#ffmpeg -i doc-reality.avi -s 240x136 -pix_fmt yuv420p doc-reality-layer0.yuv &
#ffmpeg -i doc-reality.avi -s 480x272 -pix_fmt yuv420p doc-reality-layer1.yuv &
#ffmpeg -i doc-reality.avi -s 960x544 -pix_fmt yuv420p doc-reality-layer2.yuv &
# doc-reality version 2
ffmpeg -i doc-reality.avi -s 150x80 -pix_fmt yuv420p doc-reality-layer0.yuv &
ffmpeg -i doc-reality.avi -s 300x158 -pix_fmt yuv420p doc-reality-layer1.yuv &
ffmpeg -i doc-reality.avi -s 598x314 -pix_fmt yuv420p doc-reality-layer2.yuv &
ffmpeg -i doc-reality.avi -s 1196x628 -pix_fmt yuv420p doc-reality-layer3.yuv &

wget http://nsl.cs.sfu.ca/video/library/CBC/jeux.avi
# jeux version 1
#ffmpeg -i jeux.avi -s 240x136 -pix_fmt yuv420p jeux-layer0.yuv &
#ffmpeg -i jeux.avi -s 480x272 -pix_fmt yuv420p jeux-layer1.yuv &
#ffmpeg -i jeux.avi -s 960x544 -pix_fmt yuv420p jeux-layer2.yuv &
# jeux version 2
ffmpeg -i jeux.avi -s 150x80 -pix_fmt yuv420p jeux-layer0.yuv &
ffmpeg -i jeux.avi -s 300x158 -pix_fmt yuv420p jeux-layer1.yuv &
ffmpeg -i jeux.avi -s 598x314 -pix_fmt yuv420p jeux-layer2.yuv &
ffmpeg -i jeux.avi -s 1196x628 -pix_fmt yuv420p jeux-layer3.yuv &

wget http://nsl.cs.sfu.ca/video/library/CBC/soap.avi
# soap version 1
#ffmpeg -i soap.avi -s 240x136 -pix_fmt yuv420p soap-layer0.yuv &
#ffmpeg -i soap.avi -s 480x272 -pix_fmt yuv420p soap-layer1.yuv &
#ffmpeg -i soap.avi -s 960x544 -pix_fmt yuv420p soap-layer2.yuv &
# soap version 2
ffmpeg -i soap.avi -s 150x80 -pix_fmt yuv420p soap-layer0.yuv &
ffmpeg -i soap.avi -s 300x158 -pix_fmt yuv420p soap-layer1.yuv &
ffmpeg -i soap.avi -s 598x314 -pix_fmt yuv420p soap-layer2.yuv &
ffmpeg -i soap.avi -s 1196x628 -pix_fmt yuv420p soap-layer3.yuv &

wget http://nsl.cs.sfu.ca/video/library/CBC/sport.avi
# sport version 1
#ffmpeg -i sport.avi -s 240x136 -pix_fmt yuv420p sport-layer0.yuv &
#ffmpeg -i sport.avi -s 480x272 -pix_fmt yuv420p sport-layer1.yuv &
#ffmpeg -i sport.avi -s 960x544 -pix_fmt yuv420p sport-layer2.yuv &
# sport version 2
ffmpeg -i sport.avi -s 150x80 -pix_fmt yuv420p sport-layer0.yuv &
ffmpeg -i sport.avi -s 300x158 -pix_fmt yuv420p sport-layer1.yuv &
ffmpeg -i sport.avi -s 598x314 -pix_fmt yuv420p sport-layer2.yuv &
ffmpeg -i sport.avi -s 1196x628 -pix_fmt yuv420p sport-layer3.yuv &

wget http://nsl.cs.sfu.ca/video/library/CBC/talking_head.avi
# talking_head version 1
#ffmpeg -i talking_head.avi -s 240x136 -pix_fmt yuv420p talking_head-layer0.yuv &
#ffmpeg -i talking_head.avi -s 480x272 -pix_fmt yuv420p talking_head-layer1.yuv &
#ffmpeg -i talking_head.avi -s 960x544 -pix_fmt yuv420p talking_head-layer2.yuv &
# talking head version 2
ffmpeg -i talking_head.avi -s 150x80 -pix_fmt yuv420p talking_head-layer0.yuv &
ffmpeg -i talking_head.avi -s 300x158 -pix_fmt yuv420p talking_head-layer1.yuv &
ffmpeg -i talking_head.avi -s 598x314 -pix_fmt yuv420p talking_head-layer2.yuv &
ffmpeg -i talking_head.avi -s 1196x628 -pix_fmt yuv420p talking_head-layer3.yuv &

