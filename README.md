==================================================================================================================
  7/ 18 /19
==================================================================================================================
* ECS Completed
==================================================================================================================

The Entity Component System has been completed and is working. It is now largely a matter of getting into the swing
of properly implementing things in a DOD fashion.



==================================================================================================================
  7/ 7 /19
==================================================================================================================
*TODO("YOU HAVE A SIGNIFICANT MEMORY LEAK, FIND IT. IDK Where is COULD POSSIBLY BE AT THE MOMENT!");
==================================================================================================================

*This is a Strange Error in that I did not even know it existed until searching up some Windows API 
and toying around with it. The memory leak is not active in that it does not steadily rise.
I am currently under the impression that it may very well be memory allocated
by Assimp or SOIL which either they or myself are not properly recovering. 



==================================================================================================================
  7/ 7 /19
==================================================================================================================
1:)TODO("IMPLEMENT A LOAD IN SPRITE IN ORDER TO MAKE THE VIRTUAL FUNCTIONALITY WORK");

2:)TODO("MESH CLASS NEEDS MULTIPLE TEXTURES ADDED SO THAT THE MATERIALS ARE BOUND IN ORDER");

3:)TODO("Finished UBO class, Make the TODO have a static bool to prevent multiple iterations of it in a single block");

4:)TODO("FIX THE RENDER() Method for Texture to display a square any point on the screen desired with the Texture given");

5:)TODO("NEED TO IMPLEMENT AND BIND SURFACE ELSE THERE WILL BE NO IMAGE, PERHAPS IT WILL BE A TEXTURE IDK YET");

==================================================================================================================
1:)    Asset class has been liquidated for now so Load is unneeded at this point.
In the future Asset will be added back however it will likely be a composed
part of each class in that if a Manager for the specific class is desired then 
and only then will their be Asset functionality;

2:)    This was Accomplished sometime ago just never posted an update. The Mesh 
class contains the Material ID and the Material stores 64 bit pointers that are
sent to the Shader when the Material is bound. These pointers are OpenGLs method
of using Bindless textures as it points to the location of the texture in memory

3:)    UBO still has issues when one attempts to make multiple UBOs. The first 
instance works fine but any subsequent UBO objects do not properly update and 
send data to the shader. The TODO comment however has been fixed and Static bool
has been added inorder to prevent multiple displays of the same TODO comment on
compiling or during program loops.

4:)    Similar functionality has already been added however all 2D rendering leaves
a lot to be desired. Should work on a Sprite class ASAP as it currently is not
displaying properly. There is a Debug Quad for rendering textures in which a Quad
is renderered to the Screen and the Sampler texture desired is passed in as a
Render Parameter

5:)    In Sprite.cpp. This entire class needs much work. The bind method will take care
of setting state for the Given sprite such as its current frame











==================================================================================================================
  7/ 1 /19
==================================================================================================================
TODO("Pass the Mesh class a Scene and an Index and have it return a Mesh object 
for insertion into the Vector in the Scene class, this is currently wrong.");
==================================================================================================================





==================================================================================================================
  6/ 29/19
==================================================================================================================
TODO("Make this an Asset or make a Manager Object to handle the naming, 
asset management");
==================================================================================================================
*    Lets not and say we did. It is likely the Asset class as a whole will be 
removed soon. It has caused far to many problems and is not ready to be 
inherited from at the moment. Perhaps just having an Asset Object inside of the
given class would be prefered.






==================================================================================================================
  6/ 26/19
==================================================================================================================
*    Setup Mip mapping for Textures greatly increasing quality on detailed
textures







==================================================================================================================
  6/ 25/19
==================================================================================================================

*    Implemented Bindless textures. Textures will no be passed to the GPU using a 
64bit integer value which represents the Handle instead of the GL_ID. This has 
resulted in a significant increase in performance roughly 2x speed up or so and 
likely further increases as the number of textures goes up that I am passing to 
the GPU







==================================================================================================================
  6/ 18/19
==================================================================================================================

*    Begin Implementing the Physics Library. Created an Auto Collider creator for the 
simulation which takes in a Mesh and produces the appropriate collider of your
choosing.






==================================================================================================================
  6/ 9/19
==================================================================================================================
* Bump Mapping
==================================================================================================================
*  Implemented Bumpmapping in the Basic Shader. Fixed bug in which I had 
been calculating the tangent from the Vertexposition instead of the damn
tangent attribute due to a typo







==================================================================================================================
  6/ 8/19
==================================================================================================================
* TODO("Make a Shader function that returns a specific Attribute Location 
in that shader for whatever attribute type specified, do it for the active 
shader so it is passed an buffertype Enum and returns a GLuint for the
position");
==================================================================================================================
Shader.cpp/h
==================================================================================================================

Did that. AttributeLocation(const char *_name) now return location of given
Attribute







==================================================================================================================
  5/10/19
==================================================================================================================
*  	TODO("Setup a Better system for the Shaders. Anything attempting to be
Initialized that relies on them causes problems as GetActiveShader becomes 
a nullptr")






==================================================================================================================
Shader.cpp/h
==================================================================================================================
* Slightly better system being designed. Shaders, Uniforms, and anything associated
with the GPU program will have a base class and singleton instance of its
type accessible via various areas of the program. Is it better? Time will tell
Not exactly thrilled with this layout either




==================================================================================================================
  5/10/19
==================================================================================================================
* TODO("Smooth the Camera movements");
==================================================================================================================
Camera.cpp
==================================================================================================================
* Sort of Fixed, make the Position and Rotation effected by Delta Time also
LERP'ed between the Current Position and Target Position smoothing the 
movement somewhat. All in all it has a smoother feel to it however it is 
far from ideal and more work needs to be done to it.





==================================================================================================================
  5/4/19
==================================================================================================================
* TODO("FrameRate is part of the Window Structure, implement a real class for it instead of that hack");
* TODO("FrameRate is a Hack. Fix it and Implement in the Timer.cpp");
==================================================================================================================
Timer.cpp
==================================================================================================================

* Fixed it a while ago, just now added this entry. FrameRate has been added
as a part of the Timer classes and not an Object called FrameRate can be 
created and updated via its update method. It is far from complete but is 
still currently working.






==================================================================================================================
  4/19/19
* TODO("IMAGE.CPP NEED TO HAVE THE FUCKING TEXTURE COORDS Y FLIPPED!")
==================================================================================================================
Image.CPP
==================================================================================================================

* Created an Algorithm to flip the Image as desired now the loaded Images
have the proper orientation to display the correct way using OpenGL coords

** TODO("CAN NOT DO THIS SHADER CAN NOT BE NULLPTR");
-- Its not, its the ShaderID integer now for better or worse.





==================================================================================================================
  4/18/19
==================================================================================================================
TODO("SOIL is returning RGBA channel when the Image does not contain Alpha.
      This causes a Crash in the Texture Loading function.");
==================================================================================================================

* Fixed by forcing SOIL to load only the RGB channels. Might return in the future 
to implement this properly to include the alpha channels but currently it is
not reading my Images correctly so IDK what I can do about that





==================================================================================================================
  4/17/19
==================================================================================================================

* Created a basic Scene Loader using Assimp library




==================================================================================================================
  4/12/19
==================================================================================================================
* Fixed up the Shader class, Implemented a Basic shader





STRUCTURE


CORE 0
- Application
- EventSystem

CORE 1
-Physics

CORE 2
- Graphics Rendering
~ This is a tough one. The glContext is currently on CORE0 and as such I can not truly render from this core. Perhaps it will be best to perform Polygon sorting and other effects if possible such as Shadow mapping on this core
and pass share it with the glContext of the main thread. I am not sure because I have yet to perform multithreaded rendering using multiple context. NVidia has the ability to share the Context but most cards currently do not.