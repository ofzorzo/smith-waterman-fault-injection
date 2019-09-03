# C compiler
CC = gcc -g
SRC = smith_waterman.c
INP_GEN_SRC= random_strings.c
EXEC = smith_waterman
EXEC_INP_GEN = random_strings

INPUT1 = randInputX
INPUT2 = randInputY
REPORT = reportInput
OUTPUT = goldOutput
LASTRUN = lastDataCreated
DETECTED = /tmp/smith_waterman/detected.log

destDir = /tmp/smith_waterman

all: gen_bin gen_data mv_bin

gen_bin: $(SRC) $(INP_GEN_SRC)
	$(CC) $(SRC) -o $(EXEC)
	$(CC) $(INP_GEN_SRC) -o $(EXEC_INP_GEN) -lm

mk_dir: gen_bin
	mkdir -p $(destDir)
	mkdir -p $(LASTRUN)
	
gen_data: mk_dir
	./$(EXEC_INP_GEN)
	./$(EXEC) -x $(INPUT1) -y $(INPUT2) -o $(OUTPUT) -z $(DETECTED)

mv_bin: gen_data
	cp $(INPUT1) $(INPUT2) $(OUTPUT) $(LASTRUN)
	mv $(EXEC_INP_GEN) $(EXEC) $(INPUT1) $(INPUT2) $(OUTPUT) $(destDir)
	@echo "All done, executables should be in the $(destDir) folder. Starting fault injection: "

