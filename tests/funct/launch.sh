#!/bin/sh
cd $(dirname "$0")

line_size=90
dash_line=$(printf '━%.0s' $(seq 1 $line_size))

top_line=$(echo "┏$dash_line┓")
bot_line=$(echo "┗$dash_line┛")

path_42sh=../../src/42sh

#Black        0;30     Dark Gray     1;30
#Red          0;31     Light Red     1;31
#Green        0;32     Light Green   1;32
#Brown/Orange 0;33     Yellow        1;33
#Blue         0;34     Light Blue    1;34
#Purple       0;35     Light Purple  1;35
#Cyan         0;36     Light Cyan    1;36
#Light Gray   0;37     White         1;37

PASSED="\e[00;32mPASSED\e[0m"
FAILED="\e[00;31mFAILED\e[0m"
TIMEOUT="\e[00;31mTIMEOUT\e[0m"

PURPLE="\e[00;35m"
OTHER="\e[00;34m"
RESET="\e[0m"

file_id=0

if [ $# -ge 1 ]; then
    file_id=$1
fi

mod_id=0

if [ $# -ge 2 ]; then
    mod_id=$2
fi

print_line() {
    to_print="$1"
    to_add=0
    if [ $# -ge 2 ]; then
        to_add=$2
    fi
    color="$3"
    nb_char=$(echo "$to_print" | wc -m)
    nb_char=$(($line_size-$nb_char+$to_add+1))
    echo -n "┃"
    printf '%b' "$color"
    echo -n "$to_print"
    for _ in $(seq $nb_char); do
        echo -n " "
    done
    printf '%b' "$RESET"
    echo "┃"
}

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
}

echo $top_line
print_line "$(printf ' %.0s' $(seq 1 $((-8+line_size/2))))FUNCTIONAL TESTS"
echo $bot_line


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
    toprint="$toprint$bot_line\n"
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
