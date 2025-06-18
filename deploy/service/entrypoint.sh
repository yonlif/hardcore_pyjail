#!/bin/sh

if [ "$1" = "debug" ]; then
    echo "Starting debug shell..."
    echo "You can find the challenge files in /chroot/challenge"
    echo "Use 'chroot /chroot /bin/bash' to enter the chroot environment"
    echo "Press Ctrl+C to exit"
    sleep infinity
fi

chroot --userspec=1000:1000 /chroot \
    /bin/bash -c "LD_PRELOAD=/challenge/preload.so /bin/python3.11 -u /challenge/balloon.py"
