import numpy
import copy

PRINT_MSG=False#True
PRINT_TO_FILE=False#True

def DBG_MSG(i_message):
        if PRINT_MSG:
                print i_message

def FILE_MSG(i_message):
        fileName=r'/home/ruud/Projects/GeRoArm/tracer/stepLog.log'
        if PRINT_TO_FILE:
                with open(fileName,'a') as debugLogFile:
                        debugLogFile.write(str(i_message) +'\n')

class Trace (object) :
    """
    tracer is an helper class
    which traces the motion of the robot
    and converts steps and delays in x, y positions

    parameters:
    minimalArmLength
    maximalArmLength
    rotationPerStep
    extensionPerStep
    """

    MOTOR_TYPES = ["MOTOR_TYPE_ROTATIONAL", "MOTOR_TYPE_TRANSLATIONAL"]
    DEBUG = True
    TIMEBETWEENSTEPS = 0.0


    def __init__(self):
       "Robot settings"
       self.__rotStep = 0.001654411764706 # rotation per step
       self.__transStep = 0.005 #translation in mm

       self.__minArm = 50.0# length of the minimum arm
       self.__maxArm = 500.0#length of the arm in mm

       self.__motorTypeList = ["MOTOR_TYPE_ROTATIONAL", "MOTOR_TYPE_TRANSLATIONAL"]

       """
       #motor delay list look like [
                                    [[delay],[direction]],
                                    [[delay],[direction]]
                                   ]
       """

       "Position settings"
       #position of the head in x,y viewed from back of robot, where robot is (0,0)
       self.__currentPosition = numpy.array([0.00 ,255.00])

       #rotation in degree where 0 is perpendicular to the work piece minus angle ccw and positive cw
       self.__currentRotation = numpy.arctan2(self.__currentPosition[1],
                                              self.__currentPosition[0])*180/numpy.pi

       self.__currentExtension = (self.__currentPosition[0] * self.__currentPosition[0] +
                                 self.__currentPosition[1] * self.__currentPosition[1])**0.5

       self.__trace = []
       self.__newTrace = []
       self.__addPositionToTrace()

    def __str__(self):
      nameString = "Robot with 2 motors: {motorTypeList}\n\tAt position x:{x:0.2f} mm,\t y{y:0.2f} mm\n\tRotated {rotation:0.2f}".\
            format(motorTypeList = ", ".join(self.__motorTypeList),
            x = self.__currentPosition[0],
            y = self.__currentPosition[1],
            rotation = self.__currentRotation)
      nameString+="\nNumber of joints: %i" % len(self.__motorTypeList)
      for i in range(len(self.__motorTypeList)):
        nameString+="\n\tJoint %i, a %s" %(i,self.__motorTypeList[i])
      return nameString


    @property
    def minimalArmLength (self):
        return self.__minArm


    @minimalArmLength.setter
    def minimalArmLength (self, i_value):
        self.__minArm = i_value


    @property
    def maximalArmLength (self):
        return self.__maxArm


    @maximalArmLength.setter
    def minimalArmLength (self, i_value):
        self.__maxArm = i_value

    @property
    def rotationPerStep(self):
        return self.__rotStep


    @rotationPerStep.setter
    def rotationPerStep (self, i_value):
        self.__rotstep = i_value

    @property
    def motorType1(self):
      return self.__motorTypeList[0]

    @motorType1.setter
    def motorType1(self, i_value):
        self.__setMotorType(0, i_value)

    @property
    def motorType2(self):
      return self.__motorTypeList[1]

    @motorType2.setter
    def motorType2(self, i_value):
      self.__setMotorType(1, i_value)

    def __setMotorType(self, i_motorNumber, i_value):
      if i_value not in self.MOTOR_TYPES:
        raise ValueError("Input should be equal to one of: %s, %s" % (self.MOTOR_TYPES[:]))

      self.__motorTypeList[i_motorNumber] = i_value
      

    def getMotorFromType(self,i_type):
        if i_type not in self.MOTOR_TYPES:
            raise ValueError("Input should be equal to one of: %s, %s" % (self.MOTOR_TYPES[:]))
        
        motorIndex=self.__motorTypeList.index(i_type)
        return motorIndex
      

    def createTrace(self,i_fileName):
	with open(i_fileName,'r') as stepFile:
            DBG_MSG('lala')
            for line in stepFile.readlines():
                direction, number, newX,newY = line.strip().split(',')
                DBG_MSG( "setting {0}, step(s) in {1} direction".format(number,direction))
                self.setSteps(direction,int(number))
                self.__addCalculatedPosition([float(newX),float(newY)])


    def setSteps(self,i_direction,i_numberOfSteps):
        if i_numberOfSteps > 5:
          print i_numberOfSteps
	rotationDirections=["CCW","CW"]
	translationDirections=["IN","OUT"]
	DBG_MSG("Current position: %0.5f, %0.5f." % (self.__currentPosition[0], self.__currentPosition[1]))
	if i_direction.upper() in rotationDirections:
          self.__doRotationSteps(i_direction,i_numberOfSteps)
	elif i_direction.upper() in translationDirections:
            self.__doTranslationSteps(i_direction,i_numberOfSteps)
        else:
            raise ValueError("Unknown direction")

    def __doTranslationSteps(self,i_direction,i_numberOfSteps):
      if i_direction=="IN":
          translationPolarity=-1.0
      elif i_direction=="OUT":
          translationPolarity=1.0
      else:
          raise ValueError("Direction should be in 'IN' or 'OUT'")

      DBG_MSG( "Starting translation")
      DBG_MSG (self.__currentRotation)
      unitTranslation = numpy.array([numpy.cos(self.__currentRotation * (numpy.pi / 180.)), 
                                     numpy.sin(self.__currentRotation * (numpy.pi / 180.))])
      DBG_MSG("Current extension: " + str(self.__currentExtension))
      DBG_MSG("Current rotation: " + str(self.__currentRotation))

      self.__currentExtension += self.__transStep * translationPolarity * i_numberOfSteps
      
      DBG_MSG("New extension: " + str(self.__currentExtension))
      
      self.__currentPosition = unitTranslation * self.__currentExtension

      DBG_MSG("moved robot x: %0.2f and y: %0.2f." % (self.__currentPosition[0], self.__currentPosition[1]))
      self.__addPositionToTrace()

    def __doRotationSteps(self, i_direction,i_numberOfSteps):
      DBG_MSG ("starting rotation")
      if i_direction=="CW":
        rotationPolarity=-1.0
      elif i_direction=="CCW":
        rotationPolarity= 1.0
          
      self.__currentRotation += self.__rotStep * rotationPolarity * i_numberOfSteps
      
      DBG_MSG ("Rotated the robot to %0.3f" % self.__currentRotation)

      currentX=numpy.copy(self.__currentPosition[0])
      currentY=numpy.copy(self.__currentPosition[1])

      rotation = self.__rotStep * (numpy.pi / 180.) * rotationPolarity * i_numberOfSteps
      
      self.__currentPosition[0] = numpy.cos(rotation) * currentX - numpy.sin(rotation) * currentY
      self.__currentPosition[1] = numpy.sin(rotation) * currentX + numpy.cos(rotation) * currentY

      self.__addPositionToTrace()

      DBG_MSG( "Moved Robot to: x: {x:0.2f}, y: {y:0.2f}.".format(x = self.__currentPosition[0],
                                                                  y = self.__currentPosition[1]))


    def __addPositionToTrace(self):
      DBG_MSG( "Current Position Robot: x: {x:0.2f}, y: {y:0.2f}.".\
          format(x = self.__currentPosition[0], y = self.__currentPosition[1]))
      self.__trace.append(numpy.copy(self.__currentPosition))


    def __addCalculatedPosition(self, i_position):
            self.__newTrace.append(numpy.copy(i_position))


    def plotTrace(self):
        from matplotlib.pyplot import plot,show,figure,Circle
	myFigure=figure()
	myAxes=myFigure.add_subplot(111)
        circle1=Circle((-0, 580),325.,color='r')
	myAxes.plot(zip(*self.__trace)[0], zip(*self.__trace)[1], 'g.-')
        myAxes.plot(zip(*self.__newTrace)[0], zip(*self.__newTrace)[1], 'b.-')

	myAxes.plot([0,-44.1369],[0,15.8587])
	myAxes.plot([0,-15.8587],[0,44.1369])
        #myAxes.set_ylim([-100,100])
        #myAxes.set_xlim([-100,100])
        myAxes.add_artist(circle1)
        show()


    def plotFrequency(self):
        from matplotlib.pyplot import plot,show,figure
        values,translation,rotation=self.getFrequencies()
        plot(values,rotation)
        plot(values,translation)
        show()


    def getFrequencies(self):
	step=10**6
	begin=0
	end=0
        translationEntries=[]
        rotationEntries=[]
        values=[]
	currentStartPoint=1*10**6
        DBG_MSG("get cummalitive lists")
	translationArray=self.__convertToCummulative(self.__motorDelayLists[0][0])
        DBG_MSG( "got the translation list")
	rotationArray=self.__convertToCummulative(self.__motorDelayLists[1][0])
        DBG_MSG( "got the rotation list")
        maxValue=max([max(rotationArray),max(translationArray)])
	while end<maxValue:
	  end=begin+step
	  translationEntries.append(self.__countValuesInRange(translationArray,
                                                              begin,
                                                              end))

          rotationEntries.append(self.__countValuesInRange(rotationArray,
                                                          begin,
                                                          end))
          values.append(begin/10**6)

          begin+=step

        return values,translationEntries,rotationEntries


    def __convertToCummulative(self,i_array):
	array=[sum(i_array[i:]) for i in range(len(i_array))]
	return array


    def __countValuesInRange(self,i_array,i_start,i_end):
      DBG_MSG ("doing : %i" % i_start)
      array=numpy.array(i_array)
      counts=0
      for value in array:
          if value>=i_start and value<=i_end:
              counts+=1
      return counts

if __name__=="__main__":
    import os
    import sys

    trace=Trace()
    if len(sys.argv)==1:
	fileName=r"/home/ruud/Projects/GeRoArm/motor/stepLog.log"
        print fileName
    else:
	fileName=sys.argv[1]
        print fileName

    trace.createTrace(fileName)
    print trace
#    trace.plotFrequency()
    trace.plotTrace()

