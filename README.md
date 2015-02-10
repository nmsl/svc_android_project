Multi-Core SVC Decoder on Android
==============

We design, implement, and evaluate an H.264/SVC decoder and an HTTP video streaming client on multi-core mobile devices.
The SVC decoding parts use the library: [Open SVC Decoder](http://sourceforge.net/projects/opensvcdecoder/).
The decoder employs multiple decoder threads to leverage the multi-core CPUs, and the streaming server/client support adaptive HTTP video streaming.

This work have published on the 2013 IEEE International Conference on Multimedia and Expo (ICME 2013).

We provide our source code and encourage other people working on this field.
The following guideline have tested successfully on Ubuntu 12.04 LTS.

# Tools
1. Oracle-JDK, [SDK](http://developer.android.com/sdk/index.html), [NDK](http://developer.android.com/tools/sdk/ndk/index.html)
2. Eclipse and ADT plugin.

# Configurate USB device
If you plan to run the App on a Android device, you can follow the instructions connecting your device to Linux. 

1. Get the **Vender Number** and **Model Number** of your device

  Use commend `lsusb` then you wii see information like below. 
  
  ````
  Bus 001 Device 003: ID 0bb4:0dfb HTC (High Tech Computer Corp.) 
  Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
  Bus 002 Device 004: ID 0e0f:0008 VMware, Inc. 
  Bus 002 Device 003: ID 0e0f:0002 VMware, Inc. Virtual USB Hub
  Bus 002 Device 002: ID 0e0f:0003 VMware, Inc. Virtual Mouse
  Bus 002 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
  ````
  For example. The **Vender Number** and **Model Number** for my HTC device are `0bb4` and `0dfb`, respectively.
2. Add new USB rules to Linux 

  Create the file (if it does not exist): `/etc/udev/rules.d/51-android.rules`
  Add new rule with the **Vender Number** and **Model Number** to the file.
  ````
  SUBSYSTEM==“usb”, SYSFS{idVendor}==“0bb4”, MODE=“0dfb”
  ````
3. Change file permission such that you can execute it.

  ````
  sudo chmod a+rx /etc/udev/rules.d/51-android.rules
  ````
4. Apply your change.

  ````
  sudo restart udev
  ````
5. Check whether your device is connected

  ````
  <your Android SDK path>/platform-tools/adb kill-server 
  sudo <your Android SDK path>/platform-tools/adb devices
  ````
  You can find your device in Eclipse DDMS, if operations are succeed.
  
# Download project
This project has two sub-folder. One is android project. The other is for encoding videos.

`git clone https://github.com/nmsl/svc_android_project.git`

## How to encode videos

  
# Build source code and run the App
1. Go to the source code from `git`
2. Build library

  ````
  cd <project/folder>
  <your Android NDK path>/ndk-build 
  ````
3. The shared libraries are created. Then you can execute the Android program from Eclipse.
