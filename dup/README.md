novo run_fi.sh (considerar que na raiz do repositorio vou ter uma pasta carol-fi, outra original, outra dup e outra seldup; copiar faultinj_parser.py e outputComparer.py para a raiz das 3 ultimas pastas, além de criar uma pasta logs vazia nessas raízes):

Para as versões diferente de original, botar "-d /tmp/lreg/detected.log" após 500.

```sh
FOLDER=original

for i in 1 2 3
do
	make > dump
	cd ../carol-fi
	rm -rf logs
	./fault_injector.py -c ../smith_waterman.conf -i 500 #-d /tmp/lreg/detected.log
	
	for i in $(ls logs); do
		rsync -av logs/$i/* ../$FOLDER/logs/$i/ > dump
	done
	
	cd ../$FOLDER
	./faultinj_parser.py logs
	mv $FOLDER_*.csv faultReport
	rm -rf dump
	python3 outputComparer.py > sdcDiff.log
done
```