import Euler
--f = head [i | i<-[1..], and[mod i j == 0 && even (div i j) |j<-[1..20]] ]
 --f = head[i|i<-[10..],all ((==) 0 . mod i) $[1..20] ]
f = foldl lcm 1 [1..20]
