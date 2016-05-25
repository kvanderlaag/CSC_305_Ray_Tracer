#pragma once
#include "RaycastObject.h"
#include "RaycastObjectList.h"
#include "Triangle.h"

class Cube :
	public RaycastObject
{
public:

	Cube(const Vector3& center, double half_width, Material* m) : mat(m) {
		Vector3 cube[8];
		cube[0] = center + Vector3(-half_width, -half_width, -half_width);
		cube[1] = center + Vector3(half_width, -half_width, -half_width);
		cube[2] = center + Vector3(half_width, -half_width, half_width);
		cube[3] = center + Vector3(-half_width, -half_width, half_width);
		cube[4] = center + Vector3(-half_width, half_width, -half_width);
		cube[5] = center + Vector3(half_width, half_width, -half_width);
		cube[6] = center + Vector3(half_width, half_width, half_width);
		cube[7] = center + Vector3(-half_width, half_width, half_width);

		RaycastObject* objectList[12];

		objectList[0] = new Triangle(cube[0], cube[2], cube[1], mat);
		objectList[1] = new Triangle(cube[0], cube[3], cube[2], mat);

		// Top face
		objectList[2] = new Triangle(cube[6], cube[5], cube[4], mat);
		objectList[3] = new Triangle(cube[7], cube[6], cube[4], mat);

		// Front face
		objectList[4] = new Triangle(cube[2], cube[7], cube[3], mat);
		objectList[5] = new Triangle(cube[2], cube[6], cube[7], mat);

		// Right face
		objectList[6] = new Triangle(cube[2], cube[1], cube[6], mat);
		objectList[7] = new Triangle(cube[1], cube[5], cube[6], mat);

		// Left face
		objectList[8] = new Triangle(cube[0], cube[7], cube[4], mat);
		objectList[9] = new Triangle(cube[3], cube[7], cube[0], mat);

		// Back face
		objectList[10] = new Triangle(cube[1], cube[0], cube[5], mat);
		objectList[11] = new Triangle(cube[0], cube[4], cube[5], mat);

		triangles = new RaycastObjectList(objectList, 12);
	}

	~Cube() {
		delete mat;
		delete triangles;
	}

	virtual bool Hit(const Ray& r, double t_min, double t_max, hit_record& hit) const {
		bool hit_cube = false;
		hit_record temp_rec;
		hit_cube = triangles->Hit(r, t_min, t_max, temp_rec);
		if (hit_cube) {
			hit = temp_rec;
		}
		return hit_cube;
		
	}

	RaycastObjectList* triangles;
	Material* mat;
};

