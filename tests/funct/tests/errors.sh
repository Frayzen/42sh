###Error no binary
bidule ok

###Error assigning var number
3ok=No

###Error assigning var underscore
_ok=No

###invalid condition

if jfjf; then echo 1; fi


### if (while) 2 is correct exit code
if while false; do echo deez nutz; done;
  
### error while1  2 is correct exit code
while do

### error while2 2= correct exit code
while ! true; do echo 

### empty compoud list element1
echo hehe ; ; 

### empty compoud list element2
; ; 

### bad list seperator
echo "Apple" ;; echo "Banana"

###bad seperator2
echo "One" && ; echo "Two" || ; echo "Three"

###bad seperator3
cat file.txt |;; grep "pattern"

###bad seperator4
echo "A" ;;;; echo "B"

###bad seperator5
echo "Redirected" &;; output.txt

###compound list in if
if false
    true; then
    echo ok\
    true
    ; then
    echo\
    ls
fi

##bad_unterminated_quote
echo 'hdhdh


### Unclosed
echo "This is unclosed\" here

### If quotes
if ''; then echo ok; fi

###Unterminated simple quotes
echo 'Hello Wor

###empty command

###only spaces in command
"     "

###Unexpected EOF Var
echo ${a

###Pipe with an error 1
echo ok | thisisnotright | cat | ls

###Pipe with an error 2
oeifeojf | echo ok | cat | wc -l

###Only |
|
###Only ;
;
###Only &
&

###No word after redir cmd
echo this is a test >
###No word before redir cmd
> & echo this is a test

###No word before redir sh
> & if true; then echo this is a test; fi
###No word after redir sh
if true; then echo this is a test; fi >
