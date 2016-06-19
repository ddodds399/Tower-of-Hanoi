#ifndef RING_H
#define RING_H

#include "transform.h"
#include "mesh.h"
#include <GL\glew.h>

//Ring Object
class Ring
{
public:
	//Constructor sets mesh to the selected size of ring in resources
	Ring(int i) {
		transformR;
		if (i == 1) {
			donut.setMesh("./res/models/small.obj");
		}
		if (i == 2) {
			donut.setMesh("./res/models/med.obj");
		}
		if (i == 3) {
			donut.setMesh("./res/models/big.obj");
		}
		ringSize = i;
	}
	//Calls mesh draw method
	void Draw() {
		donut.Draw();
	}
	//Gets transformation matrix of mesh
	inline Transform GetTransformMat() { return transformR; }
	//Gets size variable of ring
	inline int GetSize() { return ringSize; }
	//Sets global position in world co-ordinates
	void SetPos(float x, float y, float z) {
		transformR.SetPos(glm::vec3(x, y, z));
	}
	//Sets x position
	void SetPosx(float x) {
		transformR.GetPos().x = x;
	}
	//Sets y position
	void SetPosy(float y) {
		transformR.GetPos().y = y;
	}
	//Sets z position
	void SetPosz(float z) {
		transformR.GetPos().z = z;
	}
protected:
private:
	Ring(const Ring& other) {}
	void operator=(const Ring& other) {}
	Mesh donut;
	Transform transformR;
	float counter = 0;
	int ringSize;
};

#endif //RING_H