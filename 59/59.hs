-- 久々に再開。。
-- 最終的には目グレップだけど、記号が出てこない奴で絞り込めばもうちょいスマートだったかもしれない
import Data.Bits
import Data.List
import Data.Char

hint = readFile "cipher1.txt" >>= \txt -> 
    let hist = foldl (\histogram pos -> update pos histogram) (replicate 255 0) (map read (split ',' txt)) in return $ unlines $ map (unlines . map show . zip [0..]) $ transpose $ splice 3 hist

solve key = readFile "cipher1.txt" >>= \txt -> 
    return $ zipWith (\x y -> chr (x `xor` y)) (cycle $ map ord key) (map read (split ',' txt))
    
keyhint :: Int -> Char -> Char
keyhint num c = chr (ord c `xor` num)

update n ls = let (l, (r:rs)) = splitAt n ls in l ++ [r+1] ++ rs

split sep = unfoldr phy 
    where 
    phy [] = Nothing
    phy ls = let (l,r) = span (sep /=) ls in Just (l, drop 1 r)

splice n = unfoldr phy 
    where 
    phy [] = Nothing
    phy ls = Just (splitAt n ls)

answer = solve "god" >>= print . sum . map ord
