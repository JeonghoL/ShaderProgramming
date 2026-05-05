#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Texture;

uniform float u_Time;
uniform sampler2D u_RGBTex;
uniform sampler2D u_CurrNumTex;
uniform sampler2D u_NumsTex;
uniform int u_InputNum;

const float c_PI = 3.141592;

void Example()
{
    float split = step(0.5, v_Texture.x);

    vec3 color = mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), split);

    FragColor = vec4(color, 1.0);
}

void Example2()
{
    float dist = length(v_Texture);
    
    if (dist > 0.5)
    {
        discard;
    }

    FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}

//------------------------------------------------------------------------------------
// 첫 영상 내용
//------------------------------------------------------------------------------------
void Flag()
{
    float amp = 0.5;
    float speed = 15;
    float sinInput = v_Texture.x * c_PI * 2 - u_Time * speed;
    float sinValue = v_Texture.x * amp * (((sin(sinInput) + 1) / 2) - 0.5) + 0.5;

    float fWidth = 0.0;
    float width = 0.5 * mix(1, fWidth, v_Texture.x);
    float grey = 0;

    if (v_Texture.y < sinValue + width / 2 && v_Texture.y > sinValue - width / 2)
    {
        grey = 1;
    }
    else
    {
        grey = 0;
        discard;
    }

    FragColor = vec4(grey);
}

void Flame()
{
    float amp = 0.5;
    float speed = 15;
    float newY = 1 - v_Texture.y;
    float sinInput = newY * c_PI * 2 - u_Time * speed;
    float sinValue = newY * amp * (((sin(sinInput) + 1) / 2) - 0.5) + 0.5;

    float fWidth = 0.0;
    float width = 0.5 * mix(1, fWidth, newY); // 이렇게 하면 아래가 Flame 처럼 아래가 굵고 위에가 얇음
    //float width = 0.5 * mix(fWidth, 1, newY); 이렇게 하면 아래가 담배연기처럼 아래가 얇고 위에가 굵음
    float grey = 0;

    if (v_Texture.x < sinValue + width / 2 && v_Texture.x > sinValue - width / 2)
    {
        grey = 1;
    }
    else
    {
        grey = 0;
        discard;
    }

    FragColor = vec4(grey);
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

void TextureSampling()
{
   vec4 c0;
   vec4 c1;
   vec4 c2;
   vec4 c3;
   vec4 c4;
   
   float offsetX = 0.01;
   c0 = texture(u_RGBTex, vec2(v_Texture.x - offsetX * 2, v_Texture.y));
   c1 = texture(u_RGBTex, vec2(v_Texture.x - offsetX * 1, v_Texture.y));
   c2 = texture(u_RGBTex, vec2(v_Texture.x - offsetX * 0, v_Texture.y));
   c3 = texture(u_RGBTex, vec2(v_Texture.x + offsetX * 1, v_Texture.y));
   c4 = texture(u_RGBTex, vec2(v_Texture.x + offsetX * 2, v_Texture.y));

   vec4 sum = c0 + c1 + c2 + c3 + c4;
   sum = sum/5;
   FragColor = sum;
}

void TextureQ1()
{
    float tx = v_Texture.x;
    float ty = 1 - abs((v_Texture.y * 2) - 1);

    vec2 tex = vec2(tx, ty);

    FragColor =  texture(u_RGBTex, tex);
}

void TextureQ2()
{
    float tx = fract(v_Texture.x * 3);  // 0 ~ 1 3번 반복
    float ty = v_Texture.y / 3; // 0 ~ 1/3 3번 반복

    float offsetX = 0;
    float offsetY = (2 - floor(v_Texture.x * 3)) / 3;

    vec2 tex = vec2(offsetX + tx, offsetY + ty);

    FragColor =  texture(u_RGBTex, tex);
}

void TextureQ3()
{
    float tx = fract(v_Texture.x * 3); 
    float ty = v_Texture.y / 3; 

    float offsetX = 0;
    float offsetY = floor(v_Texture.x * 3) / 3;

    vec2 tex = vec2(offsetX + tx, offsetY + ty);

    FragColor =  texture(u_RGBTex, tex);
}


//------------------------------------------------------------------------------------
// 영상 3번째
//------------------------------------------------------------------------------------

void TextureQ4()
{
    float offsetX = fract(ceil(v_Texture.y * 2) * 0.5);
    float offsetY = 0;

    float tx = fract(v_Texture.x * 2 + offsetX); 
    float ty = fract(v_Texture.y * 2 + offsetY); 

    vec2 tex = vec2(tx, ty);
    FragColor =  texture(u_RGBTex, tex);
}

void TextureQ41()
{
    float resolX = 2;
    float resolY = 2;
    float shear = 0.5;

    float offsetX = 0;
    float offsetY = floor(v_Texture.x * resolX) * shear;

    float tx = fract(v_Texture.x * resolX + offsetX); 
    float ty = fract(v_Texture.y * resolY + offsetY); 

    vec2 tex = vec2(tx, ty);
    FragColor =  texture(u_RGBTex, tex);
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// 영상 4번째
//------------------------------------------------------------------------------------

void Num()
{
    float tx = v_Texture.x; 
    float ty = v_Texture.y;

    float offsetX = 0;
    float offsetY = 0;

    vec2 newTex = vec2(tx + offsetX, ty + offsetY);
    FragColor =  texture(u_CurrNumTex, newTex);
}

void Nums()
{
    int index = u_InputNum;

    float tx = v_Texture.x / 5; 
    float ty = v_Texture.y / 2;

    float offsetX = (index % 5) / 5.0;
    float offsetY = (index / 5) / 2.0;

    vec2 newTex = vec2(tx + offsetX, ty + offsetY);
    FragColor =  texture(u_NumsTex, newTex);
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

void main()
{
    //TextureSampling();
    //TextureQ1();
    //TextureQ3();
    
    //Flag();
    //Flame();
    //Example();
    //Example2();

    //TextureQ41();

    //Num();
    Nums();
}
