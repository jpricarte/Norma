# Máquina Norma

Implementação da máquina teórica Norma em C++.

A implementação do Interpretador foi feita para facilitar testes da máquina,
ele não é a versão que será usada na versão final.
Portanto, ele não está otimizado, deve conter bugs e não possuí todas as
verificações de sintaxe (por exemplo, não verifica se o go_to está escrito
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

O poder computacional dessa máquina é equivalente a máquina de turing.

## Sintaxe

O interpretador utiliza a seguinte sintaxe, que é semelhante a
sintaxe criada para o simulador do prof. Rodrigo Machado (INF-UFRGS) 
[disponível aqui](http://www.inf.ufrgs.br/~rma/simuladores/norma.html):

### Main
O código executado deve estar dentro do escopo da `main { }`

### Subrotinas
Para criar um escopo de subrotinas:
```
operation foo p1,p2,p3 {

}
```

Caso queira fazer um teste, a sintaxe fica:
```
test foo p1,p2,p3 {

}
```

Onde os registradores serão associados a cada parâmetro.

**Obs:** As subrotinas foram implementadas de forma que o compilador
retorna `true` quando a subrotina termina, a não ser que seja
explicitamente dito o contrário (`goto false`).
Dessa forma, os termos `operation` e `test` são equivalentes,
mas devem ser usados para fim de documentação.

as expressões `goto true` e `goto false` funcionam como expressões
de retorno desses valores, portanto são palavras que não podem
ser usadas como tag de linha.

### Incremento
```
tag: do inc REG goto prox_linha
```
**lê-se:** *tag: incremente REG vá para prox_linha*

### Decremento
```
tag: do dec REG goto prox_linha
```
**lê-se:** *tag: decremente REG vá para prox_linha*

### Teste é_zero
```
tag: if zero REG then goto opcao1 else goto opcao2
```
**lê-se:** *tag: se REG é zero, vá para opcao1, senão vá para opcao2*

### Chamada de função
```
tag: do foo a,b,c goto id1

```
**lê-se:** *tag: execute foo(a,b,c) vá para prox_linha*
```

tag: if foo a,b,c then goto opcao1 else goto opcao2
```
**lê-se:** *tag: se foo(a,b,c) é verdadeiro, vá para opcao1, senão vá para opcao2*

### Observações necessárias

Todas as linhas devem ter uma tag no inicio e ao menos um `goto`.

O programa (ou subrotina) acaba quando for chamada uma linha não existente.
Em subrotinas, elas também acabam ao chegar em `goto true` ou `goto false`.

## Execução

Após escrever seu código em um arquivo `.mn`, você poderá executa-lo usando:
```
norma ARQUIVO.MN ENTRADA
```
Onde `norma` é o programa compilado.

## Melhorias esperadas

- Implementar uma versão melhorada do intepretador
- Implementar as funções `add REG,VALUE`, `sub REG,VALUE`, `cmp REG,VALUE` para facilitar.

## Refatoração em andamento

A versão original desse programa foi escrita em uma tarde no modelo XGH,
então claramente o código não foi feito de forma muito legível.
Estou refatorando o código aos poucos, mas caso algo no código esteja confuso ou
ilegível, coloque como issue ou mande um pull request com a mudança, que ela será
muito bem vinda!
