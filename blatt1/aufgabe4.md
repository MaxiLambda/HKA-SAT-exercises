```python
from pysat.solvers import Glucose3
from pysat.formula import CNF

def pythagorean_triples(N):
    cnf = CNF()
    
    def var(x, y):
        return x * N + y + 1

    for a in range(1, N + 1):
        for b in range(a, N + 1):
            for c in range(b, N + 1):
                if a * a + b * b == c * c:
                    clause = [var(0, a), var(1, b), var(2, c)]
                    cnf.append(clause)
    
    solver = Glucose3()
    solver.append_formula(cnf)

    if solver.solve():
        model = solver.get_model()
        for a in range(1, N + 1):
            for b in range(a, N + 1):
                for c in range(b, N + 1):
                    if a * a + b * b == c * c:
                        print(f"Pythagorean triple found: ({a}, {b}, {c})")
    else:
        print("Fehler")

pythagorean_triples(20)
```

### Zweite Aufgabe:
Finde alle Pythagoreischen Tripel \( (a, b, c) \) bis zur Zahl \( n \), die \( a^2 + b^2 = c^2 \) erfüllen, mit \( a \leq b \leq c \leq n \).

---

### Schritt 1: Anzahl möglicher Tripel

Für jeden Wert von a, b und c bis n gibt es insgesamt etwa `n^3` Kombinationen. Da `a <= b <= c` gilt, reduziert sich die Anzahl der möglichen Kombinationen auf ungefähr `n^3 / 6`.

Zur Veranschaulichung:

| n  | Mögliche Tripel (geschätzt) |
|----|------------------------------|
| 5  | ungefähr `5^3 / 6 = 20.8`   |
| 10 | ungefähr `10^3 / 6 = 166.7` |
| 20 | ungefähr `20^3 / 6 = 1333.3`|

**Ergebnis**: Etwa `n^3 / 6` mögliche Tripel.

---

### Schritt 2: Anzahl der Variablen

Für jede mögliche Kombination `(a, b, c)` wird eine Variable `x_{a,b,c}` erstellt, die "wahr" ist, wenn das Tripel `a^2 + b^2 = c^2` erfüllt. Dadurch werden etwa `n^3 / 6` Variablen benötigt.

**Ergebnis**: Ungefähr `n^3 / 6` Variablen.

---

### Schritt 3: Anzahl der Klauseln

Für jedes Tripel `(a, b, c)`, das die Gleichung `a^2 + b^2 = c^2` erfüllt, wird eine Klausel hinzugefügt.

**Beispiel**: Betrachten wir das Tripel `(3, 4, 5)`:
- Die Bedingung `3^2 + 4^2 = 5^2` ist erfüllt, da `9 + 16 = 25`.
- Für dieses Tripel wird eine Klausel erstellt, die sicherstellt, dass `x_{3,4,5}` wahr ist.

Wenn wir mehrere gültige Tripel betrachten (z. B. `(3, 4, 5)`, `(5, 12, 13)`, und `(8, 15, 17)`), erhalten wir folgende Klauseln:
- `x_{3,4,5}`: `3^2 + 4^2 = 5^2`
- `x_{5,12,13}`: `5^2 + 12^2 = 13^2`
- `x_{8,15,17}`: `8^2 + 15^2 = 17^2`

Jede Klausel stellt sicher, dass das jeweilige Tripel die Pythagoreische Bedingung erfüllt.

Die Anzahl der Klauseln entspricht daher etwa der Anzahl der gültigen Tripel, also `n^3 / 6`.

**Ergebnis**: Etwa `n^3 / 6` Klauseln.

---

### Zusammenfassung

- **Variablen**: ungefähr `n^3 / 6`
- **Klauseln**: ungefähr `n^3 / 6`

---
