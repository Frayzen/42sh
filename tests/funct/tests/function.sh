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

###simple echo toto
foo() { echo toto; }; foo

###double function
foo() { bar() { echo toto; } }; foo; bar

###function with args easy
foo() { echo $@; echo $1; echo $2; }; echo $@; foo toto tata

###function with args medium
foo() { echo $@; echo $1; echo $2; }; echo $@; foo toto tata; echo $@
