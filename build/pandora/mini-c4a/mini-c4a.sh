#!/bin/sh
export PATH=":${PATH:-"/usr/bin:/bin:/usr/local/bin"}"
export LD_LIBRARY_PATH=":${LD_LIBRARY_PATH:-"/usr/lib:/lib"}"
export HOME="/mnt/utmp/mini-c4a" XDG_CONFIG_HOME="/mnt/utmp/mini-c4a"

if [ -d /mnt/utmp/mini-c4a/share ];then
	export XDG_DATA_DIRS=/mnt/utmp/mini-c4a/share:$XDG_DATA_DIRS:/usr/share
fi
export SDL_AUDIODRIVER="alsa"

cat /proc/pandora/nub0/mode > /tmp/nub0mode_before_mini-c4a
cat /proc/pandora/nub1/mode > /tmp/nub1mode_before_mini-c4a
echo absolute > /proc/pandora/nub0/mode
echo absolute > /proc/pandora/nub1/mode

LD_LIBRARY_PATH=. ./mini-c4a $*

cat /tmp/nub0mode_before_mini-c4a > /proc/pandora/nub0/mode
cat /tmp/nub1mode_before_mini-c4a > /proc/pandora/nub1/mode
 
rm /tmp/nub0mode_before_mini-c4a /tmp/nub1mode_before_mini-c4a
