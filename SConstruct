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

"""
SCons master build file for this project
"""

import os
import re
import subprocess
import sys


# Option: Build progress
AddOption \
 ( '--tpVerbose'
 , dest = 'tpVerbose'
 , default = False
 , action = "store_true"
 , help = "display build environment progress (cf. '-silent')"
 )
# Option: Build action control
AddOption \
 ( '--tpDryRun'
 , dest = 'tpDryRun'
 , default = False
 , action = "store_true"
 , help = "Report build actions but do NOT invoke sconscripts"
 )


# Option: Build control optimization/debug
AddOption \
 ( '--tpOptimize'
 , dest = "tpOptimize"
 , default = True
 , action = "store_true"
 , help = "Build with compiler optimization settings"
 )
AddOption \
 ( '--tpDebug' # Explicit control over NOT optimize
 , dest = "tpOptimize"
 , default = True
 , action = "store_false"
 , help = "Build with compiler debug settings"
 )
AddOption \
 ( '--tpSnapshot' # Explicitly OMIT version ID
 , dest = "tpSnapshot"
 , default = False
 , action = "store_true"
 , help = "Build with*OUT* software version string"
 )

AddOption \
 ( '--tpNoAssert'
 , dest = "tpNoAssert"
 , default = False
 , action = "store_true"
 , help = "Build with compiler NDEBUG set"
 )

#
# Parse command line build options
#

# Scons/Python control vars
tpBeSilent = GetOption('silent') 
tpVerbose = GetOption('tpVerbose') 
tpDryRun = GetOption('tpDryRun') 
tpOptimize = GetOption('tpOptimize')
tpSnapshot = GetOption('tpSnapshot')
tpNoAssert = GetOption('tpNoAssert')

# Scons/Python defaults
optNoAssert = ""

# Enable s/w configuration branding
swID = ""
if not tpSnapshot :
 spVer = subprocess.Popen(["git", "describe"], stdout=subprocess.PIPE)
 (spVerOut, spVerErr) = spVer.communicate()
 swID = spVerOut.rstrip()

quote = str('\\"')
optSwVersion = str('-D SCM_VERSION_ID=') + quote + str(swID) + quote

# Enable assert()
if tpNoAssert :
 optNoAssert = "-D NDEBUG"


# Aggregate build command options
OptionCppFlags = \
 [ optSwVersion
 , optNoAssert
 ]


# Paths to include files
incPath = \
 [ '../'
 , '/usr/include/eigen3/Eigen/'
 ]

# General compilation flags for all builds
compFlagsAll = \
 [ '-std=c++11'
 , '-pthread'
 , '-fomit-frame-pointer'
 , '-pedantic'
 , '-Wall'
 , '-Wextra'
 , '-Wuninitialized'
 , '-Winit-self'
 , '-Wuninitialized'
 , '-Winit-self'
 ]

# Optimization specific flags
compFlagsOptimize = compFlagsAll + \
 [ '-O3'
 , '-s' # strip
 ]

# Debug specific flags
compFlagsDebug = compFlagsAll + \
 [ '-g' # add ' -p' for profiling
 ]

libpaths = \
 [ '/usr/lib/x86_64-linux-gnu/'
 , '/usr/lib/'
 ]

linklibs = \
 [ 'pthread'
 ]

# Workaround for g++ bug
# -- otherwise sometimes won't include libpthread.so
# -- ? perhaps? maybe? happens with all header files are involved ? ?
linkflags = \
 [ '-Wl,--no-as-needed'
 , '-lpthread'
 , '-Wl,--as-needed'
 ]

#
# Start actual build process
#

if not tpBeSilent :
 print("Top-level Sconstruct file:")

# Configure construction environment for use by modules
if tpOptimize :
 if not tpBeSilent :
  print("... setting optimize environment")
 env = Environment \
  ( CCFLAGS = compFlagsOptimize
  , CPPFLAGS = OptionCppFlags
  )
else :
 if not tpBeSilent :
  print("... setting default environment")
  print("... setting debug environment")
 env = Environment \
  ( CCFLAGS = compFlagsDebug
  , CPPFLAGS = OptionCppFlags
  )

# Note: Import TERM for gcc color reporting
#       If using colorgcc: apparently also need PATH and HOME per:
#       https://bitbucket.org/scons/scons/wiki/ColorGcc
env['ENV']['TERM'] = os.environ['TERM']

# configure common default build vars
env.Append(CPPPATH = incPath)
env.Append(LIBPATH = libpaths)
env.Append(LIBS = linklibs)
env.Append(LINKFLAGS=linkflags)

cmdout = os.popen("find . -name sconscript").read()
scfiles = cmdout.split()

if not tpBeSilent :
 print("Default build commands include ...")
 print("... swVer: ", optSwVersion)
 print("... CPPPATH: ", env.subst('$CPPPATH'))
 print("... CCCOM: ", env.subst('$CCCOM'))
 print("... LINKCOM: ", env.subst('$LINKCOM'))
 print("... LIBPATH: ", env.subst('$LIBPATH'))
 print("... LIBS: ", env.subst('$LIBS'))

def moduleNameFrom(path) :
 modname = None
 # expect something like: ['.', '', 'mod', 'sconscript']
 fields = re.split("lib|test|main|verify|sub|/", path)
 if 3 < len(fields) :
  modname = fields[2]
 if tpVerbose :
  print("Inside Func: fields", fields)
  print("Inside Func: modname", modname)
 return modname

if tpVerbose :
 print("=================")

for scfile in scfiles :
 if tpVerbose :
  print("... Module:", scfile)
 if not tpDryRun :
  modname = moduleNameFrom(scfile)
  Export('scfile')
  Export('modname')
  showSconsProgress = tpVerbose # or change name in other files
  Export('showSconsProgress')
  SConscript(scfile, exports='env')

if tpVerbose :
 print("=================")


