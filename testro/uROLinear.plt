#
# MIT License
#
# Copyright (c) 2017 Stellacore Corporation.
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject
# to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
# KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
# AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
# IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#



# unset mouse

set xlabel "RMS Angle Components [rad]" # rotate parallel
set ylabel "Err [unit]" rotate parallel

## set zrange [0:1]

set key top left
unset key

set title "True Function (black) and Linear Approx (blue)"
plot \
	  'uRO_RoFunc.dat' u (sqrt($1**2+$2**2)):3 w p pt 1 lc -1 ti "roFunc" \
	, 'uRO_LinFunc.dat' u (sqrt($1**2+$2**2)):3 w p pt 1 lc  3 ti "linFunc" \
	, 'uRO_DifErr.dat' u (sqrt($1**2+$2**2)):3 w p pt 1 lc  1 ti "linFunc" \
	, .10 * x**2 lc -1 \
	, .20 * x**2 lc -1 \
	, .30 * x**2 lc -1 \
	, .40 * x**2 lc -1 \
	, .50 * x**2 lc -1 \
	;
pause -1;

unset key
set xlabel "Mag(dPhi) [rad]" # rotate parallel
set ylabel "Mag(dTheta) [rad]" # rotate parallel
set zlabel "Err [unit]" rotate parallel

set title "Rigorous Coplanarity Values"
splot \
	  'uRO_RoFunc.dat' w d lc -1 ti "roFunc" \
	;
pause -1;

set title "Rigorous (black) vs Linear (blue) Coplanarity Values"
splot \
	  'uRO_RoFunc.dat' w d lc -1 ti "roFunc" \
	, 'uRO_LinFunc.dat' w d lc  3 ti "linFunc" \
	;
pause -1;

set title "Approximation Error (Linear-Rigorous)"
splot \
	  'uRO_DifErr.dat' w d lc  1 ti "difErr" \
	;
pause -1;

