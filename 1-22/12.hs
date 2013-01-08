import Euler
f = head [i |i <- tn 0 1, dv i >= 500]
tn n i = n + i:tn (n+i) (i+1) 
dv n =  1+2*length [i |i <- takeWhile (\j->j*j<=n) [2..],mod n i == 0]
