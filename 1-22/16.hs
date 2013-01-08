import Euler

f n =sum [read[i]|i<-w n "1"]
w 0 s = s
w n s = w (n-1) (h s)
h s = concat$reverse $ g 0 (reverse s)
g c (l:ls)  =
  let x = c + 2 * read[l] in
    show(mod x 10):g (div x 10) ls
g 0 []  = [] 
g 1 [] = ["1"]
