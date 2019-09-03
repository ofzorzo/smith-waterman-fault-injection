FOLDER=original
for i in 1 2 3 4
do
	#make > dump
	make
	cd ../carol-fi
	rm -rf logs
	./fault_injector.py -c ../original/smith_waterman.conf -i 50 -d /tmp/smith_waterman/detected.log
	
	for i in $(ls logs); do
		rsync -av logs/$i/* ../$FOLDER/logs/$i/ > dump
	done
	
	cd ../$FOLDER
	./faultinj_parser.py logs
	mv $FOLDER_*.csv faultReport
	#rm -rf dump
done
