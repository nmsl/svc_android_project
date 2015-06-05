Download JSVM
======================
[This](http://www.hhi.fraunhofer.de/departments/video-coding-analytics/research-groups/image-video-coding/research-topics/svc-extension-of-h264avc/jsvm-reference-software.html) is JSVM web page for reference.

###Following setps are the simplified steps for downloading the latest version of JSVM.
1. Install CVS by
  ````
  sudo apt-get install cvs
  ````

2. Login CVS server
  ````
  cvs -d :pserver:jvtuser:jvt.Amd.2@garcon.ient.rwth-aachen.de:/cvs/jvt login
  ````

3. Download source cod
  ````
  cvs -d :pserver:jvtuser@garcon.ient.rwth-aachen.de:/cvs/jvt checkout jsvm
  ````

How to compile JSVM
======================
There are two verions of jsvm. You can use any one of them. The encoding results are the same.

1 go to build folder
  ````
  cd JSVM/H264Extension/build/linux
  ````

2 make and compile
  ````
  make
  ````

3 after compiling, the executable files are located at `jsvm/bin/`
  The file `jsvm/bin/H264AVCEncoderLibTestStatic` is used for encode.sh which is under [stream/jsvm](../stream/jsvm).

Compling Errors:
======================
1. `'SEEK_SET' was not declared in this scope` or `'SEEK_CUR' was not declared in this scope`
  * Include header `#include <cstdio>` into `JSVM/H264Extension/include/Macros.h`

1. `'push_back' was not declared in this scope` or `'insert' was not declared in this scope` inside `JSVM/include/H264AVCCommonIf.h`
  * change this one around line 93 from
  ````
  MyList& operator += ( const MyList& rcMyList) {
    if( ! rcMyList.empty() ) {
      insert( this->end(), rcMyList.begin(), rcMyList.end()); // error here
    } 
    
    return *this; 
  } // leszek
  ````
  to
  ````
  MyList& operator += ( const MyList& rcMyList) {
    if( ! rcMyList.empty() ) {
      this->insert( this->end(), rcMyList.begin(), rcMyList.end()); // add 'this->' before insert
    } 
    
    return *this; 
  } // leszek
  ````
  * Insert `this->` before `push_back` around line 96
  * Insert `this->` before `push_front` around line 97
