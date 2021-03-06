# c-lexer1.gp

set terminal pngcairo transparent noenhanced font "arial,10" fontscale 1.0 size 900, 600
set output '../img-c-lexer1-both-1000.png'
set datafile separator "\t"

# Style for C.
set style line 1 \
    linecolor rgb '#0060ad' \
    pointtype 7 pointsize 1.5

# Style for C++.
set style line 2 \
    linecolor rgb '#ff60ad' \
    pointtype 7 pointsize 1.5

# Labels
set xlabel 'Size (bytes)' font ",16"
set ylabel 'Time (ms)' font ",16"
set title 'c-lexer1.lex, -Cf and -CJ only, median of 5 runs, 1000 scans' font ",20"

# Axis ranges
set xrange [0:185000]
set yrange [1800:2250]

# Adjust edge margins since otherwise the text gets slightly cut off.
set bmargin 4
set tmargin 4.2
set rmargin 5
set lmargin 10

plot 'operf/table-smflex-c-lexer1-both-5-1000.tsv' \
        using 3:4:5 with yerrorbars title 'C' linestyle 1, \
     '' using 3:4:1 with labels notitle offset 0, char -0.6, \
     '' using 6:7:8 with yerrorbars title 'C++' linestyle 2, \
     '' using 6:7:1 with labels notitle offset 0, char 1

# EOF
