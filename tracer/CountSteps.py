# Copyright [2015] Ruud Cools

def countSteps(i_fileName):
    steps = {}
    prev = ""
    current = 0
    with open(i_fileName) as openFile:
        for line in openFile:
          pieces = line.split(', ')
          if pieces[0] not in steps:
              steps[pieces[0]] = int(pieces[1])
          else:
              steps[pieces[0]] += int(pieces[1])
          if prev == pieces[0]:
              current += int(pieces[1])
          else:
              if current > 5:
                  print prev + ": " + str(current)
              prev = pieces[0]
              current = 0

          if int(pieces[1]) == 0:
            raise ValueError("Thingy with 0 reps")
    print steps


def countSwitch(i_fileName):
    switches = [["IN","OUT"], ["CCW","CW"]]
    state = []
    conversions = {"IN_OUT" : 0, "CCW_CW" : 0}
    hasSwitch = 0
    with open(i_fileName) as openFile:
        for line in openFile:
          pieces = line.split(', ')
          if hasSwitch > 0:
              print line
              hasSwitch -=1
          if pieces[0] not in state:
              for switch in switches:
                  print line
                  hasSwitch = 10
                  if pieces[0] in switch:
                      # add a conversion to the thingy
                      for conversionType in conversions:
                          if pieces[0] in conversionType:
                              conversions[conversionType] += 1
                      # deduce the old state
                      oldState = switch[switch != pieces[0]]
                      print "New state: " + pieces[0]+ " So the old state should be: " + oldState
                      if oldState in state:
                          state.remove(oldState)
                      state.append(pieces[0])

    print conversions

def hasMoreThen3000Steps(i_file):
    with open(i_fileName) as openFile:
        for line in openFile:
          pieces = line.split(', ')




if __name__ == "__main__" :
    import os
    import sys
    if len(sys.argv)==1:
        fileName=r"/home/ruud/Projects/GeRoArm/motor/stepLog.log"
        print fileName
    else:
        fileName=sys.argv[1]
        print fileName
    countSteps(fileName)
#    countSwitch(fileName)
