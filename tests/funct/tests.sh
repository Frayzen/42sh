#!/bin/sh
cd $(dirname "$0")

. ./utils.sh
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
    rm -rf *
    theirs="../../theirs$unique"
    theirs_err="../../theirs_err$unique"
    rm -rf *
    ours="../../ours$unique"
    ours_err="../../ours_err$unique"

    script="../../script$unique"
    code=$(echo "$1" | sed 's/ timflochaslm/\n/g')
    printf '%s' "$code" > $script
    $path_42sh $script 1> $ours 2> $ours_err
    ours_ret=$?
    rm -rf *
    bash --posix $script 1> $theirs 2> $theirs_err
    theirs_ret=$?
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
    ret_val=0
    if [ $theirs_ret -ne $ours_ret ]; then
        ret_val=1
    fi
    error=0
    if [ $res -ne 0 -o $res_err -ne 0 -o $ret_val -ne 0 ]; then
        toprint="$toprint$(printf '[%b] ' "$FAILED")"
        toprint=$toprint$modname
        error=1
    else
        if [ -n "$ERROR_ONLY" ]; then
            if [ $mod_id -eq 0 ]; then
                rm $theirs $ours $ours_err $theirs_err $script
            fi
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
        if [ $ret_val -ne 0 ]; then
            print_line "RETURN VALUE differs" 0 $PURPLE
            print_line "EXPECTED $theirs_ret but got $ours_ret" 0 $PURPLE
        fi
        if [ $res_err -ne 0 ]; then
            if [ -z "$(cat $theirs_err)" ]; then
                print_line "STDERR should be empty but isn't" 0 $PURPLE
            else
                print_line "STDERR should not be empty but is" 0 $PURPLE
            fi
        fi
        print_line "[ = END = ]" 0 $PURPLE
    fi
    if [ $mod_id -eq 0 ]; then
        rm $theirs $ours $ours_err $theirs_err $script
    fi
    if [ $error -ne 1 ]; then
        exit 0
    else
        exit 1
    fi
}

parallelize_entry() {
    cd ..
    tmp_folder=$(mktemp -d .XXXXXX)
    cd $tmp_folder
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
                        if [ ! -z "$(echo $val)" ]; then
                            toprint="$toprint$val\n"
                        fi
                    fi
                    build=""
                    curid=$(($curid+1))
                fi
                save=1
                modname="$(echo $line | cut -c4-)"
                ;;
            *)
                if [ $save -ne 0 ]; then
                    build="$build$line timflochaslm"
                fi
        esac
    done < ../funct/$entry
    if [ $save -eq 1 ]; then
        if [ $mod_id -eq 0 -o $curid -eq $mod_id ]; then
            val=$(execute "$build" "$modname" $curid)
            if [ $? -eq 1 ]; then
                errs=1
            fi
            if [ ! -z "$(echo $val)" ]; then
                toprint="$toprint$val\n"
            fi
        fi
    fi
    toprint="$toprint$bot_line\n"
    if [ $errs -eq 1 -o -z "$ERROR_ONLY" ]; then
        echo ""
        echo "$(echo "$toprint" | sed 's/\\n/\'$'\n''/g')"
    fi
    cd ..
    rm -rf $tmp_folder
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
