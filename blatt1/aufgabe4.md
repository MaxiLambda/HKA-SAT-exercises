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
