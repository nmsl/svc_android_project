cd doc-reality/
nohup ~/JSVM/jsvm/bin/H264AVCEncoderLibTestStatic -pf encoder.cfg > /tmp/log.1 2>&1 &
cd ../jeux/
nohup ~/JSVM/jsvm/bin/H264AVCEncoderLibTestStatic -pf encoder.cfg > /tmp/log.2 2>&1 &
cd ../soap/
nohup ~/JSVM/jsvm/bin/H264AVCEncoderLibTestStatic -pf encoder.cfg > /tmp/log.3 2>&1 &
cd ../sport/
nohup ~/JSVM/jsvm/bin/H264AVCEncoderLibTestStatic -pf encoder.cfg > /tmp/log.4 2>&1 &
cd ../talking_head/     
nohup ~/JSVM/jsvm/bin/H264AVCEncoderLibTestStatic -pf encoder.cfg > /tmp/log.5 2>&1 &

