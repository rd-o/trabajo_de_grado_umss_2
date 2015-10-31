set encoding utf8
set grid
set terminal postscript eps enhanced color font 'Helvetica,20'
filename = "sprint".sprint.".eps"
#set output 'introduction.eps'
set output filename
set title "Sprint ".sprint
set xlabel "Día"
set ylabel "Horas"

datafile="sprint".sprint.".dat"

plot datafile using 1:2 title "ideal" w linespoints, \
     datafile using 1:3 title "real" w linespoints
