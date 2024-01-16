###invalid condition
if jfjf; then echo toto; fi

### Command not found
nonexistent_command

### Syntax error in assignment
invalid_var=

### Invalid option for a command
ls -j


### Incorrect use of backticks
output=`ls`

### Incorrect use of command substitution in if condition
if $(true); then echo 1; fi

### Invalid elif command
if false; then echo 1; elif true; then gronkman; fi
