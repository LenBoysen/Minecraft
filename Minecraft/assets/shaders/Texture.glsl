#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in float a_FaceId;

uniform mat4 u_ViewProjection;
uniform mat4 u_TransformMatrix;

out vec2 v_TexCoord;
out float v_FaceId;


void main(){
	v_TexCoord = a_TexCoord;
	v_FaceId = a_FaceId;
	gl_Position = u_ViewProjection * u_TransformMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core
		
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_FaceId;

uniform sampler2D u_Texture;
uniform vec2 u_SubTexture;
uniform int u_TexMode;
uniform vec2 u_SideTexture;
uniform vec2 u_TopTexture;
uniform vec2 u_BotTexture;
uniform vec4 u_GrasMultColor;

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
		multColor = u_GrasMultColor;
	}
	
	color = texture(u_Texture, vec2(v_TexCoord.x, 16.0f - v_TexCoord.y) * (1/16.0f)) * multColor;

}