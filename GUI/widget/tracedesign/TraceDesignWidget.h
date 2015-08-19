 // Copyright [2014] Ruud Cools
#ifndef GUI_TRACEDESIGNWIDGET_H_
#define GUI_TRACEDESIGNWIDGET_H_

#include <QWidget>
#include <TraceListIO.h>
#include "./ui_TraceDesignWidget.h"

class Point2DWidget;

class TraceDesignWidget: public QWidget, private Ui::TraceDesignWidget {
    Q_OBJECT
    GETSET(TracePointerVector, m_vector, Vector);  // Vector which hold the traces
    Point2DWidget* startPoint;
    Point2DWidget* endPoint;
    Point2DWidget* centralPoint;

 public:
    // Constructor
    explicit TraceDesignWidget(QWidget* parent = 0);

 private:
    void initialise();
};

#endif  // GUI_TRACEDESIGNWIDGET_H_
