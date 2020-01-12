#!/bin/sh

cd ??

export PATH=/share/apps/gcc/4.9.0/bin:$PATH
export LD_LIBRARY_PATH=/share/apps/gcc/4.9.0/lib:$LD_LIBRARY_PATH
source /opt/intel/compilers_and_libraries/linux/bin/compilervars.sh intel64

make manyCore
