#!/bin/sh

fetch "http://lcamtuf.coredump.cx/afl/releases/afl-latest.tgz"
tar -xvf afl-latest.tgz
dir=`tar -tf afl-latest.tgz | head -n1`
rm afl-latest.tgz
gmake -C "${dir}"
ln -sf "${dir}/afl-fuzz" afl-fuzz
ln -sf "${dir}/afl-clang" afl-clang

