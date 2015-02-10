Multi-Core SVC Decoder on Android
==============

We design, implement, and evaluate an H.264/SVC decoder and an HTTP video streaming client on multi-core mobile devices.
The SVC decoding parts use the library: [Open SVC Decoder](http://sourceforge.net/projects/opensvcdecoder/).
The decoder employs multiple decoder threads to leverage the multi-core CPUs, and the streaming server/client support adaptive HTTP video streaming.

This work have published on the 2013 IEEE International Conference on Multimedia and Expo (ICME 2013).

We provide our source code and encourage other people working on this field.
The following guideline have tested successfully on Ubuntu 12.04 LTS.

## Environment Setup
1. Install Java JDK
2. Install Eclipse IDE and CDT package
3. Install Android SDK 
4. Setup Android plugin for Eclipse 
5. Install Android NDK

## Configurate USB device
If you plan to run the App on a Android device, you can follow the instructions connecting your device to Linux. 

1. Get the Vender Number and Model Number of your device

  Use commend `lsusb` then you can find that. 
2. Add new USB rules to Linux 

  Create the file (if it does not exist): `/etc/udev/rules.d/51-android.rules`
  Add new rule with the vender number and model number to the file:
  ````
  SUBSYSTEM==“usb”, SYSFS{idVendor}==“0bb4”, MODE=“0cd6”
  ````
3. Change file permission such that you can execute it 

  ````
  sudo chmod a+rx /etc/udev/rules.d/51-android.rules
  ````
4. Apply your change

  ````
  sudo restart udev
  ````
5. Check whether your device is connected

  ````
  <your Android SDK path>/platform-tools/adb kill-server 
  sudo <your Android SDK path>/platform-tools/adb devices
  ````
  You can find your device information if the operation is succeed.
  
## Build source code and run the App
1. Go to the source code folder 
2. Build library

  ````
  <your Android NDK path>/ndk-build 
  ````
3. The shared library for decoding will be created. Then you can execute the Android program from Eclipse IDE
