a=3; { echo $a; a=4; echo $a }; echo $a
a=3; { echo $a; { a=4; echo $a } }; echo $a

