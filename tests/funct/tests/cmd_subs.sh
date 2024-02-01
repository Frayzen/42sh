### Command substitution with "ls" ###
$(echo ls)

### Command substitution with "cat" ###
echo $(cat test.sh)

### Command substitution with "pwd" ###
$(echo ls -l)

### Command substitution with "date" ###
$(echo date)

### Command substitution with "whoami" ###
$(echo whoami)

### Command substitution with "echo" and multiple arguments ###
$(echo echo Hello World)

###with variables
x=3
y=$(echo $x)
z= $x\$(echo y)

###more varibales
x=ls
y=$($x)
echo $y

###more varibales
x=ls
y=$(echo $x)
$y
