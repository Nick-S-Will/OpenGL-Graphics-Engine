#include "Font.h"
#include "Shader.h"
#include "WindowController.h"

void Font::Create(Shader* shader, std::string name, FT_UInt size)
{
	this->shader = shader;

	AllocateBuffers();
	Initialize(name, size);
}

void Font::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
	y = WindowController::GetInstance().GetScreenSize().y - y;
	glUseProgram(shader->GetProgramID());

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	shader->SetVec3("textColor", color);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(orthographicProjection));

	for (auto characterKey = text.begin(); characterKey != text.end(); characterKey++)
	{
		Character character = characters[*characterKey];
		float xPosition = x + character.bearing.x * scale;
		float yPosition = y - (character.size.y - character.bearing.y) * scale;
		float width = character.size.x * scale;
		float height = character.size.y * scale;

		float vertices[6][4] = {
			{ xPosition, yPosition + height, 0.f, 0.f },
			{ xPosition, yPosition, 0.f, 1.f },
			{ xPosition + width, yPosition, 1.f, 1.f },

			{ xPosition, yPosition + height, 0.f, 0.f },
			{ xPosition + width, yPosition, 1.f, 1.f },
			{ xPosition + width, yPosition + height, 1.f, 0.f }
		};

		shader->SetTextureSampler("material.diffuseTexture", GL_TEXTURE0, 0, character.textureID);

		glEnableVertexAttribArray(shader->GetVertices());
		glVertexAttribPointer(shader->GetVertices(), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (character.advance >> 6) * scale;
	}

	glDisableVertexAttribArray(shader->GetVertices());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Font::Initialize(std::string fileName, FT_UInt size)
{
	glm::ivec2 screenSize = WindowController::GetInstance().GetScreenSize();
	orthographicProjection = glm::ortho(0.f, (float)screenSize.x, 0.f, (float)screenSize.y);

	M_ASSERT(FT_Init_FreeType(&library) == false, "Could not init FreeType Library");
	M_ASSERT(FT_New_Face(library, fileName.c_str(), 0, &face) == false, "Failed to set character size");
	M_ASSERT(FT_Set_Pixel_Sizes(face, 0, size) == false, "Failed to set character size");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	CreateCharacters();

	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

void Font::CreateCharacters()
{
	for (unsigned char c = 0; c < 128; c++)
	{
		M_ASSERT(FT_Load_Char(face, c, FT_LOAD_RENDER) == false, "Failed to load Glyph");

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character =
		{
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int)face->glyph->advance.x
		};
		characters.insert(std::pair<char, Character>(c, character));
	}
}

void Font::AllocateBuffers()
{
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
}
