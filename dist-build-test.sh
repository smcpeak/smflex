#!/bin/sh
# Build and test from within a freshly unpacked distribution, in the
# way I expect end users to do so.  I run this on each target platform
# to verify a release.

set -e
set -x

./configure --prefix=$(pwd)/opt
make
make check
make install
make -C test check
make bigcheck

set +x
echo "dist-build-test.sh: Success"

# EOF
