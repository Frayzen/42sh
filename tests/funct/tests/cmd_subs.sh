### Command substitution with "ls" $(echo ls)

### Command substitution with "cat" echo $(echo toto | cat)

### Command substitution with "pwd" $(echo ls -l)

### Command substitution with "date" $(echo date)

### Command substitution with "whoami" $(echo whoami)

### Command substitution with "echo" and multiple arguments $(echo echo Hello World)

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
