 #ifndef GUI_WIDGET_TRACE_TRACEGRAPHITEM
#define GUI_WIDGET_TRACE_TRACEGRAPHITEM

#include <macroHeader.h>
#include <QGraphicsObject>
#include <QList>
#include <Trace.h>

class TraceGraphPoint;
class GraphWidget;
class QGraphicsSceneMouseEvent;

class TraceGraphItem : public QGraphicsObject {
   Q_OBJECT
   GETSET(Trace::WeakTracePointer, m_trace, TracePointer);

 public:
   TraceGraphItem(Trace::TracePointer = nullptr);

   /**
    * Add edit points to the trace.
    * These will be visible if the trace is selected
    * The will enable the user to added the trace from the graph
    */
   //virtual void addEditPoint(Edge* i_editPoint);

   /**
    * Link this item to a trace in the master widget
    */
   void setTrace(const Trace::TracePointer&);

   enum { Type = UserType + 1 };
   virtual int type() const Q_DECL_OVERRIDE { return Type; }

   virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
   virtual QPainterPath shape() const Q_DECL_OVERRIDE;
   virtual void paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget) Q_DECL_OVERRIDE;
   void updatePosition();

   /// update the position of the siblings of the given point
   void updatePointPositions(TraceGraphPoint* i_point = nullptr) const;

 signals:
   void removeThisTrace(Trace::TracePointer);
   void convertThisTrace(Trace::TracePointer, Trace::TraceType);
   void reverse(Trace::TracePointer);

 protected:
   virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
   void mousePressEvent(QGraphicsSceneMouseEvent* i_event) Q_DECL_OVERRIDE;
   void mouseReleaseEvent(QGraphicsSceneMouseEvent* i_event) Q_DECL_OVERRIDE;
   void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;

 private slots:
    void handleTrigger();

 private:
    /// variable indicating the item is being moved by the user
    bool m_isBeingMoved;
    static const QString RemoveTraceActionText;
    static const QString ConvertToLineActionText;
    static const QString ConvertToCurveActionText;
    static const QString ConvertDirection;
};

#endif // GUI_WIDGET_TRACE_TRACEGRAPHITEM
