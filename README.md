# Eeyore Alarm #

This is an alarm that doesn't turn off until you leave your room.
Please do not depend on it to actually work if you need to wake up.

## Dependencies ##

This program is designed to run on an Onion Omega2. It requires mailsend to be
installed, and depends on the ugpio library.

## Building ##
Use the included makefile to build by running 'make'. If you wish to
cross-compile it for the Omega2, modify xCompile.sh as found in
[this example](https://github.com/OnionIoT/c-cross-compile-example), and use as
directed.
