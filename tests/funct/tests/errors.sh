###Error no binary
bidule ok

###invalid condition

if jfjf; then echo 1; fi


### if (while) 2 is correct exit code
if while false; do echo deez nutz; done;
  
### error while1  2 is correct exit code
while do

### error while2 2= correct exit code
while ! true; do echo 

### Unclosed
echo 'This is unclosed\' here

### If quotes
if ''; then echo ok; fi
