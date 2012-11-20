#include <camera.hpp>

Camera::Camera()
{
}

Camera::ptr
Camera::make (void)
{
	return boost::make_shared<Camera> ();
}