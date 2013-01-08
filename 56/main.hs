main = print $ maximum $ [sum [read [x] | x <- s] | s <- [ show (a ^ b) | a <- [1..100], b <- [1..100]]]
 
