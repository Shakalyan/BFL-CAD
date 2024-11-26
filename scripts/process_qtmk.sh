#!/bin/bash

PREFIX="QT6_"
tmp_file=qt-conf.mk.tmp

cp $1 $tmp_file

res=""
start_str="####### Compiler, tools and options"
end_str="####### Output directory"
wr=0

while read -r line; do
    if [ "$line" = "$start_str" ]; then
        wr=1
        continue
    fi

    if [ "$line" = "$end_str" ]; then
        wr=0
        continue
    fi

    if [ $wr = 1 ] && [ ! -z "$line" ]; then
        res+="$line\n"
    fi

done < "$tmp_file"

rm $tmp_file

res=${res%\\n}
echo -e "$res" | sed 's/^/'"$PREFIX"'/g' | sed -E 's/\$\(/\$\('"$PREFIX"'/g' > $1
