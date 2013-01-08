module Euler where
-- utility functions for project euler 
-- "cheating" functions are reinvented
-- like big integers ets.
import Data.List
import System.Random
import Data.Bits
import Network.Socket
import Network.BSD
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




-- xsのリストからn個の組み合わせの全てをリストで返す
comb _ [] = []
comb 0 _ = [[]]
comb 1 x = [[y] | y <- x]
comb d (x:xs) =  [x:y |y <- comb (d-1) xs ] ++ comb d xs

-- 文字列のm文字目とn文字目を入れ替える
swap m n list = map (\x -> case x of
                                pos | pos == m -> list !! n
                                    | pos == n -> list !! m
                                    | otherwise -> list !! x) [0..length list -1]

swp list m n = map (\x -> case x of
                                pos | pos == m -> list !! n
                                    | pos == n -> list !! m
                                    | otherwise -> list !! x) [0..length list -1]
-- リストxsから(n個の組み合わせ,その残り)を全て拾う
choice :: Int -> [a] -> [([a],[a])]
choice 0 xs = [([],xs)]
choice n [] = []
choice n (x:xs) = [(x:ys,zs) | (ys,zs) <- choice (n-1) xs] ++ [(ys,x:zs) | (ys,zs) <- choice n xs]

vecSub (a1,a2) (b1,b2) = (a1 - b1, a2 - b2)

fac n = fac' n 1
 where
 fac' k n
   | k <= 1 = n
   | otherwise =  fac' (k-1) (n *k)

-- 組み合わせ数
combNum n k = fac n `div` (fac k  * fac (n - k)  )

-- リストxsから得られる全てのd個の順列
perm d xs 
   | d < 2 = [[x]|x<-xs]
   | otherwise = [x:y|x <- xs, y <- perm (d-1) xs] 

sumSeries1 :: Integer -> Integer -> Integer
sumSeries1 s e = let n = e - s in (n * (n + 1)) `div` 2 + (s * (n + 1))

sumSeries2 :: Integer -> Integer
sumSeries2 n = (n * (n + 1) * (2 * n + 1)) `div` 6 

sumSeries3 :: Integer -> Integer
sumSeries3 n = (n*n*(n+1)*(n+1))`div`4


half = flip div 2 .length 
  
shuffle ls [] = return ls
shuffle ls (x:[]) = return (x:ls)
shuffle ls xs = 
  do
    i <- randomRIO (1,length xs - 1) 
    let (y:ys) = swp xs 0 i
    shuffle (y:ls) ys

rotate i ls = (take (length ls) . drop (i `mod` (length ls) ) ) (ls ++ ls)

toEither f b
  |f b = Left b
  |otherwise = Right b

approt ls l = tail ls ++ [l]

ffmap [] _ = []
ffmap (f:fs) ls = map f ls ++ ffmap fs ls

times p 0 = []
times p i = p:times p (i-1)



-- simplest UDP server
simpleServerUDP :: Int -- port number
                -> (String -> IO ()) -- message handler
                -> IO () 
simpleServerUDP port handler = withSocketsDo $ do
  addrinfos <- getAddrInfo (Just (defaultHints {addrFlags = [AI_PASSIVE]})) Nothing (Just (show port))
  let serveraddr = head addrinfos
  sock <- socket (addrFamily serveraddr) Datagram defaultProtocol
  bindSocket sock (addrAddress serveraddr)
  procMessages sock
  where
  procMessages sock  = do
    (msg,_,addr) <- recvFrom sock 1024
    handler msg
    procMessages sock


localhost = "127.0.0.1"

simpleSendUDP :: Int -- port number
              -> String 
              -> IO () 
simpleSendUDP port msg = do
  addrinfos <- getAddrInfo Nothing (Just localhost) (Just (show port))
  let serveraddr = head addrinfos
  sock <- socket (addrFamily serveraddr) Datagram defaultProtocol
  sendstr sock (addrAddress serveraddr) msg
  where 
  sendstr _ _ [] = return ()
  sendstr sock addr omsg = do  
    sent <- sendTo sock omsg addr
    sendstr sock addr (genericDrop sent omsg)

primes = 2:filter isPrime [3,5..]
isPrime n = null [i|i <-takeWhile (\x->x*x<=n) primes,mod n i == 0]

shiftl n ls = let(l,r)=splitAt n ls in r++l
shiftr n ls = shiftl (length ls - n)

maximum0 [] = 0
maximum0 ls = maximum ls

whether t f b 
 | b = t
 | otherwise = f

splice n ls  
  |length ls <= n = [ls]
  |1>0 = let (l,r)= splitAt n ls in l:splice n r
bigMul x n = bigSum$replicate n x
bigSum x = concat$reverse$calc 0 $transpose$map reverse x
  where calc carry (l:ls)  =  
          let y = carry + sum [read[c]|c<-l] in show (mod y 10) : calc  (div y  10) ls
        calc 0 [] = []
        calc carry [] = reverse [show carry ]

sumdiviser 1 = 1
sumdiviser 2 = 3
sumdiviser n = y 1 2 n n
  where
  y rst i m n
   | i >= m = rst
   | 1>0 = case divMod  n i of
             (l,r) | l == i-> rst
                   | r ==0 -> y (i+l+rst) (i+1) l n
                   | otherwise  ->  y rst (i+1) m n
  
diviser n = 1:g [] 2 n  n
  where
  g rst i m n
   | i >= m = rst
   | 1>0 = case divMod  n i of
             (l,r) | l == i-> rst
                   | r ==0 -> g (i:l:rst) (i+1) l n
                   | otherwise  -> g rst (i+1) m n
