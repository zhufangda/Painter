#include<QIODevice>
#include<QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTextEdit>
#include<QFileDialog>
#include<QString>
#include<iostream>
#include<QFile>
#include<QMessageBox>
#include<QComboBox>
#include<QPen>
#include<QBoxLayout>
#include<QPushButton>
#include<QSlider>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    toile(new Toile(this))
{
    //ui->setupUi(this);
    QMenuBar *menuBar = this->menuBar();
    QMenu *fileMenu = menuBar->addMenu(tr("&Fichier"));
    this->setWindowIcon(QIcon(":icone/icon.png"));

    //Open Action
    QAction *openAction = new QAction(QIcon(":icone/open.png"),tr("&Open"),this);
    openAction->setShortcut(tr("Ctrl+N"));
    openAction->setToolTip(tr("Opne a file."));
    openAction->setStatusTip(tr("Open a file."));
    fileMenu -> addAction(openAction);

    //Save Action
    QAction *saveAction = new QAction(QIcon(":icone/save.png"),tr("&Save"),this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setToolTip(tr("Save a file."));
    saveAction->setStatusTip(tr("Save a file."));
    fileMenu -> addAction(saveAction);


    //Quit Action
    QAction *quitAction = new QAction(QIcon(":icone/quit.png"),tr("&Quit"),this);
    quitAction->setShortcut(tr("Ctrl+S"));
    quitAction->setToolTip(tr("Quit a file."));
    quitAction->setStatusTip(tr("Quit a file."));
    fileMenu -> addAction(quitAction);
    //Add Toolbar
    QToolBar *fileBar = this->addToolBar(tr("File"));
    fileBar->addAction(openAction);
    fileBar->addAction(saveAction);
    fileBar->addAction(quitAction);

    QMenu *editMenu = menuBar->addMenu(tr("&Edit"));
    //Suprimer action
    QAction *undoAction= new QAction(QIcon(":icone/undo.png"),tr("&Undo"),this);
    undoAction->setShortcut(tr("Ctrl+z"));
    undoAction->setToolTip(tr("Delete last path."));
    undoAction->setStatusTip(tr("Delete last path."));
    editMenu -> addAction(quitAction);

    //Suprimer action
    QAction *clearAction= new QAction(QIcon(":icone/clear.png"),tr("&Clear"),this);
    clearAction->setShortcut(tr("Ctrl+d"));
    clearAction->setToolTip(tr("Delete all the path in the canvas."));
    clearAction->setStatusTip(tr("Delete all the path in the canvas."));
    clearAction->setData("undo");
    editMenu -> addAction(quitAction);

    QActionGroup* editActionGroup = new QActionGroup(this);
    editActionGroup->addAction(undoAction);
    editActionGroup->addAction(clearAction);

    connect(editActionGroup, SIGNAL(triggered(QAction*)), toile, SLOT(modifyPathList(QAction*)));

    QToolBar *editBar= this->addToolBar(tr("Edit"));
    editBar->addAction(undoAction);
    editBar->addAction(clearAction);

    this->addToolBar(editBar);
    //Connection
    connect(openAction, SIGNAL(triggered()),this,SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()),this,SLOT(saveFile()));
    connect(quitAction, SIGNAL(triggered()),this,SLOT(quitApp()));
    //Add Text Area
    //textEdit = new QTextEdit(this);
    statusBar()->showMessage(tr("Status Bar"));


    // Set mode button
    QPushButton *select_bn = new QPushButton(QIcon(":image/select.png"),"",this);
    select_bn->setCheckable(true);
    select_bn->autoExclusive();
    select_bn->setIconSize(QSize(40,40));
    QPushButton *line_bn = new QPushButton(QIcon(":image/line.png"),"",this);
    line_bn->setCheckable(true);
    line_bn->autoExclusive();
    line_bn->setIconSize(QSize(40,40));
    line_bn->setChecked(true);

    QPushButton *polyLine_bn = new QPushButton(QIcon(":image/polyline.ico"),"",this);
    polyLine_bn->setCheckable(true);
    polyLine_bn->autoExclusive();
    polyLine_bn->setIconSize(QSize(40,40));

    QPushButton *circle_bn = new QPushButton(QIcon(":image/circle.png"),"",this);
    circle_bn->setCheckable(true);
    circle_bn->autoExclusive();
    circle_bn->setIconSize(QSize(40,40));

    QPushButton *ellipse_bn = new QPushButton(QIcon(":image/ellipse.ico"), "",this);
    ellipse_bn->setCheckable(true);
    ellipse_bn->autoExclusive();
    ellipse_bn->setIconSize(QSize(40,40));

    QPushButton *square_bn = new QPushButton(QIcon(":image/square.png"), "",this);
    square_bn->setCheckable(true);
    square_bn->autoExclusive();
    square_bn->setIconSize(QSize(40,40));

    QPushButton *rectangle_bn = new QPushButton(QIcon(":image/rectangle.ico"),"",this);
    rectangle_bn->setCheckable(true);
    rectangle_bn->autoExclusive();
    rectangle_bn->setIconSize(QSize(40,40));

    QButtonGroup* groupForm = new QButtonGroup(this);
    groupForm->addButton(select_bn);
    groupForm->addButton(line_bn);
    groupForm->addButton(polyLine_bn);
    groupForm->addButton(circle_bn);
    groupForm->addButton(ellipse_bn);
    groupForm->addButton(square_bn);
    groupForm->addButton(rectangle_bn);

    connect(groupForm, SIGNAL(buttonClicked(int)), this->toile, SLOT(changeEtat(int)));

    QVBoxLayout *mode_layout = new QVBoxLayout();
    mode_layout->setSizeConstraint(QLayout::SetFixedSize);
    mode_layout->setAlignment(Qt::AlignTop);
    mode_layout->addWidget(select_bn);
    mode_layout->addWidget(line_bn);
    mode_layout->addWidget(polyLine_bn);
    mode_layout->addWidget(circle_bn);
    mode_layout->addWidget(ellipse_bn);
    mode_layout->addWidget(square_bn);
    mode_layout->addWidget(rectangle_bn);
    toile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *h_layout = new QHBoxLayout(this);
    h_layout->addLayout(mode_layout);
    h_layout->addWidget(toile);
    h_layout->addWidget(this->creatProperyBox());
    QWidget *centreWidget = new QWidget(this);
    this->setCentralWidget(centreWidget);
    centreWidget->setLayout(h_layout);
}

void MainWindow::openFile()
{
    qDebug() << "Open File";
    QFileDialog dialog(this);
    QStringList fileNames;

    if(dialog.exec() == QDialog::Accepted){
        fileNames = dialog.selectedFiles();
        QString firstName = fileNames[0];
        std::cout << "Open File:" << qPrintable(firstName) << std::endl;

        QFile file(firstName);
        if(!file.open(QIODevice::ReadOnly)){
            return; //TODO give erro information
        }

      QTextStream stream( &file );
      QString inputText = stream.readAll();
      //textEdit->setText(inputText);
      file.close();
    }

}

void MainWindow::saveFile()
{
    qDebug() << "Save File";
    // Get the save path
    QString firstName = QFileDialog::getSaveFileName(this,
                                                     tr("Save File"),
                                                     "/save",
                                                     tr("Html File(*.html)")
                                                     );

    std::cout << "Save File:" << qPrintable(firstName) << std::endl;

    QFile write(firstName);
    if(write.open(QIODevice::WriteOnly)){
        QTextStream out(&write);
        out.setCodec("UTF-8");
        //out<< textEdit->toHtml();
    }
    write.close();

}

void MainWindow::quitApp()
{
    qDebug()<< "Quit app";
    QMessageBox::StandardButton r = QMessageBox::warning(this
                                                         , "Quit appliaction"
                                                         , "Do you want to exit application?"
                                                         , QMessageBox::Yes | QMessageBox::No);
    switch (r) {
    case QMessageBox::Yes:
        this->close();
        break;
    default:
        break;
    }

}


void MainWindow::fillColorList(QComboBox *comboBox){
    QStringList colorList = QColor::colorNames();
    QString color;

    foreach(color, colorList){
        QPixmap pix(QSize(70,42));
        pix.fill(QColor(color));
        comboBox->addItem(QIcon(pix),color,QColor(color).name());
        comboBox->setIconSize(QSize(70,42));
        comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    }
    comboBox->setCurrentIndex(colorList.indexOf("red"));
}

QGroupBox *MainWindow::creatProperyBox(){

    this->colorComboBox = new QComboBox();
    this->fillColorList(colorComboBox);

    this->lineTypeBox = new QComboBox(this);
    lineTypeBox->setIconSize(QSize(70,42));
    lineTypeBox->addItem("No Pen");
    lineTypeBox->addItem(QIcon(":image/qpen-solid.png"),"Solid Line",static_cast<int>(Qt::SolidLine));
    lineTypeBox->addItem(QIcon(":image/qpen-dash.png"),"Dash Line",static_cast<int>(Qt::DashLine));
    lineTypeBox->addItem(QIcon(":image/qpen-dot.png"),"Dot Line",static_cast<int>(Qt::DotLine));
    lineTypeBox->addItem(QIcon(":image/qpen-dashdot.png"),"Dash Dot Line",static_cast<int>(Qt::DashDotLine));
    lineTypeBox->addItem(QIcon(":image/qpen-dashdotdot.png"),"Dash Dot Dot Line",static_cast<int>(Qt::DashDotDotLine));
    lineTypeBox->setCurrentIndex(1);

    this->penCapStyleBox = new QComboBox(this);
    penCapStyleBox->setIconSize(QSize(70,42));
    penCapStyleBox->addItem(QIcon(":image/qpen-square"), "Square Cap", static_cast<int>(Qt::SquareCap));
    penCapStyleBox->addItem(QIcon(":image/qpen-flat"), "Flat Cap", static_cast<int>(Qt::FlatCap));
    penCapStyleBox->addItem(QIcon(":image/qpen-roundcap"), "Round Cap", static_cast<int>(Qt::RoundCap));
    penCapStyleBox->setCurrentIndex(2);

    this->penJoinStyleBox = new QComboBox(this);
    penJoinStyleBox->setIconSize(QSize(128,42));
    penJoinStyleBox->addItem(QIcon(":image/qpen-bevel"), "Bevel Join", static_cast<int>(Qt::BevelJoin));
    penJoinStyleBox->addItem(QIcon(":image/qpen-miter"), "Miter Join", static_cast<int>(Qt::MiterJoin));
    penJoinStyleBox->addItem(QIcon(":image/qpen-roundjoin"), "Round Join", static_cast<int>(Qt::RoundJoin));
    penJoinStyleBox->setCurrentIndex(2);

    this->widthSlider = new QSlider(Qt::Horizontal);
    widthSlider->setMinimum(0);
    widthSlider->setMaximum(100);
    widthSlider->setValue(0);


    QVBoxLayout *v_layout = new  QVBoxLayout(this);
    v_layout->addWidget(lineTypeBox);
    v_layout->addWidget(colorComboBox);
    v_layout->addWidget(penCapStyleBox);
    v_layout->addWidget(penJoinStyleBox);
    v_layout->addWidget(widthSlider);
    v_layout->setAlignment(Qt::AlignTop);
    QGroupBox *propertyBox = new QGroupBox();
    propertyBox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::MinimumExpanding);
    propertyBox->setAlignment(Qt::AlignTop);
    propertyBox->setLayout(v_layout);

    connect(colorComboBox,SIGNAL(currentIndexChanged(int)), this->toile, SLOT(changeColor(int)));
    connect(lineTypeBox,SIGNAL(currentIndexChanged(int)), this-> toile, SLOT(changeTypeOfLine(int)));
    connect(penCapStyleBox, SIGNAL(currentIndexChanged(int)), this->toile, SLOT(changeCapStyle(int)));
    connect(penJoinStyleBox, SIGNAL(currentIndexChanged(int)), this->toile, SLOT(changeJoinStyle(int)));
    connect(widthSlider, SIGNAL(valueChanged(int)), this->toile, SLOT(changeLineWidth(int)));
    connect(this->toile, SIGNAL(objectSelected(QPen*)), this, SLOT(setPropertyPanel(QPen*)));

    return propertyBox;

}

void MainWindow::setPropertyPanel(QPen* pen){
    this->lineTypeBox->setCurrentIndex(lineTypeBox->findData(static_cast<int>(pen->style())));
    this->penCapStyleBox->setCurrentIndex(penCapStyleBox->findData(static_cast<int>(pen->capStyle())));
    this->penJoinStyleBox->setCurrentIndex(penJoinStyleBox->findData(static_cast<int>(pen->joinStyle())));
    this->widthSlider->setValue(pen->width());
    int index = this->colorComboBox->findData(pen->color().name());
    qDebug()<< "color:" << pen->color().name() <<endl
            <<"color index:" << index << endl
            << "line type:" << pen->style() <<endl;
    this->colorComboBox->setCurrentIndex(index);

}
MainWindow::~MainWindow()
{
    delete ui;
}
