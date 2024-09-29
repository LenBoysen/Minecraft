#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_ViewRotation;
uniform mat4 u_TransformMatrix;

uniform vec3 u_SkyPosition;

out vec3 v_SkyPosition;

void main(){
	v_SkyPosition = u_SkyPosition;
	gl_Position =  u_ViewRotation * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core
		
layout(location = 0) out vec4 color;


uniform vec3 u_SkyPosition;


void main(){
	
	float mix = max(dot(u_SkyPosition, vec3(0.0, 1.0, 0.0)) ,0.0);
	
	//color =  vec4(u_SkyPosition, 1f);
	color =  mix * vec4(0.47f, 0.65f, 1f, 1f);

}