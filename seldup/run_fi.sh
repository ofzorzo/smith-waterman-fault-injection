FOLDER=seldup
for i in 1 2 3 4 5
do
	make > dump
	cd ../carol-fi
	rm -rf logs
	./fault_injector.py -c ../seldup/smith_waterman.conf -i 500 -d /tmp/smith_waterman/detected.log
	
	for i in $(ls logs); do
		rsync -av logs/$i/* ../$FOLDER/logs/$i/ > dump
	done
	
	cd ../$FOLDER
	./faultinj_parser.py logs
	mv $FOLDER_*.csv faultReport
	rm -rf dump
done
