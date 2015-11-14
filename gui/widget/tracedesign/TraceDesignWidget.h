// Copyright [2014] Ruud Cools
#ifndef GUI_TRACEDESIGNWIDGET_H_
#define GUI_TRACEDESIGNWIDGET_H_

#include <QWidget>
#include <macroHeader.h>
#include <Trace.h>
#include "./TraceInfoWidget.h"
#include "./ui_TraceDesignWidget.h"

class Point2DWidget;
class TraceGraphView;

class TraceDesignWidget: public QWidget, private Ui::TraceDesignWidget {
  Q_OBJECT
  GETSET(Trace::TracePointerVector, m_vector, Vector);  // Vector which hold the traces
 public:
    /// Constructor
    explicit TraceDesignWidget(QWidget* parent = 0);
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
 private:
   /**
    * Set up the gui and creat an example trace for show and debugging purposes
    */
   void initialise();
   /**
    * The infowidget
    */
   TraceInfoWidget* m_traceInfoWidget;
   /**
    * The view widget
    */
   TraceGraphView* m_traceGraphView;

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
};

#endif  // GUI_TRACEDESIGNWIDGET_H_
