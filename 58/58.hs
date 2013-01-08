{--
 - 繝ｬ繧､繝､N縺ｮ隗堤分蜿ｷ縺ｯ縲¨*N, N*N - (N -1), N*N - (N -1)*2, N*N - (N-1)*3
--}
import  Data.Numbers.Primes

f :: [Int] -> Int -> Int -> Int -> Int
f (p:ps) l pcnt dcnt 
    | p > l * l = if dcnt < pcnt then l else f (p:ps) (l+2) (pcnt+4) dcnt
    | p == l * l - (l-1) = step
    | p == l * l - (l-1)*2 = step
    | p == l * l - (l-1)*3 = step
    | otherwise = f ps l pcnt dcnt
    where 
        step = f ps l pcnt (dcnt+10)
   
solve = f primes 3 5 0
main = print solve
