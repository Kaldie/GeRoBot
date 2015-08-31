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
    GETSET(int, m_index, Index);
    GETSET(TraceListIO::TracePointerVector, m_vector, Vector);  // Vector which hold the traces

 public:
    // Constructor
    explicit TraceDesignWidget(QWidget* parent = 0);

 public slots:
   void replaceTrace(Trace::TraceType);
   void setCurrentIndex(const int& i_index);


 private:
    void initialise();
    TraceInfoWidget* m_traceInfoWidget;
    TraceGraphView* m_traceGraphView;

 private slots:
   void updateSelectedTrace();

};

#endif  // GUI_TRACEDESIGNWIDGET_H_
