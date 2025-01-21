#version 330 core
//uniform vec4 color;
in vec4 vertexColor;
in vec2 textureCoord;
out vec4 fragColor;

// texture를 shader에서 사용
uniform sampler2D tex;
uniform sampler2D tex2;

void main() {
    //  두 sampler2D로부터 얻어온 텍스처 컬러를 5 : 5? 비율로 블랜딩
    fragColor = texture(tex, textureCoord) * 0.5 + texture(tex2, textureCoord) * 0.5;
}