#!/bin/bash

# Teste 4 - f2b
# Alterar associatividade
cd ~lucas/mc723/traces/176.gcc.f2b/
for ((j=1;j<=512;j=j*2))
do
    echo -e "Associatividade:$j\nTamanho Cache L1:64k\nTamanho Cache L2:1024k\nTamanho Bloco:128\n" >> ~/mc723/gcc_f2b_assoc.txt  
    ~/mc723/dinero4sbc/dineroIV -informat s -trname gcc_f2b -maxtrace 20 -l1-isize 64K -l1-dsize 64K -l1-ibsize 128 -l1-dbsize 128 -l2-usize 1024K -l2-ubsize 128 -l1-dassoc $j -l1-iassoc $j  | grep rate  >> ~/mc723/gcc_f2b_assoc.txt    
    echo -e "\n"
    echo -e "\n">> ~/mc723/gcc_f2b_assoc.txt    
done


