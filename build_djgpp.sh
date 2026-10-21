#!/bin/sh
# Cross-build MPlayer 1.0rc4 for DOS with DJGPP (i586-pc-msdosdjgpp, GCC 15).
#
# Usage: ./build_djgpp.sh [extra configure options]
#   DJGPP_PREFIX can point to the DJGPP cross toolchain installation.

DJGPP_PREFIX=${DJGPP_PREFIX:-/home/tho/dev/djgpp-install}
export PATH="$DJGPP_PREFIX/bin:$PATH"

# -std=gnu99: GCC 15 defaults to C23, where "int (*f)()" means "no arguments"
# and K&R style code no longer compiles.
# -fomit-frame-pointer: required on x86_32 when HAVE_EBP_AVAILABLE / HAVE_7REGS
# is set for FFmpeg inline assembly.
CFLAGS="-O3 -ffast-math -march=i686 -mtune=generic -pipe -fomit-frame-pointer -std=gnu99 \
 -Wno-incompatible-pointer-types -Wno-int-conversion \
 -Wno-implicit-function-declaration -Wno-implicit-int" \
./configure \
  --enable-cross-compile \
  --target=i586-pc-msdosdjgpp \
  --cc=i586-pc-msdosdjgpp-gcc \
  --as=i586-pc-msdosdjgpp-as \
  --ar=i586-pc-msdosdjgpp-ar \
  --ranlib=i586-pc-msdosdjgpp-ranlib \
  --host-cc=gcc \
  --enable-vesa \
  --enable-mmx \
  --enable-mmxext \
  --enable-sse \
  --disable-sse2 \
  --enable-ssse3 \
  --disable-win32dll \
  --disable-qtx \
  --disable-real \
  --disable-xanim \
  --disable-vidix \
  --disable-pthreads \
  --extra-cflags="-I$(pwd)/watt32/inc" \
  --extra-ldflags="-L$(pwd)/watt32/lib -lwatt" \
  "$@" || exit 1

make -j"$(nproc)" || exit 1

ls -l mplayer.exe mencoder.exe
