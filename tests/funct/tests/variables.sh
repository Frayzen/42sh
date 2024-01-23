###Basic variable
a=3
b=4
echo $a $b

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

###Test with functioni
a='echo    ok;'
$a

###Test variable quotes
toto=Hello World
echo 'this is $toto hehe' "This is the real $toto$" "Hello \$toto" >"$$"
cat <$$

###Hard expansions
a='sh    42'
echo 42"$a"sh 42$a\sh $a 42$a

###Double equal
how=varname=value
echo "We print variable like that : $how"


###Errors
te\}st=ok

test=ok
\{test=ok

'test'=ok

tes\t=ok

${ok}=ok

${test}=ok

test=a
$a=ok

$test=ok

${test}=ok

${test}=ok

###Order of assignment
x="oncex upon" y="time"; echo $x $y
a="oncea upon" b="ime" && echo $a $b
p="oncep upon" v="me" echo $p $v

###Check assignment before command
a=3 b=4 c=5 tot='this is a test' printenv

###Double alloc at once
a=3 q=1
echo $a $q
a=4 printenv && env

###Args
a='-ne'
echo $a    ok
b='-l'
ls $b
c='ls -l'
$c


###In for loop
b=this is a test
c='this     as     well'
for a in $b $c "$c"
do
    echo $a
done

###redir string
tat=1
echo ok $tat>tst
cat tst
tat=twok
echo ok $twok>other
cat other

###Nested echo
a=3 bash -c 'echo $a'

###Var redir output file
a=2
echo this 2>&$a
cat $a

###Unexpected EOF Var
echo ${a
