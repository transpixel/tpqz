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

#!/bin/bash
#
dowhat="Generate DOT-style dependencies from library source files."
#

if [ $# -eq 0 ] ; then
    progname=`basename "${0}"`
    echo ""
    echo "========================"
    echo "${dowhat}"
    echo "Usage: ${progname} <outpath.png> lib1/ lib2/ ..."
    echo ""
	echo "E.g.:"
	echo "> bash ${progname} <outpath.png> lib* main*"
    echo "========================"
    exit
fi

# E.g. Usage:  sh ./environment/GenerateDependencies.sh lib* main* > foo.dot
#        and:  dot -Tpng -o foo.png foo.dot "

	# force use of bash - no matter how invoked
	if [ ! "$BASH_VERSION" ] ; then
	    exec /bin/bash "$0" "$@"
	fi

	export TmpDotFile=`mktemp`
	echo "# " >> ${TmpDotFile}
	echo "# Command: e.g. to generate image foo.png " >> ${TmpDotFile}
	echo "# 	$ dot -Tpng -o foo.png dfd_0.dot " >> ${TmpDotFile}
	echo "# " >> ${TmpDotFile}
	echo "digraph Hierarchy " >> ${TmpDotFile}
	echo "{ " >> ${TmpDotFile}

	for mod in ${@:2} ; do
		echo >> ${TmpDotFile} ;
		cd $mod ;
		for dep in \
			`grep -s '^#include' *.{cpp,h,inl} \
			| grep '"lib' \
			| sed 's:/.*$::' \
			| sed 's/.*#include "//' \
			| grep -v $mod \
			| sort \
			| uniq ` \
				; do echo "    " $mod " -> " $dep >> ${TmpDotFile} ; done \
			;
		cd .. ;
	done

	echo "} " >> ${TmpDotFile}

	# Generate image frm dot spec file
	dot -Tpng -o $1 ${TmpDotFile}

	# remove temporary dot spec
	rm ${TmpDotFile}

