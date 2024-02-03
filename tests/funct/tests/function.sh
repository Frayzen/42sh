###simple function
foo() { echo toto; }; foo

###double function
foo() { bar() { echo toto; } }; foo; bar

###Function params
toto () { echo $1 ; };
toto test

###Function all params
toto () { echo $@ ; };
toto this is a very long test
