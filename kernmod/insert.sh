#!/bin/bash

MODNAME=profanity-logger

echo "Inserting module."
insmod "$MODNAME".ko || exit 1

dmesg | grep "Major number is"
echo "If major number is not above check dmesg for what major number to use for devload.sh"
echo "Module inserted."
