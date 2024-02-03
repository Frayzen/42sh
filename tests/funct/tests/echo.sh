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

###Echo true false
echo true || echo false && ! echo other && echo flo

###Paraph echo
echo Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras commodo lobortis tempor. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut euismod imperdiet nulla vel ornare. In cursus vel lectus nec scelerisque. Integer vestibulum tellus eu vehicula pharetra. Praesent porttitor libero id justo commodo consequat. Nunc nisi quam, interdum eu leo a, placerat tempor orci. Morbi nec dui vitae nisi molestie condimentum et scelerisque elit. Proin eget tempus ex, et sagittis ligula.


###Echo without E option
echo -E 'hello world \n'

###Echo with e option
echo -e 'hello world \n'

###Echo with e default option
echo 'hello world \n'

###Echo with several e E options 1
echo -e -E -e -E 'hello world \n'

###Echo with several e E options 2
echo -e -E -e 'hello world \n'

###Echo with n option
echo -n 'hello world'

###Double baskslash n no option
echo -e '\n\n'

###Double baskslash n option e
echo -e '\n\n'

###Double baskslash n option E
echo -E '\n\n'

### echo with spaces
echo      4    2    s    h

###Lots of baskslash
echo -EeE \\\\\\\\\\\\\\\\

###Spaces between arguments
echo 4	 2 s 	 h

###Big arguments
echo fuck your malloc 10000; echo aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

###Slashs
echo -e -E -e \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

###Slashs no spaces
echo -eEe \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

###Slashs 2.0
echo -E -e -E \\\\\\\\\\\\\\\\

###Two echos
echo first; echo second

###Non option dash 
echo -a

###Dash with other char
echo -neEnneEEnea 42\nsh\\\\

###Non setting dash and setting dash
echo -neEnnEeen -nef 42sh\n\\\\

###Double option set
echo -nEEennneE -nnneee 42sh\n\\\\

###Quoted tricky one
echo toto ';' ls

###interpret newline
echo -nnEeennnEe 42'\nsh'
echo ok

### wordables
echo echo echo  fi if else elif

### negation
echo   biz    ; echo yipee yep; echo hello world !; echo 1 2 3 4 5 6

###Lots of dash option
echo -n -n -e -e -n -n -n -e 42'\nsh'

###Empties n
echo -n
echo -e
echo -E
echo -Een

###Long command list
echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto;echo toto; echo toto ;echo toto;

### Echo with bsl n
echo This should be\
discard

echo "This should be\
discard"

echo 'This should be\
discard'

###e c h o 
echo e c h o

### Not backslahed
echo 'This is unclosed\' here

### Check with spaces
echo a \   b \ \ c    \ a\ v  eeee\ 

### Echo weird quotes
echo ""
echo this "" is "" a "" test
echo ok ""
echo "" ok

### Unexpected end of file on echo
echo 'Special characters: $@ \ / ` ' \" '

### Echo with space
a=a' '; echo $a\b
a=' '; echo $a\b
