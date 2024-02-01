### Subshell classic
echo ok; (echo hey; echo coucou)

### Nested subshell classic
echo ok; (echo hey; (echo ok; ls))

### Useless subshells
( ( ( (echo ok))))

###Subshell variable
a=3; ( echo $a; a=4; echo $a ); echo $a

### Subshell order
( echo ok > toto ); ( cat toto; rm toto )

###Subshell spaces
   (echo ok; (ls); (      ls      ) );a=3;(echo ok) 

### Subshell with brackets
a=3; { echo $a; a=4; echo $a }; echo $a
a=3; { echo $a; { a=4; echo $a } }; echo $a
