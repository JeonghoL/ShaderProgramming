#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Texture;

uniform sampler2D u_RGBTex;

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

void main()
{
    //TextureSampling();
    //TextureQ1();
    TextureQ3();
}
