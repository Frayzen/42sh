### And basic
echo ok && echo ok

### And basic not
! echo ok && echo ok

### Or basic
echo ok || echo ok

### Or basic not
! echo ok || echo ok

### Long and
echo ok && echo ok && ls && echo $PWD && cat file && echo ok

### Tst no space
echo OK&&echo ok||echo no

### Example SCL
false && echo foo || echo bar
true || echo foo && echo bar

###with if true
if echo a && true; then echo ok; else echo ko; fi

###with if false
if echo a && false; then echo ok; else echo ko; fi

###with if true and negation
if echo a && ! true; then echo ok; else echo ko; fi

###with if false and negation
if echo a && ! false; then echo ok; else echo ko; fi

###bad or pipe
echo hehe || | cat
echo e |||

###bad or ;
true || ;
echo; || ;

### Check and new line
echo ok &\
& echo ok

echo ok |\
| echo ok
