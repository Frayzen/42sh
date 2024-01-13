#!/bin/sh
cd $(dirname "$0")

path_42sh=../../src/42sh

PASSED="\e[00;32mPASSED\e[0m"
FAILED="\e[00;31mFAILED\e[0m"

execute() {
    modname=$2
    printf '%s\n' "$1" > code
    (timeout -k 0 .5 $path_42sh code) 1> ours 2> ours_err
    timeout=$?
    bash --posix code 1> theirs 2>theirs_err
    dif=$(diff ours theirs)
    res=$?
    dif_err=$(diff ours_err theirs_err)
    res_err=$?
    if [ $timeout -ne 0 -o $res -ne 0 -o $res_err -ne 0 ]; then
        printf '[%b] ' "$FAILED"
        echo "$modname"
    else
        printf '[%b] ' "$PASSED"
        echo "$modname"
    fi
    rm theirs ours code ours_err theirs_err
}

test_dir=./tests
for entry in "$test_dir"/*
do
    declare -u name
    name=$(basename "$entry")
    echo "[MODULE] $name"
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
                    build=$(printf '%s\n' "$build$line")
                fi
        esac
    done < $entry
    if [ $save -eq 1 ]; then
        execute "$build" "$modname"
    fi
done
