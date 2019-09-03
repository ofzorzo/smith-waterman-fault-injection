# smith-waterman-fault-injection
Smith-Waterman algorithm written in C. The algorithm was implemented using three different approaches; run-time faults were injected into each one of the three implementations, so they could be compared and analyzed. This was the final assignment for UFRGS' fault tolerance course (2018/2). 

The folders "**original**", "**dup**" and "**seldup**" contain one of the three implementations each. The "original" folder contains a basic implementation of the Smith-Waterman algorithm; the "dup" folder simply duplicates everything as a form to obtain fault tolerance; finally, the "seldup" folder performs a selective duplication of resources to achieve fault tolerance.

## Dependencies
Besides **make**, **gcc** and **python**, [**carol-fi**](https://github.com/UFRGS-CAROL/carol-fi) is used to inject faults. This repository already includes a version of carol-fi that works with my scripts and implementations of the Smith-Waterman algorithm.

## Injecting faults
Go to the directory of the implementation you want to inject faults into. From there, simply run:
```shell
./run_fi.sh
```
You can edit ***run_fi.sh*** to accomodate the number of iterations you want it to run.
