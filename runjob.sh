#!/bin/bash

echo "Running runjobs.sh" 

echo qsub -l cput=10:00:00 -l walltime=10:00:00 job_NuMICCInclusive.sh
qsub -l cput=10:00:00 -l walltime=10:00:00 job_NuMICCInclusive.sh 







