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

###Test elif FF
if false; then
    echo ok
elif false; then
    echo elif
else
    echo yes
fi

###Test elif FT
if false; then
    echo ok
elif true; then
    echo elif
else
    echo yes
fi



###Test fi newline
if false; then echo ok; fi

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


###Invaild condition
if fnj; then
    echo yeye;
fi 

###Counpoumd list
if true; then echo Never; echo Gonna; echo Give; echo You; echo Up; fi
