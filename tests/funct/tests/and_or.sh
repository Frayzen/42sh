### And basic
echo ok && echo ok

### And basic not
! echo ok && echo ok

### Or basic
echo ok || echo ok

### Or basic not
! echo ok || echo ok

### Long and
echo ok && echo ok && ls && pwd && cat file && echo ok

### Tst no space
echo OK&&echo ok||echo no

### Example SCL
false && echo foo || echo bar
true || echo foo && echo bar