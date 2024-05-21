###Negate easy
if ! true; then
    echo SHOULDNT
fi

###Negate true easy
if ! false; then
    echo SHOULD
fi

###Negate in elif easy
if false; then
    echo $PWD
elif ! false; then
    ls
fi

###Negate command
if ! echo ok; then
    echo double
fi

### Negate return builtin
! echo ok

### Negate return builtin
! ls


###double negation true
! ! true

###triple negation true
! ! ! true

###double negation false
! ! false

###triple negation false
! ! ! false

###with spaces
!        true

###with spaces double
!       ! true

### Double negation
! ! echo ok
