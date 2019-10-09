#pragma once
/*
		  Mystic Sprite Engine
_______________________________________________
This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:
1.) The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2.) Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3.) This notice may not be removed or altered from any source distribution.

*/

///      SPRITES 
/*=============================================================================================================================================================
//
//      SPRITE MODULE
//
=============================================================================================================================================================*/

/* =============================================================================================================================================================
										   //
Scene									   //
== == =									   //
--Group									   //
---- Sprite								   //

_________________						   //
-------- UVBuffer						   //
------------ Animation					   //  Set of frames Acting like a State for instance Jumping or Running
---------------- Frame					   //  UV texture Coords to select from the Sprites Image sheet
-------- VAO							   //  Obviously the Quad for the Sprite. Once batched however becomes irrelevent
-----------~ Indices					   //  
-----------~ Vertices					   //
-----------~ Normals					   //
-----------~ Colors                   	   //
=============================================================================================================================================================*/

#include"Common.h"




class Mesh;
class Sprite;


struct Grid
{// This so a Grid can be preCreated and sent into the Animation class to chop up the SpriteSheet
    Grid(int _rows, int _cols, int _cellWidth, int _cellHeight, int _border)
        :
        Width(_cellWidth),
        Height(_cellHeight),
        Rows(_rows),
        Columns(_cols),
        Border(_border) 
    {}

    int Width, Height, Border;
    int Rows, Columns;
};


struct Quad
{// Denotes a single Quad on the Sprite Sheet
	Quad()
		:
		x(NULL), y(NULL), Width(NULL), Height(NULL)
	{};

	Quad(int X, int Y, int W, int H)
		: x(X), y(Y), Width(W), Height(H)
    {}

	int x, y;
	int Height, Width;
};

struct Frame
{
public:
    Frame() = default;
	Frame(Bitmap &img, Quad f);

 	Vec2 Coords[4];

	GLuint FrameID = 0;

    GPUptr Picture;
	void Bind(Shader &_shader);
	void Unbind();
};
class Animation
{
public:
	Animation()
        :
        AnimationSpeed(NULL),
        CurrentFrame(NULL),
        TotalFrames(NULL)
    {}
	~Animation() {}

	Animation(Bitmap &img, Quad sourcerect, unsigned int numframes);
	Animation(Bitmap &img, Quad srcrect, int border, GLuint numframes);

	std::vector<Frame> Frames;

	unsigned int TotalFrames;
	unsigned int CurrentFrame;

	GLint  AnimationSpeed;
  //  std::vector<Texture>Picture;
	Animation operator +=(Frame newframe)
	{
		this->AddFrame(newframe);
		return *this;
	}
	Animation operator +=(Animation newstate)
	{
		for_loop(count, newstate.TotalFrames)
		{
			this->AddFrame(newstate.Frames[count]);
			this->TotalFrames++;
		}
		return *this;
	}

	bool PlayOnce;
	bool Stopped;

	void Bind(Shader &_shader);
	void Unbind();
private:
	std::vector<Frame>& AddFrame(Frame frame);
};


class Sprite 
	:
	public Mesh
{
public:
    Sprite() = default;
	Sprite(Bitmap &source, Vec2 dimensions, int numstates);
	Sprite(Bitmap &source, Vec2 dimensions);

	char *Name;

	int ID;
	GLuint CurrentState;
	GLuint NumberofStates;
    Texture *SpriteSheet;

	float AnimationLength;
	float Timer;

	std::vector<Animation> Animations;

// _______________________________________________________________________________________________________________________
	void SetAnimationSpeed(float speed);
	void ResetFrames();
	void AddAnimation(Animation state);
	void AddAnimation(int statenum, Animation state);

    bool Moving;     // Is it currently Animating
    bool Composite;  // Is it made up of multiple sprites

    Mat4 /// This will all go away in the Future, only for Initial test If still in existance remove
        ProjectionMatrix,
        ViewMatrix,
        ModelMatrix;
public:

	void Bind(Shader &_shader) override;
	void Render(Shader &_shader);

	void Unbind() override;
	void Update() override;

};

