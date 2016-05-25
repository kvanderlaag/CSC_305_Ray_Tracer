#include "RaycastObjectList.h"



bool RaycastObjectList::Hit(const Ray& r, double t_min, double t_max, hit_record& hit) const {
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (size_t i = 0; i < size; ++i) {
		if (list[i]->Hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			hit = temp_rec;
		}
	}
	return hit_anything;
}