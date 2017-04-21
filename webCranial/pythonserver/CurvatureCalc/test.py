import sys

#print sys.argv[1]
with open(sys.argv[1], "r") as filestream:
	i = 0
	for line in filestream:
		if i < 5:
			print line
			i+=1