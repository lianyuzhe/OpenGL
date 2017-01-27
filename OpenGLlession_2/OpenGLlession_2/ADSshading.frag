#version 400

in vec3 FrontColor;
in vec3 BackColor;
layout( location = 0 ) out vec4 FragColor;

void main() {
    //FragColor = vec4(LightIntensity, 1.0);
    if(gl_FrontFacing){
        FragColor=vec4(FrontColor,1.0);
    }else{
        FragColor=vec4(BackColor,1.0);
    }
}
