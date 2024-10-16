#version 440 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 viewPos;   
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture(texture1, TexCoord);

    // Normalize vectors
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-lightDir);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * lightColor;

    // Results
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(texColor.rgb * result, texColor.a);
    //FragColor = vec4(result, 1.0);
}
