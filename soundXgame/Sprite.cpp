#include "Sprite.h"
#include "TargetGrabber.h"

Sprite::Sprite(void)
{

}

Sprite::Sprite(char* filename)
{
	verts.push_back(Vector3(-1,1,0));
	verts.push_back(Vector3(1,1,0));
	verts.push_back(Vector3(1,-1,0));
	verts.push_back(Vector3(-1,-1,0));

	uvs.push_back(glm::vec2(0,0));
	uvs.push_back(glm::vec2(1,0));
	uvs.push_back(glm::vec2(1,1));
	uvs.push_back(glm::vec2(0,1));

	FaceShape=GL_QUADS;

	glm::vec3 temp1 = glm::vec3(0,0,-1);
	glm::vec3 temp2 = glm::vec3(1,0,0);
	glm::vec3 temp3 = glm::vec3(0,1,0);

	transform.forward = Vector3(temp1.x,temp1.y,temp1.z);
	transform.right = temp2;
	transform.up = temp3;

	//verts.push_back(glm::vec3(temp1.x,temp1.y,temp1.z));
	//verts.push_back(glm::vec3(temp2.x,temp2.y,temp2.z));
	//verts.push_back(glm::vec3(temp3.x,temp3.y,temp3.z));

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);


	IGObject::InitializeObject(true);

	SetUp(filename,true);
	SetName(filename);
}


Sprite::~Sprite(void)
{

}


void 
Sprite::SetUp(string textureFile,bool backFace)
{
	SetID(SCENE->Add(this));
	LockID();
	LoadTexture(textureFile);
	this->NoBackfaceCulling = backFace;
}

void 
Sprite::SetUp(Texture texture,bool backFace)
{
	SetID(SCENE->Add(this));
	LockID();
	this->texture = texture;
	this->NoBackfaceCulling = backFace;
}