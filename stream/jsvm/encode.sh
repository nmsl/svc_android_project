ENCODER=~/JSVM/jsvm/bin/H264AVCEncoderLibTestStatic

cd doc-reality/
nohup $ENCODER -pf encoder.cfg > ./tmp/log.1 2>&1 &
cd ../jeux/
nohup $ENCODER -pf encoder.cfg > ./tmp/log.2 2>&1 &
cd ../soap/
nohup $ENCODER -pf encoder.cfg > ./tmp/log.3 2>&1 &
cd ../sport/
nohup $ENCODER -pf encoder.cfg > ./tmp/log.4 2>&1 &
cd ../talking_head/     
nohup $ENCODER -pf encoder.cfg > ./tmp/log.5 2>&1 &

