###Basic variable
a=3
b=4
echo $a $b
env

###Shell variable

a=3
a=3 echo $a; echo $a
echo $a a=4

###Variable does not exist
echo $a; echo a$toto
echo $b

###Subsitution
a=3
b=$a
echo $b

###Variable brackets
a=3; b=4; c=toto
echo $a$b${c} Hello ${c}!
toto=$a
echo ${a}here${b} is ${toto}

###Var in external binary
a=3; b=4
file=file cat $file >$a
ls && cat $a >$b;
rm $a $b
echo $a$b

###Var with redirection
a=1
echo hehe >$a
cat $a<$a

###Hard expansions
a='sh    42'
echo 42"$a"sh 42$a\sh $a 42$a
