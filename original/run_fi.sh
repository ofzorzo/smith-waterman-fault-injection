for i in 1 2 3
do
	make > dump
	cd carol-fi
	./fault_injector.py -c ../smith_waterman.conf -i 500
	cd scripts
	./faultinj_parser.py
	cp scripts_*.csv ../../faultReport
	cd ../../
	rm -rf dump
done
