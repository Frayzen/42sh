### Unset test
abc=3
abcd=4
unset abcd
echo first $abc $abcd test

### Unset several
abc=5
abcd=6
unset abc abcd
echo first $abc $abcd test

###Unset return value
a=3
if unset a; then echo ok; fi
if unset a; then echo ok2; fi

### Unset nothing
unset

### Unset bad arg 
unset -e

### Unset bad arg
unset ---

### Unset empty var
unset

### Unset empty function
unset -f

### Unset bad arg 
unset -
