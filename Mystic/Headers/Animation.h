#include<string>
#include<vector>

#include"Common.h"
#include"Mesh.h"

struct Xform_State
{
	Vec3 Translation;
	Vec3 Rotation;// Quaternion?
	Vec3 Scale;
};

struct Sampled_Animation
{
	struct Node_info
	{
		Node_info(unsigned int sample_count);
		std::string Name;
		int ParentIndex;
		std::vector<Xform_State> State;
	} NodeInformation;

	uint16_t FrameRate;

	double Duration;
	uint32_t Flags;

	std::string Name;

	Mat4 Translation;

	// Passing in an Animation to fill for Hotloading
	static void LoadAnimation(Sampled_Animation &_animation, std::string _filename);
};


struct Joint
{
	Mat4 Transform;
	Joint *Parent{ nullptr };
	std::vector<Joint&> Children;
};


/// Animation
struct JointTransforms
{
	Vec3 Position;
	Vec4 Rotation; // Quaternion
};
struct Keyframe
{
	std::vector <JointTransforms> Transforms;
	float timeStamp;
};
struct Animation
{
	std::vector<Keyframe> Frames;
};


/// Animator
class Animator
{
	struct AnimatedModel
	{
		Mesh *Model;
		std::vector<Joint> Joints;
	};

	void RenderAnimation();
};

/*

Animated Model
--------------
~ Skin
~ Skeleton
*/


/* NOTES:
Skeletal Animation With Assimp
http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html

GamaSutra
https://www.gamasutra.com/view/feature/131694/architecting_a_3d_animation_engine.php

3D Formats
https://archicgi.com/3d-file-formats-7-best-options/

*/