#ifndef TOILE_H
#define TOILE_H

#include<QWidget>
#include<QPoint>
#include<QBrush>
#include<QGraphicsScene>
#include<QPainterPath>
#include<QMenu>

class Toile : public QWidget
{
    Q_OBJECT
public:
    explicit Toile(QWidget *parent = nullptr);
    enum Etats{
        Select = 0,
        Line = 1,
        PolyLine = 2,
        Circle = 3,
        Ellipse = 4,
        Square = 5,
        Rectangle = 6
    };

private:
    QList<QPainterPath> pathContiner;
    QList<QPen> displayList;
    QList<QString> pathNameList;
    /** Select fonction **/
    void createSelectMenu();
    QList<QAction> selectedAction;
    QMenu selectedMenu;
    QActionGroup* selectAG = nullptr;
    bool findSelectedObjects(QPoint position);
    /** propriety of line*/
    Qt::PenStyle penStyle = Qt::SolidLine;
    QBrush color = Qt::red;
    Qt::PenJoinStyle joinStyle = Qt::RoundJoin;
    Qt::PenCapStyle capStyle = Qt::RoundCap;

    int lineWidth = 3;
    int etat = Line;

    /** mouseZone reprensent the area selected by the mouse **/
    QRect mouseZone;
    /** rayon reprensent the radius of mouse zone **/
    const int rayon = 5;
    QPoint start_point;
    QPoint end_point;
    QPen pen;
    QPainterPath* currentPath = nullptr;
    QPen* currentPen = nullptr;

    QPainterPath* polyLinePath = nullptr;

    bool findSelectedObject();
    void updateMouseZone(QPoint position);

protected:
    virtual void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
signals:
    void objectSelected(QPen* pen);
public slots:
    
    void modifyPathList(QAction* action);
    void changeColor(int colorIndex);
    void changeTypeOfLine(int typeIndex);
    void changeJoinStyle(int typeIndex);
    void changeCapStyle(int typeIndex);
    void changeEtat(int etat );
    void changeLineWidth(int width);
    void selectObject(QAction* action);
};


#endif // CANVAS_H
