#version 400
layout (location = 0) in vec3 vp;
layout (location = 1) in vec2 texColorIn;
layout (location = 2) in vec3 normalIn;

out vec3 worldPos;

uniform mat4 mView;
uniform mat4 mWorld;
uniform mat4 mProj;
uniform mat4 mWVP;

// Do not modify anything. Use direct untransformed ortho projection.
void main(){
  vec4 temp = mWVP * vec4( vp, 1.0f );
  temp.x = temp.x;
  temp.y = temp.y;
  temp.z = 0.5f;
  temp.w = temp.w;
  temp.w = -temp.w;

  gl_Position = temp;
  //vec3 vptemp = vp;
  //vptemp.z = 0.5f;
  //gl_Position = vec4( vptemp, 1.0f );

}
