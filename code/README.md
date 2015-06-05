Source code of our Android SVC decoder.
==================

There are two folders: **no_SDL** and **svc_mc**.
Those two project are used by Eclipse IDE for Android development.
We use Native Development Kit (NDK) to interact with our SVC decoder.

### Porject of *no_SDL*
* Do not support DASH streaming.
* Using Android API to render the image (i,e,. bitmap and canvas classes).
* The used segments are chopped from [chopper](../stream/chopper).

### Project of *svc_mc*
* Support DASH streaming.
* Porting SDL to render the image rather than Android API.
* The used segments are generated from [DASH script](../DASH/).
