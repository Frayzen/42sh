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
