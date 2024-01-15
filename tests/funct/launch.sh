#!/bin/sh
cd $(dirname "$0")

path_42sh=../../src/42sh

PASSED="\e[00;32mPASSED\e[0m"
FAILED="\e[00;31mFAILED\e[0m"
TIMEOUT="\e[00;31mTIMEOUT\e[0m"

file_id=0

if [ $# -ge 1 ]; then
    file_id=$1
fi

mod_id=0

if [ $# -ge 2 ]; then
    mod_id=$2
fi


execute() {
    modname=$2
    id=$3
    unique="$id\_$file_id"
    theirs="../../theirs$unique"
    theirs_err="../../theirs_err$unique"

    ours="../../ours$unique"
    ours_err="../../ours_err$unique"

    script="../../script$unique"
    code=$(echo "$1" | sed 's/\\n/\'$'\n''/g')
    printf '%s' "$code" > $script
    (timeout -k 0 1 $path_42sh $script) 1> $ours 2> $ours_err
    timeout=$?
    bash --posix $script 1> $theirs 2> $theirs_err
    dif=$(diff $ours $theirs)
    res=$?
    dif_err=$(diff $ours_err $theirs_err)
    res_err=$?
    toprint="┃($id)"
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
    rm $theirs $ours $ours_err $theirs_err $script
}

echo "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓"
echo "┃                       FUNCTIONAL TESTS                         ┃"
echo "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛"


parallelize_entry() {
    entry=$1
    file_id=$2
    name=$(basename "$entry")
    toprint="[MODULE $file_id] $name\n"
    toprint="$toprint┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
    save=0
    build=""
    modname=""
    curid=1
    while IFS= read -r line; do
        case $line in
            '###'*)
                if [ $save -eq 1 ]; then
                    if [ $mod_id -eq 0 -o $curid -eq $mod_id ]; then
                        toprint="$toprint$(execute "$build" "$modname" $curid)\n"
                    fi
                    build=""
                    curid=$(($curid+1))
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
        if [ $mod_id -eq 0 -o $curid -eq $mod_id ]; then
            toprint="$toprint$(execute "$build" "$modname" $curid)\n"
        fi
    fi
    toprint="$toprint┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"
    echo ""
    echo "$(echo "$toprint" | sed 's/\\n/\'$'\n''/g')"
}

test_dir=./tests
id=0
for entry in "$test_dir"/*
do
    id=$(($id+1))
    if [ $file_id -ne 0 -a $file_id -ne $id ]; then
        continue
    fi
    parallelize_entry "$entry" $id &
done
wait
