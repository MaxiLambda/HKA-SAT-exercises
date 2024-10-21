# Aufgabe 3

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
