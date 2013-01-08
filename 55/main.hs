revval :: Integer->Integer
revval = read . reverse . show

is_palindromic x = x == revval x

iter 0 x = 1
iter i x 
   | is_palindromic x = 0
   | otherwise        = iter (i-1) (step x)

step x = revval x + x

main = print $ sum [iter 50 (step x) | x <- [1..10000]]
