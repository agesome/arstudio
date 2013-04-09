#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <opencv2/highgui/highgui.hpp>

#include <Sequence.hpp>

using namespace Workspace;

class Bitmap : public Item
{
public:
	typedef boost::shared_ptr<Bitmap> ptr;
	static ptr make (void)
	{
		return boost::make_shared<Bitmap> ();
	};

	cv::Mat bitmap;
};
#endif // BITMAP_HPP
