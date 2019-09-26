#include"ShadowMapper.h"

extern Camera3D TestCamera;

ShadowMap::ShadowMap()
    :
    LightMatrix(Mat4(1))
{
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &FrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader

	glGenTextures(1, &DepthTexture);
	glBindTexture(GL_TEXTURE_2D, DepthTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //GL_CLAMP?
	 
 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTexture, 0);
 
	TODO("Fix this to take in a Texture instead of creating the Texture here so I can have Handle access");

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		__debugbreak();
	}
	Program = new Shader("Resources\\Shaders\\Shadow.sfx");
	// Compute the MVP matrix from the light's point of view
	Program->AttributeLocation("VertexPosition");
	ProjectionMatrix = glm::ortho<float>(-2000, 2000, -2000, 2000, -2500, 2000);
 
	DepthTextureHandle = glGetTextureHandleARB(DepthTexture);
	glMakeTextureHandleResidentARB(DepthTextureHandle);
}

void  ShadowMap::Bind()
{
	Program->Enable();
	//glUseProgram(Program->GL_Handle);
	glViewport(0, 0, 4096, 4096);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, DepthTexture);
	//glEnableVertexAttribArray(0);
}
void  ShadowMap::Unbind()
{
	glViewport(0, 0, 1280, 960);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Program->Disable();
}				 
void ShadowMap::Render(Vec3 _position)
{
	LightPosition = _position;
	glm::vec3 lightInvDir = glm::normalize(_position);
	ViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	LightMatrix = ProjectionMatrix * ViewMatrix;
    Program->SetUniform("LightMatrix", LightMatrix);
}
void ShadowMap::RenderScene(Scene& Scene)
{
	for (auto& M : Scene.Meshes)
	{///THIS WILL BE A PER MESH IDEA. MESH LINKS TO UNIFORM. WE BIND A UBO LIKE OBJECT THEN RENDER MESH. MESH IS NOT MATERIAL. MESH HAS MATERIAL ID
		M.Bind(*Program);
//		glEnableVertexAttribArray(0);
		glDrawElements(M.PrimativeType, M.VAO->ElementCount, GL_UNSIGNED_INT, nullptr);
		M.Unbind();
		for (auto& C : M.Children)
		{
			C.Render(*Program);
		}
	}
}


// http://ogldev.atspace.co.uk/www/tutorial24/tutorial24.html
//  
//  
//  compute_view_matrix_for_rotation(glm::vec3 origin, glm::vec3 rot)
//  {
//  	glm::mat4 mat(1.0);
//  	float rx = DEG_TO_RAD(rot.x);
//  	float ry = DEG_TO_RAD(rot.y);
//  	float rz = DEG_TO_RAD(rot.z);
//  	mat = glm::rotate(mat, -rx, glm::vec3(1, 0, 0));
//  	mat = glm::rotate(mat, -ry, glm::vec3(0, 1, 0));
//  	mat = glm::rotate(mat, -rz, glm::vec3(0, 0, 1));
//  	mat = glm::translate(mat, -origin);
//  	return mat;
//  }
//	Program->SetUniform("ModelMatrix", Mat4(1));
//	Program->SetUniform("ProjectionMatrix", ProjectionMatrix);