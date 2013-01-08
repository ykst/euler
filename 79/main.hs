import List
main = print . nub . solve [] . map read . words =<< readFile "keylog.txt"

solve :: [String] -> [Integer] -> [String]
solve res [] = res
solve []  (l:ls) = solve [show l] ls
solve res (l:ls) = solve (nub $ concat [nub $ map nub $ shuffle  x (show l) | x <- res]) ls

shuffle :: String -> String -> [String]
shuffle []  ys        = [ys]
shuffle xs  []        = [xs]
shuffle (x:xs) (y:ys) = 
    concat $ [ [(y:ns)] | ns <- shuffle (x:xs) ys] ++
             [ [(x:ns)] | ns <- shuffle xs (y:ys)] 
  
