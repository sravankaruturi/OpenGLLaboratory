#pragma once
#include <string>

namespace olab
{

	class Texture
	{

	protected:

		unsigned int textureId;
		bool loaded = false;

	public:
		bool IsLoaded() const
		{
			return loaded;
		}

		void SetLoaded(bool _loaded)
		{
			loaded = _loaded;
		}

		unsigned GetTextureId() const
		{
			return textureId;
		}

		void SetTextureId(unsigned _textureId)
		{
			textureId = _textureId;
		}

		void Bind(unsigned int _slot = 0) const;

		static void UnBind();

		explicit Texture(const std::string& _imagePath, bool _flip_image = true);

		~Texture();
	};
	
}
