###Default cd
mkdir folder
echo $OLDPWD
echo $PWD
cd folder
echo $OLDPWD
echo $PWD

###Using HOME
HOME=/tmp
cd
echo $OLDPWD
echo $PWD
cd
echo $OLDPWD
echo $PWD

###Cd with minus
for a in 0 1 2 3
do
    mkdir folder
    cd folder
    echo $PWD
    echo $OLDPWD
done
cd -
echo $PWD
echo $OLDPWD

