#!/bin/bash

echo "Type DoSubmit to submit jobs to the cluster." 


logpath=/home/deltutto/NuMICCInclusive/logs
mkdir -p $logpath
outdir=/home/deltutto/NuMICCInclusive/outputs
mkdir -p $outdir


DoSubmit() {

  namebase="numiccinclusive"

  rm $logpath/*

  elog=$logpath/e_${namebase}.txt
  olog=$logpath/o_${namebase}.txt
  echo
  echo Submitting job now.
  echo
  echo  
  echo qsub -l cput=10:00:00 -l walltime=10:00:00 job_NuMICCInclusive.sh
  qsub -l cput=10:00:00 -l walltime=10:00:00 job_NuMICCInclusive.sh
  echo

}








