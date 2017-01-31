#version 400

in vec4 position;
in vec3 norm;
layout( location = 0 ) out vec4 FragColor;
struct LightInfo{
    vec4 Position;
    vec3 Intensity;
    bool enableSpotLight;
    vec3 direction;
    float exponent;
    float cutoff;
};
uniform LightInfo Lights[5];

uniform vec3 Ka;
uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ks;
uniform vec3 spotKa;
uniform vec3 spotKd;
uniform vec3 spotKs;
uniform float Shininess;
uniform float spotShininess;
vec3 ads(int lightIndex,vec4 position,vec3 norm){
    vec3 s;
    if(Lights[lightIndex].Position.w==0.0)//directional light source
        s = normalize( vec3(Lights[lightIndex].Position) );
    else
        s = normalize( vec3(Lights[lightIndex].Position - position) );
    vec3 v = normalize(vec3(-position));
    vec3 h=normalize(v+s);
    vec3 I = Lights[lightIndex].Intensity;
    float angle;
    if(Lights[lightIndex].enableSpotLight){
            angle=acos(dot(-s,normalize(Lights[lightIndex].direction)));
        if(angle<Lights[lightIndex].cutoff){
            float spotFactor=pow(dot(-s,Lights[lightIndex].direction),Lights[lightIndex].exponent);
            if(spotFactor<0)
                spotFactor=-spotFactor;
            return I * ( spotKa +spotFactor*spotKd * max( dot(s, norm), 0.0 ) +spotFactor*spotKs * pow( max( dot(h,norm), 0.0 ), spotShininess ));
        }else{
            return I*spotKa;
        }
    }else{
        return I * ( Ka +Kd * max( dot(s, norm), 0.0 ) +/*Ks * pow( max( dot(r,v), 0.0 ), Shininess )*/Ks * pow( max( dot(h,norm), 0.0 ), Shininess ));
    }
}
void main() {
    vec3 Color=vec3(0.0);
    for(int i=0;i<5;i++){
        Color+=ads(i,position,norm);
    }
    FragColor = vec4(Color, 1.0f);
}
