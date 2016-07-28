#!/bin/bash

echo
echo "This is job_NuMICCInclusive.sh."
echo

cd $TMPDIR
source /home/deltutto/NuMICCInclusive/setupNuMICCInclusive.sh

echo
echo "******************** RUNNING NuMICCInclusive"
echo

/home/deltutto/NuMICCInclusive/NuMICCInclusive

echo cp *.C ${outdir}
cp *.C ${outdir}

echo
echo "This is the end of job_NuMICCInclusive.sh."
echo
