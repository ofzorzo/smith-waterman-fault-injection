#!/usr/bin/env python3
'''
	Script para comparar os logs com o gold
'''
import os
import re

rootDir = 'logs'
outputLog = 'compareLog.log'
goldOutput = 'lastDataCreated/goldOutput'

outs = [] # lista com diretórios que tem um arquivo output com sdc

for dirName, subdirList, fileList in os.walk(rootDir):
	if re.search("sdcs", dirName) and not re.search("detected", dirName):
		for file in fileList:
			if file == 'output':
				outs.append(dirName +'/output' )

	
sdcCounter = 1		
gold = open(goldOutput)
goldLines = gold.readlines()
goldenVals = [float(x) for x in goldLines]
for out in outs:
	print("#SDC " + str(sdcCounter) + ' => ' + out)
	sdcOut = open(out)
	pos = 1
	for line in sdcOut:
		if(len(goldenVals)>=pos):
			if float(line) != goldenVals[pos-1]:
				print ("#ERR p: [" + str(pos) + "], r: " + str(float(line)) + ", e: "+ str(goldenVals[pos-1]))
			pos+=1
		elif pos == len(goldenVals)+1:
			print ("#ERR p: [" + str(pos) + "], r: " + str(float(line)) + ", e: "+ str(0.0))
			pos+=1
	sdcCounter+=1
	while pos < len(goldenVals):
		print ("#ERR p: [" + str(pos) + "], r: " + str(0.0) + ", e: "+ str(goldenVals[pos-1]))
		pos+=1
