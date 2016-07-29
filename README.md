# NuMICCInclusive

Code to study NuMI NuMu CC Inclusive Cross Section at MicroBooNE.

## Setup

Run `source utils/setupNuMICCInclusive.sh`.

## Compile

Compile with `make`.

## Execute

Execute with `./NuMICCInclusive`.

## Submit job to a pbs cluster.
Open runjobs.sh and change the output and log path, then:
```
cd pbs_cluster
source runjobs.sh
DoSubmit
```
