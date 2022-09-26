#!/bin/bash

run_injections(){
	cur_mode=$1
	cur_iter=$2
	root_dir=$PWD

	# generate necessary binaries and data; move them to the apropriate directories
	make -C $root_dir/$cur_mode
	mkdir -p $root_dir/output/$cur_mode/data >/dev/null 2>/dev/null
	mv $root_dir/$cur_mode/data/* $root_dir/output/$cur_mode/data
	rm -rf $root_dir/$cur_mode/data

	# inject faults
	rm -rf $root_dir/carol-fi/logs
	cd $root_dir/carol-fi
	echo "Starting fault injection:"
	echo "##################################################"
	echo ""
	./fault_injector.py -c $root_dir/$cur_mode/smith_waterman.conf -i $cur_iter -d /tmp/smith_waterman/detected.log
	echo "##################################################"
	cd $root_dir

	# move fault injection logs to the apropriate directories
	mkdir -p $root_dir/output/$cur_mode/reports >/dev/null 2>/dev/null
	mkdir -p $root_dir/output/$cur_mode/logs >/dev/null 2>/dev/null
	for i in $(ls $root_dir/carol-fi/logs); do
		rsync -av $root_dir/carol-fi/logs/$i/* $root_dir/output/$cur_mode/logs/$i/ >/dev/null 2>/dev/null
	done

	# parse fault injection data
	cd $root_dir/output/$cur_mode
	$root_dir/carol-fi/scripts/faultinj_parser.py $root_dir/output/$cur_mode
	mv *.csv $root_dir/output/$cur_mode/reports
}

helpFunction(){
	echo "Usage: $0 -m mode -i iterations"
	echo -e "\t-m Hardening technique to be tested. Valid values are \"original\", \"dup\" and \"seldup\"."
	echo -e "\t-i Number of times the fault injector will be ran with the given configuration file. Must be greater than zero."
	exit 1 # Exit script after printing help
}

while getopts "m:i:h" opt
	do
		case "$opt" in
			m ) mode="$OPTARG" ;;
			i ) iterations=$OPTARG ;;
			h ) parameterH=true ;;
			? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
		esac
	done

# Print helpFunction when necessary
if [ "$parameterH" ] || [ -z "$iterations" ] || [ -z "$mode" ]; then
	helpFunction
fi

# Test if hardening modes are valid
if [ "$mode" ]; then
	declare -a MODES=("original" "dup" "seldup")
	if [[ ! " ${MODES[*]} " =~ " ${mode} " ]]; then
		echo "\"$mode\" is not a valid hardening mode. Valid values are \"original\", \"dup\" and \"seldup\"."
		exit 1
	fi
fi

# Test if number of iterations is correct
if (( $iterations <= 0 )); then
	echo "Number of iterations must be grater than zero."
	exit 1
fi

run_injections $mode $iterations