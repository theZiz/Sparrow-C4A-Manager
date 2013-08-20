#!/bin/sh
export PATH=":${PATH:-"/usr/bin:/bin:/usr/local/bin"}"
export LD_LIBRARY_PATH=":${LD_LIBRARY_PATH:-"/usr/lib:/lib"}"
export HOME="/mnt/utmp/Sparrow-C4A-Manager" XDG_CONFIG_HOME="/mnt/utmp/Sparrow-C4A-Manager"

if [ -d /mnt/utmp/Sparrow-C4A-Manager/share ];then
	export XDG_DATA_DIRS=/mnt/utmp/Sparrow-C4A-Manager/share:$XDG_DATA_DIRS:/usr/share
fi
export SDL_AUDIODRIVER="alsa"

cat /proc/pandora/nub0/mode > /tmp/nub0mode_before_Sparrow-C4A-Manager
cat /proc/pandora/nub1/mode > /tmp/nub1mode_before_Sparrow-C4A-Manager
echo absolute > /proc/pandora/nub0/mode
echo absolute > /proc/pandora/nub1/mode

LD_LIBRARY_PATH=. ./Sparrow-C4A-Manager $*

cat /tmp/nub0mode_before_Sparrow-C4A-Manager > /proc/pandora/nub0/mode
cat /tmp/nub1mode_before_Sparrow-C4A-Manager > /proc/pandora/nub1/mode
 
rm /tmp/nub0mode_before_Sparrow-C4A-Manager /tmp/nub1mode_before_Sparrow-C4A-Manager
