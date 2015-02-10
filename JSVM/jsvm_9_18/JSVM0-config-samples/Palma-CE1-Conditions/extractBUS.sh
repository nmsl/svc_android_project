#!/bin/sh

# =========== MUNICH TEST POINTS ===========

batch/extract.sh	BUS_352x288_30_512	BUS_352x288_15_384	-e 352x288@15:384
batch/extract.sh     BUS_352x288_15_384   BUS_352x288_15_192	-e 352x288@15:192
batch/extract.sh     BUS_352x288_15_192   BUS_176x144_15_96   -e 176x144@15:96 
batch/extract.sh     BUS_176x144_15_96   BUS_176x144_7.5_64	-e 176x144@7.5:64

# =========== OTHER TEST POINTS ===========

batch/extract.sh	BUS_352x288_30_512	BUS_352x288_30_448	-e 352x288@30:448
batch/extract.sh	BUS_352x288_30_512	BUS_352x288_30_384	-e 352x288@30:384
batch/extract.sh	BUS_352x288_30_512	BUS_352x288_30_320	-e 352x288@30:320
batch/extract.sh	BUS_352x288_30_512	BUS_352x288_30_256	-e 352x288@30:256

batch/extract.sh	BUS_352x288_15_384	BUS_352x288_15_320	-e 352x288@15:320
batch/extract.sh	BUS_352x288_15_384	BUS_352x288_15_256	-e 352x288@15:256
batch/extract.sh	BUS_352x288_15_384	BUS_352x288_15_224	-e 352x288@15:224

batch/extract.sh	BUS_352x288_15_384	BUS_352x288_7.5_256	-e 352x288@7.5:256
batch/extract.sh	BUS_352x288_7.5_256	BUS_352x288_7.5_224	-e 352x288@7.5:224
batch/extract.sh	BUS_352x288_7.5_256	BUS_352x288_7.5_192	-e 352x288@7.5:192
batch/extract.sh	BUS_352x288_7.5_256	BUS_352x288_7.5_160	-e 352x288@7.5:160
batch/extract.sh	BUS_352x288_7.5_256	BUS_352x288_7.5_128	-e 352x288@7.5:128

batch/extract.sh	BUS_352x288_15_384	BUS_176x144_15_192	-e 176x144@15:192
batch/extract.sh	BUS_176x144_15_192	BUS_176x144_15_160	-e 176x144@15:160
batch/extract.sh	BUS_176x144_15_192	BUS_176x144_15_128	-e 176x144@15:128
batch/extract.sh	BUS_176x144_15_192	BUS_176x144_15_112	-e 176x144@15:112

batch/extract.sh	BUS_176x144_15_192	BUS_176x144_7.5_128	-e 176x144@7.5:128
batch/extract.sh	BUS_176x144_7.5_128	BUS_176x144_7.5_112	-e 176x144@7.5:112
batch/extract.sh	BUS_176x144_7.5_128	BUS_176x144_7.5_96	-e 176x144@7.5:96
batch/extract.sh	BUS_176x144_7.5_128	BUS_176x144_7.5_80	-e 176x144@7.5:80

