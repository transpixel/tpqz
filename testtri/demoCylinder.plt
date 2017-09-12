#
#
#

set title "Iso-Tritille Representation of Cylindrical Cloud\nRADIAL interpolation"
set xlabel "X"
set ylabel "Y"
set zlabel "Z"

set term gif animate delay 50 size 750,750
set output "anim.gif"
set size square

set xrange [ 94:112]
set yrange [191:208]
set zrange [300:305]
set xyplane at 300

delView =  5;
numView = 360/delView;
rad(xx) = 2. * pi * xx / 180.
del(xx) = .5 * (cos(rad(xx)) + 1.);
zen(ii) = 90.*del(delView*ii);
azm(ii) = delView*ii;
do for [ii=0:numView] {

	set view zen(ii),azm(ii)

# source data
splot \
	  'test_pnts.dat' w d lc 1 ti "Surface" \
	;
## pause -1;

# node fit
splot \
	  'test_pnts.dat' w d lc 1 ti "Surface" \
	, 'test_nodes.dat' u 6:7:8 w p pt 9 lc -1 ti "Nodes" \
	;
## pause -1;

#T # tritille structure
#T splot \
#T 	  'test_pnts.dat' w d lc 1 ti "Surface" \
#T 	, 'test_nodes.dat' u 6:7:8 w p pt 9 lc -1 ti "Nodes" \
#T 	, 'test_edgesMu.dat' w l lc 7 ti "EdgesMu" \
#T 	, 'test_edgesNu.dat' w l lc 5 ti "EdgesNu" \
#T 	, 'test_edgesDi.dat' w l lc 4 ti "EdgesDi" \
#T 	;
#T ## pause -1;

splot \
	  'test_pnts.dat' w d lc 1 ti "Surface" \
	, 'test_nodes.dat' u 6:7:8 w p pt 9 lc -1 ti "Nodes" \
	, 'test_edgesAll.dat' w l lc 3 ti "TriTille" \
## pause -1;

splot \
	  'test_pnts.dat' w d lc 1 ti "Surface" \
	, 'test_nodes.dat' u 6:7:8 w p pt 9 lc -1 ti "Nodes" \
	, 'test_edgesAll.dat' w l lc 3 ti "TriTille" \
	, 'test_interp.dat' w p pt 1 lc 3 ti "Interpolated" \
	;
## pause -1;

splot \
	  'test_pnts.dat' w d lc 1 ti "Surface" \
	, 'test_interp.dat' w p pt 1 lc 3 ti "Interpolated" \
	;
## pause -1;

#splot \
#	  'test_interp.dat' w d lc 3 ti "Interpolated" \
#	;
### pause -1;

#splot \
#	  'test_pnts.dat' w d ti "Surface" \
#	, 'test_interp.dat' w d lc 3 ti "Interpolated" \
#	;
### pause -1;

}

