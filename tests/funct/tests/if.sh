###Basic if
if true; then
    echo ok
fi

###Basic if false
if false; then
    echo ok
fi

###Basic if false
if false; then
    echo ok
fi

###Basic if else
if false; then
    echo ok
else
    echo not
fi

###Basic if else false
if true; then
    echo ok
else
    echo not
fi

###Basic if else lots of condition
if true; false; true; echo ok; true; then
    echo ok
else
    echo not
fi

###Lots of if else
if false; then
    echo ok
elif false; then
    echo ok
elif false; then
    echo ok
elif false; then
    echo ok
elif false; then
    echo ok
elif false; then
    echo ok
else
    echo not
fi
