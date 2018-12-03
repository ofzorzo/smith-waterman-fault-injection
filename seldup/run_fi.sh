FOLDER=seldup
for i in 1
do
	make > dump
	cd ../carol-fi
	rm -rf logs
	./fault_injector.py -c ../seldup/smith_waterman.conf -i 15 -d /tmp/smith_waterman/detected.log
	
	for i in $(ls logs); do
		rsync -av logs/$i/* ../$FOLDER/logs/$i/ > dump
	done
	
	cd ../$FOLDER
	./faultinj_parser.py logs
	mv $FOLDER_*.csv faultReport
	rm -rf dump
done
