#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <QMainWindow>
#include <QtGui/QCheckBox>
#include <QWidget>
#include <QVBoxLayout>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/make_shared.hpp>

namespace Workspace
{
class Scenegraph : public QWidget
{
    Q_OBJECT
public:
    explicit Scenegraph(QWidget *parent = 0);
    Scenegraph(std::list<std::string> params, QMainWindow *MainWindow=0);
private:
    boost::shared_ptr<QVBoxLayout> layout;
    boost::shared_array<QCheckBox> boxes;
signals:
    
public slots:
    
};
}
#endif // SCENEGRAPH_H
