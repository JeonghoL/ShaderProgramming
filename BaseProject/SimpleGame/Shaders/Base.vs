#version 330

in vec3 a_Pos;
in vec2 a_Tex;

out vec2 v_Texture;

void main()
{
	vec4 newPosition = vec4(a_Pos, 1);

	v_Texture = a_Tex;

	gl_Position = newPosition;
}
