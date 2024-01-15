#!/bin/sh
cd $(dirname "$0")

path_42sh=../../src/42sh

PASSED="\e[00;32mPASSED\e[0m"
FAILED="\e[00;31mFAILED\e[0m"
TIMEOUT="\e[00;31mTIMEOUT\e[0m"

execute() {
    modname=$2

    theirs="../../theirs$modname"
    theirs_err="../../theirs_err$modname"

    ours="../../ours$modname"
    ours_err="../../ours_err$modname"

    code=$(echo "$1" | sed 's/\\n/\'$'\n''/g')
    printf '%s' "$code" > code
    (timeout -k 0 1 $path_42sh code) 1> $ours 2> $ours_err
    timeout=$?
    bash --posix code 1> $theirs 2> $theirs_err
    dif=$(diff $ours $theirs)
    res=$?
    dif_err=$(diff $ours_err $theirs_err)
    res_err=$?
    toprint="┃"
    toadd=0
    res_err=0
    if [ -s $theirs_err -a ! -s $ours_err ]; then
        res_err=1
    fi
    error=0
    if [ $timeout -ne 0 -o $res -ne 0 -o $res_err -ne 0 ]; then
        toprint="$toprint$(printf '[%b] ' "$FAILED")"
        if [ $timeout -ne 0 ]; then
            toprint="$toprint$(printf ' (%b) ' "$TIMEOUT")"
            toadd=$(($toadd+12))
        fi
        toprint=$toprint$modname
        error=1
    else
        toprint="$toprint$(printf '[%b] ' "$PASSED")"
        toprint="$toprint$modname"
    fi
    nb_char=$(echo $toprint | wc -m)
    nb_char=$((78-$nb_char+$toadd))
    echo -n $toprint
    for _ in $(seq $nb_char); do
        echo -n " "
    done
    echo "┃"
    rm $theirs $ours $ours_err $theirs_err code
}

echo "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓"
echo "┃                       FUNCTIONAL TESTS                         ┃"
echo "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛"


parallelize_entry() {
    entry=$1
    name=$(basename "$entry")
    toprint="[MODULE] $name\n"
    toprint="$toprint┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
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
        toprint="$toprint$(execute "$build" "$modname")\n"
    fi
    toprint="$toprint┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"
    echo "$toprint"
}

test_dir=./tests
for entry in "$test_dir"/*
do
    parallelize_entry "$entry" &
done

wait

