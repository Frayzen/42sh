### Export unkown
export baab

### Export a b c
export a=3 b=4
c=5
echo $(echo $a $b $c)

### Export bad name
export first=tata @name=test other=toto
echo $first $other
echo $(echo $first $other)
