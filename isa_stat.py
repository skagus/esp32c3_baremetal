import re


pat = '([0-9a-f]+):\t([0-9a-f]+)[ \t]+([a-z]+)'
repat = re.compile(pat, re.IGNORECASE)

file_name = "out/baremetal.lst"
f = open(file_name, 'r')

isa_count = {}   # dictionary.

while 1:
    line = f.readline()
    if not line:    # Check line is.
        break

    mt = repat.match(line)
    if (mt == None):    # Check the pattern isn't meet.
#        print("NMAT:", line)
        continue
    
    all = mt.groups()
    addr = all[0]
    binary = all[1]
    inst = all[2]

    if inst in isa_count :
        isa_count[inst] = isa_count[inst] + 1
    else:
        isa_count[inst] = 1

stat_list =[]
for inst, count in isa_count.items() :
    #print("inst: ", inst, " count: ", count)
    stat_list.append((inst, count))

#print(stat_list)

freq_sorted = sorted(stat_list, key=lambda aa:aa[1], reverse=True);

print(freq_sorted)
