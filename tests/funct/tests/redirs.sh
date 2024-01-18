### Simple output redirection
echo "This is the script part" > output.txt

### Appending to a file
echo "This is first line" > existing_file.txt
echo "This is the script part" >> existing_file.txt

### Redirecting STDERR
echo "This is the script part" 2> error_log.txt

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

