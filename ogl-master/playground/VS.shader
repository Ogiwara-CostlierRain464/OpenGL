#version 330 core

layout(location = 0) in vec3 vertexPositionModelSpace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormalModelSpace;

out vec2 uv;
out vec3 positionWorldSpace;
out vec3 normalCameraSpace;
out vec3 eyeDirectionCameraspace;
out vec3 lightDirectionCameraspace;

uniform mat4 mvp;
uniform mat4 v;
uniform mat4 m;
uniform vec3 lightPositionWorldSpace;

void main(){
	gl_Position =  mvp * vec4(vertexPositionModelSpace,1);
	positionWorldSpace = (m * vec4(vertexPositionModelSpace,1)).xyz;
	
	vec3 vertexPositionCameraSpace = ( v * m * vec4(vertexPositionModelSpace,1)).xyz;
	eyeDirectionCameraspace = vec3(0,0,0) - vertexPositionCameraSpace;
	
	vec3 lightPositionCameraSpace = ( v * vec4(lightPositionWorldSpace,1)).xyz;
	lightDirectionCameraspace = lightPositionCameraSpace + eyeDirectionCameraspace;
	
	normalCameraSpace = (v * m * vec4(vertexNormalModelSpace,0)).xyz;
	
	uv = vertexUV;
}
