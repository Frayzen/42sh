###Check the pwd
mkdir ok toto tata
echo $PWD
cd ok
echo $PWD
mkdir toto
cd toto
echo $PWD
cd ../../toto
echo $PWD
cd ../tata
mkdir ./thisisatest
cd ./thisisatest
echo $PWD

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

###Weird cds
mkdir toto tata
mkdir toto/a toto/b toto/c
mkdir tata/1 tata/2 tata/3
cd ./toto/../tata/././../tata/1/../2/../../toto/./a
echo $PWD
cd ../../
echo $PWD
cd toto/../tata/././../tata/1/../2/../../toto/./a
echo $PWD
cd ../..
echo $PWD

###Cd with link
mkdir -p test_dir
ln -s test_dir link
cd link
echo $PWD
