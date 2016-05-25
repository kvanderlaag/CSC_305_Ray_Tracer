#pragma once
#include "RaycastObject.h"

class RaycastObjectList : public RaycastObject
{
public:
	RaycastObjectList() {};
	RaycastObjectList(RaycastObject** l, size_t n) : list(l), size(n) {};

	~RaycastObjectList() {
		for (int i = 0; i < size; ++i) {
			delete list[i];
		}
		delete list;
	}

	virtual bool Hit(const Ray& r, double t_min, double t_max, hit_record& hit) const;

	RaycastObject** list;
	size_t size;

};

