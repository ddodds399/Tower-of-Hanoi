#ifndef STAND_H
#define STAND_H

#include "transform.h"
#include "mesh.h"
#include <GL\glew.h>

//Stand Object
class Stand
{
public:
	//Constructor sets mesh to the stand.obj in resources
	Stand() {
		Sstand.setMesh("./res/models/stand.obj");
	}
	//Calls draw method of mesh class
	void Draw() {
		Sstand.Draw();
	}
	//Gets transformation matrix of mesh
	inline Transform GetTransformMat() { return transformS; }
	//Sets global position in world co-ordinates
	void SetPos(float x, float y, float z) {
		transformS.SetPos(glm::vec3(x, y, z));
	}
	//Sets x position
	void SetPosx(float x) {
		transformS.GetPos().x = x;
	}
	//Sets y position
	void SetPosy(float y) {
		transformS.GetPos().y = y;
	}
	//Sets z position
	void SetPosz(float z) {
		transformS.GetPos().z = z;
	}
	//Sets scale of mesh
	void SetScale(float scale) {
		transformS.SetScale(glm::vec3(scale, scale, scale));
	}

protected:
private:
	Stand(const Stand& other) {}
	void operator=(const Stand& other) {}
	Mesh Sstand;
	Transform transformS;
};

#endif //STAND_H