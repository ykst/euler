import Euler

f = g 2 []$reverse $take 100 $ cycle [leap,noleap,noleap,noleap]
 

g carry (m:ms) ys = carry `seq`  let nc = cnt carry m in whether 1 0 (carry == 0) + g nc ms ys
g carry [] (y:ys) = g carry y ys
g _ [] [] =  0 


cnt carry days = mod (days + carry) 7

noleap = [31,28,31,30,31,30,31,31,30,31,30,31]
leap = [31,29,31,30,31,30,31,31,30,31,30,31]
