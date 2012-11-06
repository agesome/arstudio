#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <QMainWindow>
#include <QtGui/QCheckBox>
#include <QWidget>
#include <QVBoxLayout>
#include <string>
namespace Workspace
{
class Scenegraph : public QWidget
{
    Q_OBJECT
public:
    explicit Scenegraph(QWidget *parent = 0);
    Scenegraph(std::list<std::string> params, QMainWindow *MainWindow=0);
private:
    QVBoxLayout *layout;
    QCheckBox *boxes;
signals:
    
public slots:
    
};
}
#endif // SCENEGRAPH_H
