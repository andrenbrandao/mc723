#!/bin/bash

# Teste 3 - m2b
# Alterar tamanho do bloco
cd ~lucas/mc723/traces/176.gcc.m2b/
for ((j=8;j<=4096;j=j*2))
do
    echo -e "Associatividade:1\nTamanho Cache L1:8k\nTamanho Cache L2:1024k\nTamanho Bloco:$j\n" >> ~/mc723/m2b/gcc_m2b_bloco.txt 
    ~/mc723/dinero4sbc/dineroIV -informat s -trname gcc_m2b -maxtrace 20 -l1-isize 8K -l1-dsize 8K -l1-ibsize $j -l1-dbsize $j -l2-usize 1024K -l2-ubsize $j | grep rate  >> ~/mc723/m2b/gcc_m2b_bloco.txt 
    echo -e "\n"
    echo -e "\n">> ~/mc723/m2b/gcc_m2b_bloco.txt 
done


