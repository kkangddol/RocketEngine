#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "MathHeader.h"
#include "eSoundGroup.h"

namespace Rocket::Core
{
	struct transfromData
	{
		std::string name;
		std::vector<std::string> child;
		std::string parent;
		std::string type;
		float position_x = 0.f;
		float position_y = 0.f;
		float position_z = 0.f;

		float rotation_w = 0.f;
		float rotation_x = 0.f;
		float rotation_y = 0.f;
		float rotation_z = 0.f;

		float scale_x = 0.f;
		float scale_y = 0.f;
		float scale_z = 0.f;

		bool isNodeOpen = false;

	};

	struct cameraData
	{
		std::string name;

		float _nearZ = 0.f;				// frustum�� ����� �������� �Ÿ�
		float _farZ = 0.f;				// frustum�� �� �������� �Ÿ�
		float _aspect = 0.f;				// ���� / ���� ����
		float _fovY = 0.f;				// fov������ 60�й����� ��������
		float _nearWindowHeight = 0.f;	// frustum�� ����� ����� ����
		float _nearWindowWidth = 0.f;		// frustum�� �� ����� ����

		// ��İ�
		Matrix _viewMatrix;		// ī�޶��� ������ǥ'��'
		Matrix _projMatrix;		// ī�޶��� ���� ���
	};

	struct rayCastData
	{
		std::string name;

	};

	// object

	struct meshRendererData
	{
		std::string name;

		std::string FBXpath;
		std::string ShaderPath;
		std::string DiffusePath;
		std::string NormalPath;

		// material Data
		Color ambient = { 0, 0, 0, 0 };
		Color diffuse = { 0, 0, 0, 0 };
		Color specular = { 0, 0, 0, 0 };
		Color reflect = { 0, 0, 0, 0 };

		// pbr materia Data;
		Color pbr_albedo = { 0, 0, 0, 0 };
		float pbr_metalic = 0.f;
		float pbr_roughness = 0.f;
		float pbr_specular = 0.f;
		float pbr_emission = 0.f;
	};

	struct SkinnedMeshRendererData
	{
		std::string name;

		std::string FBXpath;
		std::string ShaderPath;
		std::string DiffusePath;
		std::string NormalPath;

		// material Data
		Vector4 ambient = { 0, 0, 0, 0 };
		Vector4 diffuse = { 0, 0, 0, 0 };
		Vector4 specular = { 0, 0, 0, 0 };
		Vector4 reflect = { 0, 0, 0, 0 };

		// pbr materia Data;
		Vector4 pbr_albedo = { 0, 0, 0, 0 };
		float pbr_metalic = 0.f;
		float pbr_roughness = 0.f;
		float pbr_specular = 0.f;
		float pbr_emission = 0.f;
	};

	// UI

	struct spriteRendererData
	{
		std::string name;

		std::string SpritePath;
		float width = 0.f;
		float height = 0.f;
		int sortOrder = 0;
	};

	struct textBoxData
	{
		std::string name;

		std::string text;
		float fontSize = 0.f;
		Vector4 color = {};

		float width = 0.f;
		float height = 0.f;
		int sortOrder = 0;
	};

	struct buttonData
	{
		std::string name;

		// size
		float width = 0.f;
		float height = 0.f;
		int sortOrder = 0.f;
		bool isFocused = 0.f;

		/// �Լ� ������ �ø�������� ��� ����غ� �� 2023.08.14 ������ && ������
		//std::function<void()> onClickEvent;
	};

	struct boxColliderData
	{
		std::string name;

		std::vector<std::string> servants;

		float positionOffset[3] = {};
		float rotationOffset[4] = {};
		float scaleOffset[3] = {};

		float width = 0.f;
		float height = 0.f;
		float depth = 0.f;

		Vector3 GetPositionOffset();
		Quaternion GetRotationOffset();
		Vector3 GetScaleOffset();
	};

	struct capsuleColliderData
	{
		std::string name;

		std::vector<std::string> servants;

		float positionOffset[3] = {};
		float rotationOffset[4] = {};
		float scaleOffset[3] = {};

		float radius = 0.f;
		float halfHeight = 0.f;

		Vector3 GetPositionOffset();
		Quaternion GetRotationOffset();
		Vector3 GetScaleOffset();
	};

	struct sphereColliderData
	{
		std::string name;

		std::vector<std::string> servants;

		float positionOffset[3] = {};
		float rotationOffset[4] = {};
		float scaleOffset[3] = {};

		float radius = 0.f;

		Vector3 GetPositionOffset();
		Quaternion GetRotationOffset();
		Vector3 GetScaleOffset();
	};

	struct planeColliderData
	{
		std::string name;

		float positionOffset[3] = {};
		float rotationOffset[4] = {};
		float scaleOffset[3] = {};

		Vector3 GetPositionOffset();
		Quaternion GetRotationOffset();
		Vector3 GetScaleOffset();
	};

	struct staticBoxColliderData
	{
		std::string name;

		float positionOffset[3] = {};
		float rotationOffset[4] = {};
		float scaleOffset[3] = {};

		float width = 0.f;
		float height = 0.f;
		float depth = 0.f;

		Vector3 GetPositionOffset();
		Quaternion GetRotationOffset();
		Vector3 GetScaleOffset();
	};

	struct animationData
	{
		std::string name;

		std::string stateName;
		std::string aniDescName;
		std::string modelPath;
		std::string animationPath;
		bool isLoop = false;
	};

	struct animationControllerData
	{
		std::string name;

		std::vector<animationData*> states;
		std::unordered_map<std::string, bool> conditions;
	};

	struct audioData
	{
		std::string name;
		std::string path;
		bool is3D;
		eSoundGroup type;
		float volume = 0.f;
	};

	struct audioClipData
	{
		std::string name;

		std::vector<audioData*> audioList;
	};

	//light

	struct directionalLightData
	{
		std::string name;

		float intensity = 0.f;

		// base
		Color ambient = { 0, 0, 0, 0 };
		Color diffuse = { 0, 0, 0, 0 };
		Color specular = { 0, 0, 0, 0 };

	};

	struct pointLightData
	{
		std::string name;

		float range = 0.f;

		//�ش� �Ÿ��� �Ѿ�� ���� ���� ����
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;

		// base
		Color ambient = { 0, 0, 0, 0 };
		Color diffuse = { 0, 0, 0, 0 };
		Color specular = { 0, 0, 0, 0 };
	};

	struct spotLightData
	{
		std::string name;

		float range = 0.f;
		float spot = 0.f;


		//�ش� �Ÿ��� �Ѿ�� ���� ���� ����
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;

		// base
		Color ambient = { 0, 0, 0, 0 };
		Color diffuse = { 0, 0, 0, 0 };
		Color specular = { 0, 0, 0, 0 };
	};

	struct DataStruct
	{
		transfromData* transform = nullptr;
		cameraData* camera = nullptr;
		rayCastData* raycast = nullptr;

		meshRendererData* meshRenderer = nullptr;
		SkinnedMeshRendererData* skinnedMeshRenderer = nullptr;

		spriteRendererData* sprite = nullptr;
		buttonData* btn = nullptr;
		textBoxData* txtBox = nullptr;

		animationControllerData* aniController = nullptr;
		audioClipData* audio = nullptr;

		boxColliderData* boxColli = nullptr;
		capsuleColliderData* capsuleColli = nullptr;
		sphereColliderData* sphereColli = nullptr;
		planeColliderData* planeColli = nullptr;
		//staticBoxColliderData* staticBoxColli = nullptr;
		std::vector<staticBoxColliderData*> staticBoxColli;

		directionalLightData* dirLight = nullptr;
		spotLightData* spotLight = nullptr;
		pointLightData* pointLight = nullptr;

		Vector3 GetPosition();
		Quaternion GetRotation();
		Vector3 GetScale();
	};
}