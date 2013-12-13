infile = open("bigfribus.txt").read()
newline = infile.split('\n')
num = len(newline)

punc = (",./;'[]&-")
with open("test.adjlist","w") as f:
        for i in range(num-1):
                strp = newline[i].translate(None,punc).replace(':',' ')
                f.writelines(strp +'\n')

