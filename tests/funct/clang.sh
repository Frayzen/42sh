#!/bin/sh
cd $(dirname "$0")
source ./utils.sh

cd $(git rev-parse --show-toplevel)

files=$(find . -name "*.c" -o -name "*.h")
output="$(clang-format -style=file --dry-run --Werror $files 2>&1)"
r=$?

if [ $r -ne 0 ]; then
    ERROR="\e[00;31m [CLANG FORMAT] /!\\ CLANG FORMAT FAILED /!\\ \e[0m"
    printf '\n%b\n' "$ERROR"
    echo $top_line
    while read line;
    do
        print_line "$line" 0 "$PURPLE"
    done < <(echo "$output")
    echo $bot_line
    exit 1
fi

if [ $ERROR_ONLY -eq 0 ]; then
    echo ""
    echo "[CLANG FORMAT]"
    echo "$top_line"
    print_line "The CLANG FORMAT has been $(printf "%b%s%b" "$GREEN" "CHECKED" "$RESET")" 12
    echo "$bot_line"
fi

exit 0
