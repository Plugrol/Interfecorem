#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace core::graphic::resource::images {

	class ImageRenderer
	{
		int m_width{}, m_height{}, m_channels{};
		stbi_uc* m_pixels = nullptr;
	public:
		ImageRenderer();

		explicit ImageRenderer(const char* path);

		~ImageRenderer();

		void destroy();

		[[nodiscard]] int getWidth() const;

		[[nodiscard]] int getHeight() const;

		[[nodiscard]] int getSize() const;

		[[nodiscard]] stbi_uc* getRaw() const;

		static int getChannels();
	};
}
