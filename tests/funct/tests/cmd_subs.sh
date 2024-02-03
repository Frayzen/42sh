### Command substitution with "ls"
$(echo ls)

### Command substitution with "ls -l"
$(echo ls -l)

### Command substitution with "cat"
echo $(echo toto | cat)

### Command substitution with "pwd"
directory=$(pwd)
echo "Current directory is: $directory"

### Command substitution with "date"
$(echo date)

### Command substitution with "whoami"
$(echo whoami)

### Command substitution with "echo" and multiple arguments
$(echo echo Hello World)

### Get date function
get_date() {
    echo "Today's date is: $(date)"
}
get_date

### Sed
echo "Content after replacing 'foo' with 'bar': $(echo 'foo baz' | sed 's/foo/bar/')"

### Empty command output
echo "Result: $(echo)"

### Empty command
echo "Result: $( )"
echo Result: $( )

### Seq
echo "Result: $(seq 10)"
echo Result: $(seq 10)

### Undefined command
echo "Result: $(undefined_command)"

### Quotes
echo "Result: "$(echo "Command substitution within quotes")""

###with variables
x=3
y=$(echo $x)
z= $x\$(echo y)

###more varibales
y=$(echo $(echo toto))
echo $y

###more varibales
x=ls
y=$(echo $x)
$y

###recursive
echo $(echo $(echo $(echo $(ls))))

### ls |
echo $(ls | cat) $(echo ok)

### last
echo  $(ls | cat) $(ls | cat)

###check out with files
$(echo ok >test && cat ok >also); $(mkdir toto && echo $(ls)); mv test also toto
$(cd toto; ls)

### Check unclosed
$($($($($($($(echo ok))))))

### Check too much
$($($($($($($(echo ok))))))))

### Check command
$(echo $($(echo ls)))
ls $(cd .. && cd -); $(cd /); ls
ok=3 echo $(echo $ok) >test
b=3
$(echo $b)
echo $(cat test)

###Invalid close
$(echo this is a\)

###Invalid end
$(echo this is a\
)

### Subcommand, 300th test
$(echo echo) $($(echo 'echo')$(seq 100)))

### Sub command echo new lines
echo "Result: $(echo -e "Line 1\nLine 2\nLine 3")"

### Special chars
echo "Result: $(echo Special characters: $@ \ /)"
