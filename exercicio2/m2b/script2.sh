#!/bin/bash

# Teste 2 - m2b
# Alterar tamanho da cache L2
cd ~lucas/mc723/traces/176.gcc.m2b/
for ((j=128;j<=4096;j=j*2))
do
    echo -e "Associatividade:1\nTamanho Cache L1:8K\nTamanho Cache L2:$j K\nTamanho Bloco:32\n" >> ~/mc723/m2b/gcc_m2b_L2Cache.txt 
    ~/mc723/dinero4sbc/dineroIV -informat s -trname gcc_m2b -maxtrace 20 -l1-isize 8K -l1-dsize 8K -l1-ibsize 32 -l1-dbsize 32 -l2-usize ${j}K -l2-ubsize 32 | grep rate  >> ~/mc723/m2b/gcc_m2b_L2Cache.txt  
    echo -e "\n"
    echo -e "\n">> ~/mc723/m2b/gcc_m2b_L2Cache.txt 
done
