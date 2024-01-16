#!/bin/sh

cd $(dirname "$0")
. ./utils.sh

cd $(git rev-parse --show-toplevel)

error_header() {
    printf "%b%s%b\n" "$RED" "Watch for the header guard in $header" "$RESET"
    echo "$top_line"
    print_line "The header guard should be $(printf "%b%s%b" "$RED" "$hguard" "$RESET")" 12
    echo "$bot_line"
    error=1
}

error=0
headers=$(find . -name "*.h" -type f)
for header in $headers;
do
    expect=$(echo $(basename -s '.h' $header) | sed -r 's/([a-z0-9])([A-Z])/\1_\L\2/g')
    hguard=$(echo "$(echo $expect | tr a-z A-Z)_H")
    first_line=$(echo $(head -n 1 "$header"))
    second_line=$(echo $(sed -n '2p' "$header"))

    if [ "$first_line" != "#ifndef $hguard" ]; then
        error_header
    elif [ "$second_line" != "#define $hguard" ]; then
        error_header
    elif [ $(cat $header | grep $hguard | wc -l ) -ne 3 -o $(cat $header | tail -n 1 | grep $hguard | wc -l ) -ne 1 ]; then
        error_header
    fi
done

if [ $error -eq 0 -a -z "$ERROR_ONLY" ]; then
    echo ""
    echo "[HEADER FILES]"
    echo "$top_line"
    print_line "The header guard have been $(printf "%b%s%b" "$GREEN" "CHECKED" "$RESET")" 12
    echo "$bot_line"
fi

exit $error
