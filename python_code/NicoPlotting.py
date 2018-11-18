import matplotlib.pyplot as plt
global str
file = open("measurementNicoChannel.txt", "r")
for line in file:
    str=line.rstrip()
channels=bytearray(50)
str=str.strip()
a=0
for x in range(0,50):
    y="0x"+str[a:a+2]
    channels[x]=int(y,16)
    a=a+2
b=0
c=0
for x in range(len(channels)):
    if channels[x]== 1:
        b=b+1
    else:
        c=c+1
print "Res Slot 1:",b,", Res Slot 2:",c
plt.plot(channels)
plt.xlabel('Channels')
plt.show()

#Jetzt noch die Zeiten
file = open("measurementNicoTimes.txt", "r")
for line in file:
    str=line.rstrip()
channels=bytearray(50)
ausgabe=([0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00])
str=str.strip()
a=0
for x in range(0,50):
    y="0x"+str[a:a+2]
    channels[x]=int(y,16)
    ausgabe[x]= channels[x]/33
    print ausgabe[x]
    a=a+2

plt.plot(ausgabe)
plt.ylabel('ms')
plt.show()
