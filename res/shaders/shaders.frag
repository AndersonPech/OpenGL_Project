#version 330 core

in vec2 vTexCoord;
in vec3 frag_pos;
in vec3 nrm;

uniform sampler2D uTex;
uniform sampler2D u_specular;

uniform vec3 light_pos;
uniform vec3 light_ambient;
uniform vec3 light_diffuse;
//uniform mat4 uModel;

// Material parameters


out vec4 color;

struct PointLight {
    vec3 position;
};


vec3 CalcDirLight();
vec3 CalcPointLight(PointLight light);
uniform PointLight pointLights[30];

void main() {
    vec3 result = CalcDirLight();
    for(int i = 0; i < 30; ++i)
    {
        result += CalcPointLight(pointLights[i]);
    }

    color = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight()
{
    // setup ambient light
    vec3 ambient = light_ambient * texture(uTex, vTexCoord).rgb;

    // calc light_dir
    vec3 light_dir = normalize(light_pos - frag_pos);
    vec3 n = normalize(nrm);

    // setup diffuse light
    float diff_str = max(dot(n, light_dir), 0.0);
    vec3 diffuse = light_diffuse * diff_str * texture(uTex, vTexCoord).rgb;
    
    // float specularStrength = 5;
    // vec3 reflectDir = reflect(-light_dir, n);  
    // float spec = pow(max(dot(light_dir, reflectDir), 0.0), specularStrength);
    // vec3 specular = specularStrength * spec * texture(u_specular, vTexCoord).rgb;  

    vec3 result = (ambient + diffuse);
    return result;
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light)
{
    // setup ambient light
    vec3 ambient = vec3(1.0f, 0.5f, 0.0f) * texture(uTex, vTexCoord).rgb;;

    // calc light_dir
    vec3 light_dir = normalize(light.position - frag_pos);
    float light_dist = length(light.position - frag_pos);
    vec3 n = normalize(nrm);

    // setup diffuse light
    float diff_str = max(dot(n, light_dir), 0.0);
    vec3 diffuse = light_diffuse * diff_str;
    
    float light_attenuation = 1;
    float attenuation = 1.0 / (1.0 + light_attenuation * light_dist);

    float specularStrength = 40;
    vec3 reflectDir = reflect(-light_dir, n);  
    float spec = pow(max(dot(light_dir, reflectDir), 0.0), specularStrength);
    vec3 specular = specularStrength * spec * texture(u_specular, vTexCoord).rgb * vec3(1.0f, 0.0f, 0.0f);  
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = (ambient + diffuse + specular) * vec3(1.0f, 0.0f, 0.0f) ;
    return result;
}

