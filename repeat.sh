#!/bin/bash

logfile=log
maxrun=0	# Run qemu for $maxrun times. When this value is 0, qemu will run forever (use ctrl+C to stop running).
timeinterval=1	# Each run lasts for $timeinterval second.
i=1

function die {
    pkill -9 qemu
    echo -e $1 1>&2
    stty echo
    exit
}

make disk.img
if [ $? != 0 ]
then
    exit
fi

while true
do
    echo -e "\nstart the ${i}th test..."

    make run 2>&1 | tee $logfile &
    sleep $timeinterval

    if grep -q 'system panic' $logfile
    then
        die '\n\033[1;31mSystem panic is triggered!\033[0m'
    elif grep -q 'qemu: fatal:' $logfile
    then
        die '\n\033[1;31mqemu has crashed!\033[0m'
    elif [ `grep -c 'Hello, OS World!' $logfile` -ge 2 ]
    then
        echo -e "\n" `grep -c 'Hello, OS World!' $logfile`
        die '\n\033[1;31mMysterious reboot is detected!\033[0m'
    elif [ $i == $maxrun ]
    then
        die '\n\033[1;31mReach maximal run!\033[0m'
    elif [ $(grep -c 'Now: current is #0' $logfile) -ge 20 ]
    then
        die '\n\033[1;31mMysterious repetition is detected!\033[0m'
    fi

    pkill -9 qemu
    i=$((i + 1))
done
