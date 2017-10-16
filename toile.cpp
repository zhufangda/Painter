#include<QPainter>
#include<QMouseEvent>
#include<QPen>
#include<iostream>
#include "toile.h"
#include<QPoint>
#include<QDebug>
#include<QtMath>
#include<QAction>
Toile::Toile(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumSize(800,600);
}

void Toile::paintEvent(QPaintEvent* e){
    QWidget::paintEvent(e);
    QPainter painter(this);


    this->pen.setStyle(this->penStyle);
    this->pen.setWidth(this->lineWidth);
    this->pen.setBrush(this->color);
    this->pen.setJoinStyle(this->joinStyle);
    this->pen.setCapStyle(this->capStyle);

    painter.setPen(this->pen);


    qDebug() <<QString("paintEvent:") << etat;
    switch(this->etat)
    {
        case Select:
        {
            if(currentPen != nullptr)
            {
                this->currentPen->setStyle(this->penStyle);
                this->currentPen->setWidth(this->lineWidth);
                this->currentPen->setBrush(this->color);
                this->currentPen->setJoinStyle(this->joinStyle);
                this->currentPen->setCapStyle(this->capStyle);
                painter.drawPath(*currentPath);
                painter.drawEllipse(mouseZone);
            }
            break;
        }
        case Line:
        case PolyLine:
            if(this->polyLinePath != nullptr)
                painter.drawPath(*this->polyLinePath);
            painter.drawLine(start_point, end_point);
            break;
        case Circle:
        {
            QPoint tmpPoint(end_point - start_point);
            int r = qSqrt(tmpPoint.x() * tmpPoint.x() + tmpPoint.y() * tmpPoint.y());
            painter.drawEllipse(start_point,r,r);
            break;
        }
        case Ellipse:
        {
            int rx = qAbs((end_point- start_point).x());
            int ry = qAbs((end_point - start_point).y());
            painter.drawEllipse(start_point,rx,ry);
            break;
        }
        case Square:
        {
            painter.drawRect(start_point.x(),
                             start_point.y(),
                             end_point.x() - start_point.x(),
                             end_point.x() - start_point.x()
                            );
            break;
        }
        case Rectangle:
        {
            painter.drawRect(start_point.x(),
                             start_point.y(),
                             end_point.x()-start_point.x(),
                             end_point.y()-start_point.y()
                            );
            break;
        }
    }


    int size = this->pathContiner.size();

    for(int i = 0; i< size;i++){
        painter.setPen(this->displayList.at(i));
        painter.drawPath(this->pathContiner.at(i));
    }
}

// Event Mouse
void Toile::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->start_point= event->pos();
        switch(this->etat){
            case Select:
                //TODO
                if(findSelectedObjects(start_point)){
                    this->selectedMenu.clear();
                    selectedMenu.addActions(selectAG->actions());
                    selectedMenu.popup(event->globalPos());
                    selectedMenu.exec();
                }
                break;
            case PolyLine:
                this->setMouseTracking(true);
                if(polyLinePath == nullptr)
                    this->polyLinePath = new QPainterPath(start_point);
                else
                    this->polyLinePath->lineTo(start_point);
                break;
        }
    }
}

void Toile::mouseMoveEvent(QMouseEvent *event){
    this-> end_point = event->pos();
    updateMouseZone(event->pos());
    this-> update();
    if(this->etat == Select && event->button() == Qt::RightButton);
}

void Toile::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->end_point = event->pos();
        QPainterPath newPath(start_point);
        switch(this->etat)
        {
            case Select:
            {
                for(int i = displayList.count() -1; i>=0; i--){
                    if(this->pathContiner.at(i).contains(end_point)){

                    }
                }
                break;
            }
            case Line:
            {
                newPath.lineTo(end_point);
                pathNameList.append("Line");
                break;
            }
            case PolyLine:
            {
                break;
            }
           case Circle:
            {
                QPoint tmpPoint(end_point - start_point);
                int r = qSqrt(tmpPoint.x() * tmpPoint.x() + tmpPoint.y() * tmpPoint.y());
                newPath.addEllipse(start_point,r,r);
                tmpPoint.~QPoint();
                this->pathNameList.append("Circle");
                break;
            }

            case Ellipse:
            {
                int rx = qAbs((end_point- start_point).x());
                int ry = qAbs((end_point - start_point).y());
                newPath.addEllipse(start_point,rx,ry);
                this->pathNameList.append("Ellipse");
                break;
            }
            case Square:
            {
                newPath.addRect(start_point.x(),
                                start_point.y(),
                                end_point.x() - start_point.x(),
                                end_point.x() - start_point.x()
                                );

                this->pathNameList.append("Square");
                break;
            }
            case Rectangle:
            {
                newPath.addRect(start_point.x(),
                                start_point.y(),
                                end_point.x()-start_point.x(),
                                end_point.y()-start_point.y()
                               );
                this->pathNameList.append("Rectangle");
               break;
            }
        }
        if(!newPath.isEmpty()){
            this->pathContiner.append(newPath);
            this->displayList.append(this->pen);
        }
    }
    end_point.setX(start_point.x());
    end_point.setY(start_point.y());
    update();
 }

void Toile::mouseDoubleClickEvent(QMouseEvent *event){
    switch (this->etat){
        case PolyLine:
            this->setMouseTracking(false);
            if(this->polyLinePath == nullptr) break;
            pathContiner.append(*this->polyLinePath);
            displayList.append(this->pen);
            pathNameList.append("Polyline");
            update();
            this->polyLinePath = nullptr;
            break;
        default:
            break;
    }
}

// SLOTs
void Toile::changeColor(int colorIndex){
    QStringList colorList = QColor::colorNames();
    this->color.setColor(QColor(colorList[colorIndex]));
    repaint();
}

void Toile::changeTypeOfLine(int typeIndex){
    switch(typeIndex){
        case 0: this->penStyle = Qt::NoPen;break;
        case 1: this->penStyle = Qt::SolidLine;break;
        case 2: this->penStyle = Qt::DashLine;break;
        case 3: this->penStyle = Qt::DotLine;break;
        case 4: this->penStyle = Qt::DashDotLine;break;
        case 5: this->penStyle = Qt::DashDotDotLine;break;
    }
    std::cout << typeIndex;
    update();
}

void Toile::changeJoinStyle(int typeIndex){
    switch(typeIndex){
        case 0: this->joinStyle= Qt::BevelJoin;break;
        case 1: this->joinStyle= Qt::MiterJoin;break;
        case 2: this->joinStyle= Qt::RoundJoin;break;
    }
    update();
}


void Toile::changeCapStyle(int typeIndex){
    switch(typeIndex){
        case 0: this->capStyle= Qt::SquareCap;break;
        case 1: this->capStyle = Qt::FlatCap;break;
        case 2: this->capStyle = Qt::RoundCap;break;
    }
    update();
}

void Toile::changeEtat(int etat){
    this->etat = - etat -2 ;
    if(this->etat == Select){
        setMouseTracking((true));
    }else{
        setMouseTracking(false);
    }
    qDebug() <<"changeEtat:" <<etat;
    update();
}

void Toile::changeLineWidth(int width){
   this->lineWidth = width;
    update();
}
/** Find if there are some objects in the postion specified by arguments.
 * This fonction will update the list of action.
*@arg Position existe un position
*@return ture if there this
**/
bool Toile::findSelectedObjects(QPoint position){
    updateMouseZone(position);
    if(selectAG != nullptr){
        QList<QAction*> list = selectAG->actions();
        for(int i=0; i<list.size();i++){
            delete list.at(i);
        }

        delete selectAG;
        selectAG = nullptr;
    }

    selectAG = new QActionGroup(this);

    for(int i= this->displayList.size()-1;i>=0;i--){
        if(this->pathContiner.at(i).intersects(mouseZone)){
            QAction* action = new QAction(QString::number(i) + QString(" ") +pathNameList.at(i) );
            action->setData(QVariant(i));
            selectAG->addAction(action);
        }
    }

    if(selectAG->actions().size() > 0){
        connect(selectAG, SIGNAL(triggered(QAction*)), this, SLOT(selectObject(QAction*)));
        return true;
    }else{
        return false;
    }
}

/** Update the mouse zone
*@arg the position of the mouse
*/
void Toile::updateMouseZone(QPoint position){
    mouseZone.setCoords(position.x() - rayon,
                        position.y() - rayon,
                        position.x() + rayon,
                        position.y() + rayon);
}

bool Toile::findSelectedObject()
{
    for(int i = this->displayList.count()-1; i>=0;i--){
        if(this->pathContiner.at(i).intersects(mouseZone)){
            this->currentPath = & this->pathContiner[i];
            this->currentPen = & this->displayList[i];
            qDebug() << "Find object:" << i;
            return true;
        }
    }
    qDebug()<< "Don't fint any object" ;
    return false;
}

void Toile::modifyPathList(QAction* action){
    qDebug() << "Slot - modifyPathList :" << action->text();
    if(action->text() == "&Undo" && !displayList.isEmpty()){
        this->displayList.pop_back();
        this->pathContiner.pop_back();
        this->pathNameList.pop_back();
    }else if(action->text() == "&Clear"){
        this->displayList.clear();
        this->pathContiner.clear();
        this->pathNameList;
    }

    update();
}

void Toile::selectObject(QAction* sender){
    int i = sender->data().toInt();
    qDebug() << "Select object:" << i <<" "<< pathNameList.at(i);
    this->currentPath = & this->pathContiner[i];
    this->currentPen = & this->displayList[i];
    emit objectSelected(currentPen);
    update();
    qDebug() << "Find object:" << i;
}
