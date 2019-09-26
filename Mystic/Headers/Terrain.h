#pragma once
#include"Mesh.h"
#include"Common.h"
#define TERRAIN_MAXWIDTH   0xFFFF
class Terrain : public Mesh
{
public:
	Terrain()
		: MapID(0)
	{
		DEBUG_TRACE("MAKE");
        TODO("Need new Gradiant array, this entire Algorithm sucks write a new one!");
	}
	~Terrain()
	{
		DEBUG_TRACE("DEL");
	}

	Terrain(GLuint x, GLuint y, GLuint width, GLuint height);
	Terrain(Vec3 position, int width, int height, Texture *groundtex);


	GLfloat *HeightMap = nullptr;
	float SampleTerrain(float x, float z);

private:
	float Gradient[1000][1000][2]; /// Compiler loses heap space due to this, Need new Perline noise Generator, this one sucks

	GLuint  MapID;


	float OneDimension[TERRAIN_MAXWIDTH];
	float PerlinNoise1D();
	float PerlinNoise2D(float x, float y);
	float PerlinNoise3D();

	Vec2 Size;
	Vec2 GridSize;

	int Width, Height;

	float Gradiant(float p)
	{
		const float texture_width = 256.0;
		float  v = OneDimension[int(p / texture_width)];
		return v > 0.5 ? 1.0 : -1.0;
	}
	float Fade(float t)
	{
		return t * t*t*(t*(t*6.0 - 15.0) + 10.0);
	}
	float Noise(float p)
	{
		float p0 = floor(p);
		float p1 = p0 + 1.0;

		float t = p - p0;
		float fade_t = Fade(t);

		float g0 = Gradiant(p0);
		float g1 = Gradiant(p1);

		return (1.0f - fade_t)*g0*(p - p0) + fade_t * g1*(p - p1);
	}

	int Get_Vertex(int x, int z) { return (x + Width * z); }
	Vec3 GetNormal(Vec3 v1, Vec3 v2, Vec3 v3);

	//Vec3 Terrain::CollisionDetection(CollisionSphere ball)

	int 
		VertexCount,
		ColorsCount,
		IndexCount,
		NormalCount;
	std::vector<Vec3> VecVertices;
	std::vector<Vec3> VecNormals;
	double Lerp(float a0, float a1, float w);
	float DotGridGradient(int ix, int iy, float x, float y);

	//void Bind();
	//void Unbind();
	float Max_Height = 0;
	void Update();
};