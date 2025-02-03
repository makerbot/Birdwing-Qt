#!/usr/bin/env python

# this script installs the development environment for qt4.8
# qt 5 seems to be targeted at accelerated hardware, which the AM1808 does not have
# additionally, the install for qt5 requires some extra work (and potentially debugging)  
# in the future we may get an accelerated hardware chip and the install for qt5 may become
# more supported.  It appears that porting applications from 4.8 to 5.0 is relatively straightforward

# syntax: ./build_qt.py /path/to/nfs

import sys
import os
import subprocess
import shutil

thisDir = os.path.dirname(os.path.abspath(__file__))
baseDir = os.path.abspath(os.path.join(thisDir, os.pardir))

qtSrcDir = thisDir

if 'clean' in sys.argv:
    print('Cleaning Qt...')
    subprocess.check_call(['make', 'clean'], cwd = qtSrcDir)
    sys.exit(0)

# Qt needs the cross-linaro toolchain to build
linaro_path = os.path.join(baseDir, 'Birdwing-Cross-Compile-Tools', 'cross-linaro', 'bin')
os.environ['PATH'] = linaro_path + os.pathsep + os.environ['PATH']

# Remove some arm-angstrom variables
os.environ.pop('CPATH', None)

tmpInstallDir = os.path.join(qtSrcDir, 'obj-install')
openSslIncludeDir = os.path.join(qtSrcDir, 'openssl', 'include')

# here are some docs related to the Qt configure utility
# http://qt-project.org/doc/qt-4.8/qt-embedded-crosscompiling.html
# http://qt-project.org/doc/qt-4.8/configure-options.html
configureCmd = ['./configure',
    '-debug',
    '-prefix', tmpInstallDir,
    '-embedded', 'arm',
    '-xplatform', 'qws/linux-arm-gnueabi-mb-g++',
    '-no-opengl',
    '-no-webkit',
    '-no-phonon',
    '-no-phonon-backend',
    '-no-script',
    '-no-scripttools',
    '-no-qt3support',
    '-no-multimedia',
    '-little-endian',
    '-opensource',
    '-qt-gfx-linuxfb',
    '-qt-zlib',
    '-qt-libjpeg',
    '-qt-libpng',
    '-confirm-license',
    '-nomake', 'demos',
    '-nomake', 'examples',
    '-DQT_QLOCALE_USES_FCVT',
    '-openssl',
    '-I', openSslIncludeDir
]

# TODO: some kind of way to rerun make when the repository has been updated,
#   but will not not in any other way touch Qt's monstrousity of a makefile
if not os.path.isdir(tmpInstallDir):
    print(' '.join(configureCmd))
    subprocess.check_call(configureCmd, cwd = qtSrcDir)
    subprocess.check_call(['make', 'install'], cwd = qtSrcDir)

def install_tree(src, tgt):
    srcpath = os.path.join(tmpInstallDir, src)
    tgtpath = os.path.join(sys.argv[1], tgt)
    print('Installing %s to %s'% (srcpath, tgtpath))
    if os.path.exists(tgtpath):
        shutil.rmtree(tgtpath)
    shutil.copytree(srcpath, tgtpath)

# Need to match a glob pattern and preserve symlinks on copy
# Would be a real pain to do this properly in python
'''
subprocess.check_call('cp -a %s %s' %
    (os.path.join(tmpInstallDir, 'lib', '*.so*'), 
     os.path.join(sys.argv[1], 'usr', 'lib')), shell=True)

install_tree('lib/fonts', 'usr/lib/fonts')
if os.path.exists(os.path.join(sys.argv[1], 'usr/apps/printerpanel')):
    install_tree('plugins/imageformats', 'usr/apps/printerpanel/imageformats')
'''
