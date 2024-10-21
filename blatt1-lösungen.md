# Aufgabe 2 - Sudoku Solver
Mit einem IPASIR SAT Solver sollen nxn Sodokus gelöst werden.

### Codierung: 
* Jedes Feld wird als Bit-Vektor der Länge n dargestellt.
* Nur ein Bit in einem Bit-Vektor darf gesetzt sein
* Ein Bit-Vektor wird durch n-Variablen dargestellt

=> Die Anzahl der Felder ist unbekannt => Es muss eine Liste an Bit-Vektoren genutzt werden

=> Ein nxn Sudoku hat n^2 Felder. Somit besteht das Sudoku aus n^3 Variablen.

=> Darstellung als bool[][][] (3D-Liste) oder als bool[] (1D-Liste).
Darstellung als 1D Liste lässt sich einfach auf SAT Problem mappen, da der Index als Variablennamen im SAT-Solver 
abgebildet werden kann.
Jedoch ist der Zugriff auf den Speicher dann rechenaufwändiger.

## Relation zwischen SAT-Variable/1D-Index und 3D Index
Der 1D-Index i kann als Polynom 2. Grades in Abhänigkeit von der Zeile a, der Spalte b, dem Feld c und n geschrieben werden :
`i = an^2 + bn + c`

Abbildung für Zeile und Spalte. Die Erhaltene Zahl muss noch um ein Offset aus [0,n-1] verschoben werden (Zugriff auf die einzelnen Felder)
|0|n|2n|...|(n-1)n|
|-|-|-|-|-|
|n^2|n^2+n|n^2+2n|...|...|
|2n^2|...|...|...|...|
|3n^2|...|...|...|...|
|...|...|...|...|...|
|(n-1)n^2|...|...|...|(n-1)n^2+(n-1)n|


# Aufgabe 3 - Van-der-Waerde Zahl

Sei W(2,k) diejenige Van-der-Waerde Zahl, für die es in einer Binarären Zeichenkette der Längr n nicht mehr möglich ist, k Wiederholungen in einem festen Abstand zu vermeiden.

Wie viele Klauseln und Variablen sind zum prüfen von `W(2,k) > n` notwendig?

Es sind n Variablen notwendig. Jedes Element der Zeichenkette benötigt eine Varible.

Beweis:
Sei o.B.d.A. d der Abstand. 

Annahme:
Zum prüfen ob eine binäre Zeichenkette der Längen n k Wiederholungen aufweist sind `n - d(k - 1)` Klauseln notwendig.

Jede Abfolge a mit k Wiederholungen mit einem Abstand d > 1 hat eine Länge l von `d(k - 1) + 1`.

```
n = 7; d = 1; k = 3
-------
xxx
=> c = 1(3 - 1) + 1= 3

n = 7; d = 2; k = 3
-------
xoxox
=> c = 2(3 - 1) + 1 = 5
```

Dementsprechend gibt es `n - c` weitere Möglichkeiten um Abfolgen aus der Zeichenkette zu entnehemen.

=> es gibt `n - c + 1` Möglichkeiten Abfolgen aus der Kette zu entnehmen.
```
n - c + 1              =
n - (d(k - 1) + 1) + 1 =
n - d(k - 1) - 1 + 1   = 
n - d(k - 1)
```
q.e.d.

Es muss für jeden möglichen Abstand d geprüft werden, ob die Zeichenkette k Wiederholungen aufweist.
Eine Abfolge aus k Wiederholungen mit einem Abstand d hat eine Länge von `d(k - 1) + 1`. Das bedeutet, dass eine Prüfung für alle d, für die gilt `d(k - 1) + 1 <= n` erforderlich ist.
```
d(k - 1) + 1 <= n      <=>
d(k - 1)     <  n      <=>
d            < n/(k-1)
```
Es sind also `SUM(d,d < n / (k - 1)) = n - d(k - 1)` Klauseln und `n` Variablen notwendig.
