#pragma once

namespace syn_eye
{
	class Renderer;

	class Drawable
	{
	public:
		virtual void draw(Renderer* renderer) = 0;
	};
}