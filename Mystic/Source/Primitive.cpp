#include<map>

#include"GLBuffers.h"
#include"Primitive.h"

///
///
///Cube::Cube(Vec3 pos, float size)
///{
///	*this = Cube(pos, size);
///	///Add(mat);
///}
///
///Block::Block(Vec3 pos, float size)
///{
///	*this = Block(pos, size);
//////	Add(mat);
///}
///Plane::Plane(Vec3 pos, Vec3 rotation, float width, float height)
///{
///	*this = Plane(pos, rotation, width, height);
//////	Add(mat);
///}
///Sphere::Sphere(Vec3 pos, float radius, int sectors)
///{
///	*this = Sphere(pos, radius, sectors);
//////	Add(mat);
//}



std::vector<Batch> Sphere::BatchList;


Sphere Sphere::BatchVAO;
//======================================================================================================================================================================================
//                                         PRIMATIVES TO TEST WITH                                                                                                                                                   
//======================================================================================================================================================================================

Box::Box(Vec3 _pos, float _length, float _width, float _height)
	:
	Length(_length),
	Width(_width),
	Height(_height)
{
	Position = _pos;
	_length *= .5;
	_width *= .5;
	_height *= .5;
	std::vector<Vec3>    VertexList;
	std::vector<Vec3>    NormalList;
	std::vector<Vec2>    TextureCoords;
	std::vector<Vec4>    ColorList;
	std::vector<GLuint>  IndexList;

	Vec3 V[] =
	{
		Vec3(  _pos.x - _length,  _pos.y - _width,  _pos.z + _height), // FRONT    // 0                                             
		Vec3(  _pos.x + _length,  _pos.y - _width,  _pos.z + _height),             // 1                                    
		Vec3(  _pos.x + _length,  _pos.y + _width,  _pos.z + _height),             // 2                                    
		Vec3(  _pos.x - _length,  _pos.y + _width,  _pos.z + _height),             // 3         
															 
		Vec3(  _pos.x + _length,  _pos.y + _width,  _pos.z + _height), // RIGHT    // 4    2                                        
		Vec3(  _pos.x + _length,  _pos.y + _width,  _pos.z - _height),             // 5                                    
		Vec3(  _pos.x + _length,  _pos.y - _width,  _pos.z - _height),             // 6                                    
		Vec3(  _pos.x + _length,  _pos.y - _width,  _pos.z + _height),             // 7    1       
															 
		Vec3(  _pos.x - _length,  _pos.y - _width,  _pos.z - _height), // BACK     // 8                                            
		Vec3(  _pos.x + _length,  _pos.y - _width,  _pos.z - _height),             // 9    6                               
		Vec3(  _pos.x + _length,  _pos.y + _width,  _pos.z - _height),             // 10   5                                
		Vec3(  _pos.x - _length,  _pos.y + _width,  _pos.z - _height),             // 11
															 
		Vec3(  _pos.x - _length,  _pos.y - _width,  _pos.z - _height), // LEFT     // 12   8                                        
		Vec3(  _pos.x - _length,  _pos.y - _width,  _pos.z + _height),             // 13   7                              
		Vec3(  _pos.x - _length,  _pos.y + _width,  _pos.z + _height),             // 14   3                                
		Vec3(  _pos.x - _length,  _pos.y + _width,  _pos.z - _height),             // 15  11    
															 
		Vec3(  _pos.x + _length,  _pos.y + _width,  _pos.z + _height), // TOP      // 16   2                                      
		Vec3(  _pos.x - _length,  _pos.y + _width,  _pos.z + _height),             // 17   3                                
		Vec3(  _pos.x - _length,  _pos.y + _width,  _pos.z - _height),             // 18   11                                
		Vec3(  _pos.x + _length,  _pos.y + _width,  _pos.z - _height),             // 19   5    
															 
		Vec3(  _pos.x - _length,  _pos.y - _width,  _pos.z - _height), // BOTTOM   // 20   8                                   
		Vec3(  _pos.x + _length,  _pos.y - _width,  _pos.z - _height),             // 21   9                          
		Vec3(  _pos.x + _length,  _pos.y - _width,  _pos.z + _height),             // 22   1                          
		Vec3(  _pos.x - _length,  _pos.y - _width,  _pos.z + _height),             // 23   0                               
	};														               
	Vec2 UVs[] =
	{
		Vec2(0.00f, 0.00f),  // FRONT  // 0   
		Vec2(0.00f, 1.00f),            // 1   
		Vec2(1.00f, 1.00f),            // 2   
		Vec2(1.00f, 0.00f),            // 3   

		Vec2(0.00f, 0.00f),  // RIGHT  // 4   
		Vec2(0.00f, 1.00f),            // 5   
		Vec2(1.00f, 1.00f),            // 6   
		Vec2(1.00f, 0.00f),            // 7   

		Vec2(0.00f, 0.00f),  // BACK   // 8   
		Vec2(0.00f, 1.00f),            // 9   
		Vec2(1.00f, 1.00f),            // 10  
		Vec2(1.00f, 0.00f),            // 11  

		Vec2(0.00f, 0.00f),  // LEFT   // 12  
		Vec2(0.00f, 1.00f),            // 13  
		Vec2(1.00f, 1.00f),            // 14  
		Vec2(1.00f, 0.00f),            // 15  

		Vec2(0.00f, 0.00f),  // TOP    // 16  
		Vec2(0.00f, 1.00f),            // 17  
		Vec2(1.00f, 1.00f),            // 18  
		Vec2(1.00f, 0.00f),            // 19  

		Vec2(0.00f, 0.00f),  // BOTTOM // 20  
		Vec2(0.00f, 1.00f),            // 21  
		Vec2(1.00f, 1.00f),            // 22  
		Vec2(1.00f, 0.00f),            // 23  
	};
	Vec4 Cols[] =
	{
		Vec4(1.0, 1.0, 1.0, 1.0),  // FRONT  // 0         
		Vec4(0.0, 1.0, 1.0, 1.0),            // 1         
		Vec4(0.0, 0.5, 1.0, 1.0),            // 2         
		Vec4(0.0, 0.0, 0.0, 1.0),            // 3         

		Vec4(0.0, 0.0, 0.0, 1.0),  // RIGHT  // 4         
		Vec4(0.0, 0.0, 1.0, 1.0),            // 5         
		Vec4(0.0, 1.0, 1.0, 1.0),            // 6         
		Vec4(1.0, 1.0, 1.0, 1.0),            // 7         

		Vec4(1.0, 0.0, 1.0, 1.0),  // BACK   // 8         
		Vec4(0.0, 1.0, 0.0, 1.0),            // 9         
		Vec4(1.0, 1.0, 0.0, 1.0),            // 10        
		Vec4(1.0, 0.0, 0.0, 1.0),            // 11

		Vec4(0.50f, 1.00f, 1.00f, 1.0),  // LEFT   // 12        
		Vec4(0.25f, 1.00f, 1.00f, 1.0),            // 13        
		Vec4(0.25f, 0.66f, 0.66f, 1.0),            // 14        
		Vec4(0.50f, 0.66f, 0.66f, 1.0),            // 15        

		Vec4(0.25f, 0.33f, 0.33f, 1.0),  // TOP    // 16        
		Vec4(0.25f, 0.66f, 0.66f, 1.0),            // 17        
		Vec4(0.50f, 0.66f, 0.66f, 1.0),            // 18        
		Vec4(0.50f, 0.33f, 0.33f, 1.0),            // 19     

		Vec4(0.75f, 0.66f, 0.66f, 1.0),  // BOTTOM // 20        
		Vec4(0.75f, 0.33f, 0.33f, 1.0),            // 21        
		Vec4(1.00f, 0.33f, 0.33f, 1.0),            // 22        
		Vec4(1.00f, 0.66f, 0.66f, 1.0),            // 23        
	};
	GLuint Indexdata[] =
	{
		0,1,2,    0,2,3,     // FRONT
		6,5,4,    4,7,6,     // RIGHT 
		10,9,8,   8,11,10,   // BACK
		12,13,14, 12,14,15,  // LEFT
		18,17,16, 16,19,18,  // TOP
		20,21,22, 20,22,23   // BOTTOM
	};
	Vec3 NormalData[24];

	for_loop(Index, 24)
	{
		float x = V[Index].x - _pos.x;
		float y = V[Index].y - _pos.y;
		float z = V[Index].z - _pos.z;

		float  magnitude = sqrt(Squared(x) + Squared(y) + Squared(z));

		if (magnitude != 0)
		{
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
		}
		NormalData[Index] = Vec3(x, y, z); //  Vec3(RANDOM(2)-1,RANDOM(2)-1,RANDOM(2)-1);//
	}
	for_loop(Index, 24)
	{
		VertexList.push_back(V[Index]);
		TextureCoords.push_back(UVs[Index]);
		NormalList.push_back(NormalData[Index]);
	}
	for_loop(Index, 36)
	{
		IndexList.push_back(Indexdata[Index]);
	}

	// Averages all the normals so Each Faces Vertices have normals facing straight out to prevent interpolation of the light
	// Across the block

	for (size_t Index = 0; Index < 24; Index += 4)
	{
		Vec3 Norm = 
            NormalList[Index] +
			NormalList[Index + 1] +
			NormalList[Index + 2] +
			NormalList[Index + 3];

		Norm /= 4;

		NormalList[Index] = Norm;
		NormalList[Index + 1] = Norm;
		NormalList[Index + 2] = Norm;
		NormalList[Index + 3] = Norm;
	}


	VAO = new VertexArrayObject();
	VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec3>(&VertexList[0], 24));
	VAO->Attach(BufferTypes::UVCOORD, new VertexBufferObject<Vec2>(&TextureCoords[0], 24));
	VAO->Attach(BufferTypes::NORMAL, new  VertexBufferObject<Vec3>(&NormalList[0], 24));
	VAO->Attach(BufferTypes::COLOR, new VertexBufferObject<Vec4>(&Cols[0], 24));
	VAO->Attach(BufferTypes::INDICE, new VertexBufferObject<GLuint>(&IndexList[0], 36));

//Transform = glm::mat4(1.0f); // Set Identity and Rotate all axis followed with the Translation.
//Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
//Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
//Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
//Transform = glm::translate(Transform, Position);
}


Block::Block(Vec3 pos, float size)
{
	Position = pos;

	std::vector<Vec3>    VertexList;
	std::vector<Vec3>    NormalList;
	std::vector<Vec2>    TextureCoords;
	std::vector<Vec4>    ColorList;
	std::vector<GLuint>  IndexList;

	Vec3 V[] =
	{
		Vec3(-size,-size, size), // FRONT    // 0                                             
		Vec3(size,-size, size),             // 1                                    
		Vec3(size, size, size),             // 2                                    
		Vec3(-size, size, size),             // 3                                    

		Vec3(size, size, size), // RIGHT    // 4    2                                        
		Vec3(size, size,-size),             // 5                                    
		Vec3(size,-size,-size),             // 6                                    
		Vec3(size,-size, size),             // 7    1                                

		Vec3(-size,-size,-size), // BACK     // 8                                            
		Vec3(size,-size,-size),             // 9    6                               
		Vec3(size, size,-size),             // 10   5                                
		Vec3(-size, size,-size),             // 11

		Vec3(-size,-size,-size), // LEFT     // 12   8                                        
		Vec3(-size,-size, size),             // 13   7                              
		Vec3(-size, size, size),             // 14   3                                
		Vec3(-size, size,-size),             // 15  11                                

		Vec3(size, size, size), // TOP      // 16   2                                      
		Vec3(-size, size, size),             // 17   3                                
		Vec3(-size, size,-size),             // 18   11                                
		Vec3(size, size,-size),             // 19   5                                

		Vec3(-size,-size,-size), // BOTTOM   // 20   8                                   
		Vec3(size,-size,-size),             // 21   9                          
		Vec3(size,-size, size),             // 22   1                          
		Vec3(-size,-size, size),             // 23   0                               
	};

	Vec2 UVs[] =
	{
		Vec2(0.00f, 0.00f),  // FRONT  // 0   
		Vec2(0.00f, 1.00f),            // 1   
		Vec2(1.00f, 1.00f),            // 2   
		Vec2(1.00f, 0.00f),            // 3   

		Vec2(0.00f, 0.00f),  // RIGHT  // 4   
		Vec2(0.00f, 1.00f),            // 5   
		Vec2(1.00f, 1.00f),            // 6   
		Vec2(1.00f, 0.00f),            // 7   

		Vec2(0.00f, 0.00f),  // BACK   // 8   
		Vec2(0.00f, 1.00f),            // 9   
		Vec2(1.00f, 1.00f),            // 10  
		Vec2(1.00f, 0.00f),            // 11  

		Vec2(0.00f, 0.00f),  // LEFT   // 12  
		Vec2(0.00f, 1.00f),            // 13  
		Vec2(1.00f, 1.00f),            // 14  
		Vec2(1.00f, 0.00f),            // 15  

		Vec2(0.00f, 0.00f),  // TOP    // 16  
		Vec2(0.00f, 1.00f),            // 17  
		Vec2(1.00f, 1.00f),            // 18  
		Vec2(1.00f, 0.00f),            // 19  

		Vec2(0.00f, 0.00f),  // BOTTOM // 20  
		Vec2(0.00f, 1.00f),            // 21  
		Vec2(1.00f, 1.00f),            // 22  
		Vec2(1.00f, 0.00f),            // 23  
	};

	Vec4 Cols[] =
	{
		Vec4(1.0, 1.0, 1.0, 1.0),  // FRONT  // 0         
		Vec4(0.0, 1.0, 1.0, 1.0),            // 1         
		Vec4(0.0, 0.5, 1.0, 1.0),            // 2         
		Vec4(0.0, 0.0, 0.0, 1.0),            // 3         

		Vec4(0.0, 0.0, 0.0, 1.0),  // RIGHT  // 4         
		Vec4(0.0, 0.0, 1.0, 1.0),            // 5         
		Vec4(0.0, 1.0, 1.0, 1.0),            // 6         
		Vec4(1.0, 1.0, 1.0, 1.0),            // 7         

		Vec4(1.0, 0.0, 1.0, 1.0),  // BACK   // 8         
		Vec4(0.0, 1.0, 0.0, 1.0),            // 9         
		Vec4(1.0, 1.0, 0.0, 1.0),            // 10        
		Vec4(1.0, 0.0, 0.0, 1.0),            // 11

		Vec4(0.50f, 1.00f, 1.00f, 1.0),  // LEFT   // 12        
		Vec4(0.25f, 1.00f, 1.00f, 1.0),            // 13        
		Vec4(0.25f, 0.66f, 0.66f, 1.0),            // 14        
		Vec4(0.50f, 0.66f, 0.66f, 1.0),            // 15        

		Vec4(0.25f, 0.33f, 0.33f, 1.0),  // TOP    // 16        
		Vec4(0.25f, 0.66f, 0.66f, 1.0),            // 17        
		Vec4(0.50f, 0.66f, 0.66f, 1.0),            // 18        
		Vec4(0.50f, 0.33f, 0.33f, 1.0),            // 19     

		Vec4(0.75f, 0.66f, 0.66f, 1.0),  // BOTTOM // 20        
		Vec4(0.75f, 0.33f, 0.33f, 1.0),            // 21        
		Vec4(1.00f, 0.33f, 0.33f, 1.0),            // 22        
		Vec4(1.00f, 0.66f, 0.66f, 1.0),            // 23        
	};
	GLuint Indexdata[] =
	{
		0,1,2,    0,2,3,     // FRONT
		6,5,4,    4,7,6,     // RIGHT 
		10,9,8,   8,11,10,   // BACK
		12,13,14, 12,14,15,  // LEFT
		18,17,16, 16,19,18,  // TOP
		20,21,22, 20,22,23   // BOTTOM
	};

	Vec3 NormalData[24];

	for_loop(Index, 24)
	{
		float x = V[Index].x;
		float y = V[Index].y;
		float z = V[Index].z;

		float  magnitude = sqrt(Squared(x) + Squared(y) + Squared(z));

		if (magnitude != 0)
		{
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
		}
		NormalData[Index] = Vec3(x, y, z); //  Vec3(RANDOM(2)-1,RANDOM(2)-1,RANDOM(2)-1);//
	}
	for_loop(Index, 24)
	{
		VertexList.push_back(V[Index]);
		TextureCoords.push_back(UVs[Index]);
		NormalList.push_back(NormalData[Index]);
	}
	for_loop(Index, 36)
	{
		IndexList.push_back(Indexdata[Index]);
	}

	// Averages all the normals so Each Faces Vertices have normals facing straight out to prevent interpolation of the light
	// Across the block

	for (size_t Index = 0; Index < 24; Index += 4)
	{
		Vec3 Norm = NormalList[Index] +
			NormalList[Index + 1] +
			NormalList[Index + 2] +
			NormalList[Index + 3];

		Norm /= 4;

		NormalList[Index] = Norm;
		NormalList[Index + 1] = Norm;
		NormalList[Index + 2] = Norm;
		NormalList[Index + 3] = Norm;
	}

	//	Polygons = new VAOBuffer();
	PrimativeType = GL_TRIANGLES;
	VAO = new VertexArrayObject();
	VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec3>(&VertexList[0], 24));
	VAO->Attach(BufferTypes::UVCOORD, new VertexBufferObject<Vec2>(&TextureCoords[0], 24));
	VAO->Attach(BufferTypes::NORMAL, new  VertexBufferObject<Vec3>(&NormalList[0], 24));
	VAO->Attach(BufferTypes::COLOR, new VertexBufferObject<Vec4>(&Cols[0], 24));

	//VAO.Attach(new ElementArrayBuffer(&IndexList[0], 36));
	//VAO.Attach(BufferTypes::INDICE, new VertexBufferObject<GLuint>(&IndexList[0], 36));

	Transform = glm::mat4(1.0f); // Set Identity and Rotate all axis followed with the Translation.
	Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
	Transform = glm::translate(Transform, Position);
	Rotation = Vec3(rand() % 360, rand() % 360, rand() % 360);
}
Cube::Cube(Vec3 pos, float size)

{
	Position = pos;

	std::vector<Vec3>    VertexList;
	std::vector<Vec3>    NormalList;
	std::vector<Vec2>    TextureCoords;
	std::vector<Vec4>    ColorList;
	std::vector<GLuint>  IndexList;

	Vec3 V[] =
	{
		Vec3(-size,-size, size), // FRONT    // 0                                             
		Vec3(size,-size, size),             // 1                                    
		Vec3(size, size, size),             // 2                                    
		Vec3(-size, size, size),             // 3                                    

		Vec3(size, size, size), // RIGHT    // 4    2                                        
		Vec3(size, size,-size),             // 5                                    
		Vec3(size,-size,-size),             // 6                                    
		Vec3(size,-size, size),             // 7    1                                

		Vec3(-size,-size,-size), // BACK     // 8                                            
		Vec3(size,-size,-size),             // 9    6                               
		Vec3(size, size,-size),             // 10   5                                
		Vec3(-size, size,-size),             // 11

		Vec3(-size,-size,-size), // LEFT     // 12   8                                        
		Vec3(-size,-size, size),             // 13   7                              
		Vec3(-size, size, size),             // 14   3                                
		Vec3(-size, size,-size),             // 15  11                                

		Vec3(size, size, size), // TOP      // 16   2                                      
		Vec3(-size, size, size),             // 17   3                                
		Vec3(-size, size,-size),             // 18   11                                
		Vec3(size, size,-size),             // 19   5                                

		Vec3(-size,-size,-size), // BOTTOM   // 20   8                                   
		Vec3(size,-size,-size),             // 21   9                          
		Vec3(size,-size, size),             // 22   1                          
		Vec3(-size,-size, size),             // 23   0                               
	};
#if _CUBEMAPS_
	Vec2 UVs[] =
	{
		Vec2(0.00f, 0.66f),  // FRONT  // 0   
		Vec2(0.00f, 0.33f),            // 1   
		Vec2(0.25f, 0.33f),            // 2   
		Vec2(0.25f, 0.66f),            // 3   

		Vec2(0.25f, 0.33f),  // RIGHT  // 4   
		Vec2(0.50f, 0.33f),            // 5   
		Vec2(0.50f, 0.00f),            // 6   
		Vec2(0.25f, 0.00f),            // 7   

		Vec2(0.75f, 0.66f),  // BACK   // 8   
		Vec2(0.75f, 0.33f),            // 9   
		Vec2(0.50f, 0.33f),            // 10  
		Vec2(0.50f, 0.66f),            // 11  

		Vec2(0.50f, 1.00f),  // LEFT   // 12  
		Vec2(0.25f, 1.00f),            // 13  
		Vec2(0.25f, 0.66f),            // 14  
		Vec2(0.50f, 0.66f),            // 15  

		Vec2(0.25f, 0.33f),  // TOP    // 16  
		Vec2(0.25f, 0.66f),            // 17  
		Vec2(0.50f, 0.66f),            // 18  
		Vec2(0.50f, 0.33f),            // 19  

		Vec2(0.75f, 0.66f),  // BOTTOM // 20  
		Vec2(0.75f, 0.33f),            // 21  
		Vec2(1.00f, 0.33f),            // 22  
		Vec2(1.00f, 0.66f),            // 23  
	};
#else

	Vec2 UVs[] =
	{
		Vec2(0.00f, 0.00f),  // FRONT  // 0   
		Vec2(0.00f, 1.00f),            // 1   
		Vec2(1.00f, 1.00f),            // 2   
		Vec2(1.00f, 0.00f),            // 3   

		Vec2(0.00f, 0.00f),  // RIGHT  // 4   
		Vec2(0.00f, 1.00f),            // 5   
		Vec2(1.00f, 1.00f),            // 6   
		Vec2(1.00f, 0.00f),            // 7   

		Vec2(0.00f, 0.00f),  // BACK   // 8   
		Vec2(0.00f, 1.00f),            // 9   
		Vec2(1.00f, 1.00f),            // 10  
		Vec2(1.00f, 0.00f),            // 11  

		Vec2(0.00f, 0.00f),  // LEFT   // 12  
		Vec2(0.00f, 1.00f),            // 13  
		Vec2(1.00f, 1.00f),            // 14  
		Vec2(1.00f, 0.00f),            // 15  

		Vec2(0.00f, 0.00f),  // TOP    // 16  
		Vec2(0.00f, 1.00f),            // 17  
		Vec2(1.00f, 1.00f),            // 18  
		Vec2(1.00f, 0.00f),            // 19  

		Vec2(0.00f, 0.00f),  // BOTTOM // 20  
		Vec2(0.00f, 1.00f),            // 21  
		Vec2(1.00f, 1.00f),            // 22  
		Vec2(1.00f, 0.00f),            // 23  
	};
#endif

	Vec4 Cols[] =
	{
		Vec4(1.0, 1.0, 1.0, 1.0),  // FRONT  // 0         
		Vec4(0.0, 1.0, 1.0, 1.0),            // 1         
		Vec4(0.0, 0.5, 1.0, 1.0),            // 2         
		Vec4(0.0, 0.0, 0.0, 1.0),            // 3         

		Vec4(0.0, 0.0, 0.0, 1.0),  // RIGHT  // 4         
		Vec4(0.0, 0.0, 1.0, 1.0),            // 5         
		Vec4(0.0, 1.0, 1.0, 1.0),            // 6         
		Vec4(1.0, 1.0, 1.0, 1.0),            // 7         

		Vec4(1.0, 0.0, 1.0, 1.0),  // BACK   // 8         
		Vec4(0.0, 1.0, 0.0, 1.0),            // 9         
		Vec4(1.0, 1.0, 0.0, 1.0),            // 10        
		Vec4(1.0, 0.0, 0.0, 1.0),            // 11

		Vec4(0.50f, 1.00f, 1.00f, 1.0),  // LEFT   // 12        
		Vec4(0.25f, 1.00f, 1.00f, 1.0),            // 13        
		Vec4(0.25f, 0.66f, 0.66f, 1.0),            // 14        
		Vec4(0.50f, 0.66f, 0.66f, 1.0),            // 15        

		Vec4(0.25f, 0.33f, 0.33f, 1.0),  // TOP    // 16        
		Vec4(0.25f, 0.66f, 0.66f, 1.0),            // 17        
		Vec4(0.50f, 0.66f, 0.66f, 1.0),            // 18        
		Vec4(0.50f, 0.33f, 0.33f, 1.0),            // 19     

		Vec4(0.75f, 0.66f, 0.66f, 1.0),  // BOTTOM // 20        
		Vec4(0.75f, 0.33f, 0.33f, 1.0),            // 21        
		Vec4(1.00f, 0.33f, 0.33f, 1.0),            // 22        
		Vec4(1.00f, 0.66f, 0.66f, 1.0),            // 23        
	};
	GLuint Indexdata[] =
	{
		0,1,2,    0,2,3,     // FRONT
		6,5,4,    4,7,6,     // RIGHT 
		10,9,8,   8,11,10,   // BACK
		12,13,14, 12,14,15,  // LEFT
		18,17,16, 16,19,18,  // TOP
		20,21,22, 20,22,23   // BOTTOM
	};

	Vec3 NormalData[24];

	for_loop(Index, 24)
	{
		float x = V[Index].x;
		float y = V[Index].y;
		float z = V[Index].z;

		float  magnitude = sqrt(Squared(x) + Squared(y) + Squared(z));

		if (magnitude != 0)
		{
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
		}
		NormalData[Index] = Vec3(x, y, z); //  Vec3(RANDOM(2)-1,RANDOM(2)-1,RANDOM(2)-1);//
	}
	for_loop(Index, 24)
	{
		VertexList.push_back(V[Index]);
		TextureCoords.push_back(UVs[Index]);
		NormalList.push_back(NormalData[Index]);
	}
	for_loop(Index, 36)
	{
		IndexList.push_back(Indexdata[Index]);
	}

	// Averages all the normals so Each Faces Vertices have normals facing straight out to prevent interpolation of the light
	// Across the block

	for (int Index = 0; Index < 24; Index += 4)
	{
		Vec3 Norm = NormalList[Index] +
			NormalList[Index + 1] +
			NormalList[Index + 2] +
			NormalList[Index + 3];

		Norm /= 4;

		NormalList[Index] = Norm;
		NormalList[Index + 1] = Norm;
		NormalList[Index + 2] = Norm;
		NormalList[Index + 3] = Norm;

	}


	VAO = new VertexArrayObject();
	VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec3>(&VertexList[0], 24));
	VAO->Attach(BufferTypes::UVCOORD, new VertexBufferObject<Vec2>(&TextureCoords[0], 24));
	VAO->Attach(BufferTypes::NORMAL, new  VertexBufferObject<Vec3>(&NormalList[0], 24));
	VAO->Attach(BufferTypes::COLOR, new VertexBufferObject<Vec4>(&Cols[0], 24));
	VAO->Attach(BufferTypes::INDICE, new VertexBufferObject<GLuint>(&IndexList[0], 36));

	Transform = glm::mat4(1.0f); // Set Identity and Rotate all axis followed with the Translation.
	Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
	Transform = glm::translate(Transform, Position);
}
Plane::Plane(Vec3 pos, Vec3 rotation, float width, float height)
{
	Position = pos;
	Rotation = rotation;
	Size = Vec2(width, height);

	width *= .5f;
	height *= .5f;

	Transform = glm::rotate(Mat4(1.0f), glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
	Transform = glm::translate(Transform, Position);
	VAO = new VertexArrayObject();

	Vec3 V[] =
	{
		Vec3(Position.x - width,    Position.y - height,     Position.z), // FRONT    // 0                                             
		Vec3(Position.x + width,    Position.y - height,     Position.z),             // 1                                    
		Vec3(Position.x + width,    Position.y + height,     Position.z),             // 2                                    
		Vec3(Position.x - width,    Position.y + height,     Position.z),             // 3        
	};
	Vec3 N[] =
	{
		Vec3(1.0f / Position.x - width,1.0f / Position.y - height,1.0f / Position.z), // FRONT    // 0                                             
		Vec3(1.0f / Position.x + width,1.0f / Position.y - height,1.0f / Position.z),             // 1                                    
		Vec3(1.0f / Position.x + width,1.0f / Position.y + height,1.0f / Position.z),             // 2                                    
		Vec3(1.0f / Position.x - width,1.0f / Position.y + height,1.0f / Position.z),             // 3        
	};
	Vec2 UV[] =
	{
		Vec2(0.0f, 0.0f),
		Vec2(0.0f, 1.0f),
		Vec2(1.0f, 1.0f),
		Vec2(1.0f, 0.0f),
	};
	GLuint I[] =
	{
		3,0,1,   1,2,3,  3,1,0, 1,3,2
	};

	Vec3 Norm;

	for_loop(Index, 4)
	{
		Norm += N[Index];
	}
	Norm /= 4;
	for_loop(Index, 4)
	{
		N[Index] = Norm;
	}

	VAO = new VertexArrayObject();
	VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec3>(&V[0], 4));
	VAO->Attach(BufferTypes::UVCOORD, new VertexBufferObject<Vec2>(&UV[0], 4));
	VAO->Attach(BufferTypes::NORMAL, new  VertexBufferObject<Vec3>(&N[0], 4));
	VAO->Attach(BufferTypes::INDICE, new VertexBufferObject<GLuint>(&I[0], 12));
}
Sphere::Sphere(Vec3 pos, float radius, int sectors)
	:
	Radius(radius)
{
	PrimativeType = GL_TRIANGLES;
	Position = pos;

	std::vector<Vec3> Vertices = vertices;
	std::vector<Triangle> Triangles = triangles;
	std::vector<Vec3> Tangent;
	for (int i = 0; i < sectors; ++i)
	{
		Triangles = subdivide(Vertices, Triangles);
	}
	std::vector<Vec3>Normals;
	std::vector<Vec3>Colors;

	for (auto &V : Vertices)
	{
		V *= radius;
		float x = V.x;
		float y = V.y;
		float z = V.z;
	
		float  magnitude = sqrt(Squared(x) + Squared(y) + Squared(z));

		if (magnitude != 0)
		{
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
		}
		Normals.push_back(Vec3(x, y, z));  
		Colors.push_back(Vec3(x, y, z));
		
	}


	std::vector<Vec2> UV;
	for (auto &N : Normals)
	{
		float theta = (atan2f(N.x, N.z) / M_PI) / 2.f + 0.5f;
		float phi = (asinf(-N.y) / (M_PI / 2.0f)) / 2.f + 0.5f;
		UV.push_back(Vec2(theta, phi));
	}
	std::vector<Vec3> Tan;
	for (size_t i = 0; i < Vertices.size(); i += 3)
	{
		glm::vec3 v0 = Vertices[i + 0];
		glm::vec3 v1 = Vertices[i + 1];
		glm::vec3 v2 = Vertices[i + 2];

		// Shortcuts for UVs
		glm::vec2 uv0 = UV[i + 0];
		glm::vec2 uv1 = UV[i + 1];
		glm::vec2 uv2 = UV[i + 2];

		// Edges of the triangle : position delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;

		Tan.push_back(tangent);
		Tan.push_back(tangent);
		Tan.push_back(tangent);
	}

	for (int i = 0; i < Vertices.size(); i++)
	{
		Vec3 n = Normals[i];
		Vec3 t = Tan[i];

		// Gram-Schmidt orthogonalize
		Vec3 tangent = (t - n * (glm::dot(glm::normalize(n), glm::normalize(t))));

		// Calculate handedness
		//tangent.w = (glm::dot(glm::cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
		Tangent.push_back(tangent);
	}


	VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec3>(&Vertices[0], Vertices.size()));
	VAO->Attach(BufferTypes::NORMAL, new VertexBufferObject<Vec3>(&Normals[0], Normals.size()));
	VAO->Attach(BufferTypes::COLOR, new VertexBufferObject<Vec3>(&Colors[0], Colors.size()));
	VAO->Attach(BufferTypes::UVCOORD, new VertexBufferObject<Vec2>(&UV[0], UV.size()));
	VAO->Attach(BufferTypes::TANGENT, new VertexBufferObject<Vec3>(&Tangent[0], Tangent.size()));
	VAO->Attach(BufferTypes::INDICE, new VertexBufferObject<GLuint>(&Triangles[0].vertex[0], Triangles.size() * 3));

	Transform = glm::mat4(1.0f); // Set Identity and Rotate all axis followed with the Translation.
	Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
	Transform = glm::translate(Transform, Position);

 	std::vector<GLuint> inds;
	for (auto& T : Triangles)
	{
		inds.push_back(T.vertex[0]);
		inds.push_back(T.vertex[1]);
		inds.push_back(T.vertex[2]);
	}
	BatchList.push_back
	(
		{
			Vertices,
			Normals,
			Colors,
			UV,
			Tangent,
			inds,
			Position,
			Rotation
		}
	);
}

void Sphere::MakeBatch()
{
	std::vector <Vec3> Vertices;
	std::vector <Vec3> Normals;
	std::vector <Vec3> Colors;
	std::vector <Vec3> Tangents;
	std::vector <Vec2> UV;
	std::vector <GLuint> Indices;

	for (auto& B : BatchList)
	{
		for (auto &I : B.Indices)
		{
			Indices.push_back(I + Vertices.size());
		}
		for (auto& V : B.Vertices)
		{
	 		Vertices.push_back(V + (B.Position));
	 	}
		Normals.insert(Normals.end(), B.Normals.begin(), B.Normals.end());
		Tangents.insert(Tangents.end(), B.Tangents.begin(), B.Tangents.end());
		Colors.insert(Colors.end(), B.Colors.begin(), B.Colors.end());
		UV.insert(UV.end(), B.UV.begin(), B.UV.end());
	}

	BatchVAO.Position = Vec3(0);
	BatchVAO.Rotation = Vec3(0);
	BatchVAO.PrimativeType = GL_TRIANGLES;

	BatchVAO.VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec3>(&Vertices[0], Vertices.size()));
	BatchVAO.VAO->Attach(BufferTypes::NORMAL, new VertexBufferObject<Vec3>(&Normals[0], Normals.size()));
	BatchVAO.VAO->Attach(BufferTypes::COLOR, new VertexBufferObject<Vec3>(&Colors[0], Colors.size()));
	BatchVAO.VAO->Attach(BufferTypes::UVCOORD, new VertexBufferObject<Vec2>(&UV[0], UV.size()));
	BatchVAO.VAO->Attach(BufferTypes::TANGENT, new VertexBufferObject<Vec3>(&Tangents[0], Tangents.size()));
	BatchVAO.VAO->Attach(BufferTypes::INDICE, new VertexBufferObject<GLuint>(&Indices[0], Indices.size()));

	BatchVAO.Transform = glm::mat4(1.0f); // Set Identity and Rotate all axis followed with the Translation.
	BatchVAO.Transform = glm::rotate(BatchVAO.Transform, glm::radians(BatchVAO.Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
	BatchVAO.Transform = glm::rotate(BatchVAO.Transform, glm::radians(BatchVAO.Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
	BatchVAO.Transform = glm::rotate(BatchVAO.Transform, glm::radians(BatchVAO.Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
	BatchVAO.Transform = glm::translate(BatchVAO.Transform, BatchVAO.Position);

//BatchVAO.IndirectDraw.count = Vertices.size();
//BatchVAO.IndirectDraw.instanceCount;
//BatchVAO.IndirectDraw.firstIndex;
//BatchVAO.IndirectDraw.baseVertex;
//BatchVAO.IndirectDraw.baseInstance;

	std::vector<Batch> emp;
	BatchList.swap(emp);
}

Torus::Torus(Vec3 position, int numc, int numt, float scale)
{
	Print("ERROR: TORUS PRIMATIVE NEVER COMPLETED!");
	Scale = Vec3(scale);
	Position = position;

	std::vector<Vec3> Verts;
	std::vector<Vec3> Cols;
	std::vector<Vec3> Norms;
	std::vector<Vec2> UVs;
	std::vector<GLuint> Inds;

	//	ObjectType = Torus_t;
	//	List = glGenLists(1);
	//	glNewList(List, GL_COMPILE);

	int i, j, k;
	double s, t, x, y, z, twopi;

	twopi = 2 * (double)M_PI;
	numc += (numc % 3);	numt += (numt % 3);
	for (i = 0; i < numc; i++) {
		for (j = 0; j <= numt; j++) {
			for (k = 1; k >= 0; k--) {
				s = (i + k) % numc + 0.5;
				t = j % numt;

				x = (float)Scale.x * ((1 + .1 * cos(s * twopi / numc)) * cos(t * twopi / numt));
				y = (float)Scale.y * ((1 + .1 * cos(s * twopi / numc)) * sin(t * twopi / numt));
				z = (float)Scale.z *      (.1 * sin(s * twopi / numc));

				//glColor3f(GL_Color(x* 255.0), GL_Color(y* 255.0), GL_Color(z * 255.0));
				Cols.push_back(Vec3(GL_Color(x* 255.0), GL_Color(y* 255.0), GL_Color(z * 255.0)));

				//glVertex3f(x, y, z);
				Verts.push_back(Vec3(x, y, z));
				float Mag = sqrt(Squared(x) + Squared(y) * Squared(z));
				Norms.push_back(Vec3(x / Mag, y / Mag, z / Mag));
				UVs.push_back(Vec2(Norms.back().x, Norms.back().y));
			}
		}
	}

	// Getting Displayed as a Quad Strip so must implement the indices to behave as such.
	//glBegin(GL_QUAD_STRIP);
	for_loop(Index, Verts.size())
	{
		//		Inds.push_back(Index + 0);

	}
	for_loop(Index, Verts.size())
	{
		//	Inds.push_back(Index + 0);		Inds.push_back(Index + 0);
		// Inds.push_back(Index + 1);
	}
}
Diamond::Diamond(Vec3 position, float size)
{
}












// Vertices.insert(Vertices.begin(), B.Vertices.begin(), B.Vertices.end());
//  for (auto& S : )
//  {
//  	BatchList.Vertices.insert(Vertices.begin(), S->VAO. );
//  }
//  	myvector.insert(myvector.begin(), myarray, myarray + 3);
/*

send(to,from, count)
reguister short *to, *from;
register count;

{
register n=(count+7)/8
switch(count%8)
{
case 0: do{ *to = *from++;
case 7: *to = *from++;
case 6: *to = *from++;
case 5: *to = *from++;
case 4: *to = *from++;
case 3: *to = *from++;
case 2: *to = *from++;
case 1: *to = *from++;
}while(--n>0);
}
} DUFF MACHINE
*/