#ifndef __SHAPE_TEST_H__
#define __SHAPE_TEST_H__

namespace CNU_DDS
{
	typedef struct ShaptType_t
	{
		long		lengthOfColor;	// NULL length 포함
		char		color[128];	// 4단위로 끊음
		long		x;
		long		y;
		long		shapesize;
	}ShapeType;

	void*	write_shapetype(void* arg);
	void	ShapeTest();
}

#endif