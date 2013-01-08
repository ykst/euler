import Euler
import List
f n=  h [1..n] 0--map (sum . diviser) [1..10000]
  where h [] rst = rst
        h (w:ws) rst = 
          let l = sumdiviser w 
              r = sumdiviser l 
              nex = delete l ws in 
              if w == r && l /= w && l <= n then h nex (l + r + rst)  else h nex rst

sumdiviser n = y 1 2 n n
y rst i m n
 | i >= m = rst
 | 1>0 = case divMod  n i of
           (l,r) | l == i-> rst
                 | r ==0 -> y (i+l+rst) (i+1) l n
                 | otherwise  ->  y rst (i+1) m n
  
diviser n = 1:g [] 2 n  n
g rst i m n
 | i >= m = rst
 | 1>0 = case divMod  n i of
           (l,r) | l == i-> rst
                 | r ==0 -> g (i:l:rst) (i+1) l n
                 | otherwise  -> g rst (i+1) m n
