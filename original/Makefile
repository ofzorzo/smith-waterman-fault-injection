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

destDir = /tmp/smith_waterman

all: gen_bin gen_data mv_bin

gen_bin: $(SRC) $(INP_GEN_SRC)
	$(CC) $(SRC) -o $(EXEC)
	$(CC) $(INP_GEN_SRC) -o $(EXEC_INP_GEN) -lm
	
gen_data: gen_bin
	./$(EXEC_INP_GEN)
	./$(EXEC) -x $(INPUT1) -y $(INPUT2) -o $(OUTPUT)

mv_bin: gen_data
	mkdir -p $(destDir)
	mkdir -p $(LASTRUN)
	cp $(INPUT1) $(INPUT2) $(OUTPUT) $(LASTRUN)
	mv $(EXEC_INP_GEN) $(EXEC) $(INPUT1) $(INPUT2) $(OUTPUT) $(destDir)
	@echo "All Done, executables should be in the $(destDir)"

