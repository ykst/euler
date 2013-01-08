
f n = head$g n 2 [2] 
g 1 t ls = ls
g v t ls = if mod v t == 0 then g (v `div` t) t (t:ls) else g v (t+1) ls
 
