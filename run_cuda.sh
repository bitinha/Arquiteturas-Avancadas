#!/bin/sh
#
#PBS -N matMult_cuda
#PBS -l walltime=??
#PBS -l nodes=1:ppn=1:k20
#PBS -j oe

module load gcc/5.3.0
module load cuda/9.1

cd ??

make cuda

set -x
./bin/matMultGPU 16
