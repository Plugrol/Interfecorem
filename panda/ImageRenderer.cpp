#include "ImageRenderer.h"

#include <stdexcept>

core::graphic::resource::images::ImageRenderer::ImageRenderer(const char* path)
{
	m_pixels = stbi_load(path, &m_width, &m_height, &m_channels, STBI_rgb_alpha);

	if (m_pixels == nullptr)
	{
		throw std::invalid_argument("Could not load image.");
	}
}

core::graphic::resource::images::ImageRenderer::~ImageRenderer()
{
	destroy();
}

void core::graphic::resource::images::ImageRenderer::destroy()
{
	if (m_pixels != nullptr)
	{
		stbi_image_free(m_pixels);
		m_pixels = nullptr;
	}
}

int core::graphic::resource::images::ImageRenderer::getWidth() const {
	return m_width;
}

int core::graphic::resource::images::ImageRenderer::getHeight() const {
	return m_height;
}

int core::graphic::resource::images::ImageRenderer::getSize() const
{
	return getWidth() * getHeight() * getChannels();
}

stbi_uc* core::graphic::resource::images::ImageRenderer::getRaw() const {
	return m_pixels;
}

int core::graphic::resource::images::ImageRenderer::getChannels()
{
	return 4;
}
