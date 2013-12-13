infile = open("test.adjlist").read()
newline = infile.split('\n')
num = len(newline)

f = open("sna.lgl","w")
for i  in range(num-1):
	tmpline = newline[i].split()
	f.write("# " + tmpline.pop(0) + '\n')
	for tmp in tmpline:
		f.write(tmp + '\n')

f.close()

