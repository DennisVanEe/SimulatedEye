#version 330 core

in vec2 textCoord;

uniform sampler2D textDiffuse0; uniform bool useTD0;
uniform sampler2D textDiffuse1; uniform bool useTD1;
uniform sampler2D textDiffuse2; uniform bool useTD2;
uniform sampler2D textDiffuse3; uniform bool useTD3;
uniform bool noText;

uniform sampler2D textSpec0;
uniform sampler2D textSpec1;
uniform sampler2D textSpec2;
uniform sampler2D textSpec3;

out vec4 fragColor;

void main()
{
	fragColor = vec4(0.f, 0.f, 0.f, 1.f);
	if (useTD0)
		fragColor = texture(textDiffuse0, textCoord);
	if (useTD1)
		fragColor = texture(textDiffuse1, textCoord);
	if (useTD2)
		fragColor = texture(textDiffuse2, textCoord);
	if (useTD3)
		fragColor = texture(textDiffuse3, textCoord);

	if (noText)
		fragColor = vec4(0.f, 1.f, 0.f, 1.f);
	else
	{
		if (fragColor.x > 0.99f && fragColor.y > 0.99f && fragColor.z > 0.99f)
		{
			discard;
		}
	}
}