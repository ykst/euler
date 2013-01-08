module Main where

import Euler


f = maximum [v |i<-[100..999],j<-[i..999],let {v = i*j;s = show v}, s == reverse s]
main = print f
