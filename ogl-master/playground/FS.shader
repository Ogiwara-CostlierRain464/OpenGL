#version 330 core

in vec2 uv;
in vec3 positionWorldSpace;
in vec3 normalCameraSpace;
in vec3 eyeDirectionCameraspace;
in vec3 lightDirectionCameraspace;

out vec3 color;

uniform sampler2D textureSampler;
uniform mat4 mv;
uniform vec3 lightPositionWorldSpace;


void main(){

	vec3 lightColor = vec3(1,1,1);
	float lightPower = 50.0f;
	
	vec3 materialDiffuseColor = texture( textureSampler, uv ).rgb;
	vec3 materialAmbientColor = vec3(0.1,0.1,0.1) * materialDiffuseColor;
	vec3 materialSpecularColor = vec3(0.3,0.3,0.3);

	float distance = length( lightPositionWorldSpace - positionWorldSpace);

	vec3 n = normalize( normalCameraSpace);
	vec3 l = normalize( lightDirectionCameraspace );

	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	vec3 E = normalize(eyeDirectionCameraspace);

	vec3 R = reflect(-l,n);

	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	color = 
		materialAmbientColor +

		materialDiffuseColor * lightColor * lightPower * cosTheta / (distance*distance) +

		materialSpecularColor * lightColor * lightPower * pow(cosAlpha,5) / (distance*distance);
}