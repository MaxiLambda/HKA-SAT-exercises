a) Wie kann man einen at-least-k contraint der Form durch einen
at-most-k' constraint ausdrücken

=> Sei n die Anzahl der Variablen im Problem <br/>
=> Sei eine Formel für ein at-most-k' Problem gegeben <br/>
=> k + k' = n, wobei k die Anzahl der negierten Variablen ist <br/>
Eine Formel zum validieren eines at-most-k' Problems stellt sicher, dass maximal k' der n Variablen negiert vorkommen. 
Dementsprechend gibt es mindestens (at-least-k) Variablen, die nicht negiert vorkommen.

=> eine at-most-k' constraint ist äquivalent zu einer at-least-(n-k') 'negated' constraint <br/>
=> durch Invertierten Literale einer at-most-k' constraint wird diese zu einer at-least-k constraint mit k = n - k' 

b) <br/>
![rules](./seq.png "img")

=> every Clause with j can be ignored as it has to satisfy 1 < j < k wihich is not possible with k = 1 <br/>
```
-x 1       ||  s  1   ,1 
-x i       ||  s  i   ,1 
-s (i-1),1 ||  s  i   ,1 
-x i       || -s (i-1),1 
-x n       || -s (n-1),1 
```
=> we can't use unit propagation as we don't have any units

c)
The regular counter checks that no overflow occures
We can use the counter value to assert the exact value
As there are no sum s i,j values for, i = n, we need to create the value we need 
=> either the previous counter was already at k and x n is false or the previous counter was at k-1 and the x n is true
=> (s(n-1,k) && -x(n)) || (s(n,k-1) && x(n))
=> In CNF ((s(n-1,k) ∨ s(n,k-1)) ∧ ((s(n-1,k) ∨ x(n)) ∧ ( -x(n) ∨ s(n,k-1)) ∧ ( -x(n) ∨ x(n))
=> remove last clause because it is always true ((s(n-1,k) ∨ s(n,k-1)) ∧ ((s(n-1,k) ∨ x(n)) ∧ ( -x(n) ∨ s(n,k-1))
```
-x 1       ||  s  1   ,1 
-x i       ||  s  i   ,1 
-s (i-1),1 ||  s  i   ,1 
-x i       || -s (i-1),1
-x n       || -s (n-1),1 
 s (n-1),k ||  s  n   ,k-1
 x  n      ||  s (n-1),k  
-x n       ||  s  n   ,k-1 
```
