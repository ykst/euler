import Euler

f n =  sum[read[i]|i<-g n "1"]
g x n
 | x < 1 =  n
 | 1>0 = g (x-1) (bigMul n x)
