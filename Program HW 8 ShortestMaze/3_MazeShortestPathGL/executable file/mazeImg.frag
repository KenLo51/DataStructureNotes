#version 460 core

in VertexData{
  vec2 TexCoord;
} inData;

out vec4 FragColor;

uniform ivec2 screenSize;
uniform sampler2D image;

uniform ivec2 begPoint;
uniform ivec2 endPoint;

void main()
{
    ivec2 imageSize = textureSize(image, 0);

    vec2 scale = vec2(screenSize)/vec2(imageSize);
    scale = scale / (min(scale.x, scale.y));

    vec2 samplePos = (inData.TexCoord - vec2(0.5, 0.5)) * scale + vec2(0.5, 0.5);
    if(samplePos.x<0.0 || samplePos.x>1.0) discard;
    if(samplePos.y<0.0 || samplePos.y>1.0) discard;
    //vec2 samplePos = inData.TexCoord;
    samplePos.y = 1.0f-samplePos.y;

    float gval = (texture2D(image, samplePos).r*255 > 0.5f)? 0.2: 1.0;
    FragColor = vec4(gval, gval, gval, 1.0f);
    vec2 mazeBlockIdx = floor(samplePos * imageSize);
    if(mazeBlockIdx == begPoint)
        FragColor *= vec4(0.2f, 0.8f, 0.2f, 1.0f);
    else if(mazeBlockIdx == endPoint)
        FragColor *= vec4(0.8f, 0.2f, 0.2f, 1.0f);


    //FragColor = texture2D(image, inData.TexCoord)+0.5;
    //FragColor = vec4(samplePos, 1.0f, 1.0f);
} 
