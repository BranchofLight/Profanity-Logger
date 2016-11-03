#!/bin/bash

MODNAME=profanity-logger
DEVNAME="Profanity Logger"

echo "Removing module."
rmmod "$MODNAME".ko || exit 1

rm -f "/dev/$DEVNAME"
echo "Removed /dev/$DEVNAME"

echo "Remove complete."
