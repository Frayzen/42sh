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
    echo Outer loop one
    for j in true true false true true; do
        echo Inner loop one
        if ! $j; then
            continue 1
        fi
        echo Inner loop two
    echo Outer loop two
    done
done

###Nested for loop continue 2
for i in true false true; do
    echo Outer loop one
    for j in true true false true true; do
        echo Inner loop one
        if ! $j; then
            continue 2
        fi
        echo Inner loop two
    echo Outer loop two
    done
done

### Using while loop
echo Using while loop
i=true
while $i; do
    echo Outer loop $i
    j=true
    while $j; do
        echo Inner loop $j
        k=true
        while $k; do
            echo Deep loop $k
            if $k; then
                echo Continuing 3 loops...
                k=false
                j=false
                i=false
                continue 3
            elif $j; then
                echo Breaking 5 loops...
                k=false
                j=false
                i=false
                break 5
            elif $i; then
                echo Continuing...
                k=false
                j=false
                continue
            elif $j; then
                echo Breaking 2 loops...
                k=false
                j=false
                break 2
            fi
        done
    done
done

### Using for loop
echo Using for loop:
for l in true true true; do
    echo Outer loop iteration: $l
    m=true
    while $m; do
        echo Inner loop iteration: $m
        if $l; then
            echo Breaking 2 loops...
            m=false
            break 2
        fi
    done
done

### Using until loop
echo Using until loop:
n=true
until ! $n; do
    echo Outer loop iteration: $n
    o=true
    until ! $o; do
        echo Inner loop iteration: $o
        if $n; then
            echo Continuing...
            o=false
            continue
        fi
    done
    n=false
done
