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
