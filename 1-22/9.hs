import Euler
f  = head [i * j * (1000 - i - j) |i<-[1..1000],j <-[1..i],i^2 + j^2 == (1000 - i - j)^2]
