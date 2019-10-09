#pragma once

#include<stack>
#include"Renderer.h"
//#include"GameObject.h"

//== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == 
///        CAMERA 
/*===============================================================================================================================================================

CAMERA MODULE

  Data Members
  ------------
  _______
  PUBLIC:
  _______
											  Floating points
											  ---------------
  FOV                                         - Field of view in Degrees
  AspectRatio                                 - Aspect Ratio of the Screen
  Near,                                       - Near clipping range
  Far;                                        - Far clipping range
											  Mat4 Matrix
											  ---------------
  Matrix   ProjectionMatrix; 				  - Current Projection Matrix
  Matrix   ViewMatrix;						  - Current ViewMatrix

											  - std::stack<Matrix>
											  ----------------------
  std::stack<Matrix> ViewMatrixStack;         - Current View Matrix Stack.
  std::stack<Matrix> ProjectionMatrixStack;   - Current Projection Matrix Stack.
											  - Viewport
											  ------------------
  static Viewport *Camera;                    - Static Currently Active Camera
  ________
  PRIVATE:
  ________
											  - Vec3
											  ------------------
  Position,									  - Current Position state
  Rotation,									  - Current Rotation state
  Forward,									  - Current Forward Vector the direction the Camera is facing
  Right,									  - Current Right facing Vector
  Up;                                         - Current Up Vector
											  - Matrix
											  -----------
  Matrix Identity = Matrix(1.0f);             - Returns an Identity Matrix
________________________________________________________________________________________________________________________________________________________________________________________________________________________________
================================================================================================================================================================================================================================
  API:
   Viewport(Vec3 positon, Vec3 Rotation);
   --------------------------------------

   void PushProjectionMatrix();               -  Pushes the Current Camera Projection Matrix to the Stack
   void PushViewMatrix();                     -  Pushes the Current Camera View Matrix to the Stack
   void PopProjectionMatrix();	              -  Pop the Current Camera Projection Matrix to the Stack setting the previous Matrix as the current
   void PopViewMatrix();		              -  Pop the Current Camera View Matrix to the Stack setting the previous Matrix as the current

   //=======================================================================================================================================================
   // FUNCTIONS FOR THE CAMERA CLASS
   //=======================================================================================================================================================

   Matrix GetViewMatrix();                    - Return Current Active ViewMatrix

   void Update();				              - Updates the Transforms for Camera Location
   void Bind();					              - Sets the Uniforms of the Currently Active Shader for the Projection and Model Matrices
   void Unbind();				              - Currently Nothing
   void Render();				              - FUTURE: Render The Scene Associate with the Camera here, possibly shadow mapping but that might get contained in the Light class

   void Rotate(float pitch, float yaw);       - Set the current Rotation of the Camera
   void MoveForward(float speed);			  - Move the current Position Forward at @param Speed
   void MoveBack(float speed);				  - Move the current Position back at @param Speed
   void MoveLeft(float speed);				  - Move the current Position Left at @param Speed
   void MoveRight(float speed);				  - Move the current Position Right at @param Speed
   //=======================================================================================================================================================

   void RotateX(GLfloat Angle);				  - Apply X Axis Rotation
   void RotateY(GLfloat Angle);				  - Apply Y Axis Rotation
   void RotateZ(GLfloat Angle);				  - Apply Z Axis Rotation
   void Translate(Vec3 pos);				  - Apply Transform for the Current Position

   void Set_ViewMatrix(Matrix mat);           - Set the Current view Matrix via @param Matrix mat
   void Set_ProjectionMatrix(Matrix mat);     - Set the Current Projection Matrix via @param Matrix mat
=================================================================================================================================================================
*/

class Viewport 
{
public:
	Viewport() = default;

	float
		FOV = 60,
		AspectRatio,
		Near,
		Far;

	void PushProjectionMatrix();
	void PushViewMatrix();
	void PopProjectionMatrix();
	void PopViewMatrix();

	Mat4 ProjectionMatrix = Mat4(0);
	Mat4 ViewMatrix = Mat4(0);
	Mat4 Identity = Mat4(1.0f);
	//=======================================================================================================================================================
	// FUNCTIONS FOR THE CAMERA CLASS
	//=======================================================================================================================================================

	Mat4 GetViewMatrix();

	//=======================================================================================================================================================

	void Set_ViewMatrix(Mat4 mat);
	void Set_ProjectionMatrix(Mat4 mat);

	static Viewport *Camera;
	static void SetCamera(Viewport* _cam) { Camera = _cam; }
private:

	std::stack<Mat4> ViewMatrixStack;
	std::stack<Mat4> ProjectionMatrixStack;
};


Mat4 OrthographicMatrix(int width, int height);

class Camera3D : public Viewport
{
public:
	Camera3D() = default;
	Camera3D(Vec3 position, Vec3 Rotation);	
	
	Mat4 LookAt(Vec3 eye, Vec3 target, Vec3 up);

	void RotateX(GLfloat Angle);
	void RotateY(GLfloat Angle);
	void RotateZ(GLfloat Angle);
	void Translate(Vec3 pos);

	void Rotate(float pitch, float yaw);
	void MoveBack(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);
	void MoveForward(float speed);
	Vec3
		Position = { 0,0,0 },
		Rotation = { 0,0,0 },
		Forward = { 0,0,0 },
		Right = { 0,0,0 },
		Up = { 0,0,0 };

	Vec3 
		Target_Position = Vec3(0),
		Target_Rotation = Vec3(0);

	Vec3 Camera_Direction = Vec3(0);
	float Delta_Time = 0.0f;
	float Current_Time = 0.0f;

	void Update();
	void Bind();
	void Bind(Shader* _shader);

	void Unbind();
	void Render();
};

class Camera2D : public Viewport
{
public:
	Camera2D() = default;
	Camera2D(Vec2 size);
    Camera2D(float _left, float _right, float _top, float _bottom);
    Vec2 Position;
	float Rotation;
 
	void Update();
	void Bind(Shader *_shader);

	float
		Top,
		Bottom,
		Right,
		Left;

	void Rotate(float _angle);
	const int Height() { return Size.y; }
	const int Width() { return Size.x; }

	void Translate(Vec2 pos);
private:
	Vec2 Size;
};

//=======================================================================================================================================================
//
//                                               NOTES
//
//=======================================================================================================================================================

//	glm::mat4 Pitch = glm::rotate(Identity, Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
//	glm::mat4 Yaw   = glm::rotate(Identity, Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
//	glm::mat4 Roll  = glm::rotate(Identity, Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
//	ViewMatrix = (Roll * Pitch *  Yaw) * (glm::translate(glm::mat4(1.0f), -Position));
//    0           1            2         3
//0  RightX      RightY      RightZ      0
//1  UpX         UpY         UpZ         0
//2  LookX       LookY       LookZ       0
//3  PosX        PosY        PosZ        1

//
//
//class Camera2d
//{
//public:
//	inline void	set_zoom(const float zoom) { _zoom = zoom; }
//	inline float get_zoom()const { return _zoom; }
//
//protected:
//	void recreate_tranformation_matrix();
//
//protected:
//	// Saves Last Property
//	// This is used for knowing when to update the Matrix
//	struct Transformation
//	{
//		Mat4       Transform;
//		Vec2         _last_pos;
//		float        _last_zoom;
//		Vec2        _last_origin;
//		float        _last_rotation;
//
//		void Update(const Vec2& p, const Vec2& origin, const float zoom, const float rotation)
//		{
//			_last_pos = p;
//			_last_origin = origin;
//			_last_zoom = zoom;
//			_last_rotation = rotation;
//		}
//	};
//protected:
//	float		_zoom;
//	Transformation  _transform;
//public:
//	Vec2	_pos;
//	Vec2	_origin;
//	float	_rotation;
//};