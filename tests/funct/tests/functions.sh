###simple function
foo() { echo toto; }; foo

###double function
foo() { bar() { echo toto; } }; foo; bar

###triple function
foo() { bar() { popo() { echo toto; } } }; foo; bar; popo
