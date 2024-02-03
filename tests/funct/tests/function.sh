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
foo() { echo $@; echo $1; echo $2; echo $7}; echo $@ $1 $7; foo toto tata

###function with args medium
foo() { echo $@; echo $1; echo $2; }; echo $@ $1 $7; foo toto tata; echo $@ $1 $7

###simple function
foo() { echo toto; }; foo

###double function
foo() { bar() { echo toto; } }; foo; bar

###function with args easy
foo() { echo $@; echo $1; echo $2; echo $7 }; echo $@; foo toto tata

###function with args medium
foo() { echo $@; echo $1; echo $2;  echo $7; echo $@ $#}; echo $@ $7 $# $1; foo toto tata; echo $@ $# $7 $1

###simple function
foo() { echo toto; }; foo

###double function
foo() { bar() { echo toto; } }; foo; bar

###Function params
toto () { echo $1 ; };
toto test

###Function all params
toto () { echo $@ ; };
toto this is a very long test toto



### Simple Function
foo() { echo toto; }; foo

### Double Function
foo() { bar() { echo toto; } }; foo; bar

### Function with Parameters
toto() { echo $1 ; };
toto test

### Function with All Parameters
toto() { echo $@ ; };
toto this is a very long test

### Simple Echo "toto"
foo() { echo toto; }; foo

### Double Function
foo() { bar() { echo toto; } }; foo; bar

### Function with Easy Args
foo() { echo $@; echo $1; echo $2; echo $7 }; echo $@ $1 $7; foo toto tata

### Function with Medium Args
foo() { echo $@; echo $1; echo $2; }; echo $@ $1 $7; foo toto tata; echo $@ $1 $7

### Simple Function
foo() { echo toto; }; foo

### Double Function
foo() { bar() { echo toto; } }; foo; bar

### Function with Easy Args
foo() { echo $@; echo $1; echo $2; echo $7}; echo $@ $1 $7; foo toto tata

### Function with Medium Args
foo() { echo $@; echo $1; echo $2;  echo $7; echo $@ $#}; echo $@ $7 $# $1; foo toto tata; echo $@ $# $7 $1

### Simple Function
foo() { echo toto; }; foo

### Double Function
foo() { bar() { echo toto; } }; foo; bar

### Function with Parameters
toto () { echo $1 ; };
toto test

### Function with All Parameters
toto () { echo $@ ; };
toto this is a very long test toto

### Function with Local Variable
foo() {
    local var="local_variable"
    echo $var
}
foo

### Nested Function with Arguments
foo() {
    bar() {
        echo $@
    }
    foo_param="nested"
    bar $foo_param
}

foo

### Function with Conditional
foo() {
    if [ "$1" == "test" ]; then
        echo "Condition met"
    else
        echo "Condition not met"
    fi
}

foo test
foo hello

### Function Returning Value
foo() {
    echo "Function returning value"
}

result=$(foo)
echo $result

### Function with Default Parameter
foo() {
    local param=${1:-default_value}
    echo $param
}

foo
foo custom_value

### Function with Command Substitution
foo() {
    echo "Function with command substitution: $(date)"
}

foo

### Function with I/O Redirection
foo() {
    echo "Function output redirection" > output_file.txt
}

foo
cat output_file.txt
