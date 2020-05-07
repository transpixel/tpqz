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
SCons master build file for this project.
"""

import os
import re
import subprocess
import sys


#
#===========================================================================
# General build options
#===========================================================================
#

# Option: Build progress
AddOption \
    ( '--tpVerbose'
    , dest = 'tpVerbose'
    , default = False
    , action = "store_true"
    , help = "display build environment progress (cf. '-silent')"
    )
AddOption \
    ( '--tpDryRun'
    , dest = 'tpDryRun'
    , default = False
    , action = "store_true"
    , help = "Report build actions but do NOT invoke sconscripts"
    )
AddOption \
    ( '--tpSnapshot' # Explicitly OMIT version ID
    , dest = "tpSnapshot"
    , default = False
    , action = "store_true"
    , help = "Build with*OUT* software version string"
    )

# Option: Build environment
AddOption \
    ( '--tpCompiler'
    , dest = "tpCompiler"
    , type = "string"
    , nargs = 1
    , default = "g++"
    )

# Option: Application optimization/debug
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
    ( '--tpNoAssert'
    , dest = "tpNoAssert"
    , default = False
    , action = "store_true"
    , help = "Build with compiler NDEBUG set"
    )

#
#===========================================================================
# Custom gathering of build options
#===========================================================================
#

# Build options retrieved from command line
class TpBuild:
    """Custom options added by this SConstruct file"""

    # set build control flags from scons command line options
    def __init__(self):
        """Build control"""
        self.tpBeSilent = GetOption('silent') 
        self.tpVerbose = GetOption('tpVerbose') 
        self.tpDryRun = GetOption('tpDryRun') 
        self.tpCompiler = GetOption('tpCompiler')
        self.tpOptimize = GetOption('tpOptimize')
        self.tpSnapshot = GetOption('tpSnapshot')
        self.tpNoAssert = GetOption('tpNoAssert')

    def compilerCommand(self):
        """Command for compiler to run"""
        cmdStr = self.tpCompiler
        return cmdStr;

    # Compile option to en/dis-able assert()
    def optNoAssert(self):
        """Compiler '-D ...' option for disabling assert() statements"""
        optStr = ""
        if tpBuild.tpNoAssert :
            optStr = "-D NDEBUG"
        return optStr

    # String with software version ID extracted from source control system
    def buildIdentifier(self):
        """Software source code identification string"""
        swId = ""
        if not tpBuild.tpSnapshot :
            spVer = subprocess.Popen \
                (["git", "describe"], stdout=subprocess.PIPE)
            (spVerOut, spVerErr) = spVer.communicate()
            gotStr = spVerOut.rstrip()
            if gotStr:
                swId = gotStr
        return swId

    # String with software version ID extracted from source control system
    def optSwVersion(self):
        """Compiler '-D ...' option for setting SCM_VERSION_ID variable"""
        optDefStr = ""
        swID = self.buildIdentifier()
        quote = str('\\"')
        optDefStr = str('-D SCM_VERSION_ID=') + quote + str(swID) + quote
        return optDefStr

    def optCppFlags(self):
        """All compiler '-D ...' options"""
        # Enable s/w configuration branding
        optStr = \
            [ self.optSwVersion()
            , self.optNoAssert()
            ]
        return optStr

    def logProgress(self, msg):
        """Display msg to log progress (if not tpBeSilent)"""
        if not self.tpBeSilent:
            print(msg)

    def logDetail(self, title, args):
        """Display (title, args) information (if tpVerbose enabled)"""
        if self.tpVerbose:
            print (title, args)

    def logBuildInfo(self, anEnv):
        if not self.tpBeSilent:
            print("Build variables include ...")
            print("... CPPPATH: ", anEnv.subst("$CPPPATH"))
            print("... CCCOM: ", anEnv.subst("$CCCOM"))
            print("... LINKCOM: ", anEnv.subst("$LINKCOM"))
            print("... LIBPATH: ", anEnv.subst("$LIBPATH"))
            print("... LIBS: ", anEnv.subst("$LIBS"))

    # descriptive information about current settings
    def __str__(self):
        """String describing current build configuration request"""
        info = "\nCurrent TpBuild configuration:"
        info += "\n tpBeSilent:   " + str(self.tpBeSilent)
        info += "\n tpVerbose:    " + str(self.tpVerbose)
        info += "\n tpDryRun:     " + str(self.tpDryRun)
        info += "\n tpCompiler:   " + str(self.tpCompiler)
        info += "\n tpSnapshot:   " + str(self.tpSnapshot)
        info += "\n tpOptimize:   " + str(self.tpOptimize)
        info += "\n tpNoAssert:   " + str(self.tpNoAssert)
        info += "\n optNoAssert:  " + str(self.optNoAssert())
        info += "\n optSwVersion: " + str(self.optSwVersion())
        info += "\n"
        return info

#
#===========================================================================
# Compile/Link paths and options
#===========================================================================
#


# Paths to include files
incPath = \
    [ '../'
    , '/usr/include/eigen3/Eigen/'
    ]


# General compilation flags for all builds
compFlagsAll = \
    [ '-std=c++11'
    , '-Wc++11-compat'
    , '-pthread'
    , '-fomit-frame-pointer'
    , '-pedantic-errors'
    , '-Wall'
    , '-Wextra'
    , '-Wuninitialized'
    , '-Winit-self'
    , '-Wuninitialized'
    , '-Winit-self'
    , '-fno-nonansi-builtins'
    , '-fno-operator-names'
    , '-fstrict-enums'

    # , '-Weffc++' # complaints about default ctors (known gcc 'issues')
    # , '-Wold-style-cast' # complains: extstb, exthalf, Eigen3
    # , '-Wzero-as-null-pointer-constant'  # complains: extstb, Eigen3
    # , '-Wuseless-cast'  # complains: extstb, exthalf,

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
#===========================================================================
#  Configure Scons components considering custom options
#===========================================================================
#


# Configure build process
tpBuild = TpBuild()
tpBuild.logProgress(str(tpBuild))

# fetch current software build options
optCppFlags = tpBuild.optCppFlags()

tpBuild.logProgress("Top-level Sconstruct file:")

# Configure construction environment for use by modules
if tpBuild.tpOptimize:
    tpBuild.logProgress("... setting optimize environment")
    env = Environment \
        ( CCFLAGS = compFlagsOptimize
        , CPPFLAGS = optCppFlags
        )
else:
    tpBuild.logProgress("... setting default environment")
    tpBuild.logProgress("... setting debug environment")
    env = Environment \
        ( CCFLAGS = compFlagsDebug
        , CPPFLAGS = optCppFlags
        )

# Configure compiler to use
env["CC"] = tpBuild.compilerCommand()
env["CXX"] = tpBuild.compilerCommand()

# Note: Import TERM for gcc color reporting
#       If using colorgcc: apparently also need PATH and HOME per:
#       https://bitbucket.org/scons/scons/wiki/ColorGcc
env['ENV']['TERM'] = os.environ['TERM']

# configure common default build vars
env.Append(CPPPATH = incPath)
env.Append(LIBPATH = libpaths)
env.Append(LIBS = linklibs)
env.Append(LINKFLAGS=linkflags)

#
#===========================================================================
# Start actual build process
#===========================================================================
#

tpBuild.logBuildInfo(env)

tpBuild.logDetail("=================", "")

def moduleNameFrom(pathSubDir, tpBuild):
    """Extract module identity from subDir name"""
    modname = None
    # expect something like: ['.', '', 'mod', 'sconscript']
    fields = re.split("lib|test|main|verify|sub|/", pathSubDir)
    if 3 < len(fields) :
        modname = fields[2]
    tpBuild.logDetail("Inside Func: fields", fields)
    tpBuild.logDetail("Inside Func: modname", modname)
    return modname

# process all subDirs containing sconscript files
cmdout = os.popen("find . -name sconscript").read()
scfiles = cmdout.split()
for scfile in scfiles:
    tpBuild.logDetail("... Module:", scfile)
    if not tpBuild.tpDryRun:
        modname = moduleNameFrom(scfile, tpBuild)
        Export('scfile')
        Export('modname')
        showSconsProgress = tpBuild.tpVerbose # or change name in other files
        Export('showSconsProgress')
        SConscript(scfile, exports='env')

tpBuild.logDetail("=================", "")

#
#===========================================================================
#===========================================================================
#

# vim: ts=4
# vim: set autoindent
# vim: set nocindent
# vim: set nosmartindent
