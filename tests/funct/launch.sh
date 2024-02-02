#!/bin/sh
cd $(dirname "$0")
. ./utils.sh

modules="tests.sh clang.sh hguards.sh io.sh huge.sh"

echo $top_line
print_line "$(printf ' %.0s' $(seq 1 $((-8+line_size/2))))FUNCTIONAL TESTS"
echo $bot_line

if [ $# -ge 1 ]; then
    if [ "$1" = "errors" ]; then
        export ERROR_ONLY=1
    fi
fi

retval=0
for module in $modules;
do
    timeout -k 0 20 ./$module
    if [ $? -ne 0 ]; then
        retval=1
    fi
done

exit $retval
