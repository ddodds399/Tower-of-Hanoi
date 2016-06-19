#ifndef POINTER_H
#define POINTER_H

#include "transform.h"
#include "mesh.h"
#include <GL\glew.h>

//Pointer Object
class Pointer
{
public:
	//Constructor sets mesh to the pointer.obj in resources and initialises it position variable.
	Pointer() {
		point.setMesh("./res/models/pointer.obj");
		currPos = 1;
	}
	//Calls the mesh draw method
	void Draw() {
		point.Draw();
	}
	//Gets transformation matrix of mesh
	inline Transform GetTransformMat() { return transformP; }
	//Sets global position in world co-ordinates
	void SetPos(float x, float y, float z) {
		transformP.SetPos(glm::vec3(x, y, z));
	}
	//Sets x position
	void SetPosx(float x) {
		transformP.GetPos().x = x;
	}
	//Sets y position
	void SetPosy(float y) {
		transformP.GetPos().y = y;
	}
	//Sets z position
	void SetPosz(float z) {
		transformP.GetPos().z = z;
	}
	//Sets scale of mesh
	void SetScale(float scale) {
		transformP.SetScale(glm::vec3(scale, scale, scale));
	}
	//Returns refernce of current position, can be used to set.
	inline int &GetCurrPos() { return currPos; }

protected:
private:
	Pointer(const Pointer& other) {}
	void operator=(const Pointer& other) {}
	Mesh point;
	Transform transformP;
	int currPos;
};

#endif //POINTER_H