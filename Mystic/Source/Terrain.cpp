#include"Common.h"
#include"Terrain.h"

#define my_sizeof(type) ((char *)(&type+1)-(char*)(&type))


Terrain::Terrain(GLuint x, GLuint y, GLuint width, GLuint height)
	:
	Size(0)
{

	HeightMap = nullptr;

	glGenTextures(1, &MapID);
	glBindTexture(GL_TEXTURE_2D, MapID);

	//  Sampling mode for blending textals
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//  Texture Layout when sampling past texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, Size.x, Size.y, 0, GL_RED, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
}

const int Limit = 100;

Terrain::Terrain(Vec3 position, int width, int height, Texture *groundtex)
	:
	MapID(0),
	HeightMap(nullptr)
{
	Position = position;
	Rotation = Vec3(0);
	PrimativeType = GL_TRIANGLES;

	VertexCount = 0, ColorsCount = 0, NormalCount = 0, IndexCount = 0;

	GridSize = Vec2(.5f);
	Position =
		Vec3
		(// Fix this later to give a position to the Terrain
			position.x - (width * (.5f * GridSize.x)),
			position.y + 50,
			position.z - (width * (.5f * GridSize.y))
		);

	Width = width;    // Size.x; // HeightMapFile->Width;  // 
	Height = height;   // Size.y; // HeightMapFile->Height; // 
	Size = Vec2(Width, Height);

	for_loop(x, 1000)
	{
		for_loop(y, 1000)
		{
			Gradient[x][y][0] = RANDOM(8);
			Gradient[x][y][1] = RANDOM(8);
		}
	}

	HeightMap = new GLfloat[(size_t)Width * (size_t)Height];
	for_loop(y, Size.y)
	{
		for_loop(x, Size.x)
		{
			HeightMap[(size_t)x * (Width) +(size_t)y] = ///warning C6386: Buffer overrun while writing to 'HeightMap':  the writable size is 'Width*Height*4' bytes, but '8' bytes might be written.
				//PerlinNoise2D(x * (PerlinNoise2D(x * .1, y * .2) * .25), y * (PerlinNoise2D(x * .2, y * .3)));
				(PerlinNoise2D(x * .01f, y * .01f)) +
				(PerlinNoise2D(x * .01f, y * .02f) * .25f) +
				(PerlinNoise2D(x * .05f, y * .05f))+ (PerlinNoise2D(x * .5f, y * .5f)* .1f);

            if (HeightMap[x * (Width)+y] > Max_Height)
            {
                Max_Height = HeightMap[(size_t)x * (size_t)(Width) + (size_t)y]; /// warning C6385: Reading invalid data from 'HeightMap':  the readable size is 'Width*Height*4' bytes, but '8' bytes may be read.
            }
		}
	}
	///warning C26451: Arithmetic overflow: Using operator '*' on a 4 byte value and then casting the result to a 8 byte value. Cast the value to the wider type before calling operator '*' to avoid overflow (io.2).
	/// ALL LIKELY BULLSHIT
	Vec3 *Vertices = new Vec3[(size_t)Width * (size_t)Height];
	fRGB *Colors = new Vec3[(size_t)Width * (size_t)Height];
	Vec3 *Normals = new Vec3[(((size_t)Width - 1)) * (((size_t)Height - 1) * 2)];
	GLuint *Indices = new GLuint[(((size_t)Width) * ((size_t)Height) * 3) * 2];
	std::vector<Vec3> Verts;
	std::vector<Vec3> Cols;
	std::vector<GLuint> Inds;
	//--------------------------------------------- CALCULATE VERTICES -------------------------------------------------------------------------
	float h = 0;
	float ColorNormal =  1.0f / Max_Height;
	for_loop(Z, Height)
	{
		for_loop(X, Width)
		{
			float TerrainHeight = HeightMap[X + Width * Z];
			h += .001f;
			Vertices[VertexCount].x = X * GridSize.x;
			Vertices[VertexCount].y = TerrainHeight * 2;
			Vertices[VertexCount].z = Z * GridSize.y;
			Verts.push_back(Vec3(X * GridSize.x, TerrainHeight * 2, Z * GridSize.y));

			Colors[ColorsCount].r = TerrainHeight * ColorNormal; // 1;//GL_Color((ColorsCount % 4) * 64);
			Colors[ColorsCount].g = TerrainHeight * ColorNormal;// .1;//GL_Color(((ColorsCount +1) % 4)* 64);
				Colors[ColorsCount].b =  TerrainHeight * (ColorNormal * 2);// 1;//GL_Color(((ColorsCount +2) % 4)* 64);
			Cols.push_back(Colors[ColorsCount]);
			VertexCount++;
			ColorsCount++;
		}
	}
	// --------------------------------------------- CALCULATE INDICES ------------------------------------------------------------------------
	for_loop(Y, Height - 1)
	{
		for_loop(X, Width - 1)
		{
			

			Indices[IndexCount] = X + Width * Y;     // 1
			Indices[IndexCount + 1] = (X + 1) + Width * Y;      // 2
			Indices[IndexCount + 2] = X + Width * (Y + 1); // 4

			Indices[IndexCount + 3] = (X + 1) + Width * (Y + 1); // 3
			Indices[IndexCount + 4] = X + Width * (Y + 1); // 4
			Indices[IndexCount + 5] = (X + 1) + Width * Y;       // 2

			Inds.push_back(X + Width * Y);
			Inds.push_back((X + 1) + Width * Y);
			Inds.push_back(X + Width * (Y + 1));
			Inds.push_back((X + 1) + Width * (Y + 1));
			Inds.push_back(X + Width * (Y + 1));
			Inds.push_back((X + 1) + Width * Y);

			IndexCount += 6;
		}
	}
	std::vector<Vec3> Norms;
	// --------------------------------------------- CALCULATE NORMALS ---------------------------------------------------------------------   
	for_loop(Y, Height - 1)
	{
		for_loop(X, Width - 1)
		{
			Normals[NormalCount] = GetNormal(Vertices[Get_Vertex(X, Y)],           // 1 
				Vertices[Get_Vertex(X + 1, Y)],       // 2 
				Vertices[Get_Vertex(X, Y + 1)]);      // 4 

			Normals[NormalCount + 1] = GetNormal(Vertices[Get_Vertex(X + 1, Y + 1)], // 3
				Vertices[Get_Vertex(X, Y + 1)],     // 4
				Vertices[Get_Vertex(X + 1, Y)]);    // 2
			NormalCount += 2;
			Norms.push_back(Normals[NormalCount]);	
			Norms.push_back(Normals[NormalCount + 1]);
		}
	}

	std::vector<Vec2> textcoords;

	//Vec2 UVs[4] = 
	//{                                                   
	//    Vec2(0.66f, 0.50f),            // 17    
	//    Vec2(0.66f, 0.00f),            // 18     
	//    Vec2(0.345f, 0.00f),            // 19         
	//    Vec2(0.345f, 0.50f)  // TOP    // 16     
	//};

	for_loop(count, VertexCount / 4)
	{

		// textcoords.push_back(Vec2(0.00f, 1.00f));
		// textcoords.push_back(Vec2(0.33f, 1.00f));
		// textcoords.push_back(Vec2(0.33f, 0.50f));
		// textcoords.push_back(Vec2(0.00f, 0.50f));
		//
		// textcoords.push_back(Vec2(0.33f, 0.50f));
		// textcoords.push_back(Vec2(0.66f, 0.50f));
		// textcoords.push_back(Vec2(0.66f, 1.00f));
		// textcoords.push_back(Vec2(0.33f, 1.00f));
		//
		// textcoords.push_back(Vec2(1.00f, 1.00f));
		// textcoords.push_back(Vec2(0.66f, 1.00f));
		// textcoords.push_back(Vec2(0.66f, 0.50f));
		// textcoords.push_back(Vec2(1.00f, 0.50f));
		//
		// textcoords.push_back(Vec2(0.00f, 0.50f));
		// textcoords.push_back(Vec2(0.33f, 0.50f));
		// textcoords.push_back(Vec2(0.33f, 0.00f));
		// textcoords.push_back(Vec2(0.00f, 0.00f));
		//
		// textcoords.push_back(Vec2(0.67f, 0.50f));
		// textcoords.push_back(Vec2(0.67f, 0.00f));
		// textcoords.push_back(Vec2(1.00f, 0.00f));
		// textcoords.push_back(Vec2(1.00f, 0.50f));



	}
	for_loop(y, Height / 2)
	{
		for_loop(x, Width)
		{


			textcoords.push_back(Vec2(0.66f, 0.00f));
			textcoords.push_back(Vec2(0.345f, 0.50f));
		}
		for_loop(x, Width)
		{
			textcoords.push_back(Vec2(0.66f, 0.00f));
			textcoords.push_back(Vec2(0.345f, 0.50f));
			textcoords.push_back(Vec2(0.345f, 0.00f));
			textcoords.push_back(Vec2(0.66f, 0.50f));

		}

	}	// ---------------------------------------------- Allocate Buffers ----------------------------------------------------------------------------   
	VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec3>(&Verts[0], Verts.size()));
	VAO->Attach(BufferTypes::COLOR, new  VertexBufferObject<Vec3>(&Cols[0], Cols.size()));
	VAO->Attach(BufferTypes::NORMAL, new  VertexBufferObject<Vec3>(&Norms[0], Norms.size()));
	VAO->Attach(BufferTypes::UVCOORD, new VertexBufferObject<Vec2>(&textcoords[0], textcoords.size()));
	VAO->Attach(BufferTypes::INDICE, new VertexBufferObject<GLuint>(&Inds[0], Inds.size()));
	Transform = glm::mat4(1.0f); // Set Identity and Rotate all axis followed with the Translation.
	Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
	Transform = glm::translate(Transform, Position);

	delete[](Vertices);
	delete[](Colors);
	delete[](Normals);
	delete[](Indices);
}
void  Terrain::Update()
{
	Transform = glm::mat4(1.0f);
	Transform = glm::translate(Transform, Position);
	Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));

}
// void  Terrain::Render()
// {
// 	///__debugbreak();
// 	/// NOT SURE IF WE WILL GET HERE AT ALL
// 	//glPushMatrix();
// 	//glTranslatef(Position.x, Position.y, Position.z);
// 	Bind();
// 	glDrawElements(GL_TRIANGLES, VAO->Indices->ElementCount, GL_UNSIGNED_SHORT, nullptr);
// 	Unbind();
// 	glPopMatrix();
// 
// #if 0
// 
// 	int NCount = 0,
// 		VCount = 0,
// 		count = 0;
// 
// 	Vec3 pos,
// 		pos2;
// 
// 	float t = 5;
// 
// 	for_loop(Y, depth - 1)
// 	{
// 		for_loop(X, width - 1)
// 		{
// 			//===================================================================
// 			pos = VAO.Vertices->Data[Get_Vertex(X, Y)] +                     // 1 
// 				VAO.Vertices->Data[Get_Vertex(X + 1, Y)] +                     // 2 
// 				VAO.Vertices->Data[Get_Vertex(X, Y + 1)];                       // 4 
// 			pos = pos / 3;
// 
// 			//====================================================================   
// 			pos2 = VAO.Vertices->Data[Get_Vertex(X + 1, Y + 1)] +                 // 3
// 				VAO.Vertices->Data[Get_Vertex(X, Y + 1)] +                 // 4
// 				VAO.Vertices->Data[Get_Vertex(X + 1, Y)];                      // 2
// 			pos2 = pos2 / 3;
// 
// 			//===========================================================================================
// 			glBegin(GL_LINES);
// 			glColor3f(GL_Color(255), GL_Color(0), GL_Color(0));
// 			glVertex3f(Position.x + pos.x,
// 				Position.y + pos.y,
// 				Position.z + pos.z);
// 			glVertex3f(Position.x + pos.x + VAO.Normals->Data[NCount].x * (t),
// 				Position.y + pos.y + VAO.Normals->Data[NCount].y * (t),
// 				Position.z + pos.z + VAO.Normals->Data[NCount].z * (t));
// 
// 			glColor3f(GL_Color(0), GL_Color(255), GL_Color(0));
// 			glVertex3f(Position.x + pos2.x,
// 				Position.y + pos2.y,
// 				Position.z + pos2.z);
// 			glVertex3f(Position.x + pos2.x + VAO.Normals->Data[NCount + 1].x * (t),
// 				Position.y + pos2.y + VAO.Normals->Data[NCount + 1].y * (t),
// 				Position.z + pos2.z + VAO.Normals->Data[NCount + 1].z * (t));
// 			glEnd();
// 			//===========================================================================================
// 			VCount++;
// 			NCount = VCount * 2;
// 
// 		}
// 	}
// 
// #endif
// }
//void  Terrain::AttachTexture(Texture *groundtex)
//{
//	//GroundTextures.push_back(groundtex);
//
//	Vec2 UV[2] = { Vec2(0,0), Vec2(1,1) };
//	Vec2 UVs[4] =
//	{
//		Vec2(0.66f, 0.50f),            // 17    
//		Vec2(0.66f, 0.00f),            // 18     
//		Vec2(0.345f, 0.00f),            // 19         
//		Vec2(0.345f, 0.50f)  // TOP    // 16     
//	};
//}
float Terrain::DotGridGradient(int ix, int iy, float x, float y) {

	// Precomputed (or otherwise) gradient vectors at each grid node
    // extern float Gradient[IYMAX][IXMAX][2];
	// Compute the distance vector

	Vec2 Distance(x - (float)ix,
		y - (float)iy);

	// Compute the dot-product
	return (Distance.x * (Gradient[iy][ix][0]) +
		Distance.y * (Gradient[iy][ix][1]));
}
float Terrain::PerlinNoise2D(float x, float y)
{

	// Determine grid cell coordinates
	int x0 = int(x);
	int x1 = x0 + 1;

	int y0 = int(y);
	int y1 = y0 + 1;

	// Determine interpolation weights
	// Could also use higher order polynomial/s-curve here
	Vec2 s_curve = Vec2(x - (float)x0,
		y - (float)y0);

	// Interpolate between grid point gradients
	float n0, n1, ix0, ix1, value;

	n0 = DotGridGradient(x0, y0, x, y);
	n1 = DotGridGradient(x1, y0, x, y);
	ix0 = Lerp(n0, n1, s_curve.x);

	n0 = DotGridGradient(x0, y1, x, y);
	n1 = DotGridGradient(x1, y1, x, y);
	ix1 = Lerp(n0, n1, s_curve.x);

	value = Lerp(ix0, ix1, s_curve.y);

	return value;
}
float Terrain::SampleTerrain(float x, float z)
{

	int ax = 0;
	int az = 0;

	int bx = 0;
	int bz = 0;

	float fracX = 0;
	float fracY = 0;

	float sx = 0;
	float sz = 0;

	float tl = 0;
	float tr = 0;
	float bl = 0;
	float br = 0;
	float Height = 0;

	Vec2 Top(0, 0);
	Vec2 Bottom(0, 0);


	float Nx = x / (float)GridSize.x,
		Ny = z / (float)GridSize.y;

	int NX = (int)Nx,
		NY = (int)Ny;

	//Assume that grid's center is at the origin. 
	x += (Width * .5f);
	z += (Height * .5f);

	if (x < 0.0f || x > Width * GridSize.x || z < 0.0f || z > Height * GridSize.y)
	{
		return -FLT_MAX;
	}

	sx = x / (float)GridSize.x; // NOTES:Translates Position to Grid Space.
	sz = z / (float)GridSize.y; //

	ax = (int)sx;
	az = (int)sz;

	bx = std::min((float)ax + 1, (float)Width);
	bz = std::min((float)az + 1, (float)Height);

	fracX = sx - ax;
	fracY = sz - az;

	///=========================================================================================================================
	TODO("The following portions need the Terrains Vertices pushed back so that I can sample at a later time- Do the same with the NORMALS vector");
	///=========================================================================================================================

	Top.x = VecVertices[ax + az * Width].y;   //tl  1    We're going to need these for linearily interpolate over a square
	Top.y = VecVertices[bx + az * Width].y;   //tr  2    {										   //      
	Bottom.x = VecVertices[ax + bz * Width].y;  //bl  3     
	Bottom.y = VecVertices[bx + bz * Width].y;  //br  4    }
	///=========================================================================================================================

	Height = Lerp(Lerp(Top.x, Top.y, fracX),
		Lerp(Bottom.x, Bottom.y, fracX),
		fracY); // Lerp between the left .. right values  

	int modx = int(x) % (int)(GridSize.x);
	int modz = int(z) % (int)(GridSize.y);

	Vec3 Norms;
	if ((modz + modx) < GridSize.x)
	{
		Norms = VecNormals[int((float)(ax + (Width - 1)* az) * 2)];
	}
	else
	{
		Norms = VecNormals[int((float)(ax + (Width - 1)* az) * 2) + 1];
	}

	// Preserve the Y Force, Find the Normal response Vector
	//float y = Collider[ball->CollisionID]->Body.Force.y;

	//    if (Collider[ball->CollisionID]->Body.Position.y + Collider[ball->CollisionID]->Radius >=  Height)
	//    {
	//        Collider[ball->CollisionID]->Body.Force += Norms;
	//        Collider[ball->CollisionID]->Body.Force.y = y;
	//        Collider[ball->CollisionID]->Body.Position.y = Height - Collider[ball->CollisionID]->Radius;
	//    }



	return  Height;
	// tl ---- tr
	//  |      |
	//  |      |
	// bl ---- br
	//
	// 2 ---- 3
	//  | \   |
	//  |   \ |
	// 1 ---- 4       
}
double Terrain::Lerp(float a0, float a1, float w)
{
	return (1.0 - w) * a0 + w * a1;
}
Vec3  Terrain::GetNormal(Vec3 v1, Vec3 v2, Vec3 v3)
{
	Vec3 Normal;
	// Calculate vectors
	float var1_x = v2.x - v1.x;
	float var1_y = v2.y - v1.y;
	float var1_z = v2.z - v1.z;

	float var2_x = v3.x - v1.x;
	float var2_y = v3.y - v1.y;
	float var2_z = v3.z - v1.z;

	// Get cross product of vectors
	Normal.x = (var1_y * var2_z) - (var2_y * var1_z);
	Normal.y = (var1_z * var2_x) - (var2_z * var1_x);
	Normal.z = (var1_x * var2_y) - (var2_x * var1_y);

	// Normalise final vector
	float vLen = sqrt((Normal.x * Normal.x) +
		(Normal.y * Normal.y) +
		(Normal.z * Normal.z));

	Normal.x = Normal.x / vLen;
	Normal.y = Normal.y / vLen;
	Normal.z = Normal.z / vLen;
	return Normal;
}
