#!/bin/bash

# Teste 1 - m2b
# Alterar tamanho da cache L1
cd ~lucas/mc723/traces/176.gcc.m2b/
for ((j=2;j<=256;j=j*2))
do
    echo -e "Associatividade:1\nTamanho Cache L1:$j K\nTamanho Cache L2:1024k\nTamanho Bloco:32\n" >> ~/mc723/m2b/gcc_m2b_L1Cache.txt 
    ~/mc723/dinero4sbc/dineroIV -informat s -trname gcc_m2b -maxtrace 20 -l1-isize ${j}K -l1-dsize ${j}K -l1-ibsize 32 -l1-dbsize 32 -l2-usize 1024K -l2-ubsize 32 | grep rate  >> ~/mc723/m2b/gcc_m2b_L1Cache.txt  
    echo -e "\n"
    echo -e "\n">> ~/mc723/m2b/gcc_m2b_L1Cache.txt 
done
