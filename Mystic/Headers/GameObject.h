#pragma once
#include"Asset.h"

class Shader;

class GameObject
{
public:
	GameObject()
		:
		Scale(0),
		Position(0),
		Rotation(0),
		Transform(Mat4(0)),
        Object_ID(ObjectCount++)
	{
		Transform = glm::mat4(1.0f); //  Set Identity and Rotate all axis followed with the Translation.
		Transform = glm::translate(Transform, Position);
		Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
		Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
		Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
		
	}
	GameObject(Vec3 _pos, Vec3 _rot, Vec3 _scale)
		:
		Position(_pos),
		Rotation(_rot),
		Scale(_scale),
        Object_ID(ObjectCount++)
	{
		Transform = glm::mat4(1.0f); //  Set Identity and Rotate all axis followed with the Translation.
		Transform = glm::translate(Transform, Position);
		Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
		Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
		Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
	}

	virtual void Update() = pure_virtual;
	virtual void Unbind() = pure_virtual;

    virtual void Bind(Shader &_shader) = pure_virtual;
	virtual void Render(Shader &_shader) = pure_virtual;

	TagID GetID() { return Object_ID; }

	Mat4 Transform;
	Vec3 Position, Rotation, Scale;

	static int ObjectCount;
private:
	TagID Object_ID = 0;
};