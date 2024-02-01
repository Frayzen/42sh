#!/bin/sh

cd $(dirname "$0")

. ./utils.sh
path_42sh=../../src/42sh

echo echo '$(head -n 10000 /dev/urandom)' | $path_42sh >/dev/null 2>&1
ret=$?

if [ $ret -ne 0 ]; then
  echo "[ HUGE INPUT ]"
  echo $top_line
  print_line " [ ERROR ] The input was not properly handled" 0 $RED
  echo $bot_line
fi

exit $ret
