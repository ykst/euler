{--
 - リハビリをかねてHaskellで。
 - Intで桁溢れに気付かずひとはまり
 - もっと短く美しくなるとは思うが・・・
--}
f t = foldr iter (2,1) (take t $ 0:repeat 1)

iter a (n,d) = (d + n * (a + 1),n)

digits :: Integer -> Int
digits = length . show

answer = sum [let (a,b) = f x in if (digits $ a) > (digits $ b) then 1 else 0 | x <- [1..1000]]

