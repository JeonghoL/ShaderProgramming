#version 330

uniform float u_Time;

in vec3 a_Position;
in float a_Mass;
in vec2 a_Vel;
in float a_randomValue;
in float a_randomValue1;

const float PI = 3.141592;
const vec2 Gravity = vec2(0, -9.8);

void Basic()
{
	float t = mod(u_Time, 1);

	vec4 newPosition;
	newPosition = vec4(a_Position.x + t, a_Position.y, a_Position.z, 1);
	gl_Position = newPosition;
}

void Sin1()
{
	float t = mod(u_Time, 1);

	vec4 newPosition;
	newPosition = vec4(a_Position.x + t, a_Position.y + 0.5 * sin(t * 2 * 3.141592), a_Position.z, 1);
	gl_Position = newPosition;
}

void Sin2()
{
	float t = mod(u_Time * 5, 2);

	vec4 newPosition;
	newPosition.x = a_Position.x + t - 1;
	newPosition.y = a_Position.y + 0.5 * sin(t * 3.141592);
	newPosition.z = a_Position.z;
	newPosition.w = 1.0;
	gl_Position = newPosition;
}

void Sin3()
{
	float t = mod(u_Time * 5, 2);

	vec4 newPosition;
	newPosition.x = a_Position.x + t - 1;
	newPosition.y = a_Position.y + 0.5 * sin(t * 2 * 3.141592);
	newPosition.z = a_Position.z;
	newPosition.w = 1.0;
	gl_Position = newPosition;
}

void Circle1()
{
	float t = mod(u_Time * 3, 2);

	vec4 newPosition;
	newPosition.x = a_Position.x + 0.5 * cos(t * 2 * 3.141592);
	newPosition.y = a_Position.y + 0.5 * sin(t * 2 * 3.141592);
	newPosition.z = a_Position.z;
	newPosition.w = 1.0;
	gl_Position = newPosition;
}

void Star()
{
	float PI = 3.14159265;
	float outerR = 0.8;
	float innerR = 0.4;

	float t = mod(u_Time * 3, 1.0);
	float segment = t * 10.0;         // 10 segments (5 outer + 5 inner corners)
	float segIdx = floor(segment);
	float segFrac = fract(segment);

	float angleStep = 2.0 * PI / 10.0; // 36 degrees per vertex
	float angle1 = segIdx * angleStep - PI / 2.0;
	float angle2 = (segIdx + 1.0) * angleStep - PI / 2.0;

	// Alternate between outer tip and inner corner radii
	float r1 = mix(outerR, innerR, mod(segIdx, 2.0));
	float r2 = mix(innerR, outerR, mod(segIdx, 2.0));

	vec2 p1 = vec2(r1 * cos(angle1), r1 * sin(angle1));
	vec2 p2 = vec2(r2 * cos(angle2), r2 * sin(angle2));

	vec2 pos = mix(p1, p2, segFrac);

	vec4 newPosition;
	newPosition.x = a_Position.x + pos.x;
	newPosition.y = a_Position.y + pos.y;
	newPosition.z = a_Position.z;
	newPosition.w = 1.0;
	gl_Position = newPosition;
}

float pseudoRandom(float n)
{
	return fract(sin(n) * 43758.5453123);
}

// S = So + Vo*t + (1/2)*g*t^2
void Falling()
{
	// emitTime
	float newTime = u_Time - a_randomValue1;

	if (newTime > 0)
	{
		float scale = pseudoRandom(a_randomValue1);
		float t = mod(newTime, 1.0);

		float theta = a_randomValue * 2 * PI;
		float initPosX = a_Position.x * scale + sin(theta);
		float initPosY = a_Position.y * scale + cos(theta);

		vec4 newPosition;
		newPosition.x = initPosX + a_Vel.x * t + 0.5 * Gravity.x * (t * t);
		newPosition.y = initPosY + a_Vel.y * t + 0.5 * Gravity.y * (t * t);
		newPosition.z = 0;
		newPosition.w = 1.0;
		gl_Position = newPosition;
	}
	else
		gl_Position = vec4(10000, 100, 0, 1.0);
}

void Fountain()
{
	const vec2 FountainGravity = vec2(0, -2.0);
	float newTime = u_Time - a_randomValue1;

	if (newTime > 0.0)
	{
		float t = mod(newTime, 1.5);

		float angle = a_randomValue * PI / 6.0;
		float speed = 1.5 + pseudoRandom(a_randomValue) * 0.5;
		float vx = sin(angle) * speed;
		float vy = cos(angle) * speed;

		vec4 newPosition;
		newPosition.x = a_Position.x + vx * t;
		newPosition.y = a_Position.y + (-0.5 + vy * t + 0.5 * FountainGravity.y * (t * t));
		newPosition.z = 0;
		newPosition.w = 1.0;
		gl_Position = newPosition;
	}
	else
		gl_Position = vec4(10000, 100, 0, 1.0);
}

void main()
{
	Fountain();
}
