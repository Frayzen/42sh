#!/bin/sh
cd $(dirname "$0")

source ./utils.sh
path_42sh=../../src/42sh

file_id=0

if [ $# -ge 1 ]; then
    file_id=$1
fi

mod_id=0

if [ $# -ge 2 ]; then
    mod_id=$2
fi

execute() {
    modname=$(echo $2)
    id=$3
    unique="$(echo $id)_$file_id"
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
    dif=$(diff -q $ours $theirs)
    res=$?
    dif_err=$(diff -q $ours_err $theirs_err)
    res_err=$?
    toprint="($id)"
    toadd=12
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
        if [ $ERROR_ONLY -eq 1 ]; then
            exit 0
        fi
        toprint="$toprint$(printf '[%b] ' "$PASSED")"
        toprint="$toprint$modname"
    fi
    print_line "$toprint" $toadd
    if [ $error -eq 1 ]; then
        print_line "[ = REPORT = ]" 0 $PURPLE
        if [ $res -ne 0 ]; then
            print_line "= STDOUT differs" 0 $PURPLE
            for difline in "$(diff -W 10 $ours $theirs))"
            do
                print_line "$(echo $difline | head -c $line_size)" 0 $OTHER
            done
            print_line "= END differs" 0 $PURPLE
        fi
        if [ $res_err -ne 0 ]; then
            print_line "STDERR empty" 0 $PURPLE
        fi
        print_line "[ = END = ]" 0 $PURPLE
    fi
    rm $theirs $ours $ours_err $theirs_err $script
    if [ $error -ne 1 ]; then
        exit 0
    else
        exit 1
    fi
}

parallelize_entry() {
    entry=$1
    file_id=$2
    name=$(basename "$entry")
    toprint="[MODULE $file_id] $name\n"
    toprint="$toprint$top_line\n"
    save=0
    build=""
    modname=""
    curid=1
    errs=0
    while IFS= read -r line; do
        case $line in
            '###'*)
                if [ $save -eq 1 ]; then
                    if [ $mod_id -eq 0 -o $curid -eq $mod_id ]; then
                        val=$(execute "$build" "$modname" $curid)
                        if [ $? -eq 1 ]; then
                            errs=1
                        fi
                        toprint="$toprint$val\n"
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
            val=$(execute "$build" "$modname" $curid)
            if [ $? -eq 1 ]; then
                errs=1
            fi
            toprint="$toprint$val\n"
        fi
    fi
    toprint="$toprint$bot_line\n"
    if [ $errs -eq 1 -o $ERROR_ONLY -eq 0 ]; then
        echo ""
        echo "$(echo "$toprint" | sed 's/\\n/\'$'\n''/g')"
    fi
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
