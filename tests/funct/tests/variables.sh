###Test with functioni
a='echo    ok;'
$a

### Echo diff var empty str
echo this $a '' $a$b this

###Basic variable
a=3
b=4
echo $a $b

### Assignment quotes
'val'=ok; echo $val
val='ok'; echo $val

###Shell variable

a=3
a=3 echo $a; echo $a
echo $a a=4

###Variable does not exist
echo $a; echo a$toto
echo $b a$b\c a$b $b\c

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
file=file
cat $file >$a
ls && cat $a >$b;
rm $a $b
echo $a$b


###Test variable quotes
toto="Hello World"
echo 'this is $toto hehe' "This is the real $toto$" "Hello \$toto" >"$UID"
cat <$UID

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
testa=3 testabc=4 testabcef=5 testseofia='this is a test' printenv | grep 'test.*=' | sort

###Double alloc at once
a=3 q=1
echo $a $q
a=4 echo $a
echo $a

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

###Print args
for a in "$*"; do
    echo $a
done
for a in "$@"; do
    echo $a
done

###question mark
cat heheh
echo $?

### Check IFS print
echo $IFS

