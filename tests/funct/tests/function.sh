###simple function
foo() { echo toto; }; foo

###double function
foo() { bar() { echo toto; } }; foo; bar
