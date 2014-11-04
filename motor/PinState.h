#ifndef PinState_H
#define PinState_H

class PinState
{
  private:

    GETSET(PinVector,m_pinVector,PinVector);
    GETSET(int,m_numericValue,NumericValue);       
    
		int getPinValue(const int&); //represents the integer value of a pin, where the pin number is the position of the bit
    int getNumericPinValue(const int&)const;
    
  public:

    void setPins(const PinVector&);
    void setPinsToDefault();

    void update(const int&,const int&);
    void update(const PinStateMap&);
    void update(const PinState&);

		int getPinState(const int&)const; //get the set of a specific pin

    //    const int getNumericValue() const;

    void resetPinState();
    
    virtual void displayPinState()const;

    //constructors
    PinState();

    PinState(const PinVector&); //vector of pin numbers which will be set to default value

};

#endif // PinState
