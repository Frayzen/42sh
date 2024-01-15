#!/bin/sh
cd $(dirname "$0")
source ./utils.sh


cd $(git rev-parse --show-toplevel)

output="$(clang-format -style=file --dry-run --Werror **/*.c **/*.h 2>&1)"
r=$?

if [ $r -ne 0 ]; then
    ERROR="\e[00;31m [FORMAT] /!\\ CLANG FORMAT FAILED /!\\ \e[0m"
    printf '\n\n%b\n' "$ERROR"
    echo $top_line
    while read line;
    do
        print_line "$line" 0 "$PURPLE"
    done < <(echo "$output")
    echo $bot_line
    exit 1
fi
exit 0
