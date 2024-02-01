### Default ifs
b=" 	 this	 is 	a test	ok"
for a in $b; do
    echo $a
done
echo $b "$b"

### Basic ifs
# IFS=" :;"
# b="this;is:a; test  ;: ok"
# for a in $b; do
#     echo $a
# done
# echo $b "$b"
# for a in "$b"; do
#     echo $a
# done

### IFS colon
IFS=:
mystring="foo:bar baz rab"
for word in $mystring; do
  echo "Word: $word"
done


###No IFS
IFS=
b="this;is:a; test  ;: ok"
for a in $b; do
    echo $a
done
echo $b "$b"
for a in "$b"; do
    echo $a
done

### Unset ifs
unset IFS
b=" 	 this	 is 	a test	ok"
for a in $b; do
    echo $a
done
echo $b "$b"
