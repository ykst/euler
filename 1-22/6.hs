import Euler
l = [1..100]
f = (sum l)^2 - sum[i^2|i<-l]
