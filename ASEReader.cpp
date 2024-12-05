#include "ASEReader.h"

void ASEReader::ParseASEFile(const char* fileName)
{
	std::vector<std::string> ASEFile;
	ReadFile(fileName, ASEFile);

	ParseMaterials(ASEFile);
	ParseGeoObjects(ASEFile);
}

void ASEReader::ParseMaterials(std::vector<std::string>& file)
{
	std::vector<std::string> materials;
	GetBlock("*MATERIAL_LIST ", file, materials);

	int materialCount = std::stoi(ExtractValue("*MATERIAL_COUNT", materials));
	for (int i = 0; i < materialCount; i++)
	{
		std::string materialTag = std::string("*MATERIAL ") + std::to_string(i);
		std::vector<std::string> materialBlock;
		GetBlock(materialTag.c_str(), materials, materialBlock);

		Material* material = new Material();
		material->Index = i;
		material->Name = ExtractValue("*MATERIAL_NAME ", materialBlock);
		material->Class = ExtractValue("*MATERIAL_CLASS ", materialBlock);
		material->AmbientColor = ParseStringToVec3(ExtractValue("*MATERIAL_AMBIENT ", materialBlock));
		material->DiffuseColor = ParseStringToVec3(ExtractValue("*MATERIAL_DIFFUSE ", materialBlock));
		material->SpecularColor = ParseStringToVec3(ExtractValue("*MATERIAL_SPECULAR ", materialBlock));
		material->Shine = std::stof(ExtractValue("*MATERIAL_SHINE ", materialBlock));
		material->ShineStrength = std::stof(ExtractValue("*MATERIAL_SHINESTRENGTH ", materialBlock));
		material->Transparency = std::stof(ExtractValue("*MATERIAL_TRANSPARENCY ", materialBlock));
		material->WireSize = std::stof(ExtractValue("*MATERIAL_WIRESIZE ", materialBlock));
		material->Shading = ExtractValue("*MATERIAL_SHADING ", materialBlock);

		ParseMap("MAP_DIFFUSE", "DIFFUSE", materialBlock, material);
		ParseMap("*MAP_SPECULAR", "SPECULAR", materialBlock, material);
		ParseMap("*MAP_BUMP", "BUMP", materialBlock, material);

		Materials.push_back(material);
	}
}

void ASEReader::ParseGeoObjects(std::vector<std::string>& file)
{
	std::vector<std::string> geometryObject;
	int index = GetBlock("*GEOMOBJECT ", file, geometryObject);

	while (index != -1)
	{
		GeoObject* geoObject = new GeoObject();
		geoObject->Name = ExtractValue("*NODE_NAME ", geometryObject);
		geoObject->MaterialID = std::stoi(ExtractValue("*MATERIAL_REF", geometryObject));
		geoObject->MeshInfo.TimeValue = std::stoi(ExtractValue("*TIMEVALUE ", geometryObject));
		geoObject->MeshInfo.VertexCount = std::stoi(ExtractValue("*MESH_NUMVERTEX ", geometryObject));
		geoObject->MeshInfo.FaceCount = std::stoi(ExtractValue("*MESH_NUMFACES ", geometryObject));

		std::vector<std::string> dataBlock;

		FindAllTags("*MESH_VERTEX ", geometryObject, dataBlock);
		ExtractVectorList(geoObject->MeshInfo.Vertices, dataBlock);

		FindAllTags("*MESH_TVERT ", geometryObject, dataBlock);
		ExtractVectorList(geoObject->MeshInfo.TextureVertices, dataBlock);

		FindAllTags("*MESH_TFACE ", geometryObject, dataBlock);
		ExtractVectorList(geoObject->MeshInfo.TextureFaces, dataBlock);

		FindAllTags("*MESH_VERTEXNORMAL ", geometryObject, dataBlock);
		ExtractVectorList(geoObject->MeshInfo.Normals, dataBlock);

		FindAllTags("*MESH_FACE ", geometryObject, dataBlock);
		for (auto& face : dataBlock)
		{
			std::replace(face.begin(), face.end(), '\t', ' ');
			face.erase(0, face.find_first_not_of(' '));

			std::vector<std::string> result;
			std::stringstream stringStream(face);
			std::string item = "";

			while (getline(stringStream, item, ' '))
			{
				if (item != "")
				{
					result.push_back(item);
				}
			}
			geoObject->MeshInfo.Faces.push_back(glm::vec3(std::stof(result[3]), std::stof(result[5]), std::stof(result[7])));
		}

		GeoObjects.push_back(geoObject);

		index = GetBlock("*GEOMOBJECT ", file, geometryObject, index + 1);
	}
}

void ASEReader::ReadFile(const char* fileName, std::vector<std::string>& file)
{
	std::string line;
	std::ifstream fileStream(fileName);
	while (getline(fileStream, line))
	{
		file.push_back(line);
	}

	fileStream.close();
}

int ASEReader::GetBlock(const char* tag, std::vector<std::string>& lines, std::vector<std::string>& block, int start)
{
	block.clear();
	for (int i = start; i < (int)lines.size(); i++)
	{
		if (lines[i].find(tag) != std::string::npos)
		{
			int indent = 0;
			i++;
			for (int j = i; j < (int)lines.size(); j++)
			{
				if (lines[j].find('{') != std::string::npos) indent++;
				if (lines[j].find('}') != std::string::npos)
				{
					if (indent == 0) return i;
					indent--;
				}
				block.push_back(lines[j]);
			}
		}
	}

	return -1;
}

void ASEReader::FindAllTags(const char* tag, std::vector<std::string>& lines, std::vector<std::string>& block)
{
	block.clear();
	for (int i = 0; i < (int)lines.size(); i++)
	{
		if (lines[i].find(tag) != std::string::npos)
		{
			block.push_back(lines[i]);
		}
	}
}

std::string ASEReader::ExtractValue(const char* tag, std::vector<std::string>& lines)
{
	for (int i = 0; i < (int)lines.size(); i++)
	{
		if (lines[i].find(tag) != std::string::npos)
		{
			std::string line = lines[i];

			std::replace(line.begin(), line.end(), '\t', ' ');
			line.erase(0, line.find_first_not_of(" \n\r\t"));
			line.erase(0, line.find(" ") + 1);
			line.erase(std::remove(line.begin(), line.end(), '\"'), line.end());
			line.erase(0, line.find_first_not_of(" \n\r\t"));

			return line;
		}
	}

	return std::string();
}

glm::vec3 ASEReader::ParseStringToVec3(std::string values)
{
	std::vector<std::string> result;
	std::stringstream stringStream(values);
	std::string item = "";
	while (getline(stringStream, item, ' '))
	{
		result.push_back(item);
	}

	return glm::vec3(std::stof(result[0]), std::stof(result[1]), std::stof(result[2]));
}

void ASEReader::ParseMap(const char* mapName, const char* name, std::vector<std::string>& materialBlock, ASEReader::Material* material)
{
	std::vector<std::string> mapBlock;
	GetBlock(mapName, materialBlock, mapBlock);

	if (mapBlock.size() > 0)
	{
		ASEReader::Map map;
		map.Name = name;
		map.TextureFileName = ExtractValue("*BITMAP ", mapBlock);
		map.UVW_U_Offset = std::stof(ExtractValue("*UVW_U_OFFSET ", mapBlock));
		map.UVW_V_Offset = std::stof(ExtractValue("*UVW_V_OFFSET ", mapBlock));
		map.UVW_U_Tiling = std::stof(ExtractValue("*UVW_U_TILING ", mapBlock));
		map.UVW_V_Tiling = std::stof(ExtractValue("*UVW_V_TILING ", mapBlock));
		material->Maps.push_back(map);
	}
}

void ASEReader::ExtractVectorList(std::vector<glm::vec3>& list, std::vector<std::string>& vertexList)
{
	for (auto& vertex : vertexList)
	{
		std::replace(vertex.begin(), vertex.end(), '\t', ' ');
		vertex.erase(0, vertex.find_first_not_of(' '));

		std::vector<std::string> result;
		std::stringstream stringStream(vertex);
		std::string item = "";

		while (getline(stringStream, item, ' '))
		{
			if (item != "")
			{
				result.push_back(item);
			}
		}
		list.push_back(glm::vec3(std::stof(result[2]), std::stof(result[3]), std::stof(result[4])));
	}
}