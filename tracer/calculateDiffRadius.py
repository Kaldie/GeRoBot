
import os,sys
fucked = []
centerPoint = (-0, 580)
radius = 580 - 255
with open(sys.argv[1],'r') as openedFile:
    for line in openedFile.readlines():
        pieces = line.split(', ')
        fucked.append(radius - (
            (centerPoint[0]- float(pieces[2]))*(centerPoint[0]- float(pieces[2])) +
            (centerPoint[1]- float(pieces[3]))*(centerPoint[1]- float(pieces[3])))**0.5)
from  matplotlib.pyplot import *
plot(fucked)
show()
