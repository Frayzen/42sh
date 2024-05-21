### Basic
case i in a | b | c) echo toto;; i) echo tata;; esac

### Regex
case 17ahhhh9 in [1-9]*) echo toto;; [1-2]{2}ah*[1-9]) echo bar;; esac

### Default
case freuifkdfw in bbdeiwke | dhewuofe | deyw) echo toto;; *) echo gh;; esac

### Multiple *
case 1 in 2 | 3) echo toto;; ******) echo tata;; 1) echo bar;; esac

### Multiple commands
case a in a) ls; echo toto; ls; for i in 1 2 3; do if false; then echo else; elif true; then echo else; fi;  done ;; esac

### Empty command
case a in a) echo toto ;;b) ;;c) echo toto;; esac

### echo esac
case a in a) echo esac;; esac

### Bad grammar no ;;
case a in b) echo toto esaccasesh

### Bad grammar no esac
case a in b) echo toto esaccasesh;;

### Bad grammar, | but no element
case i in a) echo toto ;;b | ) echo tata 

### Bad grammar no )
case i in a) echo toto ;;b) echo tata ;; esac

### Bad grammar no body
case a esac

### Bad grammar after in
case a in toto

### Bad grammar no pipe
case a i a) echo toto;; b c) echo tata;; esac

### Case space
case ' ' in a) echo toto;; o | ' ') echo tata;; esac

### Case space in string
case a' ' in a) echo toto;; a' ') echo tata;; esac

### Case escaped characters
string="a*"
case "$string" in
    "a\*")
        echo "Matched 'a\*'"
        ;;
    "a*")
        echo "Matched 'b*'"
        ;;
    *)
        echo "No match"
        ;;
esac

### Complex escaped
string="abc\*def"
case "$string" in
    "123\\456" | "abc\*def")
        echo "Matched '123\\456' or 'abc\*def'"
        ;;
    "xyz*" | "abc\*def")
        echo "Matched 'xyz*' or 'abc\*def'"
        ;;
    "foo\?bar")
        echo "Matched 'foo?bar'"
        ;;
    *)
        echo "No match"
        ;;
esac

### Syntax Error - Missing ;; after the first command
case x in a) echo "Matched a" esac

### Syntax Error - Missing in keyword
case x a) echo "Matched a";;

### Syntax Error - Invalid pattern (missing quotes)
case x in a*) echo "Matched a";;

### Syntax Error - Invalid pattern (missing quotes and parentheses)
case x in a*) echo "Matched a" ;;

### Variable Usage
pattern="b"
case x in a) echo "Matched a";; $pattern) echo "Matched b";; esac

### Variable Usage with Special Characters
pattern="a*"
case x in $pattern) echo "Matched a*";; esac

### Variable Expansion in Commands
var="world"
case x in a) echo "Hello $var";; esac

### Quotes in Commands - Nested Quotes
case x in "a") echo "Matched a";;

### Quotes in Commands with Variables - Nested Quotes
var="world"
case x in 'a') echo "Hello $var";;

### Quotes in Multiple Commands - Nested
