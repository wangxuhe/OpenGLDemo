#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    // texurre 用于采样纹理的颜色(纹理采样器， 纹理坐标)
    //FragColor = texture(ourTexture, TexCoord);
    
    // 纹理变色
    FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
}
