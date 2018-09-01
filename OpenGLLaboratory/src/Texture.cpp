#include "Texture.h"
#include "Renderer.h"
#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../external_files/stb/stb_image.h"
#endif

namespace olab
{
	void Texture::Bind(unsigned _slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + _slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, textureId));
	}

	void Texture::UnBind()
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	Texture::Texture(const std::string& _imagePath, bool _flip_image)
		:textureId(0)
	{

		GLCall(glGenTextures(1, &textureId));
		GLCall(glBindTexture(GL_TEXTURE_2D, textureId));
		// set the texture wrapping parameters
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));	// set texture wrapping to GL_REPEAT (default wrapping method)
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		// set texture filtering parameters
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		stbi_set_flip_vertically_on_load(_flip_image);

		int width, height, nrChannels;

		unsigned char * data = stbi_load(_imagePath.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{

			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, (nrChannels == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data));

			GLCall(glGenerateMipmap(GL_TEXTURE_2D));
			loaded = true;
		}
		else
		{
			std::cout << "Cannot Load " << _imagePath << " as a Texture" << std::endl;
			loaded = false;
		}

		stbi_image_free(data);

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	}

	Texture::~Texture()
	{
		GLCall(glDeleteTextures(1, &textureId));
	}
}
