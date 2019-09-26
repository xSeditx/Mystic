#pragma once
//#include"renderer.h"

#include"Common.h"
#include"Mesh.h"
/// Make a Submit for a full renderer that mimics what Batch Currently does, the Renderable will have an ID that marks its start offset and Size in each Attribute, possible interlace 
/// the data as it can be simpler to just place a stride into the Mesh object telling other objects to skip that many bytes
struct Batch
{// Adhoc class thrown together to turn many small VertexArrayObjects into a Single renderable
	Batch
	(
		std::vector <Vec3> _V,
		std::vector <Vec3>_N,
		std::vector <Vec3> _C,
		std::vector <Vec2> _U,
		std::vector <Vec3> _T,
		std::vector <GLuint> _I,
		Vec3 _pos,
		Vec3 _rot
	)
		:
		Vertices(_V),
		Normals(_N),
		Colors(_C),
		UV(_U),
		Tangents(_T),
		Indices(_I),
		Rotation(_rot),
		Position(_pos)
	{}
	std::vector <Vec3>   Vertices;
	std::vector <Vec3>   Normals;
	std::vector <Vec3>   Colors;
	std::vector <Vec2>   UV;

	std::vector <GLuint> Indices;
	std::vector <Vec3>   Tangents;
	Vec3 Rotation;
	Vec3 Position;
};
class MYSTIC Block :public Mesh
{
public:
	Block() = default;
	Block(Vec3 pos, float size);
};


class  MYSTIC Box : public Mesh
{
public:
	Box(Vec3 _pos, float _length, float _width, float _height);

	float Length, Width, Height;
};

class  MYSTIC Cube :public Mesh
{
public:
	Cube() = default;
	Cube(Vec3 pos, float size);
};

class  MYSTIC Sphere : public Mesh
{
public:
	Sphere() = default;
	Sphere(Vec3 pos, float radius, int sectors);

	float Radius = 0;

	static void MakeBatch();
	static std::vector<Batch> BatchList;
	static Sphere BatchVAO;


	struct Triangle
	{
		GLuint vertex[3];
	};
  

	//float Scale = 5.0;
	const float X =  .525731112119133606f ;
	const float Z =  .850650808352039932f;
	const float N = 0.f;

	const std::vector<Vec3> vertices =
	{
		{ -X, N, Z },{  X, N, Z },{ -X,  N, -Z },{  X, N,-Z },
		{  N, Z, X },{  N, Z,-X },{  N, -Z,  X },{  N,-Z,-X },
		{  Z, X, N },{ -Z, X, N },{  Z, -X,  N },{ -Z,-X, N }
	};
	const std::vector<Triangle> triangles =
	{
		{ 1,4,0 },{ 4,9,0 },{ 4,5,9 },{ 8,5,4 },{ 1,8,4 },
		{ 1,10,8 },{ 10,3,8 },{ 8,3,5 },{ 3,2,5 },{ 3,7,2 },
		{ 3,10,7 },{ 10,6,7 },{ 6,11,7 },{ 6,0,11 },{ 6,1,0 },
		{ 10,1,6 },{ 11,0,9 },{ 2,11,9 },{ 5,2,9 },{ 11,2,7 }
	};

//{ 0, 4, 1 }, { 0,9,4 }, { 9,5,4 }, { 4,5,8 }, { 4,8,1 },
//{ 8,10,1 }, { 8,3,10 }, { 5,3,8 }, { 5,2,3 }, { 2,7,3 },
//{ 7,10,3 }, { 7,6,10 }, { 7,11,6 }, { 11,0,6 }, { 0,1,6 },
//{ 6,1,10 }, { 9,0,11 }, { 9,11,2 }, { 9,2,5 }, { 7,2,11 }


  	using Lookup = std::map<std::pair<GLuint, GLuint>, GLuint>;

	GLuint vertex_for_edge(Lookup& lookup, std::vector<Vec3>& vertices, GLuint first, GLuint second)
	{
		Lookup::key_type key(first, second);
		if (key.first > key.second)
			std::swap(key.first, key.second);

		auto inserted = lookup.insert({ key, (GLuint)vertices.size() });
		if (inserted.second)
		{
			auto& edge0 = vertices[first];
			auto& edge1 = vertices[second];
			auto point = normalize(edge0 + edge1);
			vertices.push_back(point);
		}

		return inserted.first->second;
	}
	std::vector<Triangle> subdivide(std::vector<Vec3>& vertices, std::vector<Triangle> triangles)
	{
		Lookup lookup;
		std::vector<Triangle> result;

		for (auto&& each : triangles)
		{
			GLuint mid[3];
			for (int edge = 0; edge < 3; ++edge)
			{
				mid[edge] = vertex_for_edge(lookup, vertices,
					each.vertex[edge], each.vertex[(edge + 1) % 3]);
			}

			result.push_back({ each.vertex[0] , mid[0] , mid[2]  });
			result.push_back({ each.vertex[1] , mid[1] , mid[0]  });
			result.push_back({ each.vertex[2] , mid[2] , mid[1]  });
			result.push_back({         mid[0] , mid[1] , mid[2]  });
		}
		return result;
	}
};

class MYSTIC Plane : public Mesh
{
public:
	Plane() = default;
	Plane(Vec3 pos, Vec3 rotation, float width, float height);

	Vec2 Size;
};
class MYSTIC Torus : public Mesh
{// TODO: FIX THIS MESS, IMPROPER KNITTING OF THE VERTICES. VERTICES POSITIONS ARE CORRECT BUT INDICES ARE NOT
public:
	Torus(Vec3 position, int numc, int numt, float scale);
	Torus(Vec3 position, int numc, int numt);

//	void Render(Shader *_shader);
};
class MYSTIC Diamond : public Mesh
{
	Diamond(Vec3 position, float size);
};


class Rectangle2D : public Mesh
{
public:
	Rectangle2D(int _w, int _h)
	{
		Vec2 Position = Vec2(0);
		Vec2 Offset = Vec2(_w / 2.0f, _h / 2.0f);
		Vec2 newVertices[6] =
		{
			  Vec2(Position.x - Offset.x, Position.y + Offset.y),
			  Vec2(Position.x - Offset.x, Position.y - Offset.y),
			  Vec2(Position.x + Offset.x, Position.y - Offset.y),
			  Vec2(Position.x - Offset.x, Position.y + Offset.y),
			  Vec2(Position.x + Offset.x, Position.y - Offset.y),
			  Vec2(Position.x + Offset.x, Position.y - Offset.y)
		};

		VAO->Attach(VERTEX, new VertexBufferObject<Vec2>(&newVertices[0], 6));
		VAO->Attach(UVCOORD, new VertexBufferObject<Vec2>(&TexCoords[0], 6));
		Program = new Shader("Resources\\DebugQuad.sfx");
	}

	void Render(uint64_t tex)
	{
		__debugbreak();// because idk about texture handles yet
		Program->Enable();
		{
			VAO->Bind();
			{
				Program->SetTextureUniform("DiffuseTexture", tex);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			VAO->Unbind();
		}
		Program->Disable();
	}
	Texture *Picture;
	Shader *Program;

	Vec2 Vertices[6];
	Vec2 TexCoords[6] =
	{
	    Vec2(0.0f, 1.0f),
	    Vec2(0.0f, 0.0f),
	    Vec2(1.0f, 0.0f),
	    Vec2(0.0f, 1.0f),
	    Vec2(1.0f, 0.0f),
	    Vec2(1.0f, 1.0f)
	};
};


//Vec4 Vertices[6] =
//{ // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
//	// positions   // texCoords
//	Vec4(-1.0f, +0.5f,  0.0f, 1.0f),
//	Vec4(-1.0f, -1.0f,  0.0f, 0.0f),
//	Vec4(+0.5f, -1.0f,  1.0f, 0.0f),
//
//	Vec4(-1.0f, +0.5f,  0.0f, 1.0f),
//	Vec4(+0.5f, -1.0f,  1.0f, 0.0f),
//	Vec4(+0.5f, +0.5f,  1.0f, 1.0f)
//};



class MYSTIC DebugQuad : public Mesh
{
public:
	DebugQuad()
	{
        VAO = new VertexArrayObject();
		VAO->Attach(VERTEX, new VertexBufferObject<Vec4>(&Vertices[0], 6));
		Program = new Shader("Resources\\Shaders\\DebugQuad.sfx");
	}

	void Render(GPUptr tex)
	{
		Program->Enable();
		VAO->Bind();
		Program->SetTextureUniform("DiffuseTexture", tex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		VAO->Unbind();
		Program->Disable();
	}
	Texture *Picture;
	Shader *Program;
	Vec4 Vertices[6] =
	{ // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
	    Vec4(-1.0f,  -0.0f,  0.0f, 1.0f),
	    Vec4(-1.0f, -1.0f,  0.0f, 0.0f),
	    Vec4(-0.0f, -1.0f,  1.0f, 0.0f),

	    Vec4(-1.0f,  -0.0f,  0.0f, 1.0f),
	    Vec4(-0.0f, -1.0f,  1.0f, 0.0f),
	    Vec4(-0.0f, -0.0f,  1.0f, 1.0f)
	};
};



class MYSTIC ScreenQuad : public Mesh
{
public:
	ScreenQuad()
	{
		VAO->Attach(VERTEX, new VertexBufferObject<Vec4>(&Vertices[0], 6));
	}
	void Bind()
	{
		VAO->Bind();
	}
	void Unbind()
	{
		VAO->Unbind();
	}

	Texture *Picture;

	Vec4 Vertices[6] =
	{ // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		Vec4(-1.0f,  1.0f,  0.0f, 1.0f),
		Vec4(-1.0f, -1.0f,  0.0f, 0.0f),
		Vec4( 1.0f, -1.0f,  1.0f, 0.0f),

		Vec4(-1.0f,  1.0f,  0.0f, 1.0f),
		Vec4( 1.0f, -1.0f,  1.0f, 0.0f),
		Vec4( 1.0f,  1.0f,  1.0f, 1.0f)
	};

};







