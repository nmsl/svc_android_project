Source code of our Android SVC decoder.
==================

There are two folders: **no_SDL** and **svc_mc**.

Those two project are used by Eclipse IDE for Android development.

We use Native Development Kit (NDK) to interact with our SVC decoder library.

### Project of *no_SDL*
* Support simple HTTP streaming.
* Using Android API to render the image (i,e,. Bitmap and Canvas classes).
* Using menu button to trigger the switch events.
* The used segments are chopped from [chopper](../stream/chopper).

### Project of *svc_mc*
* Support DASH streaming.
* Porting SDL to render the image rather than Android API.
* Using gesture, which provided by SDL library, to trigger the switch events . 
* The used segments are generated from [DASH script](../DASH/).
