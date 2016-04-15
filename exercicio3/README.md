# André Nogueira Brandão - RA 116130
# Exercício 3

Obs: Tive que remover pastas grandes como archc e systemc para enviar ao github.

Ao abrir o arquivo **mips1_isa.cpp** encontramos a função **ac_behavior(add)** que descreve a funcionalidade da instrução ADD em MIPS.

Depois editamos o arquivo **mips_isa.cpp** para contar o número de vezes que uma instrução ADD é executada. Porém, mesmo editando o código do HelloWorld para fazer operações de adição, não notamos nenhum resultado.

    #include<stdio.h>

    int main()
    { int a,b, c;
      a= 3;
      b= 2;
      c = a+b;
        printf("Hello World");
        return 0;
    }

Assim, utilizamos o objdump para verificar todas as instruções executadas:

    mips-newlib-elf-objdump -d hello.mips > saida.txt

    00000128 <main>:
         128:	27bdffd8 	addiu	sp,sp,-40
         12c:	afbf0024 	sw	ra,36(sp)
         130:	afbe0020 	sw	s8,32(sp)
         134:	03a0f021 	move	s8,sp
         138:	24020003 	li	v0,3
         13c:	afc20010 	sw	v0,16(s8)
         140:	24020002 	li	v0,2
         144:	afc20014 	sw	v0,20(s8)
         148:	8fc30010 	lw	v1,16(s8)
         14c:	8fc20014 	lw	v0,20(s8)
         150:	00000000 	nop
         154:	00621021 	addu	v0,v1,v0
         158:	afc20018 	sw	v0,24(s8)

Assim, notamos que nenhuma instrução ADD foi executada, mas sim uma instrução ADDU, que no caso foi escolhida por não causar uma exception no caso de um **overflow**.

## Avaliando o Desempenho

### HelloWorld

Geramos estatísticas de simulação agora para o HelloWorld e rodamos o código. A saída está em **hellostats.txt**.

Calculamos então o número para os seguintes tipos de instruções:

| Loads/Stores | Branchs/Jumps | Outras |
|---|---|---|
| 987 | 270 | 1294 |

Assim, utilizando a tabela de CPI médio fornecida:

| Categoria | CPI Médio |
|---|---|
|Acesso à memória | 10 |
| Controle (branch/jump) | 3 |
| Outras | 1 |

Temos então **Total de Ciclos = 11.974** para o HelloWorld que criamos.

### 3 Programas da tabela

Meu RA é 116130, logo temos que executar os programas listados pelos últimos 3 dígitos: 1 3 0.

Os programas são: **qsort (small)**, **sha (small)**, **susan corners (large)**.

### Qsort

#### QsortSmall

Para o QsortSmall, rodamos o programa e obtivemos os seguintes resultados:

| Loads/Stores | Branchs/Jumps | Outras |
|---|---|---|
| 15797640 | 3800371 | 19856177 |

Temos então **Total de Ciclos = 189.233.690** para o QsortSmall.

### Sha

Existem dois tipos de input: small e large. Assim, executamos o small, como foi pedido.

#### Input small

| Loads/Stores | Branchs/Jumps | Outras |
|---|---|---|
| 2579559 | 606895 | 9989213 |

Temos então **Total de Ciclos = 37.605.488** para o ShaSmall.

### Susan

Para o Susan também existem dois tipos de input, mas executamos o large:

Rodamos com a opção soothing para comparar os resultados.

#### Input large

| Loads/Stores | Branchs/Jumps | Outras |
|---|---|---|
| 130505397 | 28198783 | 237042624 |

Temos então **Total de Ciclos = 1.626.692.943** para o SusanLarge.
