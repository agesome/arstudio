#ifndef PCLOUD_H
#define PCLOUD_H

#include <list>

#include <Point3D.hpp>
#include <Sequence.hpp>

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
