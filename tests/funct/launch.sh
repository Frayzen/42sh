#!/bin/sh
cd $(dirname "$0")
source ./utils.sh

modules="tests.sh clang.sh hguards.sh"

echo $top_line
print_line "$(printf ' %.0s' $(seq 1 $((-8+line_size/2))))FUNCTIONAL TESTS"
echo $bot_line

export ERROR_ONLY=0
if [ $# -ge 1 ]; then
    if [ "$1" = "errors" ]; then
        ERROR_ONLY=1
    fi
fi

retval=0
for module in $modules;
do
    ./$module
    if [ $? -eq 1 ]; then
        retval=1
    fi
done

exit $retval
