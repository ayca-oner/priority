import matplotlib.pyplot as plt

global str
list = []
list2 = []

file = open("measurementNicoEnhancedDAGroot.txt", "r")
for line in file:
    str=line.rstrip()
    str=str.strip()
    list.append(str)
file.close

file = open("measurementNicoEnhancedMote.txt", "r")
for line in file:
    str=line.rstrip()
    str=str.strip()
    list2.append(str)
file.close

#If sequence number of the mote's transmitted and the DAGroot received packet is equal, we can copy the transmitted ASN
#for x in range(len(list)):
#    for y in list2[x:]:
#        z = list[x]
#        if z[30:32] == y[0:2]:
#            z +=y[14:24]
#            list3.append(z)
#            break

list4 = []

for x in list:
    #get new sequence number
    seq = x[30:32] 
    #check if we already have it
    exist = 0
    list5 = []
    list6 = []
    list3 = []

    for y in list4:
        if seq == y:
            exist = 1
    if exist == 0:
        list4.append(seq)
     
        #add all from DAGroot
        for z in list:
            if seq == z[30:32]:
                list5.append(z)
        #now iterate through mote list and add every same sequence number
        for z in list2:
            if seq == z[0:2]:
                list3.append(z)
        #now print the result
        print "--------------------------------------------------"
        print "Sequence Number: ", seq
        print "Sent:"
        for z in list3:
            print "ASN: ",z[14:24]
        print "Received:"
        for z in list5:
            print "Channel: ",z[0:2],"Offset: ", z[2:4],"Source: ",z[4:20],"ASN: ",z[20:30]
                      

transmitted = 0
initSlot = 0
res1 = 0
res2 = 0

for x in list:
    transmitted+=1
    if x[2:4]=="04":
        initSlot+=1
    if x[2:4]=="06":
        res1+=1
    if x[2:4]=="08":
        res2+=1


print "Total received: ",transmitted," in MAC INIT:",initSlot," in RESOLUTION1: ",res1, "in RESOLUTION2: ",res2

#Now diagram plotting sent and received in channels
transmitted = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
received = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
legend =[10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,25]

for x in list:
    if x[0:2] == "0a":
        received[0] = received[0] +1
    elif x[0:2] == "0b":
        received[1] = received[1] +1
    elif x[0:2] == "0c":
        received[2] = received[2] +1
    elif x[0:2] == "0d":
        received[3] = received[3] +1
    elif x[0:2] == "0e":
        received[4] = received[4] +1
    elif x[0:2] == "0f":
        received[5] = received[5] +1
    elif x[0:2] == "10":
        received[6] = received[6] +1
    elif x[0:2] == "11":
        received[7] = received[7] +1
    elif x[0:2] == "12":
        received[8] = received[8] +1
    elif x[0:2] == "13":
        received[9] = received[9] +1
    elif x[0:2] == "14":
        received[10] = received[10] +1
    elif x[0:2] == "15":
        received[11] = received[11] +1
    elif x[0:2] == "16":
        received[12] = received[12] +1
    elif x[0:2] == "17":
        received[13] = received[13] +1
    elif x[0:2] == "18":
        received[14] = received[14] +1
    elif x[0:2] == "19":
        received[15] = received[15] +1
    elif x[0:2] == "1a":
        received[16] = received[16] +1

for x in list2:
    if x[24:26] == "0a":
        transmitted[0] = transmitted[0] +1
    elif x[24:26] == "0b":
        transmitted[1] = transmitted[1] +1
    elif x[24:26] == "0c":
        transmitted[2] = transmitted[2] +1
    elif x[24:26] == "0d":
        transmitted[3] = transmitted[3] +1
    elif x[24:26] == "0e":
        transmitted[4] = transmitted[4] +1
    elif x[24:26] == "0f":
        transmitted[5] = transmitted[5] +1
    elif x[24:26] == "10":
        transmitted[6] = transmitted[6] +1
    elif x[24:26] == "11":
        transmitted[7] = transmitted[7] +1
    elif x[24:26] == "12":
        transmitted[8] = transmitted[8] +1
    elif x[24:26] == "13":
        transmitted[9] = transmitted[9] +1
    elif x[24:26] == "14":
        transmitted[10] = transmitted[10] +1
    elif x[24:26] == "15":
        transmitted[11] = transmitted[11] +1
    elif x[24:26] == "16":
        transmitted[12] = transmitted[12] +1
    elif x[24:26] == "17":
        transmitted[13] = transmitted[13] +1
    elif x[24:26] == "18":
        transmitted[14] = transmitted[14] +1
    elif x[24:26] == "19":
        transmitted[15] = transmitted[15] +1
    elif x[24:26] == "1a":
        transmitted[16] = transmitted[16] +1


plt.plot(legend, received, 'b')
plt.plot(legend, transmitted, 'r')
plt.show()


