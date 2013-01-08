import List
f=sum $ filter even $ fst $ span (<4000000) a
a=1:1:zipWith (+) a (tail a)
