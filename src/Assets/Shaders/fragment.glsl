#version 330 core

struct Light
{
    vec3 color;
    vec3 direction;
};

uniform Light uLight;
uniform vec3 uCameraPos;
uniform vec3 color;

out vec4 FragColor;

in vec2 vUV;
in vec3 vNormal;
in vec3 vFragPos;

uniform sampler2D baseColorTexture;

void main()
{
    vec3 norm = normalize(vNormal);

    // diffuse漫反射
    vec3 lightDir = normalize(-uLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLight.color;


    // specular镜面反射
    vec3 viewDir = normalize(uCameraPos - vFragPos);
    vec3 redlectDir = reflect(-lightDir, norm);                 //reflect(入射向量I, 法向量N)，返回反射光线方向。                
    //spec：高光强度系数 0~1。
    //点积，即视线在反射上的投影。
    //幂次就是高光粗糙度 / 反光集中度，指数 32：高光区域很小、光斑锐利，像金属、光滑塑料
    float spec = pow(max(dot(viewDir, redlectDir), 0.0), 32.0); 
    //强度系数
    float specularStrength = 0.5;               
    vec3 specular = specularStrength * spec * uLight.color;


    
    // ambient环境光
    const float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * uLight.color;

    vec4 texColor = texture(baseColorTexture, vUV);
    vec3 result = (diffuse + specular + ambient) * texColor.xyz * color;//vec3 result = (diffuse + specular)* texColor.xyz;加入环境光前    

    FragColor = vec4(result, 1.0);
}