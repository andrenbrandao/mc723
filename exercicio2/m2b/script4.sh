#!/bin/bash

# Teste 4 - m2b
# Alterar associatividade
cd ~lucas/mc723/traces/176.gcc.m2b/
for ((j=1;j<=512;j=j*2))
do
    echo -e "Associatividade:$j\nTamanho Cache L1:64k\nTamanho Cache L2:1024k\nTamanho Bloco:128\n" >> ~/mc723/m2b/gcc_m2b_assoc.txt 
    ~/mc723/dinero4sbc/dineroIV -informat s -trname gcc_m2b -maxtrace 20 -l1-isize 8K -l1-dsize 8K -l1-ibsize 64 -l1-dbsize 64 -l2-usize 1024K -l2-ubsize 128 -l1-dassoc $j -l1-iassoc $j  | grep rate  >> ~/mc723/m2b/gcc_m2b_assoc.txt 
    echo -e "\n"
    echo -e "\n">> ~/mc723/m2b/gcc_m2b_assoc.txt 
done


