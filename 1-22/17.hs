import Euler

f n = sum [cnt i 0|i<-[1..n]] + 11
--f n = [cnt2 i "" |i<-[1..n]]  
cnt n c
 | n >= 100 = (d1 !! div n 100) + 7 + cnt (mod n 100) 3
 | n >= 20 = (d2 !! div n 10) + c + cnt (mod n 10) 0
 | n >= 1 = c + d1 !! n
 | n == 0 = 0

cnt2 n c
 | n >= 100 = (e1 !! div n 100) : "hundred" :  cnt2 (mod n 100) "and"
 | n >= 20 = c :(e2 !! div n 10) :cnt2 (mod n 10) ""
 | n >= 1 = c : [e1 !! n]
 | n == 0 = []
d1= map length e1
d2= map length e2 
e1=["","one","two","three","four","five","six","seven","eight","nine","ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"]
e2=["","ten","twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"]
