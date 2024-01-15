line_size=90
dash_line=$(printf '━%.0s' $(seq 1 $line_size))

top_line=$(echo "┏$dash_line┓")
bot_line=$(echo "┗$dash_line┛")

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
    echo  ┃
}

