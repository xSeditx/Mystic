#include<memory>
#include<chrono>

#include"Application.h"
#include"OpenGL.h"
#include"Mesh.h"
#include"Sprite.h"


//=========================================================================================================================================
//_______________________________________  Frame Class ____________________________________________________________________________________
//                                                                                                                                  
//Each Frame is a UV buffer whos Index is Bound prior to rendering in accordance with the current timestep for the Animated Sprite
//=========================================================================================================================================
// I suppose I could also store the Frame Data linearly here and
// Offset to the frame that I wish to render from
// That might be the best method honestly.


Frame::Frame(Bitmap& img, Quad f) // IDK, possibly pass the UnitX/Y instead of Image. Like to just pass Quad but how... Normalize in Shader? Use Values for finding proper index? Suck it up and just leave it the fuck alone....
{
    float UnitX = (1.0f / img.Width());
    float UnitY = (1.0f / img.Height());
    float UnitXH = UnitX * .5f;
    float UnitYH = UnitY * .5f;
    
    
    	Coords[0] = Vec2((float)f.x * UnitX + (float)f.Width  * UnitX - UnitXH,
    		             (float)f.y * UnitY + (float)f.Height * UnitY - UnitYH);
    
    	Coords[1] = Vec2((float)f.x * UnitX + UnitXH,
    		             (float)f.y * UnitY + (float)f.Height * UnitY - UnitYH);
    
    	Coords[2] = Vec2((float)f.x * UnitX + UnitXH,
    		             (float)f.y * UnitY + UnitYH);
    
    	Coords[3] = Vec2((float)f.x * UnitX + (float)f.Width * UnitX - UnitXH,
    	           	     (float)f.y * UnitY + UnitYH);
    
	_GL(  glGenBuffers(1, &FrameID) );
	_GL(  glBindBuffer(GL_ARRAY_BUFFER, FrameID) ); 
    _GL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * 4, Coords, GL_STATIC_DRAW));
    _GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
void Frame::Bind(Shader &_shader)
{
    _GL( glBindBuffer(GL_ARRAY_BUFFER, FrameID) ); 	// or even better an index that resprents how much to multiply U and V by to get the correct coords since that could greatly increase performance....
    GLint TCoords = glGetAttribLocation(_shader.g_ID(), "TextureCoord");

    _GL( glEnableVertexAttribArray(TCoords));
    _GL( glVertexAttribPointer(TCoords, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)NULL) );
}
void Frame::Unbind()
{
    _GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

//=========================================================================================================================================
//_______________________________________  Animation Class ____________________________________________________________________________________
//                                                                                                                                  
//   Each Animation is a state that an Entity can be in, Dead alive etc and is a group of frames that represents a state of the StaticSprite
//=========================================================================================================================================

Animation::Animation(Bitmap &spr, Quad srcrect, GLuint numframes)
	:
	TotalFrames(numframes),
	AnimationSpeed(NULL),
    PlayOnce(false)
{
	CurrentFrame = 0;
	Frames.reserve(numframes);
	GLuint x = srcrect.x;
	for_loop(Count, numframes)
	{
		Quad  Source(x, srcrect.y, srcrect.Width, srcrect.Height);
		Frame frame(spr, Source);
		x += srcrect.Width;
		*this += frame; /// warning C26444: Avoid unnamed objects with custom construction and destruction (es.84).
	}
}
Animation::Animation(Bitmap &spr, Quad srcrect, int border, GLuint numframes) // SAME HERE
	:
	TotalFrames(numframes),
	AnimationSpeed(NULL),
    CurrentFrame(NULL),
    PlayOnce(false)
{
	Frames.reserve(numframes);

	GLuint x = srcrect.x;
    AnimationSpeed = 100;
	for_loop(Count, numframes)
	{
		Quad  Source(x, srcrect.y, srcrect.Width, srcrect.Height);
		Frame frame(spr,Source);
		x += srcrect.Width + border;
		*this += frame; ///warning C26444: Avoid unnamed objects with custom construction and destruction (es.84).
	}
}
void Animation::Bind(Shader &_shader)
{
	Frames[CurrentFrame].Bind(_shader);
}
void Animation::Unbind()
{
	Frames[CurrentFrame].Unbind();
}
std::vector<Frame>& Animation::AddFrame(Frame frame)
{// Adds a frame to the Animation
	Frames.emplace_back(frame);
	return Frames;
} 


//=========================================================================================================================================
//_______________________________________  Sprite Class ___________________________________________________________________________________
//                                                                                                                                  
// Sprite is a Collection of States which can be Enumerated or Defined so Various Animations can play our in accordance with Gameplay
//=========================================================================================================================================


Sprite::Sprite(Bitmap &source, Vec2 dimensions)
     :
     Mesh(),
     ProjectionMatrix(glm::ortho(0.0f, 960.0f, 1280.0f, 0.0f, -1.0f, 1.0f)),
     ViewMatrix(glm::translate(Mat4(1.0f), Vec3(-100, 0, 0))),
     SpriteSheet(new Texture(source)),
     AnimationLength(NULL),
     CurrentState(NULL),
     NumberofStates(1),
     Moving(true),
     Timer(NULL),
     ID(NULL)
{ // Fix this function to make a single Sprite out of an Image so its a basic sprite with no animation
     __debugbreak();// Possibly derive static sprite
	*this = Sprite(source, dimensions, 1);
     AddAnimation(0, Animation(source, { 0, 0, (int)source.Width(), (int)source.Height() }, 1)); // Single hand Item Display
	 this->Rotation = Vec3(0);
}

Sprite::Sprite(Bitmap &source, Vec2 dimensions, int numstates)
	:
    Mesh(), 
    ProjectionMatrix(glm::ortho(0.0f, 960.0f, 1280.0f, 0.0f, -1.0f, 1.0f)), 
    ViewMatrix(glm::translate(Mat4(1.0f), Vec3(-100, 0, 0))),
    SpriteSheet(new Texture(source)),
	NumberofStates(numstates),
    AnimationLength(NULL),
	CurrentState(NULL),
	Moving(true),
    Timer(NULL),
	ID(NULL)
{ // Dimensions are those On the Screen. How large it will be displayed

    Animations.reserve(numstates);
	for_loop(count, NumberofStates)
	{
		Animation temp;
		Animations.emplace_back(temp);
	}

	Vec2 VertexData[] =  // DO I SERIOUSLY NOT HAVE A SIMPLE QUAD ABSTRACTED OUT YET? WHAT IS WRONG WITH ME!
	{
		Vec2(0,0),
		Vec2(dimensions.x, 0),
		Vec2(dimensions.x, dimensions.y),
		Vec2(0, dimensions.y),
	};
	GLuint IndexData[] =
	{
		0,3,1, 2,1,3
	};
	Vec2 Coord[] =
	{
		Vec2(0.0f,0.0f),
		Vec2(1.0f,0.0f),
		Vec2(1.0f,1.0f),
		Vec2(0.0f,1.0f)
	};

    VAO = new VertexArrayObject();
	VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec2>(&VertexData[0], 4));
    VAO->Attach(BufferTypes::UVCOORD, new VertexBufferObject<Vec2>(&Coord[0], 4));
	VAO->Attach(BufferTypes::INDICE, new VertexBufferObject<GLuint>(&IndexData[0], 6));

	ResetFrames();
}
void Sprite::ResetFrames()
{
	for (auto &A : Animations) //Animations is Equal to STATES[] in my past implementation. 
	{
		A.CurrentFrame = 0;
	}
}
void Sprite::Update()
{
    Transform = // Rotate and Translate
        glm::rotate
        (
            glm::translate
            (
                glm::mat4(1.0f),
                Position
            ),
            glm::radians(0.0f),// Just make this a rotation angle
            Vec3(0.0f, 0.0f, 1.0f)
        );

 	float DeltaTime = (glfwGetTime() * 1000) - Timer; //clock() - Timer;// std::chrono::high_resolution_clock::now() - Timer;//

	if (Moving)
	{ // Checks to see if Animation should even be currently playing.
		if (DeltaTime > Animations[CurrentState].AnimationSpeed) // / (CLOCKS_PER_SEC / 1000)
		{
			Timer = glfwGetTime() * 1000; // clock(); // 
			//float ticks = (AnimationTimer * 1000);
			Animations[CurrentState].CurrentFrame += 1;// % Animations[CurrentState].TotalFrames; // Fix this so that each sprite has its own Timer  // (int)(ticks / Animations[CurrentState].AnimationSpeed)
			Animations[CurrentState].CurrentFrame %= Animations[CurrentState].TotalFrames;
			if (Animations[CurrentState].PlayOnce == true && Animations[CurrentState].CurrentFrame == Animations[CurrentState].TotalFrames)
			{ // Checks to see if the Animation is only supposed to play a single time then stop.
				Animations[CurrentState].CurrentFrame = 0; // Maybe remove this. Perhaps a Default frame would be an OK idea.
				Moving = false;
			}
		}
	}
}
void Sprite::SetAnimationSpeed(float speed)
{
	for (auto &A : Animations)
	{
		A.AnimationSpeed = speed;
	}
}
void Sprite::Render(Shader &_shader)
{
	Update();
	Bind(_shader);
    _GL( glDrawElements(GL_TRIANGLES, VAO->ElementCount, GL_UNSIGNED_INT, nullptr) );
}

void Sprite::Bind(Shader &_shader)
{
	VAO->Bind();
    Mat4 Identity = Mat4(1.0f);

    _shader.SetTextureUniform("DiffuseTexture", SpriteSheet->Handle);
    _shader.SetUniform("ProjectionMatrix", ProjectionMatrix);
    _shader.SetUniform("ViewMatrix", Identity);
    _shader.SetUniform("ModelMatrix", Transform);
    Animations[CurrentState].Bind(_shader);
}
void Sprite::Unbind()
{
	VAO->Unbind();
	Animations[CurrentState].Unbind();
}
void Sprite::AddAnimation(Animation state)
{
	Animations[CurrentState] = state;
}
void Sprite::AddAnimation(int statenum, Animation state)
{
	Animations[statenum] = state;
}