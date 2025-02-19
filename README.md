Birdwing-Qt
===========

This repository contains the source code for Qt as used on Replicator series
machines (internally referred to by the codename "birdwing").  The code in
this repository is licenced under the GNU LGPL version 2.1 -- see LICENSE.LGPL
for details.

In order to compile this library for use on Replicator series machines, a
suitable cross compiler must be used.  We strongly recommend the use of the
cross linaro toolchain at https://github.com/makerbot/Birdwing-Cross-Compile-Tools
as this is what is used for all official UltiMaker firmware builds.  (This
toolchain as well as the build instructions here require the use of linux.)

The build script `build.py` is provided as an example of how to configure
and build the Qt library for use on Replicator series machines.  The script
as written expects this repository to be cloned into a sibling directory to
a directory in which https://github.com/makerbot/Birdwing-Cross-Compile-Tools
was cloned (ie this repository exists at /home/user/build/Birdwing-Qt and that
repository exists at /home/user/build/Birdwing-Cross-Compile-Tools.  The build
script can be invoked with no arguments and will produce all library components
distributed in the firmware (along with other components not distributed in the
firmware) in the `obj-install` directory.  A path to an external directory can
also be provided as a command line argument to this script, which will copy
only the library components distributed in the firmware into this directory.

Modifying Firmware
------------------

The most straightforward way to install a modified version of the Qt library
onto a Replicator series machine is to enable ssh access on that machine.
SSH access via a public key over a local network connection can be enabled
using the script `enable_ssh.sh` checked into this repository.  (Since we
only support logging in as root via ssh, we also only support public key
authentication for ssh connections.)  To use this script (or to use ssh),
you need to separately generate a public/private keypair with ssh-keygen,
and also you need to determine the local network IP address of the printer.

With ssh enabled, the machine must be prepared to allow modifications to the
filesystem.  For security the filesystem defaults to read-only, so to modify
anything it must be remounted as read only.  For a printer at an IP address
of 192.168.0.10, the ssh command to do this would be:

```
ssh root@192.168.0.10 'mount -o remount,rw /'
```

For modifying Qt specifically it is also recommended to stop any running
applications that are using Qt, which can be accomplished by the command:

```
ssh root@192.168.0.10 '/etc/init.d/S07* stop'
```

One convenient way to install a modified build of Qt onto a printer is to
set up sshfs, which can be used to mount the root filesystem of the printer
onto a local directory on the computer used for compiling Qt, then having
the build.py script here directly copy the modified files into that directory:

```
mkdir -p $HOME/rootfs_target
sshfs root@192.168.0.10:/ $HOME/rootfs_target -o uid=$(id -u),gid=$(id -g)
python build.py $HOME/rootfs_target
fusermount -u $HOME/rootfs_target
```
