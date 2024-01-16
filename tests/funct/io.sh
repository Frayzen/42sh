#!/bin/sh

cd $(dirname "$0")

. ./utils.sh
path_42sh=../../src/42sh

file="../io_file"
str="../io_str"
stdin="../io_stdin"

$path_42sh ./io/basics.sh > $file
file_out=$?

$path_42sh -c "$(cat ./io/basics.sh)" > $str
str_out=$?

(echo "$(cat ./io/basics.sh)" | $path_42sh) > $stdin
stdin_out=$?

err=0
if ! $(diff -q $str $file); then
    echo $top_line
    print_line "The output by STRING (-c) is different from the one by FILE" 0 "$RED"
    for difline in "$(diff -W 10 $str $file))"
    do
        print_line "$(echo $difline | head -c $line_size)" 0 $OTHER
    done
    echo $bot_line
    err=1
fi

if ! $(diff -q $stdin $file); then
    echo $top_line
    print_line "The output by STDIN is different from the one by FILE" 0 "$RED"
    echo $bot_line
    for difline in "$(diff -W 10 $str $stdin))"
    do
        print_line "$(echo $difline | head -c $line_size)" 0 $OTHER
    done
    err=1
fi
if [ $err -eq 0 -a -z "$ERROR_ONLY" ]; then
    echo ""
    echo "[IO CHECKS]"
    echo $top_line
    print_line "The IO BACKEND has been $(printf '%b%s%b' "$GREEN" "CHECKED" "$RESET")" 12 
    echo $bot_line
fi
exit $err
