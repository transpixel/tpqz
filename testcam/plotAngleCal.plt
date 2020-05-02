#


set xrange [3060:3180]
set yrange [0:.1]

set xtics 10
set mxtics 10

set title \
	"Camera Principal Distance\nMonte Carlo Estimation Probability Density"
set xlabel "Principal Distance Value [pix]"
set ylabel "Probability [#]"

unset mouse
unset key
plot \
	'hist_tmp.dat' w l
	;
#pause -1;

# Normal probability function
prob(x,mean,sig) = exp(-.5*(x-mean)**2/sig**2)/sqrt(2.*pi)/sig

mean = 3110.
sig = 10.
fit prob(x,mean,sig) 'hist_tmp.dat' via mean,sig

#replot prob(x,mean,sig) w p lc -1

