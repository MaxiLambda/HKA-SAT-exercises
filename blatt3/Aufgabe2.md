m aus {M1, M2, M3}
g aus {G1, G2}

 I) m = M1 => g = G1 <-> m = -M1 | g = G1
II) !(m = M2 & g = G1) <-> m = -M2 | g = G2

a)one-hot-encoding
1 = M1
2 = M2
3 = M3
4 = G1
5 = G2

1 2 3	//M muss gesetzt sein
4 5     //G muss gesetzt sein
-1 -2   //M hat höchstens einen Wert
-1 -3
-2 -3
-4 -5   //G hat höchstens einen Wert

-1  4	//  I)
-2 -4	// II) 

b)log encoding
-1 -2 = M1
-1  2 = M2
 1 -2 = M3
 3    = G1
-3    = G2
-1 -2		//1 2 ist keinem M zugeordnet und muss ungültig sein

 1  2  3	//  I) 1 2 ist falsch <-> M1, deshalb 3 = G1 erforderlich
 1 -2 -3	// II) 1 -2 ist falsch <-> M2, deshalb -3 = G2
