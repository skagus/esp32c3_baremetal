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

def pre_proc(fname):
    print("pre_proc")
    lines = open(fname+'.ins','r').read().split('\n')
    aIns = []
    aIns2Id = {}
    aId2Ins = {}
    nInsId = 0x100
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
        
        nKey = 0xFFFF
        if sline[1] in aIns2Id:
            nKey = aIns2Id[sline[1]]
        else:
            nKey = nInsId
            aIns2Id[sline[1]] = nInsId
            aId2Ins[nKey] = sline[1]
            nInsId = nInsId + 1
        aIns.append((nKey, p0, p1, p2, sline[0]))
#    print(aIns)
    return aIns, aId2Ins

def stat_freq(fname):
    print("Frequency analysis")
    aIns, aTr = pre_proc(fname)

    isa_count = {}   # dictionary.
    for line in aIns:
        inst = line[0]
        if inst in isa_count :
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


# 연속된 두개의 instruction을 한개로 merge해 보려는 시도.
def stat_merge(fname):
    print("Analysis2")
    aIns, aTr = pre_proc(fname)
    nCntLine = len(aIns)

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

def _pat_name(aIns, aNames):
    return '_'.join(list(map(lambda x: aNames[x], aIns)))
     
# Command pattern찾기.(paramter는 제외)
def stat_cmd_pat(fname, pat_len):
    aIns, aTr = pre_proc(fname)
    nCntLine = len(aIns)
    
    aCmdList = []
    aDone = []
    aAccDone = {}
    for nIdx in range(nCntLine):
        aCmdList.append(aIns[nIdx][0])

    for nPtn in range(nCntLine - pat_len):
        if aCmdList[nPtn:nPtn+pat_len] in aDone :
            continue
#        szPatName = _pat_name(aCmdList[nPtn:nPtn+pat_len], aTr)
        szPatName = '_'.join(list(map(lambda x: aTr[x], aCmdList[nPtn:nPtn+pat_len])))
        aDone.append(aCmdList[nPtn:nPtn+pat_len])

        nCnt = 1
        for nLine in range(nPtn + pat_len, nCntLine - pat_len):
            if aCmdList[nPtn:nPtn+pat_len] == aCmdList[nLine:nLine+pat_len]:
#                print("Match: ", nPtn, nLine, aCmdList[nPtn:nPtn+pat_len], 
#                    list(map(lambda x: aTr[x], aCmdList[nPtn:nPtn+pat_len])))
                nCnt = nCnt + 1
        aAccDone[szPatName] = nCnt
        print(szPatName, '\t', nCnt)

#    print(aAccDone)

extract(file_name)
#stat_freq(file_name)
#stat_merge(file_name)
stat_cmd_pat(file_name, 3)


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