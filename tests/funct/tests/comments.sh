###Comments basic
#This is a very basic comment to test

###Comment escaped
echo \#escaped

###Comment quoted
echo '#'escaped

###Comment end
echo not#first

###Comments hard
echo \#escaped "#"quoted not#first #commented

### Comment escaped
echo \#escaped

### Comment quoted
echo '#'escaped

### Comment end
echo not#first

### Comments hard
echo \#escaped "#"quoted not#first #commented

### Empty comment
echo ""

### Comment with special characters
echo "#$%^&*()"

### Multiple comments in one line
echo "Multiple comments" # Comment 1 # Comment 2

### Comment with spaces
echo "Space after comment" # This is a comment

### Comment within a string
echo "This is a #comment within a string"

### Comment after a command
echo "Hello World" # This is a comment after a command

### Nested comments
echo "Outer comment" ## Inner comment ## Another inner comment

### Comment with escaped characters
echo "Escaped comment" \# This is an escaped comment

### Comment with quoted characters
echo "Quoted comment" '#' This is a quoted comment

### Comment with special characters and spaces
echo "Special chars and spaces" # $ % ^ & *

### Comment at the beginning of the line
# This is a comment at the beginning
echo "Line with a comment at the beginning"

### Comment with numbers
echo "Numbers in comment" # 12345

### Comment with a mix of characters
echo "Mix of characters" # abc123 !@#$%^&*

### Comment with newline character
echo "Newline in comment" # This is a comments


### LS with a comment
ls # This is a comment

### Find command with a comment
find . -name "*.txt" # Find all text files in the current directory

### Grepping with a comment
grep "pattern" file.txt # Searching for a pattern in a file

### AWK command with a comment
awk '{print $1}' data.txt # Extracting the first column using AWK

### SED command with a comment
sed 's/old/new/' input.txt # Replacing text using SED

### Sort command with a comment
sort file.txt # Sorting the contents of a file

### Cut command with a comment
cut -f1,2 data.csv # Extracting columns 1 and 2 from a CSV file

### WC command with a comment
wc -l textfile.txt # Counting the number of lines in a file

### Piping commands with comments
ls -l | grep "pattern" | sort # Combining commands with a pipeline

### Command substitution with a comment
result=$(echo "This is a result") # Storing the result in a variable

### Find and delete with a comment
find . -name "*.tmp" -delete # Deleting temporary files
