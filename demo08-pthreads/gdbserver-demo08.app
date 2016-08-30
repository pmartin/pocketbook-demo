#!/bin/sh

echo "Activating wifi"
/mnt/ext1/applications/devutils.app 'wifi:activate'

/mnt/ext1/system/usr/bin/gdbserver 0.0.0.0:10002 /mnt/ext1/applications/demo08.app
