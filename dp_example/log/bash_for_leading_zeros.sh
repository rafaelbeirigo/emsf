#!/bin/bash

die () {
    echo >&2 "$@"
    exit 1
}

[ "$#" -eq 2 ] || die "Usage: concatenate.sh START END"

FILENAME_ROOT="e_cnt"

if [ -f $FILENAME_ROOT".log" ]
then
    rm $FILENAME_ROOT".log"
fi

for i in $(eval echo {$1..$2})
do
    FILENAME=$FILENAME_ROOT"_"`printf %02d%s ${i%}`".log"
    (cat $FILENAME; echo) >> $FILENAME_ROOT".log"
done
