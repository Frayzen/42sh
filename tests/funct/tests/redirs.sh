### Simple output redirection
echo "This is the script part" > output.txt

### Appending to a file
echo "This is the script part" >> existing_file.txt

### Redirecting STDERR
echo "This is the script part" 2> error_log.txt

### Redirecting both STDOUT and SRDERR
echo "This is the script part" > combined_output.txt 2>&1

### Input Redirection
cat < input_file.txt

### Using Tee for Output
echo "This is the script part" | tee output_and_terminal.txt