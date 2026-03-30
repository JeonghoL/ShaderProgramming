#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;

void main()
{
	//if (v_Tex.x < 0.5)
	//{
	//	FragColor = vec4(1, 1, 1, 1);
	//}
	//else
	//{
	//	FragColor = vec4(0, 0, 0, 1);
	//}

	float col = 1 - v_Tex.x;
	FragColor = vec4(col * v_Tex.y, col * v_Tex.y, col * v_Tex.y, 1);
}
