
# Lexical Analyzer for D Language

Basic lexical Analyzer for the D language, not all the features are included.
<br>
This project has as objetive learn about compilers, lexical analyzer and tools like flex.
<br>


### How to use 

``` bash
cmake.
make
./Compiladores [option]
``` 
Option = 0 will use the lexical analyzer programmed in C 
<br>
Option = 1 will use the lexical analyzer generated by flex.

### Data
- The input system manage the load of the code that will be analyze.
<br>
- The symbol table is a hash table 
<br>
- The lexical analyzer try to emulate the behaviour of an automata
<br>
- The sintatic analyzer has been created with the objetive of easily 
connect this analyzer with future updates.
