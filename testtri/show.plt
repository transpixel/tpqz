#
#
#

set title "Iso-Tritille Representation of Cylindrical Cloud\nRADIAL interpolation"
set xlabel "X"
set ylabel "Y"
set zlabel "Z"

set xrange [ 94:112]
set yrange [191:208]
set zrange [300:305]
set xyplane at 300


splot \
	  'test_pnts.dat' w d lc 1 ti "Surface" \
	, 'test_nodes.dat' u 6:7:8 w p pt 9 lc -1 ti "Nodes" \
	, 'test_edgesAll.dat' w l lc 3 ti "TriTille" \
	, 'test_interp.dat' w d lc 3 ti "Interpolated" \
	;
pause -1;

splot \
	  'test_pnts.dat' w d lc 1 ti "Surface" \
	, 'test_interp.dat' w d lc 3 ti "Interpolated" \
	;
pause -1;

