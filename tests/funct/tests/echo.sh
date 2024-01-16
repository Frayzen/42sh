###My echo toto
echo toto

###My echo tata
echo tata

###Echo spaces
echo There '' is '' spaces

###Long echo
echo 'fuenfiue iun iuenf iuenf iunefi ufni uenfi uenfiu nefiun'

###Very long echo
echo efnienfuen iunfeiu neifun eifun ieunfi ue    fieunfiune iun iu neif

###Echo reserved
echo if fi then echo ifa fia thena;

###Paraph echo
echo Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras commodo lobortis tempor. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut euismod imperdiet nulla vel ornare. In cursus vel lectus nec scelerisque. Integer vestibulum tellus eu vehicula pharetra. Praesent porttitor libero id justo commodo consequat. Nunc nisi quam, interdum eu leo a, placerat tempor orci. Morbi nec dui vitae nisi molestie condimentum et scelerisque elit. Proin eget tempus ex, et sagittis ligula.


###Echo without E flag
echo -E 'hello world \n'

###Echo with e flag
echo -e 'hello world \n'

###Echo with e default flag
echo 'hello world \n'

###Echo with several e E flags 1
echo -e -E -e -E 'hello world \n'

###Echo with several e E flags 2
echo -e -E -e 'hello world \n'

###Echo with n flag
echo -n 'hello world'

###Double baskslash n no flag
echo -e '\n\n'

###Double baskslash n flag e
echo -e '\n\n'

###Double baskslash n flag E
echo -E '\n\n'

###Lots of baskslash
echo EeE \\\\\\\\\\\\\\\\

###Spaces between arguments
echo 4	 2 s 	 h

###Big arguments
echo fuck your malloc 10000; echo aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

###Slashs
echo -e -E -e \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

###Mixed option
echo -n -n -e -e -n -n -n -e 42\\nsh

###Slashs 2.0
echo -E -e -E \\\\\\\\\\\\\\\\

###Two echos
echo first; echo second
