#! /bin/bash

# Script creates file in func build_file() and transmits that data to /dev/ttyGS0.
# time utility is used to obtain the time spent in the operation and subsequently data rate is calculated.

LOOP_START=1
LOOP_END=15

build_file() {
        let FILE_SIZE=$1*1024*10
        dd if=/dev/zero of=test_file bs=$FILE_SIZE count=1024
}

print_stats() {
        DATA_RATE=$(echo $1 $2 / 1000 /p | dc)
        printf "[loop %d]Transmitted %d bytes in %f secs\n" $3 $1 $2
        printf "Data_rate obtained--:%f (KBps)\n" $DATA_RATE
}
for ((i=$LOOP_START; i<=$LOOP_END; ++i))
do
        build_file $i
        VAR=$((time cat test_file > /dev/ttyGS0) 2>&1)
        NEW_VAR=${VAR//[a-z]};set --$NEW_VAR;TIME=$1
        print_stats $FILE_SIZE $TIME $i
done
