module Sort where
-- hand-made implementation of sort algorithms
import Data.List (unfoldr)
import Data.Tree
-- insertion sort
isort ls = isort' [] ls
  where
  isort' ls [] = ls --O(1)
  isort' ls (r:rs) = isort' ([ll|ll <- ls, ll < r] ++ [r] ++ [lr|lr<-ls, lr >= r]) rs -- 2 * n + T(n - 1) 

-- quick sort, but not quick
qsort  (x:ls) = 
  qsort [l | l <- ls, l < x] ++ [x] ++ qsort  [r | r<- ls, r >= x] -- 2n + 2*T(n/2)
qsort [] = []

-- selection sort 
csort [] = [] -- T(1)
csort (l:ls) = let (x,rest) = smallest [] l (ls) in x: csort (rest) --n^2 + T(n-1)
  where
  smallest ls x (c:cs)= 
   if [] == [r|r <- (c:cs), r < x] then  --n
     (x,(c:cs) ++ ls) -- T(1) --ほんとはn
   else 
     smallest (x:ls) c cs -- T(n-1)
  smallest ls x _ = (x,ls)

-- merge sort
msort [] = []
msort (l:[]) = [l]
msort ls = 
  let (lft,rgt) = splitAt (length ls `div` 2) ls in merge [] (msort lft) (msort rgt) -- 2T(n/2) + n
  where
  merge res (l:ls) (r:rs)
    |l <= r    = merge (res ++ [l]) ls (r:rs) 
    |otherwise = merge (res ++ [r]) (l:ls) rs -- T(n-1)
  merge res ls rs = res ++ ls ++ rs --T(1)

-- bubble sort
bsort [] = []
bsort ls = let (rs,xs) = bubble [] ls in rs ++ (bsort xs) 
  where
  bubble ls (r1:r2:rs) = if  r1 > r2 then bubble (ls ++ [r1]) (r2:rs) else bubble (ls ++ [r2]) (r1:rs)
  bubble ls rs  = (rs,ls)

-- heap sort
hsort xs =  unfoldr f (toTree xs) 
  where
  f (Just (Node a as)) = Just (a,mergeTrees as)
  f Nothing = Nothing
  toTree :: Ord a => [a] -> Maybe (Tree a)
  toTree [] = Nothing
  toTree (x:[]) = Just (Node x [])
  toTree xs = 
    let (ls,rs) = splitAt (half xs) xs in
      mergeTree (toTree ls) (toTree rs)

  mergeTree (Just t1) (Just t2) = 
    if rootLabel t1 <= rootLabel t2 
      then Just $ Node (rootLabel t1) (t2:subForest t1)
      else Just $ Node (rootLabel t2) (t1:subForest t2)
  mergeTree Nothing (Just t) = Just t
  mergeTree (Just t) Nothing = Just t
  mergeTree Nothing Nothing = Nothing
  mergeTrees [] = Nothing
  mergeTrees xs = foldr mergeTree Nothing [Just x | x <- xs]
  
  half = flip div 2 .length 

{-
data Ord a => HTree a =  HTree a (HTree a) (HTree a) | Hole deriving (Show)
--ヒープ二分木
consHTree n i
  |i > n = Hole
  |otherwise =  HTree i (consHTree n (i*2)) (consHTree n (i*2+1)) 

consHTL ls = mapList (consHTree (length ls) 1) ls
  where
  mapList Hole _ = Hole
  mapList (HTree i tl tr) ls = HTree (ls!!(i-1)) (mapList tl ls) (mapList tr ls)

heightHTL Hole = 0
heightHTL (HTree _ tl tr) =  1 + (max (heightHTL tl) (heightHTL tr))

isMaxHTL Hole = True 
isMaxHTL t@(HTree i tl tr)  = compHTL i t
  where
  compHTL _ Hole = True
  compHTL i' (HTree i tl tr) = i' >= i && compHTL i tl && compHTL i tr

maxHeapify Hole = Hole
maxHeapify t@(HTree ii ll rr) =  
  let t@(HTree i tl tr)  = HTree ii (maxHeapify ll) (maxHeapify rr) in
    case tl of
      (HTree l l1 l2) -> 
        if i >= l 
          then t 
          else
            case tr of
              (HTree r r1 r2) ->
                if r > l 
                  then HTree r tl (maxHeapify (HTree i r1 r2))
                  else HTree l (maxHeapify (HTree i l1 l2)) tr
              _ -> HTree l (maxHeapify (HTree i l1 l2)) Hole
      _ -> t
-}

--toNodes = map (\x -> Just (Node x []))
--toTree' = mergeTrees . toNodes -- これはだめ



