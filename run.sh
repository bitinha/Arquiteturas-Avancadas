#!/bin/sh
#
#PBS -N matMult
#PBS -l walltime=??
#PBS -l nodes=1:r662:ppn=48
#PBS -j oe

module load gcc/5.3.0
module load papi/5.4.1

cd ??

make matMult

set -x
./bin/matMult ikj 16
