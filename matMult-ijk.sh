#!/bin/sh
#
#PBS -N matMult-ijk
#PBS -l walltime=00:02:00
#PBS -l nodes=1:r662:ppn=48
#PBS -j oe

module load gcc/5.3.0
module load papi/5.4.1

#gcc -O2 -o matMult-ijk matMult-ijk.c
gcc -L/share/apps/papi/5.4.1/lib -I/share/apps/papi/5.4.1/include -o matMult-ijk matMult-ijk.c -lpapi

echo '52'
for i in {1..8}
do
./matMult-ijk 52
done

echo ''
echo '147'
for i in {1..8}
do
./matMult-ijk 147
done

echo ''
echo '1619'
for i in {1..8}
do
./matMult-ijk 1619
done

echo ''
echo '3238'
for i in {1..8}
do
./matMult-ijk 3238
done
