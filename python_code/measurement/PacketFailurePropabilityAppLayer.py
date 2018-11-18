import matplotlib.pyplot as plt


list = []
list2 = []

#Read in DAGroot command responds 
file = open("measurementNicoEnhancedDAGroot.txt", "r")
for line in file:
    str=line.rstrip()
    str=str.strip()
    list.append(str)
file.close

#Read in Mote command responds
file = open("measurementNicoEnhancedMote.txt", "r")
for line in file:
    str=line.rstrip()
    str=str.strip()
    list2.append(str)
file.close

received = len(list)
transmitted = len(list2)
SuccessTX = 0
packetTX = 0

circBuffer = ["","","","",""]
bufIndex = 0

for x in range(len(list2)):
    inBuffer = False
    seq = list2[x][0:2]
    for y in circBuffer:
        if seq == y:
            inBuffer = True
    if not inBuffer
        if bufIndex > 4:
            bufIndex = 0
        circBuffer[bufIndex] = seq
        bufIndex += 1
        packetTX +=1
        helpListTX =[]
        for y in range(x+1,x+5):
            if list2[y][0:2] == seq:
                helpListTX.append(list2[y])
        for y in helpListTX:
            for z in list:
                if y[14:24] == z[20:30]:
                    if y[0:2] == z[30:32]:
                        successTX+=1
    inBuffer = False

print "Packets transmitted try: ",packetTX," Packets successful transmitted: ",SuccessTX," Packets received: ",received