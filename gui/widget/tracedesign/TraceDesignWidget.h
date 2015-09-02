 // Copyright [2014] Ruud Cools
#ifndef GUI_TRACEDESIGNWIDGET_H_
#define GUI_TRACEDESIGNWIDGET_H_

#include <QWidget>
#include <TraceListIO.h>
#include "./TraceInfoWidget.h"
#include "./ui_TraceDesignWidget.h"

class Point2DWidget;
class TraceGraphView;

class TraceDesignWidget: public QWidget, private Ui::TraceDesignWidget {
    Q_OBJECT
    GETSET(TraceListIO::TracePointerVector, m_vector, Vector);  // Vector which hold the traces

 public:
    // Constructor
    explicit TraceDesignWidget(QWidget* parent = 0);

 public slots:
   void addTrace(Trace::TracePointer);
   void removeTrace(Trace::TracePointer);
   void replaceTrace(Trace::TraceType);

 private:
   /**
    * Set up the gui and creat an example trace for show and debugging purposes
    */
   void initialise();
   /// The infowidget
   TraceInfoWidget* m_traceInfoWidget;
   /// The view widget
   TraceGraphView* m_traceGraphView;

   /**
    * Get the same Trace::TracePointer as selected in the view
    */
    int getSelectedIndex() const;

 private slots:
   void setSelectedTrace();
   void addTraceFromButton();

};

#endif  // GUI_TRACEDESIGNWIDGET_H_
