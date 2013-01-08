import Euler
import List
import qualified Data.Map as M
g n= map app2 [1..n]
f n=  maximumBy (\(_,a) (_,b) -> compare a b) $filter (\(a,_) -> a < n) $M.toAscList $snd$foldl  (\(a,m) i -> app m i) (1,M.insert 1 1 M.empty) [1..n]
app m n= case M.lookup n m of
  Nothing
   |even n -> let x = div n 2 
                  (a,m') = app m x in (1 + a,M.insert n (1+a) m')
   |n == 1 -> (1,m)
   |1>0 -> let x = 3 * n + 1 
               (a,m') =app m x in (1+a,M.insert n (1+a) m')
  Just a ->  (a,m) 

app2 n
 |even n = 1 + app2 (div n 2)
 |n == 1 = 1
 | 1>0 = 1 + app2 (3*n + 1)

