#pragma once

#include"GLBuffers.h"
#include"Renderer.h"

class Shader;



class ShadowMap
{
public:
	ShadowMap();
	void Bind();
	void Unbind();
	void Render(Vec3 _position);

	GLuint GetShadowMap()
	{
		return DepthTexture;
	}
	Mat4 GetDepthMatrix() 
	{ 
		return LightMatrix;
	}
	GLuint FrameBufferID;
	GLuint DepthTexture;
	uint64_t DepthTextureHandle;

	Vec3 LightPosition;
	Mat4 LightMatrix;
	Mat4 ViewMatrix;
	Mat4 ProjectionMatrix;
	//FrameBufferObject *FrameBuffer;
	Shader *Program;

	void RenderScene(Scene& _scene);

	Mat4 biasMatrix = Mat4
    (
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);


	void InitializeGodRays()
	{
		GodRayShader = new Shader("Resources\\GodRays.sfx");

		glGenFramebuffers(1, &GodRayFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, GodRayFrameBuffer);

		glGenTextures(1, &GodRayFBO_Texture);
		glBindTexture(GL_TEXTURE_2D, GodRayFBO_Texture);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1280, 960, 0, GL_RGBA, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GodRayFBO_Texture, 0);

		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_NONE);

		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			__debugbreak();
		}
 		//Program->AttributeLocation("VertexPosition");
		ProjectionMatrix = glm::ortho<float>(-1000, 1000, -1000, 1000, -2500, 2000);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void RenderGodray(Scene& _scene)
	{
		GodRayShader->Enable();
		//Program->SetUniform("LightScreenPosition", LightPosition);
		Program->SetUniform("ProjectionMatrix", Viewport::Camera->ProjectionMatrix);
		Program->SetUniform("ViewMatrix", Viewport::Camera->ViewMatrix);
		Program->SetUniform("ModelMatrix", Mat4(1.0) );

		_scene.Render(*GodRayShader);
		GodRayShader->Disable();
	}

	Mat4 GodrayProjectionMatrix = Mat4(0);

	Shader *GodRayShader;
	GLuint GodRayFrameBuffer = 0;
	GLuint GodRayFBO_Texture = 0;
};


//  1):Render offscreen with a FBO : the light source and the occluding objects, no shaders involved here.In order to save cycles, you can render to a lower resolution(factor 2 gives good results) and disable texturing / depth testing.
//  2):Clean the depth buffer, render the scene normally to the framebuffer.
//  3):Switch to Orthogonal projection and blend the FBO with the framebuffer, activating the shader in order to generate the "God's ray" effect .

//
//Low level description
//The keystone of the process is the shader which compute the final color by taking sample along the segment[current fragment - light position].
//One way to do this is to calculate light position in screenspace on the CPU side and pass these value as an uniform variable.Two ways to do this:
//You can use the gluProject method.
//"Manually", with the following code :
//
//Note: Of course the best would be to keep all the transformation matrix in the CPU instead of calling glGetDoublev.This means give up on GLU mmethods such as gluPerspective, glOrtho and gluLookAt...I will write an article about this later.



class Blur
{
public:
    Blur() = default;

	void Init()
	{
		FBO = FrameBufferObject(1240, 960, GL_FLOAT);
		Program = new Shader("Resources\\Blur.sfx");
		VAO.Attach(VERTEX, new VertexBufferObject<Vec4>(&Vertices[0], 6));
	}
	void Render(uint64_t _tex)
	{
		__debugbreak();// make sure you are passing the handle not the texture id
		FBO.Bind();
			Program->Enable();
				VAO.Bind(); 
					Program->SetTextureUniform("Scene", _tex);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				VAO.Unbind();
			Program->Disable();
		FBO.Unbind();
	}

	Vec4 Vertices[6] =
	{
	    Vec4(-1.0f,  1.0f, 0.0f , 1.0f),
	    Vec4(-1.0f, -1.0f, 0.0f , 0.0f),
	    Vec4( 1.0f, -1.0f, 1.0f , 0.0f),
	    Vec4(-1.0f,  1.0f, 0.0f , 1.0f),
	    Vec4( 1.0f, -1.0f, 1.0f , 0.0f),
	    Vec4( 1.0f,  1.0f, 1.0f , 1.0f)
	};

	Shader *Program;
	FrameBufferObject FBO;
	VertexArrayObject VAO;
};



//Preblur->Enable();
//{
//	TestFBO.Bind();
//	{
//		TestShader->SetTextureUniform("ShadowMap", Shadows->GetShadowMap(), 2);
//		Mat4 biasMatrix = Mat4
//		(
//			0.5, 0.0, 0.0, 0.0,
//			0.0, 0.5, 0.0, 0.0,
//			0.0, 0.0, 0.5, 0.0,
//			0.5, 0.5, 0.5, 1.0
//		);
//		TestShader->SetUniform("DepthMatrix", biasMatrix *  Shadows->GetDepthMatrix());
//		TestLight.SetPosition(Vec3(152 + (float)sin(RADIANS(Angle)) * 4000, 54 + 12000, 102 + ((float)cos(RADIANS(Angle)) * 4000)));// );  //
//		TestLight.Bind(Preblur);
//		TestCamera.Bind();
//		TestScene.Render(Preblur);
//	}
//	TestFBO.Unbind();
//}
//Preblur->Disable();
