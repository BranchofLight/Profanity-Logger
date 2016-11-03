#!/bin/bash

DEVNAME="Profanity Logger"
MAJOR=$1

# && was not producing correct results so check is done twice for now
[ "$#" -eq 1 ] || echo "No valid argument given. Please supply a major number." 
[ "$#" -eq 1 ] || exit 1

echo "Using major number: "$MAJOR"."
rm -f /dev/$DEVNAME
mknod "/dev/$DEVNAME" c $MAJOR 0

echo "/dev/$DEVNAME created."
