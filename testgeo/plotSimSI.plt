
set title "Space Intersection w/ Perturbance Simulation"
set xlabel "Baseline Axis"
set ylabel "Range Distance"
set zlabel "Elevation" rotate parallel

unset mouse

#
# detail zoom
#

set view 40,45
splot \
	  'usi_expStas.dat' w lp lw 3 lc rgb "blue" pt 8 ps 2 ti "StaPntConfig" \
	, 'usi_gotTips.dat' w lp pt 6 ps 2 lw 5 lc rgb "#cccc00" ti "EllipTips" \
	, 'usi_gotTips.dat' w lp pt 6 ps 2 lw 1 lc rgb "black" ti "" \
	, 'usi_gotLocs.dat' pt 6 lc rgb "#008800" \
	, 'usi_expLocs.dat' pt 7 ps 2 lc rgb "#880088" \
	;

pause -1;

#
# isotropic scale
#

lm = 40
set xrange[-lm/2.:lm/2.]
set yrange[-0:lm]
set zrange[-lm:lm]

set size square
set size ratio -1

splot \
	  'usi_expStas.dat' w lp lw 3 lc rgb "blue" pt 8 ps 2 ti "StaPntConfig" \
	, 'usi_gotTips.dat' w lp pt 6 ps 2 lw 5 lc rgb "#cccc00" ti "EllipTips" \
	, 'usi_gotTips.dat' w lp pt 6 ps 2 lw 1 lc rgb "black" ti "" \
	, 'usi_gotLocs.dat' pt 6 lc rgb "#008800" \
	, 'usi_expLocs.dat' pt 7 ps 2 lc rgb "#880088" \
	;

pause -1;

