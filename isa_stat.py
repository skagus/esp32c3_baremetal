import re

i_pat = '([0-9a-f]+):\t([0-9a-f]+)[ \t]+([a-z]+)(.*)'
isa_pat = re.compile(i_pat, re.IGNORECASE)
p_pat = '\t([0-9a-z,\-\(\)]+)'
param_pat = re.compile(p_pat, re.IGNORECASE)

file_name = "out/baremetal"

def extract(fname):
    fpIn = open(fname+'.lst', 'r')
    fpOut = open(fname+'.ins', 'w')
    while 1:
        line = fpIn.readline()
        if not line:    # Check line is.
            break

        mt = isa_pat.match(line)
        if (mt == None):    # Check the pattern isn't meet.
#           print("NMAT:", line)
            continue
    
        all = mt.groups()
        addr = all[0]
        binary = all[1]
        inst = all[2]
#        print(addr, inst, all[3])
        param = ''
        p_mat = param_pat.match(all[3])
        if (p_mat != None):    # Check the pattern isn't meet.
            p_all = p_mat.groups()
            param = p_all[0].replace(",", "\t")
#            print(param)
       
#        print(line, "\t\t\t\t\t", inst + '\t' + param)

        fpOut.write(addr + '\t' + inst + '\t' + param + '\n')
    fpOut.close()
    fpIn.close()


def stat(fname):
    fpIn = open(fname+'.ins', 'r')
    isa_count = {}   # dictionary.
    while 1:
        line = fpIn.readline()
        if not line:    # Check line is.
            break
        ins_line = line.split('\t')
#        print(ins_line)
        inst = ins_line[1]
        if ins_line[1] in isa_count :
            isa_count[inst] = isa_count[inst] + 1
        else:
            isa_count[inst] = 1

    stat_list =[]
    for inst, count in isa_count.items() :
        stat_list.append((inst, count))
#        print("inst: ", inst, " count: ", count)

#   print(stat_list)
    freq_sorted = sorted(stat_list, key=lambda aa:aa[1], reverse=True);
    print(freq_sorted)
    fpIn.close()


# 연속된 두개의 instruction을 한개로 merge해 보려는 시도.
def analysis2(fname):
    print("Analysis2")
    lines = open(fname+'.ins','r').read().split('\n')
    aIns = []
    nCntLine = 0
    for line in lines:
        if not line:    # Check line is.
            break
        sline = line.split('\t')
#        print(sline)
#        ins = sline[1]
        p0 = ''
        p1 = ''
        p2 = ''
        if len(sline) > 2:
            p0 = sline[2]
        if len(sline) > 3:
            p1 = sline[3]
        if len(sline) > 4:
            p2 = sline[4]
        aIns.append((sline[1], p0, p1, p2, sline[0]))
        nCntLine = nCntLine + 1
    print(aIns)
# Command is Ready.

## Find 1st Command's DST and 2nd Command's SRC.
    for nBase in range(nCntLine - 1):
#        if aIns[nBase][1] == '' or aIns[nBase+1][1] == '':
#            continue
        ins0 = aIns[nBase][0]
        ins1 = aIns[nBase+1][0]
        param = aIns[nBase][1]
        if param != aIns[nBase + 1][1]:
            continue
        for n in range(nBase + 2, nCntLine - 1):
            if (aIns[n][0] == ins0 
                and aIns[n+1][0] == ins1
                and aIns[n][1] == aIns[n+1][2]) :
                print("Match: ", aIns[nBase], aIns[nBase+1], aIns[n], aIns[n+1])

    return


extract(file_name)
stat(file_name)
analysis2(file_name)


'''
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
'''