### Default ifs
b=" 	 this	 is 	a test	ok"
for a in $b; do
    echo $a
done
echo $b "$b"

### Simple IFS
IFS=" :;"
b="test ;: ok"
for a in $b; do
    echo $a
done


### Basic ifs
IFS=" :;"
b="this;is:a; test  ;: ok"
for a in $b; do
    echo $a
done
echo $b "$b"
for a in "$b"; do
    echo $a
done

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

###Set IFS to ;
a='this    is;a     test'
IFS=;
for b in $a; do echo $b; done

###One space IFS
IFS=" "
a="a c"
for b in $a; do echo $b; done

###Semicolumn IFS
IFS=";"
a="a;;c"
for b in $a; do echo $b; done
