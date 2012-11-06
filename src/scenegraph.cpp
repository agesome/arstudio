#include <scenegraph.hpp>
#include <QVariant>


Workspace::Scenegraph::Scenegraph(std::list<std::string> params, QMainWindow *MainWindow):
     QWidget(MainWindow)
{
    this->setObjectName(QString::fromUtf8("TimeLine"));
    this->setGeometry(QRect(20, 30, 400, 90));
    layout = new QVBoxLayout(this);
    boxes = new QCheckBox[params.size()];
    std::list<std::string>::iterator i;
    int j = 0;
    for(i = params.begin(); i!= params.end(); ++i){
        boxes[j].setText(QString::fromStdString(*i));
        layout->addWidget(&boxes[j]);
        j++;
    }

}
