#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpcular;

uniform sampler2D texture_diffuse1;

layout (std140) uniform Mat
{
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
};


void main()
{   //ambient
    vec3 ambient=lightAmbient*lightColor;

    //diffuse
    vec3 norm=normalize(Normal);
    vec3 lightDir=normalize(lightPos-FragPos);
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse=lightDiffuse*lightColor*diff;

    //specular
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewPos,reflectDir),0.0),64);
    vec3 specular=lightSpcular*lightColor*spec;

    vec3 result=(ambient+diffuse)*texture(texture_diffuse1, TexCoords).rgb;
    FragColor=vec4(result,1.0);
}