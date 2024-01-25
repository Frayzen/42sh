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
    pwd
elif ! false; then
    pwd
fi

###Negate command
if ! echo ok; then
    echo double
fi

### Negate return builtin
! echo ok

### Negate return builtin
! ls

### Double negation
! ! echo ok
