#!/bin/sh

# =========== MUNICH TEST POINTS ===========

batch/extract.sh	SOCCER_704x576_60_3072	SOCCER_704x576_30_1536	-e 704x576@30:1536 
batch/extract.sh     SOCCER_704x576_30_1536   	SOCCER_352x288_30_768	-e 352x288@30:768  
batch/extract.sh     SOCCER_352x288_30_768   	SOCCER_352x288_30_384       	-e 352x288@30:384  
batch/extract.sh     SOCCER_352x288_30_384   	SOCCER_176x144_15_192	-e 176x144@15:192
batch/extract.sh     SOCCER_176x144_15_192   	SOCCER_176x144_15_96	-e 176x144@15:96

# =========== OTHER TEST POINTS ===========

batch/extract.sh	SOCCER_704x576_60_3072	SOCCER_704x576_60_2560	-e 704x576@60:2560 
batch/extract.sh	SOCCER_704x576_60_3072	SOCCER_704x576_60_2048	-e 704x576@60:2048 
batch/extract.sh	SOCCER_704x576_60_3072	SOCCER_704x576_60_1780	-e 704x576@60:1780 
batch/extract.sh	SOCCER_704x576_60_3072	SOCCER_704x576_60_1536	-e 704x576@60:1536 

batch/extract.sh	SOCCER_704x576_60_3072	SOCCER_704x576_30_2048	-e 704x576@30:2048 
batch/extract.sh	SOCCER_704x576_30_2048	SOCCER_704x576_30_1792	-e 704x576@30:1792 
batch/extract.sh	SOCCER_704x576_30_1536	SOCCER_704x576_30_1280	-e 704x576@30:1280 
batch/extract.sh	SOCCER_704x576_30_1536	SOCCER_704x576_30_1024	-e 704x576@30:1024 

batch/extract.sh	SOCCER_704x576_30_2048	SOCCER_704x576_15_1536	-e 704x576@15:1536 
batch/extract.sh	SOCCER_704x576_15_1536	SOCCER_704x576_15_1280	-e 704x576@15:1280 
batch/extract.sh	SOCCER_704x576_15_1536	SOCCER_704x576_15_1024	-e 704x576@15:1024 
batch/extract.sh	SOCCER_704x576_15_1536	SOCCER_704x576_15_896	-e 704x576@15:896  
batch/extract.sh	SOCCER_704x576_15_1536	SOCCER_704x576_15_768	-e 704x576@15:768  

batch/extract.sh	SOCCER_352x288_30_768	SOCCER_352x288_30_640	-e 352x288@30:640  
batch/extract.sh	SOCCER_352x288_30_768	SOCCER_352x288_30_512	-e 352x288@30:512  
batch/extract.sh	SOCCER_352x288_30_768	SOCCER_352x288_30_448	-e 352x288@30:448  

batch/extract.sh	SOCCER_352x288_30_768	SOCCER_352x288_15_512	-e 352x288@15:512  
batch/extract.sh	SOCCER_352x288_15_512	SOCCER_352x288_15_448	-e 352x288@15:448  
batch/extract.sh	SOCCER_352x288_15_512	SOCCER_352x288_15_384	-e 352x288@15:384  
batch/extract.sh	SOCCER_352x288_15_512	SOCCER_352x288_15_320	-e 352x288@15:320  
batch/extract.sh	SOCCER_352x288_15_512	SOCCER_352x288_15_256	-e 352x288@15:256  

batch/extract.sh	SOCCER_352x288_15_512	SOCCER_352x288_7.5_384	-e 352x288@7.5:384 
batch/extract.sh	SOCCER_352x288_7.5_384	SOCCER_352x288_7.5_320	-e 352x288@7.5:320 
batch/extract.sh	SOCCER_352x288_7.5_384	SOCCER_352x288_7.5_256	-e 352x288@7.5:256 
batch/extract.sh	SOCCER_352x288_7.5_384	SOCCER_352x288_7.5_224	-e 352x288@7.5:224 
batch/extract.sh	SOCCER_352x288_7.5_384	SOCCER_352x288_7.5_192	-e 352x288@7.5:192 

batch/extract.sh	SOCCER_176x144_15_192	SOCCER_176x144_15_160	-e 176x144@15:160
batch/extract.sh	SOCCER_176x144_15_192	SOCCER_176x144_15_128	-e 176x144@15:128
batch/extract.sh	SOCCER_176x144_15_192	SOCCER_176x144_15_112	-e 176x144@15:112
