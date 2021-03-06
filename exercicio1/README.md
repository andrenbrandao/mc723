# André Nogueira Brandão - RA 116130
# Exercício 1

Até antes do "gprof", a máquina utilizada foi:

    MacBook Pro
    2.6 GHz Intel Core i5
    8 GB 1600 MHz DDR3

Após o "gprof", foram utilizadas as máquinas do IC.

## Parte 1

O melhor tempo foi dado pelo programa com otimização O3, porém houve variações de valores a cada iteração, assim podemos considerar O2 e O3 como sendo as melhores otimizações.

*gcc primo.c -o primo*

    real	0m0.398s
    user	0m0.394s
    sys	0m0.003s

*gcc -O0 primo.c -o primo*

    real	0m0.396s
    user	0m0.392s
    sys	0m0.003s

*gcc -O1 primo.c -o primo*

    real	0m0.320s
    user	0m0.316s
    sys	0m0.002s

*gcc -O2 primo.c -o primo*

    real	0m0.294s
    user	0m0.291s
    sys	0m0.002s

*gcc -O3 primo.c -o primo*

    real	0m0.291s
    user	0m0.288s
    sys	0m0.002s

- Ao utilizarmos a opção -mtune, podemos especificar que tipo de CPU estamos utilizando e, assim, usar otmizações melhores para esse processador.
  - Com -mtune=generic utilizamos as otimizações para um número maior de processadores genéricos.
  - Com -mtune=native o código é compilado usando otimizações para a máquina local.

*gcc -mtune=generic primo.c -o primo*

    real	0m0.395s
    user	0m0.392s
    sys	0m0.002s

*gcc -mtune=generic primo.c -o primo*

    real	0m0.391s
    user	0m0.387s
    sys	0m0.003s

- Não foi possível notar nenhuma melhora utilizando otimizações mtune

## Parte 2

Um makefile foi criado para compilar os dois arquivos separados: main e calc_primo.c.
Assim, ao rodarmos a mesma otimização, foi possível notar um ligeiro aumento do tempo de execução.

Isso provavelmente se deve ao fato de a otimização ser executada separadamente para cada código, sendo menos eficiente.

    real	0m0.329s
    user	0m0.325s
    sys	0m0.002s

## Parte 3 (pasta parte3)

1. Utilizando apenas 1 arquivo fonte: primo2.c - o n escolhido foi de 105337

        real	0m1.584s
        user	0m1.572s
        sys	0m0.006s

2. Utilizando agora 2 arquivos separados: main2.c e calc_primo2.c - n escolhido de 105337

        real	0m1.928s
        user	0m1.915s
        sys	0m0.007s

Novamente, podemos tomar que há um aumento de tempo de execução ao compilarmos o programa utilizando arquivos separados.

## Parte 4 (pasta parte4)

1. Utilizando 1 arquivo fonte: primo2.c (n escolhido de 105337)

        real	0m0.771s
        user	0m0.766s
        sys	0m0.004s

 Notamos que com a modificação do código para varrer apenas número ímpares, obtemos aproxidamente a metade do tempo de execução anterior.

2. Utilizamos o gprof em uma máquina Linux do IC, retornando o seguinte resultado com a otimização -O1:

        index % time    self  children    called     name
                        0.67    0.00  105337/105337  main [2]
        [1]    100.0    0.67    0.00  105337         primo [1]
        -----------------------------------------------
                                                     <spontaneous>
        [2]    100.0    0.00    0.67                     main [2]
                        0.67    0.00  105337/105337      primo [1]
        -----------------------------------------------

 Assim, chegamos à conclusão de que a função primo foi chamada *n vezes*, tomando a maior parte do tempo do programa, sendo assim, um alvo necessário de otimização.
 
## Parte 5 (pasta parte5)

Já que notamos que as chamadas à função primo estão tomando a maior parte do tempo do programa, elas serão nossas candidatas à paralelização.

Modificamos então o código, paralelizando a função **for** que faz as chamadas de primo().

    #pragma omp parallel for private(i) schedule(dynamic) reduction(+:count)
    for(i = 1; i <= n; i ++) {
      if(primo(i))
        count++;
    }

O tempo de execução do programa passa a ser:

*gcc -fopenmp -O3 primo2.c -o primo2*

*./primo2 105337*


    real	0m0.202s
    user	0m0.780s
    sys	0m0.002s

Notamos assim, uma melhora bastante significativa com a paralelização. O que era esperado, já que o cálculo dos primos foi realizado em paralelo, diminuindo o tempo necessário para execução.

Para melhorarmos ainda mais o desempenho do programa poderíamos tentar encontrar algoritmos mais eficientes.
