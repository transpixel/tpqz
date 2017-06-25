#
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
#

#
# Generate sed commands to replace keywords
#

HashKey=GITHASHKEY
DateKey=GITDATEKEY

if [ $# -ne 2 ] ; then
	echo "Usage: $0 <filein> <fileout>"
else
	filein=$1
	fileout=$2
	tmpsed="tmp.sed"_${filein}

	# get substitution values
	HashString=`git describe`
	DateString=`git log -1 --pretty="%ci" ${filein}`

	# generate sed search and replace commands
	rm -f ${tmpsed}
	echo "s/${HashKey}/${HashString}/" >> ${tmpsed}
	echo "s/${DateKey}/${DateString}/" >> ${tmpsed}

	sed -f ${tmpsed} ${filein} > ${fileout}
	rm ${tmpsed}

fi

