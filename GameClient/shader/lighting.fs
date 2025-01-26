#version 330 core
//uniform vec4 color;
// in vec4 vertexColor;
in vec3 normal;
in vec2 textureCoord;
in vec3 position;

out vec4 fragColor;

// texture를 shader에서 사용
// uniform sampler2D tex;
// uniform sampler2D tex2;

struct Light {
    vec3 position;

    vec3 direction;
    vec2 cutoff;

    //  빛의 감쇠
    vec3 attenuation;
    //  주변광
    vec3 ambient;
    //  분산광
    vec3 diffuse;
    //  반사광
    vec3 specular;
};
uniform Light light;
 
struct Material {
    //  오브젝트 텍스처 픽셀
    sampler2D diffuse;
    //  오브젝트 specular 픽셀 
    sampler2D specular;
    
    float shininess;
};
uniform Material material;

uniform vec3 viewPos;

void main() {
    //  두 sampler2D로부터 얻어온 텍스처 컬러를 5 : 5? 비율로 블랜딩
    // fragColor = texture(tex, textureCoord) * 0.5 + texture(tex2, textureCoord) * 0.5;
    vec3 texColor = texture2D(material.diffuse, textureCoord).xyz;
    //  텍스처 주변광 적용
    vec3 ambient = texColor * light.ambient;
 
    // vec3 lightDir = normalize(light.position - position);
    // vec3 lightDir = normalize(-light.direction);

    //  빛의 위치랑 픽셀의 3D 위치의 거리
    float dist = length(light.position - position);
    //  1.0,d,d*d
    vec3 distPoly = vec3(1.0, dist, dist*dist);
    //  광원이 어느정도 거리까지 영향을 주게할 것인지
    //  1.0 / (1.0 * kc + d * kl + (d * d) * kq )
    float attenuation = 1.0 / dot(distPoly, light.attenuation);
    //  빛의 방향 벡터 노멀라이즈
    vec3 lightDir = (light.position - position) / dist;

    /*
    //  법선 벡터 노멀라이즈
    vec3 pixelNorm = normalize(normal);

    //  분산광
    float diff = max(dot(pixelNorm, lightDir), 0.0);
    vec3 diffuse = diff * texColor * light.diffuse;
 
    vec3 specColor = texture2D(material.specular, textureCoord).xyz;
    //  픽셀의 3D 위치에서 카메라 방향
    vec3 viewDir = normalize(viewPos - position);
    //  빛의 반사된 방향
    vec3 reflectDir = reflect(-lightDir, pixelNorm);
    //  반사광
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 specular = spec * specColor * light.specular;
 
    // vec3 result = ambient + diffuse + specular;
    vec3 result = (ambient + diffuse + specular) * attenuation;
    */
    float theta = dot(lightDir, normalize(-light.direction));
    vec3 result = ambient;
	float intensity = clamp((theta - light.cutoff[1]) / (light.cutoff[0] - light.cutoff[1]),0.0, 1.0);

    if (intensity > 0.0) 
    {
        vec3 pixelNorm = normalize(normal);
        float diff = max(dot(pixelNorm, lightDir), 0.0);
        vec3 diffuse = diff * texColor * light.diffuse;

        vec3 specColor = texture2D(material.specular, textureCoord).xyz;
        vec3 viewDir = normalize(viewPos - position);
        vec3 reflectDir = reflect(-lightDir, pixelNorm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spec * specColor * light.specular;

        result += (diffuse + specular) * intensity;
    }

    result *= attenuation;

    fragColor = vec4(result, 1.0);
    // fragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}