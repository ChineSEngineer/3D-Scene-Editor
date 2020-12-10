#version 150 core
in vec4 FragPosition;

uniform vec3 lightPosition;
uniform float far_plane;

void main()
{
    float lightDistance = length(FragPosition.xyz - lightPosition);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}
