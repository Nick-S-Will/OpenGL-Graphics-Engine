#pragma once
#ifndef ASEREADER_H
#define ASEREADER_H

#include "StandardIncludes.h"

class ASEReader
{
public:
	struct Map
	{
		std::string Name;
		std::string TextureFileName;
		float UVW_U_Offset;
		float UVW_V_Offset;
		float UVW_U_Tiling;
		float UVW_V_Tiling;
	};

	struct Material
	{
		int Index;
		std::string Name;
		std::string Class;
		glm::vec3 AmbientColor;
		glm::vec3 DiffuseColor;
		glm::vec3 SpecularColor;
		float Shine;
		float ShineStrength;
		float Transparency;
		float WireSize;
		std::string Shading;
		float XPFalloff;
		float SelfIllumination;
		std::string Falloff;
		std::string XPType;
		std::vector<ASEReader::Map> Maps;
	};

	struct MeshInfo
	{
		int TimeValue;

		int VertexCount;
		std::vector<glm::vec3> Vertices;
		int FaceCount;
		std::vector<glm::vec3> Faces;

		int TextureVertexCount;
		std::vector<glm::vec3> TextureVertices;
		int TextureFaceCount;
		std::vector<glm::vec3> TextureFaces;

		std::vector<glm::vec3> Normals;
	};

	struct GeoObject
	{
		std::string Name;
		MeshInfo MeshInfo;
		int MaterialID;
	};

public:
	std::string FileName;
	std::vector<Material*> Materials;
	std::vector<GeoObject*> GeoObjects;

	void ParseASEFile(const char* fileName);

private:
	void ParseMaterials(std::vector<std::string>& file);
	void ParseGeoObjects(std::vector<std::string>& file);
	void ReadFile(const char* fileName, std::vector<std::string>& file);
	int GetBlock(const char* tag, std::vector<std::string>& lines, std::vector<std::string>& block, int start = 0);
	void FindAllTags(const char* tag, std::vector<std::string>& lines, std::vector<std::string>& block);
	std::string ExtractValue(const char* tag, std::vector<std::string>& lines);
	glm::vec3 ParseStringToVec3(std::string values);
	void ParseMap(const char* mapName, const char* name, std::vector<std::string>& materialBlock, ASEReader::Material* material);
	void ExtractVectorList(std::vector<glm::vec3>& list, std::vector<std::string>& vertexList);
};

#endif // !ASEREADER_H