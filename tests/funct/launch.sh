#!/bin/sh
cd $(dirname "$0")
path_42sh=$1

execute() {
    code=$1
    ours=$($path_42sh -c "$code")
}

test_dir=./tests
for entry in "$test_dir"/*
do
    echo "[MODULE] $(basename "$entry")"
    save=0
    build=""
    while IFS= read -r line; do
        case $line in
            '###'*)
                if [ $save -eq 1 ]; then
                    execute $build
                    build=""
                fi
                save=1
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
        execute "$build"
    fi
done
