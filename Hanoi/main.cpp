#include <iostream>
#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "stb_image.h"
#include "ring.h"
#include "skybox.h"
#include "pointer.h"
#include "stand.h"
#include <list>
#include <GL\glew.h>
#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <mmsystem.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
#undef main

int main(int argc, char* argv[])
{

	//Inital text output to command console

	cout << "40103628 - Towers of Hanoi Project." << endl << endl;
	cout << "Objective: Move all the rings to the third stand." << endl << endl;
	cout << "Below are the input commands:" << endl;
	cout << "Use the 'a' & 'd' keys to orbit the scene." << endl;
	cout << "Use the 'f' key to toggle fullscreen mode." << endl;
	cout << "Use the 'r' key to restart the game." << endl;
	cout << "Use the left & right arrow keys to move the cursor over the towers." << endl;
	cout << "Use the space bar to pickup and drop a ring." << endl;
	cout << "Use the 'esc' key to exit the program." << endl;

	//Setting Window display size and resolution
	Display display(800, 600, "40103628 - Towers of Hanoi");

	//Creating instances of models
	Ring ring1(1);
	Ring ring2(2);
	Ring ring3(3);
	Pointer pointer;
	Stand stand0, stand1, stand2;

	//Setting up different shaders
	Shader basicShader("./res/shaders/basicShader");
	Shader skyboxShader("./res/shaders/skyboxShader");
	Shader reflect("./res/shaders/reflection");
	Shader phong("./res/shaders/phong");
	Shader pointerShader("./res/shaders/pointLight");

	//Initialising camera
	Camera camera(glm::vec3(0, 2, 10), 70.0f, (float)800 / (float)600, 0.01f, 1000.0f);

	//Creating skybox
	Skybox skybox;

	//Initialising lists to keep track of rings
	list<int> list1, list2, list3;

	for (int i = 1; i <4 ; i++) {
		list1.push_back(i);
	}
	
	//Variables for controlling logic and keeping track of moves
	int currentRing = 1;
	int moves = 0;
	int set = 0;
	int finishCount = 0;
	bool pickup = false;
	bool finished = false;

	// Cubemap (Skybox)
	vector<const GLchar*> skyfaces;
	skyfaces.push_back("./res/skybox/posx.jpg");
	skyfaces.push_back("./res/skybox/negx.jpg");
	skyfaces.push_back("./res/skybox/posy.jpg");
	skyfaces.push_back("./res/skybox/negy.jpg");
	skyfaces.push_back("./res/skybox/posz.jpg");
	skyfaces.push_back("./res/skybox/negz.jpg");
	Texture skyboxTex(skyfaces);

	// Cubemap (Ring) Tire Texture
	vector<const GLchar*> rfaces;
	rfaces.push_back("./res/ringmap/met.jpg");
	rfaces.push_back("./res/ringmap/met.jpg");
	rfaces.push_back("./res/ringmap/met.jpg");
	rfaces.push_back("./res/ringmap/met.jpg");
	rfaces.push_back("./res/ringmap/met.jpg");
	rfaces.push_back("./res/ringmap/met.jpg");
	Texture ringTex(rfaces);
	
	//Initialising metal stand texture
	Texture metalTex("./res/tex/standtex.jpg");

	// Create initial camera transformation
	glm::mat4 view = camera.GetViewProjection();
	glm::mat4 projection = glm::perspective(70.0f, (float)800 / (float)600, 0.1f, 100.0f);
	
	//Set initial locations of rings and cursor
	ring1.SetPos(-3, 1.5, 0);
	ring2.SetPos(-3, 1, 0);
	ring3.SetPos(-3, 0.5, 0);
	pointer.SetScale(0.1);
	pointer.SetPos(-3, 3, 0);

	//While the display window is open, the following code will run within this loop
	while (!display.IsClosed())
	{
		display.Clear(0.0f, 0.3f, 0.15f, 1.0f);

		//////////// Reset ///////////////
		//The following method allows the user to reset the game.
		//This will also be called on game completion.
		//Variables are reset to their initial values and models are moved to their initial co-ordinates.
		if (display.IsRPressed() || finished == true) {
			if (finished == true) {
				cout << endl << "Congratulations!" << endl;
				cout << "You completed set " << set << " in " << moves << " moves!" << endl;
				PlaySound(TEXT("./res/sounds/cheer.wav"), NULL, SND_SYNC);
			}
			display.ResetAll();
			ring1.SetPos(-3, 1.5, 0);
			ring2.SetPos(-3, 1, 0);
			ring3.SetPos(-3, 0.5, 0);
			pointer.SetPos(-3, 3, 0);
			currentRing = 1;
			list1.clear();
			list2.clear();
			list3.clear();
			for (int i = 1; i < 4; i++) {
				list1.push_back(i);
			}
			pointer.GetCurrPos() = 1;
			moves = 0;
			finishCount = 0;
			pickup = false;
			finished = false;
		}

		///////////// Pointer Drawing /////////////////////
		//This is where the pointer/cursor is drawn.
		//A shader in resources is loaded and bound.
		//A point light also follows the x co-ordinate of the cursor to shine on the rings.
		pointerShader.Bind();
		glm::vec3 lightPos(glm::vec3(pointer.GetTransformMat().GetPos().x, 1.0f , 0));
		pointerShader.Update(pointer.GetTransformMat(), camera);
		pointer.Draw();
		
		//////////////// Stand Drawing ////////////////
		//Here the individual stands are drawn
		//The basic shader, which allows for texturing and colouration, is bound.
		//Along with the texture to be applied to the stand.
		glDisable(GL_CULL_FACE);
		basicShader.Bind();
		metalTex.Bind();
		basicShader.Update(stand0.GetTransformMat(), camera);
		stand0.SetPosx(-3);
		stand0.Draw();
		basicShader.Update(stand1.GetTransformMat(), camera);
		stand1.SetPosx(0);
		stand1.Draw();
		basicShader.Update(stand2.GetTransformMat(), camera);
		stand2.SetPosx(3);
		stand2.Draw();
		
		///////////// Phong Lighting ////////////////////////
		//Here the phong lighting shader is bound.
		//This shader is applied to the rings.
		//As the cursor is moved a white lighting effect is shone on the rings.
		phong.Bind();
		GLint objectColorLoc = glGetUniformLocation(phong.GetProg(), "objectColor");
		GLint lightColorLoc = glGetUniformLocation(phong.GetProg(), "lightColor");
		GLint lightPosLoc = glGetUniformLocation(phong.GetProg(), "lightPos");
		GLint viewPosLoc = glGetUniformLocation(phong.GetProg(), "viewPos");
		glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightColorLoc, 0.6f, 0.6f, 0.6f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(phong.GetProg(), "model");
		GLint viewLoc = glGetUniformLocation(phong.GetProg(), "view");
		GLint projLoc = glGetUniformLocation(phong.GetProg(), "projection");
		// Pass the matrices to the shader
		glm::mat4 model;
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));	
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(phong.GetProg(), "cubeMap"), 0);

		//////////// Ring Drawing ////////////////
		//The texture to be applied to the rings is bound.
		//Then each ring is drawn individually.
		ringTex.Bind();
		phong.Update(ring1.GetTransformMat(), camera);
		ring1.Draw();
		phong.Update(ring2.GetTransformMat(), camera);
		ring2.Draw();
		phong.Update(ring3.GetTransformMat(), camera);
		ring3.Draw();

		////////////// Skybox Drawing /////////////////
		//The skybox is drawn here and any translation component of the view matrix is removed
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);
		skyboxShader.Bind();
		//Remove any translation component of the view matrix here
		view = glm::mat4(glm::mat3(camera.GetViewProjection()));	
		skyboxTex.Bind();
		skybox.Draw(skyboxShader.GetProg(), view, projection);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);

		//////////// Game Logic ////////////////

		//This controls the cursor position over the stands.
		//If right is pressed on the keyboard, and no other key is down, this statement is accessed.
		if (display.IsRightPressed() && display.IsKeyDown() == false)
		{
			switch (pointer.GetCurrPos())
			{
			case 1:
				pointer.SetPosx(0);
				pointer.GetCurrPos()++;
				break;
			case 2:
				pointer.SetPosx(3);
				pointer.GetCurrPos()++;
				break;
			default:
				break;
			}
			display.SetKeyDown(true);
			
	
		}

		//This controls the cursor position over the stands.
		//If left is pressed on the keyboard, and no other key is down, this statement is accessed.
		if (display.IsLeftPressed() && display.IsKeyDown() == false)
		{
			switch (pointer.GetCurrPos())
			{
			case 2:
				pointer.SetPosx(-3);
				pointer.GetCurrPos()--;
				break;
			case 3:
				pointer.SetPosx(0);
				pointer.GetCurrPos()--;
				break;
			default:
				break;
			}
			display.SetKeyDown(true);
		}

		//This controls the initial pickup of a ring.
		//This can only be accessed if a ring isn't currently picked by the user and no other key is down.
		if (display.IsSpacePressed() && pickup == false && display.IsKeyDown() == false) {
			//Check where the cursor is.
			switch (pointer.GetCurrPos()) {
			//If it's on the first stand enter here.
			case 1:
				//If rings are available, then their y position is set to move above the stands.
				if (!list1.empty()) {
					//The size of ring is found.
					switch (list1.front()) {
					case 1:
						ring1.SetPosy(2.5);
						currentRing = 1;
						list1.pop_front();
						break;
					case 2:
						ring2.SetPosy(2.5);
						currentRing = 2;
						list1.pop_front();
						break;
					case 3:
						ring3.SetPosy(2.5);
						currentRing = 3;
						list1.pop_front();
						break;
					}
					pickup = true;
				}
				break;
			//If it's on the second stand enter here.
			case 2:
				//If rings are available, then their y position is set to move above the stands.
				if (!list2.empty()) {
					//The size of ring is found.
					switch (list2.front()) {
					case 1:
						ring1.SetPosy(2.5);
						currentRing = 1;
						list2.pop_front();
						break;
					case 2:
						ring2.SetPosy(2.5);
						currentRing = 2;
						list2.pop_front();
						break;
					case 3:
						ring3.SetPosy(2.5);
						currentRing = 3;
						list2.pop_front();
						break;
					}
					pickup = true;
				}
				break;
			//If it's on the third stand enter here.
			case 3:
				//If rings are available, then their y position is set to move above the stands.
				if (!list3.empty()) {
					//The size of ring is found.
					switch (list3.front()) {
					case 1:
						ring1.SetPosy(2.5);
						currentRing = 1;
						list3.pop_front();
						break;
					case 2:
						ring2.SetPosy(2.5);
						currentRing = 2;
						list3.pop_front();
						break;
					case 3:
						ring3.SetPosy(2.5);
						currentRing = 3;
						list3.pop_front();
						break;
					}
					pickup = true;
				}
				break;
			}

			//Sets that a key is down.
			display.SetKeyDown(true);
			//Increases number of player moves to be displayed on game completion.
			moves++;
			
		}

		//This controls the drop of a ring onto a stand.
		//This can only be accessed if a ring is currently picked by the user and no other key is down.
		else if (display.IsSpacePressed() && pickup == true && display.IsKeyDown() == false) {
			//Check where the cursor is.
			switch (pointer.GetCurrPos()) {
			//If it's on the first stand enter here.
			case 1:
				//If other rings are present, then their y position is set to a predetermined height on the stand.
				if (!list1.empty()) {
					//If the current ring held is smaller than the top ring on the stand, then it can be dropped.
					if (currentRing < list1.front()){
						//Check the number of rings on the stand
						//Depending on the number present, the ring selection and height is adjusted.
						switch (list1.size()) {
						case 1:
							if (currentRing == 1) {
								ring1.SetPosy(1);
							}
							else if (currentRing == 2) {
								ring2.SetPosy(1);
							} else {
								ring3.SetPosy(1);
							}
							list1.push_front(currentRing);
							break;
						case 2:
							ring1.SetPosy(1.5);
							list1.push_front(currentRing);
							break;
						}
						//Pickup is reset to false to indicate the player is no longer holding a ring.
						pickup = false;
					}
					//If the current ring is bigger than the one below it, an error sound will play.
					else {
						PlaySound(TEXT("./res/sounds/error.wav"), NULL, SND_SYNC);
					}
				//If a stand is empty, the current ring is set to the bottom of the stand and it's number is added to the list containg the rings for the respective stand
				} else  {
					switch (currentRing) {
					case 1:
						ring1.SetPosy(0.5);
						list1.push_back(currentRing);
						break;
					case 2:
						ring2.SetPosy(0.5);
						list1.push_back(currentRing);
						break;
					case 3:
						ring3.SetPosy(0.5);
						list1.push_back(currentRing);
						break;
					}
					//Pickup is reset to false to indicate the player is no longer holding a ring.
					pickup = false;
				}
				break;
			//If it's on the second stand enter here.
			case 2:
				//If other rings are present, then their y position is set to a predetermined height on the stand.
				if (!list2.empty()) {
					//If the current ring held is smaller than the top ring on the stand, then it can be dropped.
					if (currentRing < list2.front()) {
						//Check the number of rings on the stand
						//Depending on the number present, the ring selection and height is adjusted.
						switch (list2.size()) {
						case 1:
							if (currentRing == 1) {
								ring1.SetPosy(1);
							}
							else if (currentRing == 2) {
								ring2.SetPosy(1);
							}
							else {
								ring3.SetPosy(1);
							}
							list2.push_front(currentRing);
							break;
						case 2:
							ring1.SetPosy(1.5);
							list2.push_front(currentRing);
							break;
						}
						//Pickup is reset to false to indicate the player is no longer holding a ring.
						pickup = false;
					}
					//If the current ring is bigger than the one below it, an error sound will play.
					else {
						PlaySound(TEXT("./res/sounds/error.wav"), NULL, SND_SYNC);
					}
				}
				//If a stand is empty, the current ring is set to the bottom of the stand and it's number is added to the list containg the rings for the respective stand
				else {
					switch (currentRing) {
					case 1:
						ring1.SetPosy(0.5);
						list2.push_back(currentRing);
						break;
					case 2:
						ring2.SetPosy(0.5);
						list2.push_back(currentRing);
						break;
					case 3:
						ring3.SetPosy(0.5);
						list2.push_back(currentRing);
						break;
					}
					//Pickup is reset to false to indicate the player is no longer holding a ring.
					pickup = false;
				}
				break;
			//If it's on the third stand enter here.
			case 3:
				//If other rings are present, then their y position is set to a predetermined height on the stand.
				if (!list3.empty()) {
					//If the current ring held is smaller than the top ring on the stand, then it can be dropped.
					if (currentRing < list3.front()) {
						//Check the number of rings on the stand
						//Depending on the number present, the ring selection and height is adjusted.
						switch (list3.size()) {
						case 1:
							if (currentRing == 1) {
								ring1.SetPosy(1);
							}
							else if (currentRing == 2) {
								ring2.SetPosy(1);
							}
							else {
								ring3.SetPosy(1);
							} 
							list3.push_front(currentRing);
							break;
						case 2:
							ring1.SetPosy(1.5);
							list3.push_front(currentRing);
							break;
						}
						//Pickup is reset to false to indicate the player is no longer holding a ring.
						pickup = false;	
					}
					//If the current ring is bigger than the one below it, an error sound will play.
					else {
						PlaySound(TEXT("./res/sounds/error.wav"), NULL, SND_SYNC);
					}
				}
				//If a stand is empty, the current ring is set to the bottom of the stand and it's number is added to the list containg the rings for the respective stand
				else {
					switch (currentRing) {
					case 1:
						ring1.SetPosy(0.5);
						list3.push_back(currentRing);
						break;
					case 2:
						ring2.SetPosy(0.5);
						list3.push_back(currentRing);
						break;
					case 3:
						ring3.SetPosy(0.5); 
						list3.push_back(currentRing);
						break;
					}
					//Pickup is reset to false to indicate the player is no longer holding a ring.
					pickup = false;
				}
				break;
			}
			
			//Sets that a key is down.
			display.SetKeyDown(true);
			//Increases number of player moves to be displayed on game completion.
			moves++;
		}

		//If a ring is pickup up, then this statement ensures that it follows the selection cursor.
		if (pickup == true) {
			//Check which ring is selected
			switch (currentRing) {
			case 1:
				ring1.SetPosx(pointer.GetTransformMat().GetPos().x);
				break;
			case 2:
				ring2.SetPosx(pointer.GetTransformMat().GetPos().x);
				break;
			case 3:
				ring3.SetPosx(pointer.GetTransformMat().GetPos().x);
				break;
			}
			
		}
		
		//If 'a' is pressed on the keyboard, the camera will orbit round the scene to the left.
		if (display.IsAPressed()) {
			camera.OrbitLeft();
		}

		//If 'd' is pressed on the keyboard, the camera will orbit around the scene to the right.
		if (display.IsDPressed()) {
			camera.OrbitRight();
		}

		//If all the rings are on the third stand, then set finished to true, which reset the game as above.
		if (list3.size() == 3) {
			if (finishCount > 0) {
				finished = true;
				set++;
			}
			finishCount++;
		}

		//Update the display
		display.Update();
		SDL_Delay(1);
	}
	
	return 0;
}

