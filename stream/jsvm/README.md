Configuration of video encoding for JSVM.
=========================

## encode.sh
This script is used for encoding five videos into SVC format.

There are five sub-folders, each sub-folder contains one **encoder.cfg** and multiple **layer#.cfg**

If your **JSVM** encoder is not under `~/JSVM`, please set the **JSVM PATH** in the **encode.sh**.

After encoding, the encoded SVC video and encoding log will be stored under each sub-folder.

* **encoder.cfg** : This file is used for JSVM. The layer#.cfg need to be indicated in the **LAYER DEFINITION** section.

* **layer#.cfg** : We need to set the **width** and **height** in **INPUT / OUTPUT** section for each layer#.cfg. The value of **width** and **height** must be the same as [raw/get.sh](../raw/get.sh)
