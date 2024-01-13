#!/bin/sh
cd $(dirname "$0")

path_42sh=../../42sh

PASSED="\e[00;32mPASSED\e[0m"
FAILED="\e[00;31mFAILED\e[0m"

execute() {
    modname=$2
    code=$(echo "$1")
    $path_42sh -c "$(echo "$code")" > ours
    theirs=$(bash --posix -c "$(echo "$code")")
    echo "$theirs" > theirs
    dif=$(diff ours theirs)
    res=$?
    if [ $res -ne 0 ]; then
        printf '[%b] ' "$FAILED"
        echo "$modname"
    else
        printf '[%b] ' "$PASSED"
        echo "$modname"
    fi
    rm theirs ours
}

test_dir=./tests
for entry in "$test_dir"/*
do
    echo "[MODULE] $(basename "$entry")"
    save=0
    build=""
    modname=""
    while IFS= read -r line; do
        case $line in
            '###'*)
                if [ $save -eq 1 ]; then
                    execute "$build" "$modname"
                    build=""
                fi
                save=1
                modname="$(echo $line | cut -c4-)"
                ;;
            *)
                if [ $save -eq 0 ]; then
                    $(line)
                else
                    build="$build$line\n"   
                fi
        esac
    done < $entry
    if [ $save -eq 1 ]; then
        execute "$build" "$modname"
    fi
done
