#ifndef PCLOUD_H
#define PCLOUD_H
#include <point3d.hpp>
#include <sequence.hpp>
#include <list>

class PointCloud : public Item
{
public:
	typedef boost::shared_ptr<PointCloud> ptr;
	static ptr make (void)
	{
		return boost::make_shared<PointCloud> ();
	};

	std::list<Point3d::ptr> cloud;
};

#endif // PCLOUD_H