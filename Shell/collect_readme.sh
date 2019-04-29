#!/bin/bash
# cd /home/black/Tiny4412
# for file in $(ls *)
# do
    # echo $file
# done

TOP_DIR=/home/black/Tiny4412
FILE=README

echo " " > $FILE

for dir in $(ls $TOP_DIR)
do
    if [ -d $dir ]; then

        for file in $(ls $dir)
        do
            if [[ $file == README.txt  ]] ;then
                echo "=================$dir================" >> $FILE
                cat $dir/$file >> $FILE
            fi
        done
    fi
done
