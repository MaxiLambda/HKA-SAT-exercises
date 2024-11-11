# How it should work

## Build instructions
`cd blatt1/aufgabe4/pytagorean-triplets`
`mkdir build && cd build`
`cmake CMakeLists.txt ..`
`make`

## Run with
`./pytagorean-triplets`

## Why using ipasir?
because it's fast and we already used it in other problems and did not want to write a DIMACS file

## Schätzen Sie die Anzahl der Variablen und Klauseln ab für die SAT-Codierung aus der Vorlesung für das Pythagoräische-Triple-Problem bis zur Zahl n.

Sei t(n) die Menge der pytagoreischen-Triple mit a < n, b < n, c < n.
Sei N(n) die Mange der Zahlen, welche Bestandteil eines pytagoreischen-Triples aus t(n) sind.

=> es werden |N(n)| Variablen für das Lösen des Problems benötigt.
=> es werden |t(n)| * 2 Klauseln zum Lösen des Problems benötigt.

|t(n)| * 2, da jedes Triple durch die Klauseln (a b c) und (-a -b -c) modeliert werden muss
