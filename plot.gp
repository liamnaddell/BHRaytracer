#!/usr/bin/env gnuplot
#set terminal png
#set output 'explicit.png'
set hidden3d
set xyplane at 0
set xlabel "x"
set ylabel "y"
set zlabel "z"
set object circle at first 0,0,0 radius char 0.5
#splot 'dat' with lp
#splot 'out_real.txt' with lp, 'out_expected.txt' with lp
splot 'out_real.txt' with lp
