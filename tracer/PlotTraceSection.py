
from matplotlib.pyplot import *

def plotPolygon(i_fileName):
    pointsX = []
    pointsY = []
    with open(i_fileName) as openFile:
        for line in openFile:
            pointsX.append(line.split(",")[0])
            pointsY.append(line.split(",")[1])
    plot(pointsX, pointsY)
    show()
    close()

if __name__ == "__main__" :
    import os
    import sys
    if len(sys.argv)==1:
        fileName=r"Polygon.pol"
        print fileName
    else:
        fileName=sys.argv[1]
        print fileName
    plotPolygon(fileName);
