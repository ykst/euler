is_circle n =
    let is_circle_sub m n r = if m == 0 then r == n else is_circle_sub (m `div` 10) n (r * 10 + m `mod` 10) in is_circle_sub n n 0

declist a b limit tail =
    if b <= a
    then (a,b):(declist (a-1) (b+1) limit tail )
    else tail

-- decendant list of pair (a,b) on value a + b
declist_rec a b limit  = declist a b limit $
    if(a + b > 2)
    then
        if (a+b-1) > limit
        then declist_rec limit (a + b -1 -limit) limit
        else declist_rec (a+b-2) 1 limit
    else []

mult_list        = map (\ (x,y) -> x * y) $ declist_rec 999 999 999
index_mult_list  = zip [1..] mult_list

main =
    let (index,first_sol) = head $ filter (is_circle.snd) index_mult_list
        root              = floor $ sqrt $ fromInteger first_sol
        search_limit      = (\x->x * x) $ (\x -> (999 - x +1)) $ root
    in
    do
        print $ maximum $ filter is_circle $ drop (index-1) $ take search_limit mult_list
