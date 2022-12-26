#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
int count = 1800;
int count2 = 1800;
int tmpPlaces[] = {-2, -2, -2};
float angle = 0.0f;
vec3 transPosition;
mat4 priviousTrans[27];
static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			if (mat[j][i] > -0.5 && mat[j][i] < 0.5)
				printf("%d ", 0);
			else
				std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}

Game::Game() : Scene()
{
}

Game::Game(float angle ,float relationWH, float near1, float far1) : Scene(angle,relationWH,near1,far1)
{ 
}

void Game::Init()
{		
	AddShader("../res/shaders/pickingShader");	
	AddShader("../res/shaders/basicShader");
	
	AddTexture("../res/textures/plane.png",false);
	int indx = 0;
	int mul = 1;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				AddShape(Cube, -1, TRIANGLES);
				SetShapeTex(indx, 0);
				shapes[indx]->MyScale(vec3(0.5, 0.5, 0.5));
				shapes[indx]->MyTranslate(vec3(mul * i, mul * j, mul * k), 0);
				indx++;
			}
		}
	}

	
	
	pickedShape = 0;
	
	MoveCamera(0,zTranslate,10);
	pickedShape = -1;
	
	//ReadPixel(); //uncomment when you are reading from the z-buffer
}


void Game::changePos(int* places, float ang, glm::vec3 transPos) {
	if (count == 1800 && count2 == 1800) {
		angle = ang/count;
		count = 0;
		tmpPlaces[0] = places[0];
		tmpPlaces[1] = places[1];
		tmpPlaces[2] = places[2];
		transPosition = transPos;
		}
}

void Game::arrowPressed(float ang, glm::vec3 transPos) {
	if (count == 1800 && count2 == 1800) {
		transPosition = transPos;
		angle = ang/1800;
		count2 = 0;

	}
}


void Game::printPlace(int x, int y, int z) {
	std::cout << x;
	std::cout << " ";
	std::cout << y;
	std::cout << " ";
	std::cout << z << std::endl;
}
void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal",Model);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else 
		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	
	
	if (count < 1800) {
		int mode = -1;
		for (int indx = 0; indx < 27; indx++) {
			mat4 checkPlace = shapes[indx]->MakeTrans();
			for (int a = 0; a < 4; a++)
				for (int b = 0; b < 4; b++) {
					checkPlace[a][b] = round(checkPlace[a][b]);
				}
			vec3 checking = vec3(checkPlace[3][0], checkPlace[3][1], checkPlace[3][2]);
			bool check = false;
			if (tmpPlaces[0] > -2) {
				check = checking[0] == tmpPlaces[0];
				//mode = 1;
			}
			else if (tmpPlaces[1] > -2) {
				check = checking[1] == tmpPlaces[1];
				//mode = 0;

			}
			else if (tmpPlaces[2] > -2) {
				check = checking[2] == tmpPlaces[2];

			}
			if (check) {
				vec3 tmp = transPosition * mat3(shapes[indx]->getRot());
				shapes[indx]->MyRotate(angle, tmp, 0);

			}
		}
		count++;
	}

	if (count2 < 1800) {
		for (int indx = 0; indx < 27; indx++) {
			vec3 tmp = transPosition * mat3(shapes[indx]->getRot());
			shapes[indx]->MyRotate(angle, tmp, 0);
		}
		count2++;
	}
	
	
	
	s->Unbind();

}

void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}

void Game::Motion()
{
	if(isActive)
	{
	}
}

Game::~Game(void)
{
}
