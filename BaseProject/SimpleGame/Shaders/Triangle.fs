#version 330

layout(location=0) out vec4 FragColor;

in float v_Grey;

void main()
{
	float age = clamp(v_Grey, 0.0, 1.0);

	// 색상 그라디언트: 흰/노랑(뜨거움) -> 주황 -> 어두운 빨강(식어감)
	vec3 col;
	if (age < 0.3)
		col = mix(vec3(1.0, 1.0, 0.8), vec3(1.0, 0.55, 0.0), age / 0.3);
	else
		col = mix(vec3(1.0, 0.55, 0.0), vec3(0.5, 0.0, 0.0), (age - 0.3) / 0.7);

	FragColor = vec4(col, 1.0);
}
