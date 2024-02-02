#!/bin/sh

cd $(dirname "$0")

. ./utils.sh
path_42sh=../../src/42sh


echo "echo '$(tr -dc A-Za-z0-9 </dev/urandom | head -c 10000)'" | $path_42sh >/dev/null 2>&1
ret=$?

if [ $ret -ne 0 ]; then
  echo "[ HUGE INPUT ]"
  echo $top_line
  print_line " [ ERROR ] The input was not properly handled" 0 $RED
  echo $bot_line
fi

exit $ret
