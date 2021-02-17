# Máquina Norma

Implementação da máquina teórica Norma em C++.

A implementação do Interpretador foi feita para facilitar testes da máquina,
ele não é a versão que será usada na versão final.
Portanto, ele não está otimizado, deve conter bugs e não possuí todas as
verificações de sintáxe (por exemplo, não verifica se o go_to está escrito
corretamente).

## Uma breve explicação da máquina Norma

A máquina Norma é uma máquina teorica de registradores de números inteiros,
que possuí infinitos registradores (tanto quanto necessários),
2 operações e 1 teste:

- incremento de registrador
- decremento de registrador
- teste é_zero do registrador

Desses infinitos registradores, dois são especiais:

- o registrador X de *entrada*
- o registrador Y de *saída*

O poder computacional dessa máquina é equivalente a máquina de turing

## Sintaxe

O interpretador utiliza a seguinte sintaxe:

### Main
O código executado deve estar dentro do escopo da `main { }`

### Subrotinas
Para criar um escopo de subrotinas:
```
operation foo p1,p2,p3 {

}
```

Onde os registradores serao associados a cada parâmetro.

Para retornar true ou false, basta colocar esses valores como
endereço da próxima instrução.

### Incremento
```
id_linha: do inc REG goto prox_linha
```
**lê-se:** *id_linha: incremente REG vá para prox_linha*

### Decremento
```
id_linha: do dec REG goto prox_linha
```
**lê-se:** *id_linha: decremente REG vá para prox_linha*

### Teste é_zero
```
id_linha: if zero REG then goto opcao1 else goto opcao2
```
**lê-se:** *id_linha: se REG é zero, vá para opcao1, senão vá para opcao2*

### Chamada de função
```
id_linha: do foo a,b,c goto id1
ou
id_linha: if foo a,b,c then goto opcao1 else goto opcao2
```

### Observações necessárias

Todas as linhas devem ter um Id, assim como a próxima linha após a execução 
dela.

O programa acaba quando for chamada uma linha não existente

## Execução
Após escrever seu código em um arquivo `.mn`, você poderá executa-lo usando:
```
norma ARQUIVO.MN ENTRADA
```
Onde `norma` é o programa compilado.

## Melhorias esperadas

- Implementar uma versão melhorada do intepretador
- Implementar as funções `add REG VALUE` e `sub REG VALUE` para facilitar


