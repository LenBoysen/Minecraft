#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_TransformMatrix;

out vec2 v_TexCoord;
out vec3 v_Normal;


void main(){
	v_TexCoord = a_TexCoord;
	v_Normal = a_Normal;
	gl_Position = u_ViewProjection * u_TransformMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core
		
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;

uniform sampler2D u_Texture;
uniform vec2 u_SubTexture;
uniform int u_TexMode;
uniform vec2 u_SideTexture;
uniform vec2 u_TopTexture;
uniform vec2 u_BotTexture;
uniform vec4 u_GrasMultColor;
uniform vec3 u_SunPosVec;

void main(){
	
	//switch(v_FaceId){
	//case 0: 
	//	float x = u_SideTexture.x;
	//	float y = u_SideTexture.y;
	//	color = texture(u_Texture, v_TexCoord * (1/16.0f) + vec2((x+0.0f)/16.0f, (-y+15.0f)/16.0f));
	//	break;
	//}
	float x;
	float y;
	vec2 TexCoord;
	vec4 multColor = vec4(1.0f);
	if(int(v_TexCoord.x) == 0 && int(v_TexCoord.y) == 0 ){
		multColor = vec4(0.35f, 0.78f, 0.29f, 1) ;
	}

	//float ambient = max(dot(u_SunPosVec, vec3(0.0, 1.0, 0.0)),0);
	//color =  vec4(vec3(max(  dot(u_SunPosVec, v_Normal)*(ambient), 0)) * (ambient * -0.3 + 0.7) + (ambient * 0.3 + 0.3), 1.0) * (texture(u_Texture, vec2(v_TexCoord.x, 16.0f - v_TexCoord.y) * (1/16.0f)) * multColor);
	color =  vec4(vec3(dot(u_SunPosVec, v_Normal)*(7.0f/10.0f) + 0.3f), 1.0) ;
	//color = vec4(vec3(dot(u_SunPosVec, v_Normal)*(7.0f/10.0f) + 0.3f), 1.0f) * (texture(u_Texture, vec2(v_TexCoord.x, 16.0f - v_TexCoord.y) * (1/16.0f)) * multColor);

}