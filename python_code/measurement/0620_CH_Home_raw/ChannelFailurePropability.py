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

#First is overall amount of transmission, second of receiving
channels = [[0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0]]

for x in list:
    if x[0:2] == "0a":
        channels[0][1] = channels[0][1] +1
    elif x[0:2] == "0b":
        channels[1][1] = channels[1][1] +1
    elif x[0:2] == "0c":
        channels[2][1] = channels[2][1] +1
    elif x[0:2] == "0d":
        channels[3][1] = channels[3][1] +1
    elif x[0:2] == "0e":
        channels[4][1] = channels[4][1] +1
    elif x[0:2] == "0f":
        channels[5][1] = channels[5][1] +1
    elif x[0:2] == "10":
        channels[6][1] = channels[6][1] +1
    elif x[0:2] == "11":
        channels[7][1] = channels[7][1] +1
    elif x[0:2] == "12":
        channels[8][1] = channels[8][1] +1
    elif x[0:2] == "13":
        channels[9][1] = channels[9][1] +1
    elif x[0:2] == "14":
        channels[10][1] = channels[10][1] +1
    elif x[0:2] == "15":
        channels[11][1] = channels[11][1] +1
    elif x[0:2] == "16":
        channels[12][1] = channels[12][1] +1
    elif x[0:2] == "17":
        channels[13][1] = channels[13][1] +1
    elif x[0:2] == "18":
        channels[14][1] = channels[14][1] +1
    elif x[0:2] == "19":
        channels[15][1] = channels[15][1] +1
    elif x[0:2] == "1a":
        channels[16][1] = channels[16][1] +1

for x in list2:
    if x[24:26] == "0a":
        channels[0][0] = channels[0][0] +1
    elif x[24:26] == "0b":
        channels[1][0] = channels[1][0] +1
    elif x[24:26] == "0c":
        channels[2][0] = channels[2][0] +1
    elif x[24:26] == "0d":
        channels[3][0] = channels[3][0] +1
    elif x[24:26] == "0e":
        channels[4][0] = channels[4][0] +1
    elif x[24:26] == "0f":
        channels[5][0] = channels[5][0] +1
    elif x[24:26] == "10":
        channels[6][0] = channels[6][0] +1
    elif x[24:26] == "11":
        channels[7][0] = channels[7][0] +1
    elif x[24:26] == "12":
        channels[8][0] = channels[8][0] +1
    elif x[24:26] == "13":
        channels[9][0] = channels[9][0] +1
    elif x[24:26] == "14":
        channels[10][0] = channels[10][0] +1
    elif x[24:26] == "15":
        channels[11][0] = channels[11][0] +1
    elif x[24:26] == "16":
        channels[12][0] = channels[12][0] +1
    elif x[24:26] == "17":
        channels[13][0] = channels[13][0] +1
    elif x[24:26] == "18":
        channels[14][0] = channels[14][0] +1
    elif x[24:26] == "19":
        channels[15][0] = channels[15][0] +1
    elif x[24:26] == "1a":
        channels[16][0] = channels[16][0] +1
failurePropability = []
for x in channels:
    if x[0] > 0:
        props = 1-x[1]/float(x[0])
    else:
        props = -1
    failurePropability.append(props)
print "------------------------------"
print " Channel failure propabilites "
print "------------------------------"
print "Channel 10: ",failurePropability[0]
print "Channel 11: ",failurePropability[1]
print "Channel 12: ",failurePropability[2]
print "Channel 13: ",failurePropability[3]
print "Channel 14: ",failurePropability[4]
print "Channel 15: ",failurePropability[5]
print "Channel 16: ",failurePropability[6]
print "Channel 17: ",failurePropability[7]
print "Channel 18: ",failurePropability[8]
print "Channel 19: ",failurePropability[9]
print "Channel 20: ",failurePropability[10]
print "Channel 21: ",failurePropability[11]
print "Channel 22: ",failurePropability[12]
print "Channel 23: ",failurePropability[13]
print "Channel 24: ",failurePropability[14]
print "Channel 25: ",failurePropability[15]
print "Channel 26: ",failurePropability[16]

