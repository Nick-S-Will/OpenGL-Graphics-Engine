#include "PostProcessor.h"
#include "Shader.h"
#include "WindowController.h"

void PostProcessor::Create(Shader* shader)
{
	this->shader = shader;

	CreateBuffers();
	CreateVertices();
}

void PostProcessor::Cleanup()
{
	glDeleteFramebuffers(1, &frameBufferLocation);
	glDeleteTextures(1, &textureColorBufferLocation);
	glDeleteRenderbuffers(1, &renderBufferObjectLocation);
}

void PostProcessor::Start()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferLocation);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void PostProcessor::End()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(shader->GetProgramID());
	shader->SetTextureSampler("screenTexture", GL_TEXTURE0, 0, textureColorBufferLocation);
	BindVertices();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(shader->GetVertices());
	glDisableVertexAttribArray(shader->GetTextureCoords());
}

void PostProcessor::CreateBuffers()
{
	glGenFramebuffers(1, &frameBufferLocation);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferLocation);

	glGenTextures(1, &textureColorBufferLocation);
	glBindTexture(GL_TEXTURE_2D, textureColorBufferLocation);
	auto screenSize = WindowController::GetInstance().GetScreenSize();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenSize.x, screenSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBufferLocation, 0);

	glGenRenderbuffers(1, &renderBufferObjectLocation);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObjectLocation);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenSize.x, screenSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObjectLocation);

	M_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::CreateVertices()
{
	float vertexData[] = {
		// Position		UV Coordinates
		-1.f, 1.f,		0.f, 1.f,
		-1.f, -1.f,		0.f, 0.f,
		1.f, -1.f,		1.f, 0.f,

		-1.f, 1.f,		0.f, 1.f,
		1.f, -1.f,		1.f, 0.f,
		1.f, 1.f,		1.f, 1.f
	};

	glGenBuffers(1, &vertexBufferLocation);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferLocation);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), &vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PostProcessor::BindVertices()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferLocation);

	glEnableVertexAttribArray(shader->GetVertices());
	glVertexAttribPointer(shader->GetVertices(), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(shader->GetTextureCoords());
	glVertexAttribPointer(shader->GetTextureCoords(), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}