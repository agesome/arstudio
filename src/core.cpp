#include "core.hpp"

Core::Core() :
    QMainWindow()
{
    central = new QWidget(this);
    this->setObjectName(QString::fromUtf8("Core"));
    this->setGeometry(10,10,800,500);
    this->setCentralWidget(central);
    pmnu   = new QMenu("&Menu");
    mnuBar.addMenu(pmnu);
    pmnu->addAction("Quit", this, SLOT(quit()));
    this->setMenuBar(&mnuBar);
    layout = new QGridLayout(central);
    mainsplitter = new QSplitter(Qt:: Horizontal, central);
    winsplitter = new QSplitter(Qt:: Vertical, central);
    layout->addWidget(mainsplitter, 0, 0 , 1, 1);
    scgr_sel = new ScenegraphSelector ();
    scgr_sel->addCheckbox(Item::CAMERA);
    scgr_sel->addCheckbox(Item::FPVEC);
    mainsplitter->addWidget(scgr_sel);
    scgr = new Workspace::Scenegraph(scgr_sel);
    wnd3d = new Window3D(scgr, central);
    wnd3d->setMinimumSize(300,300);
    winsplitter->addWidget(wnd3d);
    tmln =  new TimeLine(10,this);
    winsplitter->addWidget(tmln);
    mainsplitter->addWidget(winsplitter);
    hynta = new QWidget(central);
    mainsplitter->addWidget(hynta);
    rep = new Workspace::Repository();
    for (int i=1; i<42; i++)
            {

                //   boost::shared_ptr<Point3d> p2 = boost::static_pointer_cast<Point3d>(itemmm);

                //   Item::ptr itemm2 = boost::make_shared<Point3d>();
                boost::shared_ptr<Point3d> p2=boost::make_shared<Point3d>();;
                p2->x=i*0.1;
                p2->y=0;
                p2->z=0;
                p2->r=0;
                p2->g=0;
                p2->b=0;

                boost::shared_ptr<Camera> p1=boost::make_shared<Camera>();;
                p1->tx=0.05*i;
                p1->ty=0;
                p1->tz=0;

                rep->addItem (15, Item::FPVEC, i, p2);
                rep->addItem (13, Item::CAMERA, i,p1);



            }
    for (auto it: rep->getSequences())
        scgr->addSequence (it.second);
    tmln->setMax(41);
    connect(tmln,SIGNAL(nextFrame(int)),wnd3d,SLOT(update(int)));
    connect(wnd3d,SIGNAL(drawed()),tmln,SLOT(onDrawed()));

}
void Core::quit()
{
    qApp->exit();
}

