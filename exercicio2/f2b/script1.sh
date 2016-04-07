#!/bin/bash

# Teste 3 - f2b
# Alterar tamanho do bloco
cd ~lucas/mc723/traces/176.gcc.f2b/
for ((j=8;j<=4096;j=j*2))
do
    echo -e "Associatividade:1\nTamanho Cache L1:64k\nTamanho Cache L2:1024k\nTamanho Bloco:$j\n" >> ~/mc723/gcc_f2b_bloco2.txt  
    ~/mc723/dinero4sbc/dineroIV -informat s -trname gcc_f2b -maxtrace 20 -l1-isize 64K -l1-dsize 64K -l1-ibsize $j -l1-dbsize $j -l2-usize 1024K -l2-ubsize $j | grep rate  >> ~/mc723/gcc_f2b_bloco2.txt  
    echo -e "\n"
    echo -e "\n">> ~/mc723/gcc_f2b_bloco2.txt  
done


