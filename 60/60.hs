module Main where
-- 組み合わせを全て試すのは厳しい。

import Data.Numbers.Primes
import Debug.Trace
import Data.List
import Data.Maybe
import qualified Data.Map as M
--[["673","109","7","3"]]
--solve = sumString ["129976621","673","109","7","3"]

{-
type PList = M.Map Integer [Integer]
type RList = M.Map Integer [Integer] 

solve level = solve' M.empty M.empty (drop 1 primes)
    where
    solve' :: PList -> RList -> [Integer] -> String
    solve' map rmap (p:ps) = 
         case update map rmap (splitPrimes p) of
            Left (map', rmap') -> (trace (show p ++ "\nleft\n" ++ (show $ M.toList map') ++ "\nright\n" ++ (show $ M.toList rmap')))solve' map' rmap' ps
            --Left (map', rmap') -> (trace (show p ++ "\n" ++ (show $ M.toList map')))solve' map' rmap' ps
            -- Left (map', rmap') -> solve' map' rmap' ps
            Right ls -> show ls
    update :: PList -> RList -> [(Integer, Integer)] -> Either (PList, RList) [Integer]
    update map rmap [] = Left (map, rmap)
    update map rmap ((key, value):kvs) = 
        let map' = M.insertWithKey f key [value] map
            --rkey = nub $ sort $ key : (fromJust $ M.lookup key map')
            --rmap' = M.insertWithKey g rkey 1 rmap in
            rmap' = M.insertWithKey f value [key] rmap in
                    update map' rmap' kvs
                    {-
            case M.lookup rkey rmap' of
                Just x -> if length rkey == level  && x == level 
                    then Right rkey 
                    else update map' rmap' kvs
                _ -> update map' rmap' kvs
                -}
        where f key [x] xs = (x:xs)
              --g key new old = old + new

splitPrimes :: Integer -> [(Integer, Integer)]
splitPrimes p = 
    concat $ zipWith (\x y -> 
        if safePrime x && safePrime y 
            then [(read x, read y)] 
            else []) (inits (show p)) (tails (show p))

safePrime [] = False
safePrime ('0':_) = False
safePrime x = isPrime (read x)
-}

solve level = solve' [["3"]] $ map show (drop 2 primes)
    where
    solve' :: [[String]] -> [String] -> [[String]]
    solve' cand (p:ps) = 
        let cand' = (check cand) []
            answers = filter ((level ==) . length) cand' in 
        if null answers 
            --then if exists then (traceShow cand') solve' ([p]:cand') ps else (traceShow cand') solve' (cand') ps
            --then if longest' /= longest 
             --   then solve' (filter (((longest' - 1) <=) . length) ([p]:cand')) longest' ps 
             --   else solve' ([p]:cand') longest' ps 
            --then (traceShow $ filter (((level - 1)==) . length) cand') solve' ([p]:cand') ps 
            --then seeDetail cand' (filter (((level - 1) ==) . length) cand') -- 
            then solve' ([p]:cand') ps 
            --then  solve' ([p]:cand') ps 
            --else minimum $ map sumString answers
            else answers
            where 
            seeDetail cand' [] = solve' ([p]:cand') ps 
            seeDetail cand' (c:cs) = solve'' (take 10000 ps)
                where
                solve'' [] = solve' ([p]:cand') ps
                solve'' (p':ps') 
                    | allPrimeString (map (flip (++) p') c) && allPrimeString (map (p' ++) c) = [(p':c)]
                    | otherwise  = solve'' ps'
            check [] ret = ret
            check (c:cs) ret
                | allPrimeString (map (flip (++) p) c) && allPrimeString (map (p ++) c) = check cs ((p:c):ret)
                | otherwise  = check cs (c:ret)

allPrimeString :: [String] -> Bool
allPrimeString = and .  map (isPrime . read)

{-
solve2 = solve' ["1871","743","23","11"] $ map show primes
    where
    solve' c (p:ps) 
        | allPrimeString (map (flip (++) p) c) && allPrimeString (map (p ++) c) = (p:c)
        | otherwise  = solve' c ps
main = print (solve2)
        -}
sumString = sum . map read


