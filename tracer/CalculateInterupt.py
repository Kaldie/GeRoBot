def calculateInterupt(i_dekaHertz):
    #i_dekaHertz is 10* input hertz
    prescaler = [0, 1, 8, 64, 256, 1024]
    i = 0;
    registerValue = 0L
    while True :
        i+=1
        registerValue = (1600000L / (prescaler[i] * i_dekaHertz)) -1L;
        if registerValue < 65536 or i > 5:
            print i
            break

    if (registerValue > 65536):
        registerValue = 65536;

    TCCR1B = 0
    CS10 = 0
    CS11 = 1
    CS12 = 2
    if (1 & i):
        print "CS10"
        TCCR1B |= 1 << CS10;
    if (2 & i):
        print "CS11"
        TCCR1B |= 1 << CS11;
    if (4 & i):
        print "CS12"
        TCCR1B |= 1 << CS12;
    print TCCR1B
    print registerValue

if __name__ == "__main__" :
    import os
    import sys
    if len(sys.argv)==1:
        fileName=r"/home/ruud/Projects/GeRoArm/motor/stepLog.log"
        decaHerts = 0
    else:
        decaHerts =sys.argv[1]
    calculateInterupt(float(decaHerts))
