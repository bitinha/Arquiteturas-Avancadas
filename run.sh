#!/bin/sh
#
#PBS -N matMult
#PBS -l walltime=02:00:00
#PBS -l nodes=1:r662:ppn=48
#PBS -j oe

echo $HOSTNAME

module load gcc/5.3.0
module load papi/5.4.1

cd AATP
make clean
make matMult

set -x

./bin/matMult ikj 52
./bin/matMult ikj 52
./bin/matMult ikj 52 pm1
./bin/matMult ikj 52 pm23
./bin/matMult ikj 52 pram

./bin/matMult ijk 52 
./bin/matMult ijk 52
./bin/matMult ijk 52 pm1
./bin/matMult ijk 52 pm23
./bin/matMult ijk 52 pmram

./bin/matMult jki 52
./bin/matMult jki 52 
./bin/matMult jki 52 pm1
./bin/matMult jki 52 pm23
./bin/matMult jki 52 pmram


./bin/matMult ijk_trans 52
./bin/matMult ijk_trans 52
./bin/matMult ijk_trans 52 pm1
./bin/matMult ijk_trans 52 pm23
./bin/matMult ijk_trans 52 pmram

./bin/matMult jki_trans 52
./bin/matMult jki_trans 52
./bin/matMult jki_trans 52 pm1
./bin/matMult jki_trans 52 pm23
./bin/matMult jki_trans 52 pram


./bin/matMult ijk_block 52
./bin/matMult ijk_block 52

./bin/matMult ikj_block 52
./bin/matMult ikj_block 52

./bin/matMult jki_block 52
./bin/matMult jki_block 52




./bin/matMult ikj 147
./bin/matMult ikj 147
./bin/matMult ikj 147 pm1
./bin/matMult ikj 147 pm23
./bin/matMult ikj 147 pram

./bin/matMult ijk 147 
./bin/matMult ijk 147
./bin/matMult ijk 147 pm1
./bin/matMult ijk 147 pm23
./bin/matMult ijk 147 pmram

./bin/matMult jki 147
./bin/matMult jki 147 
./bin/matMult jki 147 pm1
./bin/matMult jki 147 pm23
./bin/matMult jki 147 pmram


./bin/matMult ijk_trans 147
./bin/matMult ijk_trans 147
./bin/matMult ijk_trans 147 pm1
./bin/matMult ijk_trans 147 pm23
./bin/matMult ijk_trans 147 pmram

./bin/matMult jki_trans 147
./bin/matMult jki_trans 147
./bin/matMult jki_trans 147 pm1
./bin/matMult jki_trans 147 pm23
./bin/matMult jki_trans 147 pram


./bin/matMult ijk_block 147
./bin/matMult ijk_block 147

./bin/matMult ikj_block 147
./bin/matMult ikj_block 147

./bin/matMult jki_block 147
./bin/matMult jki_block 147




./bin/matMult ikj 1619
./bin/matMult ikj 1619
./bin/matMult ikj 1619 pm1
./bin/matMult ikj 1619 pm23
./bin/matMult ikj 1619 pram

./bin/matMult ijk 1619 
./bin/matMult ijk 1619
./bin/matMult ijk 1619 pm1
./bin/matMult ijk 1619 pm23
./bin/matMult ijk 1619 pmram

./bin/matMult jki 1619
./bin/matMult jki 1619 
./bin/matMult jki 1619 pm1
./bin/matMult jki 1619 pm23
./bin/matMult jki 1619 pmram


./bin/matMult ijk_trans 1619
./bin/matMult ijk_trans 1619
./bin/matMult ijk_trans 1619 pm1
./bin/matMult ijk_trans 1619 pm23
./bin/matMult ijk_trans 1619 pmram

./bin/matMult jki_trans 1619
./bin/matMult jki_trans 1619
./bin/matMult jki_trans 1619 pm1
./bin/matMult jki_trans 1619 pm23
./bin/matMult jki_trans 1619 pram


./bin/matMult ijk_block 1619
./bin/matMult ijk_block 1619

./bin/matMult ikj_block 1619
./bin/matMult ikj_block 1619

./bin/matMult jki_block 1619
./bin/matMult jki_block 1619




./bin/matMult ikj 3238
./bin/matMult ikj 3238
./bin/matMult ikj 3238 pm1
./bin/matMult ikj 3238 pm23
./bin/matMult ikj 3238 pram

./bin/matMult ijk 3238 
./bin/matMult ijk 3238
./bin/matMult ijk 3238 pm1
./bin/matMult ijk 3238 pm23
./bin/matMult ijk 3238 pmram

./bin/matMult jki 3238
./bin/matMult jki 3238 
./bin/matMult jki 3238 pm1
./bin/matMult jki 3238 pm23
./bin/matMult jki 3238 pmram


./bin/matMult ijk_trans 3238
./bin/matMult ijk_trans 3238
./bin/matMult ijk_trans 3238 pm1
./bin/matMult ijk_trans 3238 pm23
./bin/matMult ijk_trans 3238 pmram

./bin/matMult jki_trans 3238
./bin/matMult jki_trans 3238
./bin/matMult jki_trans 3238 pm1
./bin/matMult jki_trans 3238 pm23
./bin/matMult jki_trans 3238 pram


./bin/matMult ijk_block 3238
./bin/matMult ijk_block 3238

./bin/matMult ikj_block 3238
./bin/matMult ikj_block 3238

./bin/matMult jki_block 3238
./bin/matMult jki_block 3238



make clean
make matMultVec

./bin/matMultVec ijk_block 52
./bin/matMultVec ijk_block 52

./bin/matMultVec ikj_block 52
./bin/matMultVec ikj_block 52

./bin/matMultVec jki_block 52
./bin/matMultVec jki_block 52


./bin/matMultVec ijk_block 147
./bin/matMultVec ijk_block 147

./bin/matMultVec ikj_block 147
./bin/matMultVec ikj_block 147

./bin/matMultVec jki_block 147
./bin/matMultVec jki_block 147



./bin/matMultVec ijk_block 3238
./bin/matMultVec ijk_block 3238

echo Com 2 processadores tamanho L3 duplica

./bin/matMultVec ijk_block 4579
./bin/matMultVec ijk_block 4579
