# smith-waterman-fault-injection
Smith-Waterman algorithm for local sequence alignment written in C. The algorithm was implemented using three different approaches; run-time faults were injected into each one of the three implementations so they could be compared and analyzed. This was the final assignment for UFRGS' fault tolerance course (2018/2).

The folders "**original**", "**dup**" and "**seldup**" contain one of the three implementations each. The "original" folder contains a basic implementation of the Smith-Waterman algorithm; the "dup" folder simply duplicates everything as a form to obtain fault tolerance; finally, the "seldup" folder performs a selective duplication of resources to achieve fault tolerance.

## Installation
This project's dependencies are: **make**, **gcc**, **gdb**, **python** and [**carol-fi**](https://github.com/UFRGS-CAROL/carol-fi). **carol-fi** is the software responsible for injecting faults in the Smith-Waterman algorithm.

If you are using a Debian-based Linux distribution, you can run the installation script without any flags and it will install everything you need:

```shell
./install.sh
```

If you are running other Linux distributions, you must install **make**, **gcc**, **gdb** and **python** by yourself. After that, install **carol-fi** by passing the `-c` flag to the installation script:


```shell
./install.sh -c
```

I strongly recommend you don't install **carol-fi** through means other than the installation script provided by this repository. **carol-fi** is hard to get up and running and has a few bugs; the installation script installs a working version with a few patches made by me.

Finally, if you encounter any problems while injecting faults, I recommend you use [**pyenv**](https://github.com/pyenv/pyenv) to install and use **Python 2.7.15**. This seems to be the Python version utilized by **carol-fi**'s authors when developing it.

## Usage
To inject faults you must run the `run_fi.sh` script with the following required arguments:

- `-m MODE`: MODE is the implementation in which faults will be injected ("original", "dup", "seldup");
- `-i ITER`: ITER is the number of times the fault injector will be ran.

### Execution example:
```shell
./run_fi.sh -i 10 -m "original"
./run_fi.sh -i 10 -m "dup"
./run_fi.sh -i 10 -m "seldup"
```
After execution, statistics about the faults injected can be found in the output directory. You can find a report that analyzes and compares the three algorithms and how they handled faults [here](/report.pdf). Since it was written for a college project, it's in Portuguese.
