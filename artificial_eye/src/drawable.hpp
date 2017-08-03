#pragma once

namespace ee
{
	class Renderer;

	class Drawable
	{
	public:
		virtual void draw(Renderer* renderer) = 0;
	};
}