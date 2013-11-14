#!/bin/bash 
g++ main.c -o a.out
g++ generate_file.c -o generate_file.out

echo "number insertion            heap           merge           quick" > result 

i=1
while [ $i -le 20 ] ; do
        ./generate_file.out $i > ./data
        ./a.out >> result
        i=`expr $i + 1`
done

i=1000
while [ $i -le 100000 ] ; do
        ./generate_file.out $i > ./data
        ./a.out >> result
#        echo $i
        i=`expr $i + 1000`
done
rm ./generate_file.out
