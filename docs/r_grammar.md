# Return Grammar
In **HLM**, it is mandatory to return a value to end the program or specific function.  
> [!NOTE]
> Best practice is to use **integer** for the main function.

$$
\begin{align}
    [\text{return}] \leftarrow EXP
\end{align}
$$  

return takes only **one** expression<sub>1</sub>. The syntax would look like this $\to$  
```
return 0;
```
So EBNF for return is this $\to$  
```ebnf
return_stmt = "return", expression, ";";
```
# Variables Grammar
$$
\begin{align}
    [\text{dataType}] \space variableName = [\text{expr}] ;
\end{align}
$$
## Data types
$$
\begin{align}
    \{EXPs \space or \space [\text{expr}] \} &\to [\text{dataType}] \to
    \begin{cases}
        \space number &\to 
        \begin{cases}
            [\text{ long }] \\
            [\text{ int }] \\
            [\text{ float }] \\
            [\text{ double }] \\
        \end{cases}
        \\
        [\text{ str }] \\
        [\text{  }] \\
    \end{cases}
\end{align}
$$