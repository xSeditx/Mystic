#include"camera.h"
#include"Shader.h"
//#include"Uniform.h"
Viewport *Viewport::Camera; // CURRENT MAIN CAMERA IN USE. Possibly not going to be needed soon.
///TODO: Camera Tutorial for Smoothing the Movement: https://learnopengl.com/Getting-started/Camera
void Viewport::PushProjectionMatrix()
{
	ProjectionMatrixStack.push(ProjectionMatrix);
}
void Viewport::PushViewMatrix()
{
	ViewMatrixStack.push(ViewMatrix);
}
void Viewport::PopProjectionMatrix()
{
	ProjectionMatrix = ProjectionMatrixStack.top();
	ProjectionMatrixStack.pop();
}
void Viewport::PopViewMatrix()
{
	ViewMatrix = ViewMatrixStack.top();
	ViewMatrixStack.pop();
}
void Viewport::Set_ViewMatrix(Mat4 mat)
{
	ViewMatrix = mat;
}
void Viewport::Set_ProjectionMatrix(Mat4 mat)
{
	ProjectionMatrix = mat;
}

Mat4 Viewport::GetViewMatrix()
{
	return ViewMatrix;
}

// HELPER FUNCTIONS
Mat4 OrthographicMatrix(int width, int height)
{
	return glm::ortho(0.0f, (float)width, (float)height, 0.0f, -0.0f, 1000.0f);
}

Camera3D::Camera3D(Vec3 position, Vec3 rotation)
{
	FOV = 60.0;
	AspectRatio = 640.0f / 480.0f;
	Near = .10f;
	Far = 4096.0f;

	Position = position;
	Rotation = rotation;

	Forward = Vec3(0.0f, 0.0f, 1.0f);
	Right = Vec3(0.1f, 0.0f, 0.0f);
	Up = Vec3(0.0f, 1.0f, 0.0f);

	ProjectionMatrix = glm::perspective(glm::radians(FOV), AspectRatio, Near, Far);
	ViewMatrix = Mat4(1.0f);
	Camera = this;
}

// GameObject API INTERFACE FUNCTIONS
void Camera3D::Bind()
{
 //	Shader::GetActiveShader()->ProjectionMatrix.Set(ProjectionMatrix);
 //	Shader::GetActiveShader()->ViewMatrix.Set(ViewMatrix);
	__debugbreak();
	//uniform_ProjectionMatrix.Set(ProjectionMatrix);
	//uniform_ViewMatrix.Set(ViewMatrix);
}
void Camera3D::Bind(Shader *_shader)
{
//	_shader->SetUniform("EyePosition", Position);
//	_shader->SetUniform("ProjectionMatrix", ProjectionMatrix);
// 	_shader->SetUniform("ViewMatrix", ViewMatrix);
}

void Camera3D::Unbind()
{
}
void Camera3D::Update()
{
	//Camera_Direction = glm::normalize();
	float Time = glfwGetTime();
	Delta_Time = (Time - Current_Time);
	Current_Time = Time;
	 
	Position += (Target_Position - Position) * 10.f * Delta_Time;
	Rotation += (Target_Rotation - Rotation) * 10.f * Delta_Time;
	//Position.y += (Obj.y - position.y) * lerp * Delta_Time;


	//Position = Target_Position;

	ViewMatrix = Mat4(1.0);

	RotateX(Rotation.x);
	RotateY(Rotation.y);
	Translate(Position);

//	RotateZ(Rotation.z);/// THIS WAS ROTATE Y.... I believe that was in Error....

	Up = glm::normalize(Vec3(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]));
	Right = glm::normalize(Vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]));
	Forward = glm::normalize(Vec3(ViewMatrix[0][2], ViewMatrix[1][2], ViewMatrix[2][2]));
	ViewMatrix = glm::lookAt(Position, Position - Forward, Up);
}
void Camera3D::Render()
{
}

// CAMERA MANIPULATIONS
void Camera3D::Rotate(float pitch, float yaw)
{
	Target_Rotation.x -= yaw * RADIANS(.9); 
	Target_Rotation.y -= pitch * RADIANS(.9);  
}
void Camera3D::MoveBack(float speed)
{
	Target_Position	-= (speed * Forward);
}
void Camera3D::MoveLeft(float speed)
{
	Target_Position	-= (speed)* Right;
}
void Camera3D::MoveRight(float speed)
{
	Target_Position	+= (speed)* Right;

}
void Camera3D::MoveForward(float speed)
{
	Target_Position	+= (speed * Forward);
}

// MATRIX TRANSFORMERS
void Camera3D::RotateX(GLfloat angle)
{
	ViewMatrix *= Mat4
	{
		1.0f,       0.0f,        0.0f, 0.0f,
		0.0f, cos(angle), -sin(angle), 0.0f,
		0.0f, sin(angle),  cos(angle), 0.0f,
		0.0f,       0.0f,        0.0f, 1.0f
	};
}
void Camera3D::RotateY(GLfloat angle)
{
	ViewMatrix *= Mat4
	{
		cos(angle), 0.0f,-sin(angle), 0.0f,
			  0.0f, 1.0f,       0.0f, 0.0f,
		sin(angle), 0.0f, cos(angle), 0.0f,
			  0.0f, 0.0f,       0.0f, 1.0f
	};
}
void Camera3D::RotateZ(GLfloat angle)
{
	ViewMatrix *= Mat4
	{
		cos(angle), 0.0f,-sin(angle), 0.0f,
		sin(angle), 1.0f, cos(angle), 0.0f,
			  0.0f, 0.0f,       1.0f, 0.0f,
			  0.0f, 0.0f,       0.0f, 1.0f
	};
}

void Camera3D::Translate(Vec3 pos)
{
	ViewMatrix *= (glm::translate(glm::mat4(1.0f), pos));
}
Mat4 Camera3D::LookAt(Vec3 eye, Vec3 target, Vec3 up)
{
	Vec3 vz = glm::normalize(eye - target);
	Vec3 vx = glm::normalize(glm::cross(up, vz));
	// vy doesn't need to be normalized because it's a cross
	// product of 2 normalized vectors
	Vec3 vy = glm::cross(vz, vx);
    Mat4 RETURN = // Inverse Matrix
    	Mat4(
    		Vec4(vx, 0),
    		Vec4(vy, 0),
    		Vec4(vz, 0),
    		Vec4(eye, 1)
    	);
	return RETURN; //inverseViewMatrix.inverse();
}// MATRIX STATE HANDLING


Camera2D::Camera2D(Vec2 size)
	:
	Size(size)
{
    TODO("Implement a 2D Camera Abstraction");
	//FOV = 60.0;
	AspectRatio = 640.0f / 480.0f;
	Near = -1.0f;
	Far = 1.0f;


	Position = Vec3(0);
	Rotation = 0;

	Top = 0;
	Left = 0;
	Bottom = size.x;
	Right = size.y;

	ProjectionMatrix = glm::ortho(0.0f, size.x, size.y, 0.0f, -1.0f, 1.0f);  //OrthographicMatrix(size.x, size.y);
	ViewMatrix = glm::translate(Mat4(1.0f), Vec3(-100,0,0));
	Camera = this;
}

Camera2D::Camera2D(float _left, float _right, float _top, float _bottom)
    :
    Top(_top),
    Left(_left),
    Bottom(_bottom),
    Right(_right),
    Size
    (
        Vec2(_right - _left, _bottom - _top)
    )
{

}


void Camera2D::Rotate(float _angle)
{

}
void Camera2D::Bind(Shader *_shader)
{
	_shader->SetUniform("ProjectionMatrix", ProjectionMatrix);
	_shader->SetUniform("ViewMatrix", ViewMatrix);
}
void Camera2D::Update()
{
	ViewMatrix = Mat4(1.0);
	Translate(Position);
}
void Camera2D::Translate(Vec2 pos)
{
	ViewMatrix *= (glm::translate(glm::mat4(1.0f), Vec3(pos.x, pos.y, 1.0f)));
}