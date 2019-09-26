#define _MY_DEBUG

#include "Application.h"
#include "Camera.h"
#include "Creature.h"
#include "GUI.h"
#include "Image.h"
#include "ImGUI.h"
#include "Lights.h"
#include "Mesh.h"
#include "Terrain.h"
#include "Test.h"
#include "ThreadPool.h"
#include "Texture.h"
#include "Shader.h"
#include "Sprite.h"
#include "ShadowMapper.h"
#include "Physics.h"
#include "Primitive.h"
#include "Window.h"


Camera3D TestCamera;


Listener MousePL([](Event msg)
{
});
Listener MousePR([](Event msg)
{
});
Listener MouseMove([](Event msg)
{
    float relX = msg.data.Mouse.relX;
    float relY = msg.data.Mouse.relY;

    TestCamera.Rotate(-relX * .5, relY*.5);
});
Listener KeyDown([](Event msg)
{
    int Keycode = msg.data.Keyboard.KeyCode;

    switch (Keycode)
    {
    case MYSTIC_KEY_SPACE:
        Shader::GetActiveShader()->Reload();
        Print("Space bar pressed");
        break;
    case MYSTIC_KEY_RIGHT:// 263:
        TestCamera.MoveRight(10.1f);
        break;
    case MYSTIC_KEY_LEFT:
        TestCamera.MoveLeft(10.1f);
        break;
    case MYSTIC_KEY_UP:
        TestCamera.MoveBack(10.1f);
        break;
    case MYSTIC_KEY_DOWN:
        TestCamera.MoveForward(10.1f);
        break;

    }
});
Listener KeyUp([](Event msg)
{
    DEBUG_TRACE("  ");
    //	Print("Keyup");
});
Listener Keyboard([](Event msg)
{
    DEBUG_TRACE("  ");
    //Print("KeyDown");
});



template<typename T>
struct PersistantShaderBufferObject
{
    PersistantShaderBufferObject() = default;
    PersistantShaderBufferObject(Shader *_program, const char *_name, unsigned int _bindLOC)
        :
        BlockSize(sizeof(T)),
        Name(_name),
        SyncObject(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0)),
        Slot(_bindLOC),
        Index(NULL),
        GL_Handle(NULL),
        Pointer(NULL),
        Program(_program->GL_Handle)
    {
        Index = fetchIndex(Name);
        SetBinding(Slot);

        Create();
        Bind();
        BindBase(Slot);

        unsigned int flags = GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glBufferStorage(GL_SHADER_STORAGE_BUFFER, BlockSize, 0, flags);
        Pointer = Map(flags);
    }
    void Update()
    {// Ensures previous write has completed and copies the contents of Data over to Pointer
        WaitBuffer();
        SetData(&Data);
        LockBuffer();
    }

    T* Map(GLuint _flags)
    {// Since it is persistant this only needs to be called once and this abstraction is likely uneeded later on.
        return (T*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, BlockSize, _flags);
    }
    void Unmap()
    {
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }
    void Create()
    {
        glCreateBuffers(1, &GL_Handle);
    }
    void BindBase(GLuint _slot)
    {

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _slot, GL_Handle);
        //	glBindBufferRange(GL_UNIFORM_BUFFER, 0, GL_Handle, 0, BlockSize);
    }
    void SetBinding(GLuint _slot)
    {// Binds the Buffer to a Specified Uniform slot on the GPU.
        glUniformBlockBinding(Program, Index, _slot);

    }

    void SetData(T* _data)
    {// When class is finished likely this will just be replaced by proper syncing and direct access to memory block elements via T
        memcpy(Pointer, _data, BlockSize);
    }
    void SetSubData(size_t _offset, size_t _range, void* _data)
    {// Fills in specified range of data. ( likely not needed in future use as Pointer and Data may become a single entity and Access will be performed directly on respective elements)
        memcpy(Pointer + _offset, _data, _range);
    }

    void Bind()
    {// Need it at Initialization and potentially if the buffer is ever needed for other use
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, GL_Handle);
    }
    void Unbind()
    {// Both likely uneeded due to persistancy of buffer
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void LockBuffer()
    {// Creates a Fence object 
        if (SyncObject)
        {// Destroys Sync Objects if it already exist
            glDeleteSync(SyncObject);
        }
        SyncObject = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    }
    void WaitBuffer()
    {// Syncs operations to ensure Buffer write is finished before proceeding 
        if (SyncObject)
        {
            while (1)
            {
                GLenum waitReturn = glClientWaitSync(SyncObject, GL_SYNC_FLUSH_COMMANDS_BIT, 1);
                if (waitReturn == GL_ALREADY_SIGNALED || waitReturn == GL_CONDITION_SATISFIED)
                    return;
            }
        }
    }

    //https://www.uni-weimar.de/fileadmin/user/fak/medien/professuren/Computer_Graphics/CG_WS_18_19/CG/06_ShaderBuffers.pdf
    //TODO: Allow Direct Acces to pointer when working correctly
    T Data;
    T *Pointer;

    // OpenGL API for Uniform Buffer information

    GLint isReferenced(ShaderType _shadertype)
    {
        GLint results = 0;
        glGetActiveUniformBlockiv(Program, Index, _shadertype, &results);
        return results;
    }
    int fetchIndex(const char *_name)
    {// Return Index of the uniform block with specific name
        return glGetProgramResourceIndex(Program, GL_SHADER_STORAGE_BLOCK, _name);
    }
    int fetchActiveBlockBinding() {
        int results = 0;
        glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_BINDING, &results);
        return results;

    }
    int fetchBlockSize()
    {
        int results = 0;
        glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_DATA_SIZE, &results);
        return results;
    }
    int fetchNameLength()
    {
        int results = 0;
        glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_NAME_LENGTH, &results);
        return results;
    }
    int fetchActiveUniforms()
    {
        int results = 0;
        glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &results);
        return results;
    }
    int fetchActiveUniformIndices()
    {
        int results = 0;
        glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &results);
        return results;
    }

private:

    unsigned int BlockSize;

    const char *Name;

    GLsync SyncObject;
    GLuint Slot;
    GLuint Index;
    GLuint GL_Handle;
    GLuint Program;
};



#include"ECS.h"
#include"ECScomponent.h"
#include"ECSsystem.h"

class MyApp : public Application
{
public:
	MyApp()
	{
		MessageHandler.RegisterListener(EVENT_MOUSEDOWN_LEFT, MousePL);
		MessageHandler.RegisterListener(EVENT_MOUSEDOWN_RIGHT, MousePR);
		MessageHandler.RegisterListener(EVENT_MOUSEMOVE, MouseMove);
		MessageHandler.RegisterListener(EVENT_KEYUP, KeyUp);
		MessageHandler.RegisterListener(EVENT_KEYDOWN, KeyDown);
		MessageHandler.RegisterListener(EVENT_KEYBOARD, Keyboard);
	}
	void SetWindowProperties()
	{
		Position.x = 10;
		Position.y = 10;
		Size.x = 1280;
		Size.y = 960;
		Title = " MyApp Window";
		ResizableWindow(false);
		//UseOpenGLProfile(GLFW_OPENGL_COMPAT_PROFILE);
	}

	///------------Various unit testing--------------- 
	Shader 
        *TestShader,
        *SpriteShader;

	Scene TestScene;
	Light TestLight;

	BenchMark TestBench;
 

	struct Matrices
	{
		Mat4 Projection;
		Mat4 View;
		Mat4 Depth;
		Vec3 EyePosition;
	};
	struct LightBlock
	{
		Vec4 Position;
		Vec4 Color;
	};
    struct SSBO
    {
        Mat4 MVP;
        Mat4 MV;
    };

	PersistantUniformBufferObject <Matrices> *TestUBO;
	PersistantUniformBufferObject <LightBlock> *LightUBO;

    std::unique_ptr<Sprite> TestSprite;
	std::unique_ptr<ShadowMap> Shadows;
	DebugQuad *DQuad;
 

    EntityComponentSystem *TestECS;
    SystemList MainSystems;

    COMPONENT(MovementComponent)
    {
        Vec3 Velocity;
        Vec3 Acceleration;
    };

    
    COMPONENT(PositionComponent) 
    {
        Vec3 Position;
    };

   
    struct MovementSystem
        :
        public BaseSystem
    {
        MovementSystem()
            :
            BaseSystem()
        {  
            AddComponentType(PositionComponent::ID);
            AddComponentType(MovementComponent::ID);
#ifdef _DEBUG
            SystemName = (typeid(this).name());
#endif
        }

        virtual void UpdateComponents(float _delta, BaseComponent** _components)
        {
            PositionComponent* Pos = (PositionComponent*)_components[0];
            MovementComponent *Movement = (MovementComponent*)_components[1];
            
            Movement->Velocity += Movement->Acceleration;
            Pos->Position += Movement->Velocity;
            Movement->Velocity *= .9;
        };
    };
    MovementSystem movementSys;
    PositionComponent PosComponent;
    MovementComponent TestMovementComponent;

	void OnEnd()
	{
	}
    
	void OnCreate()
	{
		TestShader = new Shader("Resources\\Shaders\\BasicShader.sfx");
        SpriteShader = new Shader("Resources\\Shaders\\Sprite.sfx");

        TestECS = new EntityComponentSystem();

        /// Create Entities
        EntityPTR Entity = TestECS->MakeEntity(PosComponent, TestMovementComponent);

        /// Create Systems

        MainSystems.AddSystem(movementSys);
   //     TestECS->AddSystem(movementSys);

        TestECS->AddComponent(Entity, &PosComponent);
        TestECS->AddComponent(Entity, &TestMovementComponent);

        SpriteShader->Enable();
        {
            std::unique_ptr<Image> TestSpriteSheet = std::make_unique<Image>("Resources\\Sprites\\Test.png");
            TestSprite = std::make_unique<Sprite>(*TestSpriteSheet, Vec2(112, 112), 4); /// SIGNIFICANT PROBLEM, CREATING IMAGE FOR SPRITE SHEET TWICE
            TestSprite->AddAnimation(0, Animation(*TestSpriteSheet, { 0, 0, 56, 56 }, 0, 4));
        }

        SpriteShader->Disable();
        TestShader->Enable();

		TestCamera = Camera3D(Vec3(0), Vec3(0)); // Create a Camera to view the Scene  
		Viewport::SetCamera(&TestCamera);

	 	TestScene = Scene("Resources\\SponzaFBX//Sponza.fbx");		// Scene("Resources//TestScene//source//mp_toujane.fbx"); // "C:\\Users\\DELL\\source\\repos\\Mystic\\Mystic\\Resources\\SpaceScene\\Space Station Scene.dae"); //"Resources//Wolf//wolf_fbx.fbx");//

		TestLight = Light(Vec3(0)); TestLight.Color = fRGB(.90f, .95f, 1.00f);

        Shadows = std::make_unique<ShadowMap>();
		DQuad =  new DebugQuad();

		//https://ferransole.wordpress.com/category/azdo-series/
        TestUBO = new PersistantUniformBufferObject <Matrices>(TestShader, "Matrices", 0);
        LightUBO = new PersistantUniformBufferObject<LightBlock>(TestShader, "Lights", 1 );
       
   }

    void OnUpdate()
    {
		TestCamera.Update();
     //   TestECS->UpdateSystems(MainSystems, 0.1f);
	}

///==================================================================================================================================================
///==                     RENDERING
///==================================================================================================================================================

	void OnRender()
	{
		Angle += .051f;
#if 1
		Shadows->Bind();
		{// Shadow Pass
			Shadows->Render(TestLight.Position);
			TestScene.Render(*Shadows->Program);
		}
		Shadows->Unbind();

 	  	TestShader->Enable();
		{// Scene Pass
            TestShader->SetTextureUniform("ShadowMap", Shadows->DepthTextureHandle);

			TestLight.SetPosition( Vec3(152 + (float)sin(RADIANS(Angle)) * 4000, 54 + 12000, 102 + ((float)cos(RADIANS(Angle)) * 4000)));
            TestCamera.Bind(TestShader);


 			TestLight.Bind(*TestShader);
            ///------------ Set Shader Uniform Block --------------------------------------

			LightUBO->Data.Position = Vec4(TestLight.Position, 1.0);
			LightUBO->Data.Color = Vec4(TestLight.Color, 1.0);


			TestUBO->Data.Projection = TestCamera.ProjectionMatrix;
			TestUBO->Data.View = TestCamera.ViewMatrix;

			TestUBO->Data.Depth = Mat4
            (
                0.5, 0.0, 0.0, 0.0,
                0.0, 0.5, 0.0, 0.0,
                0.0, 0.0, 0.5, 0.0,
                0.5, 0.5, 0.5, 1.0
            ) * Shadows->GetDepthMatrix();

			TestUBO->Data.EyePosition = TestCamera.Position;

			TestUBO->Update(); 
            LightUBO->Update();
            ///----------------------------------------------------------------------------

            TestScene.Render(*TestShader);
            TestUBO->LockBuffer();
            LightUBO->LockBuffer();
		}
        TestShader->Disable();
#endif
	 	
       // DQuad->Render(TestSprite->SpriteSheet->Handle);//Shadows->DepthTextureHandle); 
// SpriteShader->Enable();
// 
 TestSprite->Position = Vec3(430, 640, 0);
// TestSprite->Rotation.z += Angle;
// TestSprite->Render(*SpriteShader);
// SpriteShader->Disable();
	}
	//
	///OK... is it working now....


    float Angle = 0; /// Just a variable to move the Lighting around and not terribly important
};


#include"Native.h"
int main()
{
     TODO("SSBO Class Incomplete. Data is not sent from the GPU back to the CPU but then again I have not really tried so...");
     NativeMemory::GlobalMemState.GetMemState();
	 MyApp App;
	 App.Start(); 
	 App.Run(); // Entry point Line 66 in Application.cpp
	 App.End();
     NativeMemory::GlobalMemState.CheckMemory();
}















//		TestTerrain = new Terrain(Vec3(0, 0, 0), 50,50, &TestTexture);// To Large Must Heap allocate
/*
	 Initialization of various test items most of which are currently not in use as I am trying to get the GUI
	 System up to par so that I can get some sort of direct feedback as I design the various functionality
	 "Resources//BasicShader.sfx"
	 */
	 //std::unique_ptr<Shader> temp = std::make_unique<Shader>("Resources//BasicShader.sfx");
	 //TestShader.swap(temp);
	 //TestBench = BenchMark("Render MainThread");
	 //TestShader->Enable();
	 //  TestLight = Light(Vec3(0), Vec3(0)); // Make a Test Light, Needs to be done after Shader creation
	 //  
	 //  
 //		TestAsset = Image::Get("TestAsset");

	//for (auto &Sph : TestSpheres)
	//{// Turn the Sphere Mesh into Organisms by Mapping All Indexed polygons
	//	TestOrg.push_back(&Sph);
	//}
	//CreatureManager TestGPGPU(10);
	//TestGPGPU.Bind();
	//TestGPGPU.Compute();
//  CreatureManager::MakeGroups();
//	Organism::Manager.UpdateOrganisms();
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	ThreadPool::Instance().Async(TestFunction);


/// Paths for Various Testing purposes
///---------------------------------------------
//	TestBunny = Mesh("Resources//TestScene//source//mp_toujane.fbx"); 
//	TestBunny.Position = Vec3(0, 0, 0);		
//	TestDragon = Mesh("Resources//Dragon//Dragon//Dragon.fbx"); 
//  TestMoon = Load up a Mesh Object Moon//Moon2k.fbx"); //
//  TestScene = Scene("Resources//TestScene//source//mp_toujane.fbx");
// 	TestScene.Camera = &TestCamera;

/// Functionality for the test
///---------------------------------------------
//  for (auto& O : TestSpheres)
//  {// Render every Sphere one by one
//  	O.Render();
//  }



// https://www.slideshare.net/CassEveritt/approaching-zero-driver-overhead
// ARB_direct_state_access(DSA),
// OpenGL 4.5 core, allows direct manipulation of objects(textures, buffers, fbos…) rather than the classic “bind to edit”.Using DSA makes object manipulation cleaner as it doesn’t affect the binding state used for rendering, therefore is also middleware friendly.
// ARB_buffer_storage(core 4.4)
// should be preferred over the classic glBufferData, as it gives better usage hints than glBufferData. Ideally the developer uses persistent mapped buffers for all buffers that he expects to for reading or writing access on the CPU.This would allow an identical workflow to Vulkan.We also encourage the use of few but rather large buffers as suggested in the memory management blog post.
// ARB_texture_storage(core 4.2) 
// provides immutable textures whose definition is complete up - front.The classic glTexImage never “knew” how many mip - maps would actually be specified, often causing lazy allocation.glTextureStorage is the better alternative for the driver.
// ARB_texture_view(core 4.3) 
// introduced the concept of casting texture formats to different types as long as they have the same texel size.The developer can also use it as view on a sub - resource, for example individual texture layer or mipmap in an array texture.Similar to using larger buffers and sub - allocating from those, a developer could use texture arrays for popular dimensions and re - use individual layers.It is not as flexible as the Vulkan texture memory management, but it can reduce run - time texture creation costs.
// ARB_multi_draw_indirect(MDI),
// OpenGL 4.3 core, allows accelerating draw - call submission.Compared to instancing we can draw any sub - range of vertex / index - buffers.To make most use of this feature use the previous encouraged larger buffers for geometry data, as it will allow drawing many geometries at once via MDI.The MDI buffer can also be filled from different threads using persistent mapped memory, whose pointers are passed to threads that don’t need an OpenGL context.The buffer content does not have to be filled every frame, but can be re - used and even manipulated by the GPU directly, for example for level of detail or culling purposes.






//https://veldrid.dev/articles/implementation/opengl.html







///DEBUG: SHADER MATRICES APPEAR FINE in VALUE.
///	const GLuint WIDTH = 800;
///	const GLuint HEIGHT = 600;
/// GLfloat vertices[] = {
/// 	/*   Positions          Colors */
/// 		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
/// 		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
/// 		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f };
// glDebugMessageCallbackARB
// glDebugMessageControlARB
// glDebugMessageInsertARB
// glGetDebugMessageLogARB

// glCopyImageSubData



//for_loop(I, 1)
//{// Create N number of Spheres to display in the world.
//	TestSpheres.push_back(Sphere(Vec3(RANDOM_SPREAD(215), RANDOM_SPREAD(215), RANDOM_SPREAD(215)), 70, 3));
//}Sphere::MakeBatch(); // Merge them into a single VAO
//Sphere::BatchVAO.MaterialID = 2;
//TestSpheres.clear();
//		
////TestScene.Attach(Sphere::BatchVAO);





// Good Gun Needs Glow Texture though Railgun//Railgun_Prototype-FBX 7.4.fbx");//  

//	Cubemap *TestSkybox;
//TestSkybox = new Cubemap
//(
//	new Image("Resources\\Skybox\\Back.png"),
//	new Image("Resources\\Skybox\\Down.png"),
//	new Image("Resources\\Skybox\\Forward.png"),
//	new Image("Resources\\Skybox\\Left.png"),
//	new Image("Resources\\Skybox\\Right.png"),
//	new Image("Resources\\Skybox\\up.png")
//);		TestSkybox->Update(TestCamera.ProjectionMatrix, TestCamera.ViewMatrix);
/*
We create some basic callback Listeners for out Event System to handle basic Input
I am not really a fan to be honest of the way that GLFW handles the callback system and to be honest
I would rather do it myself like one would do in Win32 or SDL but none the least this is currently what I
have. I will likely move the input results into their own Mose...Keyboard...joystick classes etc
However II am starting to feel as though to many singleton items are starting to appear in my engine
because there is just so much functionality of which I honestly need only one global instance of.
Window... EventSyste.. Threadpool, Input... I can not find much reason in my head to create multiple Instances
of any of these classes and structures however everyone keeps pushing to avoid the Singleton design pattern
for various reasons leaving me conflicted on how I should go about handling these systems

BTW Before you see it for_loop(itr, count) is just a basic for(int itr = 0; itr < count ; ++itr) because I like it, it feels clean and I am lazy ;)
Everyone tends to bug about that so I figured I would give forwarning lol.
*/
















// http://docs.gl/gl4/glBufferSubData







//glFinish();
//glBindBufferBase(GL_UNIFORM_BUFFER, 0, GL_Handle);
//UNIFORM_BUFFER working fine, albeit slightly slower than typical uniforms Persistant mapping should fix this
//glBindBuffer(GL_UNIFORM_BUFFER, GL_Handle);
//Data = (T*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, BlockSize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
//BufferHandle
//WaitBuffer();
//{
//	memcpy(BufferHandle, &Data, sizeof(T));
//}
//glFlushMappedBufferRange(GL_UNIFORM_BUFFER, 0, BlockSize);
//glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);
//BufferHandle->Projection = Data.Projection;
//BufferHandle->View = Data.View,
//glUnmapBuffer(GL_UNIFORM_BUFFER);
//glFlushMappedBufferRange(GL_UNIFORM_BUFFER, 0, BlockSize);
//glBufferData(GL_UNIFORM_BUFFER, BlockSize, NULL, GL_DYNAMIC_DRAW);

//https://github.com/fenbf/GLSamples/blob/master/src/MultidrawIndirect.cpp



//1:)	glBufferData(GL_UNIFORM_BUFFER, BlockSize, NULL, GL_STATIC_WRITE);

        //SetData(NULL);
        //BindBase(Index);
        //* Same as above glBindBufferRange(GL_UNIFORM_BUFFER, 0, GL_Handle, 0, BlockSize); // TODO: Fix this Zero later to be accurate
        //SetSubData(0, BlockSize, NULL);



// Good tutorial
//https://paroj.github.io/gltut/Positioning/Tut07%20Shared%20Uniforms.html







//glGenBuffers(1, &vboID);
//glBindBuffer(GL_ARRAY_BUFFER, vboID);
//
//
//flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
//glBufferStorage(GL_ARRAY_BUFFER, MY_BUFFER_SIZE, 0, flags);
//
//flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
//myPointer = glMapBufferRange(GL_ARRAY_BUFFER, 0, MY_BUFFER_SIZE, flags);





/// void glUniformBlockBinding
/// (
///     GLuint program,            :<The name of a program object containing the active uniform block whose binding to assign.>
///     GLuint uniformBlockIndex,  :<The index of the active uniform block within program whose binding to assign.>
///     GLuint uniformBlockBinding :<Specifies the binding point to which to bind the uniform block with index uniformBlockIndex within program.>
/// );
/// 
/// Description
/*
glBindBufferBase binds the buffer object buffer to the binding point at index index of the array of targets specified by target.
Each target represents an indexed array of buffer binding points, as well as a single general binding point that can be used by
other buffer manipulation functions such as glBindBuffer or glMapBuffer.In addition to binding buffer to the indexed buffer binding target,
glBindBufferBase also binds buffer to the generic buffer binding point specified by target.*/

/// void glBindBufferBase
/// (	
///     GLenum target,  : <Specify the target of the bind operation. target must be one of GL_ATOMIC_COUNTER_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER or GL_SHADER_STORAGE_BUFFER.>
/// 	GLuint index,   : <Specify the index of the binding point within the array specified by target.>
/// 	GLuint buffer   : <The name of a buffer object to bind to the specified binding point.>
/// );
/*
Binding points for active uniform blocks are assigned using glUniformBlockBinding.Each of a program's active uniform blocks has a
corresponding uniform buffer binding point. program is the name of a program object for which the command glLinkProgram has been issued in the past.
If successful, glUniformBlockBinding specifies that program will use the data store of the buffer object bound to the binding point
uniformBlockBinding to extract the values of the uniforms in the uniform block identified by uniformBlockIndex.
When a program object is linked or re-linked, the uniform buffer object binding point assigned to each of its active uniform blocks is reset to zero.
*/






//		delete(Shadows);
//		delete(DQuad);
//  	delete(TestShader);
//		delete(ColorShader);
//		delete(TestUBO);
//		delete(TestSkybox);
// https://github.com/google/angle/tree/master/src/tests/gl_tests OPENGL Test
//
//switch (_shadertype)
//{
//case VERTEX:
//
//    break;
//case FRAGMENT:
//    break;
//case COMPUTE:
//    break;
//case GEOMETRY:
//    break;
//case TESSLATION_CONTROL:
//    glGetActiveUniformBlockiv(Program, Index, _shadertype, GLint &results);
//    break;
//case TESSLATION_EVALUATION:
//    break;
//}