// Copyright [2014] Ruud Cools
#ifndef GUI_TRACEDESIGNWIDGET_H_
#define GUI_TRACEDESIGNWIDGET_H_

#include <QWidget>
#include <macroHeader.h>
#include <Trace.h>
#include "./ui_TraceDesignWidget.h"

class Point2DWidget;
class TraceGraphView;
class TraceInfoWidget;
class Robot;
class RobotLocator;
class SequentialTraceCalculator;


class TraceDesignWidget: public QWidget, private Ui::TraceDesignWidget {
  Q_OBJECT
  // Vector which hold the traces
  GETSET(Trace::TracePointerVector, m_vector, Vector);
  GETSET(std::shared_ptr<Robot>, m_robot, Robot);
  GETSETPOINTER(TraceInfoWidget, m_traceInfoWidget, TraceInfoWidget);
  GETSETPOINTER(TraceGraphView, m_traceGraphView, TraceGraphView);
  GETSET(std::shared_ptr<RobotLocator>, m_robotLocator, RobotLocator);
  GETSET(std::shared_ptr<QTimer>, m_timer, Timer);
  GETSET(std::shared_ptr<SequentialTraceCalculator>, m_calculator, Calculator);
  GETSET(bool, m_startAtPrevious, StartAtPrevious);

 public:
    /// Constructor
  explicit TraceDesignWidget(const std::shared_ptr<Robot>& i_robot = nullptr,
			     QWidget* parent = 0);
    /**
     * Clearing the widget of the old stuff and showing the new stuff
     */
    void resetWidget(const Trace::TracePointerVector& i_vector);
   /**
    * clear the widget if all its Traces
    */
   void clearWidget();
 
 public slots:
   void addTrace(Trace::TracePointer);
   void removeTrace(Trace::TracePointer);
   /**
    * Calcuate the trace as they are defined in the designer
    */
   void calculateTraces();
   /**
     * Replace the currently selected trace with a new trace of type i_type
     * @param[in] i_type Trace::TraceType the new trace type
     */
   void replaceTrace(Trace::TraceType);

   /**
     * Replace the trace with a new trace of type i_type
     * @param[in] i_pointer Trace::TracePointer the new trace type
     * @param[in] i_type Trace::TraceType the new trace type
     */
    void replaceTrace(Trace::TracePointer i_pointer,
		      Trace::TraceType i_type);

    /**
     * Given the a pointer, change the direction of the trace.
     */
    void reverse(Trace::TracePointer i_pointer);


    /**
     * Update the start at previous setting of this widget
     */
    void updateStartAtPrevious(const bool&);

    
    /**
     * Update the snap to trace mode for the rest
     */
    void updateSnapToTrace(const bool& i_snapToTrace);
    
 private:

    /**
    * Set up the gui and creat an example trace for show and debugging purposes
    */
   void initialise();

   /**
    * Get the index of the currently selected TraceGraphitem,
    * if none or multiple are found the size of the vector is returned
    */
   int getSelectedIndex() const;

   /**
    * Get the index of the of the given pointer
    * @param [in] i_pointer Trace::TracePointer which will be looked up in the vector
    */
   int getIndex(const Trace::TracePointer& i_pointer) const;

   /**
    * Replace the trace based on the index and type
    * This is the actual work horse, other methods call
    * this private method after determining the index
    * @param[in] i_index index of the trace that needs to be replace in the vector
    * use getSelectedIndex or getIndex(const Trace::TracePointer&) to get the index
    */
   void replaceTrace(const unsigned int& i_index,
         const Trace::TraceType& i_type);

   /**
    * Create a replacement based of the trace in the vector at i_index.
    * Set the start and stop position of the replacement to that of the previous.
    * If possible also do this for the centre point if necessary.
    * Otherwise use the suggestion from RotationTrace::suggestcentralpoint
    */
   void setupReplacementPointer(Trace::TracePointer i_replacement,
        const int& i_index);

 private slots:
   void setSelectedTrace();
   void addTraceFromButton();
   /**
   * update the robot locator
   */
   void updateRobotLocator();
};

#endif  // GUI_TRACEDESIGNWIDGET_H_
