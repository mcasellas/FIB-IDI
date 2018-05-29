#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

out vec4 vertexFS;
out vec3 normalFS;

out vec3 matambFS;
out vec3 matdiffFS;
out vec3 matspecFS;
out float matshinFS;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

void main() {
    // Passem a coordenades d'observador
    vec4 vertexSCO = view*TG*vec4(vertex, 1.0); // Vèrtex
    mat3 normalMatrix = inverse(transpose(mat3(view*TG))); // Normal
    vec3 normalSCO = normalMatrix*normal;

    // Bypass cap al Fragment Shader
    vertexFS = vertexSCO;
    normalFS = normalSCO;

    matambFS = matamb;
    matdiffFS = matdiff;
    matspecFS = matspec;
    matshinFS = matshin;


    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
