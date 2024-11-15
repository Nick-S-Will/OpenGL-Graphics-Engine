#include "Skybox.h"
#include "OBJ_Loader.h"

Skybox::~Skybox()
{
	Cleanup();
}

void Skybox::Create(Shader* shader, std::string file, std::vector<std::string> faces)
{
	this->shader = shader;

	objl::Loader loader;
	M_ASSERT(loader.LoadFile(file), "Failed to load mesh.");

	for (int i = 0; i < loader.LoadedMeshes.size(); i++)
	{
		objl::Mesh mesh = loader.LoadedMeshes[i];
		for (int j = 0; j < mesh.Vertices.size(); j++)
		{
			vertexData.push_back(mesh.Vertices[j].Position.X);
			vertexData.push_back(mesh.Vertices[j].Position.Y);
			vertexData.push_back(mesh.Vertices[j].Position.Z);
		}
	}

	texture = Texture();
	texture.LoadCubemap(faces);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
}

void Skybox::Cleanup()
{
	if (vertexBuffer != 0)
	{
		glDeleteBuffers(1, &vertexBuffer);
		vertexBuffer = 0;
	}
	texture.CleanUp();
}

void Skybox::Render(glm::mat4 pv)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(shader->GetProgramID());
	SetShaderVariables(pv);
	BindAttributes();
	glDrawArrays(GL_TRIANGLES, 0, vertexData.size());
	glDisableVertexAttribArray(shader->GetNormals());

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void Skybox::SetShaderVariables(glm::mat4 pv)
{
	shader->SetMat4("PV", pv);
	shader->SetTextureSampler("skyboxTexture", GL_TEXTURE_CUBE_MAP, 0, texture.GetTexture());
}

void Skybox::BindAttributes()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glEnableVertexAttribArray(shader->GetVertices());
	glVertexAttribPointer(shader->GetVertices(), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}