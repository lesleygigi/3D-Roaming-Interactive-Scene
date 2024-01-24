#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform sampler2D floor;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpcular;

void main()
{             
    // ambient
    vec3 ambient=lightAmbient*lightColor;

    //diffuse
    vec3 norm=normalize(Normal);
    vec3 lightDir=normalize(lightPos-FragPos);
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse=lightDiffuse*diff*lightColor;

    //specular
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewPos,reflectDir),0.0),16);
    vec3 specular=lightSpcular*spec*lightColor;

    vec3 result=0.1*(ambient+diffuse+specular)+0.9*texture(floor, TexCoords).rgb;
    FragColor=vec4(result,1.0);
}