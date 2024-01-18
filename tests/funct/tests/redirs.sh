### Simple output redirection
echo "This is the script part" > output.txt

### Appending to a file
echo "This is first line" > existing_file.txt
echo "This is the script part" >> existing_file.txt

### Redirecting STDERR
cat not_exist 2> error_log.txt
cat error_log.txt

### Redirecting both STDOUT and SRDERR
echo "This is the script part" > combined_output.txt 2>&1

### Input Redirection
cat < input_file.txt

### First tricky
echo hey 1>file

### Second tricky
echo hey 1

### Backslashed 1
echo hey 1 \>2

### Backslashed 2
echo hey 1 >\&2

###Several redirs
cat not_exist 2>&1 1>file_err
cat file_err

###Weird behavior
echo ok 1>&2 2>&1 1>/dev/null 2>/dev/null

###Hard redirs and pipe
if echo ok 2>&1 1>&2 | cat; then echo ok 2>file | cat; fi | cat
