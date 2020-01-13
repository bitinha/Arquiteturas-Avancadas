#!/bin/sh
#
#PBS -N lab4
#PBS -l walltime=02:00:00
#PBS -l nodes=1:r662:ppn=4
#PBS -q mei

cd $HOME/temp/

module load gcc/4.9.0
module load papi/5.4.1

source /share/apps/intel/parallel_studio_xe_2019/compilers_and_libraries_2019/linux/bin/compilervars.sh intel64

source /share/apps/intel/advisor_2019.4.0.597843/advixe-vars.sh

make

advixe-cl -collect roofline -stack -project-dir=. -- bin/matMult
