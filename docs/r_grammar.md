# Return Grammar
$$
\begin{align}
    [\text{return}] \to EXPs \to [\text{int\_lit}]
\end{align}
$$
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