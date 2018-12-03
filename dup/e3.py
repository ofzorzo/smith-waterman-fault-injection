gap_value = -2
match_value = 1
mismatch_value = -1

def calcFinalScore(alignmentA, alignmentB):
	finalScore = 0
	for i in range(0, len(alignmentA)):
		if alignmentA[i] == alignmentB[i]:
			finalScore += match_value
		elif alignmentA[i] == '-' or alignmentB[i] == '-':
			finalScore += gap_value
		else:
			finalScore += mismatch_value
	return finalScore

def matchMisScore(i,j, seq, seq2):
	if seq[j] == seq2[i]:
		return match_value
	else:
		return mismatch_value

def isMatch(i, j, seq, seq2):
	if seq[j-1] == seq2[i-1]:
		return True
	else:
		return False

def initialize(seq, seq2):

	width = len(seq)+1
	height = len(seq2)+1
	
	#creates a list containing height lists, each of width items
	Matrix = [[0 for x in range(width)] for y in range(height)]
	
	# for i in range(0, height):		#linha a linha
		# print("\n", end='')
		# for j in range(0,width):	#coluna a coluna
			# print(Matrix[i][j], end='')
	

	for i in range(1, height):		#line by line
		for j in range(1,width):	#column by column
			if isMatch(i, j, seq, seq2):
				Matrix[i][j] = max(0, Matrix[i-1][j-1]+match_value, Matrix[i][j-1]+gap_value, Matrix[i-1][j]+gap_value)
			else:
				Matrix[i][j] = max(0, Matrix[i-1][j-1]+mismatch_value, Matrix[i][j-1]+gap_value, Matrix[i-1][j]+gap_value)

	return width, height, Matrix
	
def align(width, height, Matrix, seq, seq2):

	alignmentA = ""
	alignmentB = ""
	maxValue = 0
	
	for i in range(1, height):		#line by line
		for j in range(1,width):	#column by column
			if i==1 and j==1:
				maxValue = Matrix[i][j]
				y, x = i, j
			if maxValue < Matrix[i][j]:
				maxValue = Matrix[i][j]
				y, x = i, j
	print(seq)
	print(seq2)
	while y > 0 and x > 0 and Matrix[y][x] != 0:
		score = Matrix[y][x]
		scoreDiag = Matrix[y-1][x-1]
		scoreUp = Matrix[y-1][x]
		scoreLeft = Matrix[y][x-1]
				
		if score == (scoreDiag+matchMisScore(y-1,x-1, seq, seq2)):
			alignmentA = seq[x-1] + alignmentA
			alignmentB = seq2[y-1] + alignmentB
			y -= 1
			x -= 1
			
		elif score == (scoreLeft+gap_value):
			alignmentA = seq[x-1] + alignmentA
			alignmentB = "-" + alignmentB
			x -= 1
			
		elif score == (scoreUp+gap_value):
			alignmentA = "-" + alignmentA
			alignmentB = seq2[y-1] + alignmentB
			y -= 1
					
	
	# while y > 0:
		# alignmentB = seq2[y-1] + alignmentB
		# alignmentA = "-" + alignmentA
		# y -= 1
	# while x > 0:
		# alignmentB = "-" + alignmentB
		# alignmentA = seq[x-1] + alignmentA
		# x -= 1
	#print(alignmentA, alignmentB)
	return alignmentA, alignmentB
	
def smithWaterman(seq, seq2):

	
	width, height, Matrix = initialize(seq, seq2)
	alignmentA, alignmentB = align(width, height, Matrix, seq, seq2)
	finalScore = calcFinalScore(alignmentA, alignmentB)
		
	sameChars = 0
	for i in range(0, len(alignmentB)):
		if alignmentA[i] == alignmentB[i]:
			sameChars += 1
	identity = (sameChars/len(alignmentB))*100
		
	return Matrix, identity, finalScore, alignmentA, alignmentB, height, width

def treatFile(file):
	fastaFile = open(file, "r")		
	fileString = fastaFile .read()
	table = str.maketrans(dict.fromkeys('\n'))
	numberOfSeq = 0
	newSeq = 0
	listOfSeq = []
	scoreList = [] # each element is a 8-tuple that stores the current sequence, the matrix, score and identity
				   # generated when the needleman-wunsch algorithm is applied on the first and current sequence
				   # in the file, the alignments generated and the height and width of the matrix
	flagEndOfHeader = 0
	sequences = ""
	for i in fileString:
		if i == '>':
			numberOfSeq += 1
	if numberOfSeq > 1:
		for i in range(0, len(fileString)):
			if fileString[i] == '>':
				newSeq += 1
			if newSeq != 2:
				sequences += fileString[i]
			else:
				newSeq = 1
				listOfSeq.append(sequences)
				sequences = ">"
			if i == len(fileString)-1:
				listOfSeq.append(sequences)
		#print(listOfSeq)
		#print(len(listOfSeq))
	#A PARTIR DAQUI COMPARAR TODAS AS STRINGS COM A PRIMEIRA, E ARMAZENAR TODOS OS RESULTADOS, ASSIM COMO QUAL STRING GEROU CADA RESULTADO, EM UMA LISTA
		fstSeq = ""
		fstHeader = ""
		for i in range(0,len(listOfSeq[0])):
			if listOfSeq[0][i] == '\n':
				flagEndOfHeader = 1
			if flagEndOfHeader == 1:
				fstSeq += listOfSeq[0][i]		
			if flagEndOfHeader == 0:
				fstHeader += listOfSeq[0][i]
		fstSeq = fstSeq.translate(table)
		for i in range(1, numberOfSeq):
			sndSeq = ""
			flagEndOfHeader = 0
			for j in range(0,len(listOfSeq[i])):
				if listOfSeq[i][j] == '\n':
					flagEndOfHeader = 1
				if flagEndOfHeader == 1:
					sndSeq += listOfSeq[i][j]
			sndSeq = sndSeq.translate(table)
			Matrix, identity, score, alignmentA, alignmentB, height, width = smithWaterman(fstSeq, sndSeq)
			scoreList.append([listOfSeq[i], Matrix, score, identity, alignmentA, alignmentB, height, width])
		#print(scoreList)
	else:
		print("Erro. Apenas uma sequencia no arquivo.")
		
	fastaFile.close()
	return scoreList, fstHeader	
	
func = 5
while func != '3':
	print("O que gostaria de fazer agora?\n1 - Analisar duas sequencias arbitrarias\n2 - Comparar a primeira sequencia de um arquivo com suas demais sequencias;\n3 - Sair.")
	func = input()
	if func == '1':
		seq = input("Digite a primeira sequencia:\n")
		seq2 = input("Digite a sequencia a ser comparada com a primeira:\n")
		Matrix, identity, finalScore, alignmentA, alignmentB, height, width = smithWaterman(seq, seq2)
		outpFile = open("twoArbitrarySequencesComparison.txt", "w")
		outpFile.write("Alinhamento local das sequencias:\n")
		outpFile.write("Sequencia A: ")
		outpFile.write(alignmentA)
		outpFile.write("\nSequencia B: ")
		outpFile.write(alignmentB)
		outpFile.write("\n\nScore:\n")
		outpFile.write(str(finalScore))
		outpFile.write("\nIdentidade:\n")
		outpFile.write(str(identity))
		outpFile.write("%")	
		outpFile.write("\n")	
		for i in range(0, height):		#linha a linha
			outpFile.write("\n")
			for j in range(0,width):	#coluna a coluna
				if Matrix[i][j]<0:
					outpFile.write("  ")
					outpFile.write(str(Matrix[i][j]).ljust(4))
					outpFile.write("  ")
				else:
					outpFile.write("   ")
					outpFile.write(str(Matrix[i][j]).ljust(4))
					outpFile.write(" ")
		print("Arquivo twoArbitrarySequencesComparison.txt criado com a resposta!\n")
		outpFile.close()
	elif func == '2':
		file = input("Digite o nome do arquivo a ser analisado, junto com extensao:\n")
		outpFile = open("sequencesInFileComparison.txt", "w")
		try:
			scoreList, fstHeader = treatFile(file)
			endOfHeader = 0
			sortingList = []
			outpFile.write("Header da sequencia inicial:\n")		
			outpFile.write(fstHeader)
			outpFile.write("\n\nHeader(s) e pontuação da(s) sequencia(s) em relacao a inicial:\n")
			for i in range(0,len(scoreList)):
				sortingList.append(scoreList[i][2])
			sortingList.sort(reverse=True)
			#print(sortingList)
			for i in range(0,len(scoreList)):
				sndHeader = ""
				endOfHeader = 0
				sortingList[0] = scoreList[i][2]
				if scoreList[i][2] == sortingList[0]:
					for char in scoreList[i][0]:
						if char == '\n':
							endOfHeader = 1
						if endOfHeader == 0:
							sndHeader += char
					outpFile.write("\n")
					outpFile.write(sndHeader)
					outpFile.write("\n")
					outpFile.write("Alinhamento local:\n")
					outpFile.write("Sequencia inicial: ")
					outpFile.write(scoreList[i][4])
					outpFile.write("\nSegunda sequencia: ")
					outpFile.write(scoreList[i][5])
					outpFile.write("\nScore:\n")
					outpFile.write(str(scoreList[i][2]))
					outpFile.write("\nIdentidade:\n")
					outpFile.write(str(scoreList[i][3]))
					outpFile.write("%")
					outpFile.write("\nMatriz gerada:")
					Matrix = scoreList[i][1]
					for ii in range(0, scoreList[i][6]):		#linha a linha
						outpFile.write("\n")
						for jj in range(0, scoreList[i][7]):	#coluna a coluna
							if Matrix[ii][jj]<0:
								outpFile.write("  ")
								outpFile.write(str(Matrix[ii][jj]).ljust(4))							
								outpFile.write("  ")
							else:
								outpFile.write("   ")
								outpFile.write(str(Matrix[ii][jj]).ljust(4))
								outpFile.write(" ")
					outpFile.write("\n")
			print("Arquivo sequencesInFileComparison.txt criado com a resposta!\n")
			outpFile.close()
		except:
			print("Arquivo nao encontrado na pasta onde o .py esta.\n")