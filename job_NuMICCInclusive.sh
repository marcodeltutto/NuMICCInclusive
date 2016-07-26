#!/bin/bash

echo
echo "This is job_NuMICCInclusive.sh."
echo

cd $TMPDIR
#source /home/deltutto/NuMIProduction/setupNuMIProduction.sh

source /cvmfs/fermilab.opensciencegrid.org/products/larsoft/setup
source /cvmfs/uboone.opensciencegrid.org/products/setup

setup root v6_06_04b -q e10:nu:prof


echo
echo "******************** RUNNING NuMICCInclusive"
echo

/home/deltutto/NuMICCInclusive/NuMICCInclusive

echo cp *.C /home/deltutto/NuMICCInclusive/
cp *.C /home/deltutto/NuMICCInclusive/

echo
echo "This is the end of job_NuMICCInclusive.sh."
echo
