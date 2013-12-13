infile = open("busfri.u8").read()
newline = infile.split('\n')
num = len(newline)

newlist = []
ppl = []
time = []

for i in range(num-1):
        temp = newline[i].split(',')
        newlist.append(temp[2])
        ppl.append(temp[1])
        time.append(temp[8])

newhour = []
from datetime import datetime
for tt in time:
        hh = datetime.strptime(tt, '%Y-%m-%d %H:%M:%S')
        newhour.append(hh)

newdict = {}
for d in newlist:
        newdict[d] = newdict.get(d,0) +1

buskey = newdict.keys()

def list_duplicates_of(seq,item):
        start_at = -1
        locs = {}
        while True:
                try:
                        loc = seq.index(item , start_at+1)
                except ValueError:
                        break
                else:
                        locs[ppl[loc]] = newhour[loc]
                        start_at = loc
        return locs

tem = {}
for bus in buskey:
        wei = list_duplicates_of(newlist, bus)
        tem[bus] = wei

tkey=[]
tval=[]
tmpkey=[]
tmpval=[]
pplonbus = {}
for kk in tem.values():
        tkey = kk.keys()
        tval = kk.values()
        for nn in range(0,len(tkey)):
                tmpkey = tkey[nn:] + tkey[:nn]
                tmpval = tval[nn:] + tval[:nn]
                for pplc in range(1,len(tmpkey)):
                        diff = tmpval[pplc]-tmpval[0]
                        if (diff.days == 0 and diff.seconds <= 1800) or (diff.days == -1 and diff.seconds >= 84600):
                                group = pplonbus.setdefault(tmpkey[0],[])
				group.append(tmpkey[pplc])

merged = {}
for yy,zz in pplonbus.items():
        merged.setdefault(yy,[]).extend(zz)

for wakao in merged:
        merged[wakao]=list(set(merged[wakao]))

with open("bigbusfri.txt","w") as f:
	for yy,zz in merged.items():
                f.write(yy+":"+str(zz)+'\n')

