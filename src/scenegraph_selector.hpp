#ifndef SCENEGRAPH_SELECTOR_H
#define SCENEGRAPH_SELECTOR_H

#include <QMainWindow>
#include <QtGui/QCheckBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QVariant>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/make_shared.hpp>
#include <sequence.hpp>

namespace Workspace
{

class ScenegraphSelector : public QWidget
{
	Q_OBJECT
public:
	typedef boost::shared_ptr<ScenegraphSelector> ptr;

	ScenegraphSelector (QMainWindow * MainWindow = nullptr);
	static ptr make (QMainWindow * MainWindow = nullptr);
	void addCheckbox (Item::type);
	Item::typemask getSelections (void);
private:
	typedef boost::shared_ptr<QCheckBox> qbox_ptr;

	boost::shared_ptr<QVBoxLayout> layout;
	std::list< std::pair<Item::type, qbox_ptr> > boxes;
signals:

public slots:

};

}
#endif // SCENEGRAPH_SELECTOR_H
