// Copyright [2014] <Ruud Cools>

#include <QtGui>
#include <macroHeader.h>
#include "../Point2DWidget.h"
#include "./TraceDesignWidget.h"
#include "./TraceInfoWidget.h"

TraceDesignWidget::TraceDesignWidget(QWidget *parent)
    : QWidget(parent) {
    initialise();
}

void TraceDesignWidget::initialise() {
    setupUi(this);
    pointFrame->layout()->addWidget(new TraceInfoWidget(this));
}
