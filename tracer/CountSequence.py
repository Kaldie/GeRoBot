
def countSequence(i_fileName):
    translationDirectionPin = 3
    translationPullPin = 4
    rotationDirectionPin = 6
    rotationPullPin = 7
    stateTranslationPin = True
    stateRotationPin = True
    steps = {"TH":0, "TL":0, "RH":0, "RL":0}
    i = 0
    zeroReps = 0
    with open(i_fileName) as openFile:
        for line in openFile:
            i += 1
            splits = line.split(" ; ")
            reps = splits[0].split(' , ')[0]
#            print line[:-1]
 #           print steps
            for state in splits[1:-1]:
                if stateTranslationPin == 0:
      #              print "checking if it up again!"
                    if (int(state.split(' , ')[0]) >> translationPullPin) % 2:
     #                   print "Yay it is up agian!"
                        if (int(state.split(' , ')[0]) >> translationDirectionPin) % 2:
    #                        print "set steps: " + str(1 * int(reps))
                            steps["TH"] += int(reps)
                        else:
   #                         print "set steps: " + str(1 * int(reps))
                            steps["TL"] += int(reps)
                stateTranslationPin = (int(state.split(' , ')[0]) >> translationPullPin) % 2
                if stateRotationPin == 0:
                    if (int(state.split(' , ')[0]) >> rotationPullPin) % 2:
     #                   print "Yay it is up agian!"
                        if (int(state.split(' , ')[0]) >> rotationDirectionPin) % 2:
    #                        print "set steps: " + str(1 * int(reps))
                            steps["RH"] += int(reps)
                        else:
   #                         print "set steps: " + str(1 * int(reps))
                            steps["RL"] += int(reps)
                stateRotationPin = (int(state.split(' , ')[0]) >> rotationPullPin) % 2
    print steps
    return

if __name__ == "__main__":
    import os
    import sys
    if len(sys.argv)==1:
        fileName=r"/home/ruud/Projects/GeRoArm/motor/stepLog.log"
        print fileName
    else:
        fileName=sys.argv[1]
        print fileName
    countSequence(fileName)
#    countSwitch(fileName)
