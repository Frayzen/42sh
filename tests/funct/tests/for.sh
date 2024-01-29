###No item for
for i in; do echo toto; done

###No assignment
for i in a b c; do echo toto; done;

###No assigment multiple cmds
for i in a b c; do echo toto; echo tata; done

###echo item list
for i in a b c d e f g; do echo "$i"; done

###for if
for i in true false true; do if $i; then echo yes; else echo no; fi; done

###find
for i in "*.txt" "*.c" "*.sh"; do find . -name "$i"; done

###returns error
for i in true false; do $i; done;

###continue simple
for i in true false true; do if $i; then continue; fi; echo toto; done

###Nested for loop continue 1
for i in true false true; do
    echo "Outer loop iteration: $i"
    for j in true true false true true; do
        echo "Inner loop iteration: $j"
        if $j; then
            continue 1
        fi
    done
done

###Nested for loop continue 2
for i in true false true; do
    echo "Outer loop iteration: $i"
    for j in true true false true true; do
        echo "Inner loop iteration: $j"
        if $j; then
            continue 2
        fi
    done
done
