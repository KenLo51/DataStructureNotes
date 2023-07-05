#version 460 core

in VertexData{
  vec2 TexCoord;
} inData;

out vec4 FragColor;

uniform ivec2 screenSize;
uniform sampler2D image;

void main()
{
    ivec2 imageSize = textureSize(image, 0);

    vec2 scale = vec2(screenSize)/vec2(imageSize);
    scale = scale / (min(scale.x, scale.y));

    vec2 samplePos = (inData.TexCoord - vec2(0.5, 0.5)) * scale + vec2(0.5, 0.5);
    samplePos.y = 1.0f-samplePos.y;
    float isClicked = 1;
    if(samplePos.x<0.0 || samplePos.x>1.0) isClicked = 0;
    if(samplePos.y<0.0 || samplePos.y>1.0) isClicked = 0;

    FragColor = vec4(samplePos.x, samplePos.y, isClicked, 1.0f);

} 
