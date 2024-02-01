###Echo piped
echo ok | cat

###Echo piped lots of cat
echo ok | cat | cat | cat | cat | cat

###Pipe using binaries
touch this is several files
find -type f | grep '*s*' | cat

###Pipe using binaries
echo testoutput > file
cat file | echo
cat file | grep 'test'

###Pipe using binaries
echo testoutput > file
cat file | echo
cat file | grep 'test'

### Count the number of line in a file
echo -e "There is \n several lines\n\nHere" > file
cat file | wc -l | cat
cat file | wc -c | cat

### Generate random letter
tree | tr -dc 'A-Z' | head -n 1 | wc -c
