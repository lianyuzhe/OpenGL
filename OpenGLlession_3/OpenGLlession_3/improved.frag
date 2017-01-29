#version 400

in vec3 positioN;
in vec3 norM;
layout( location = 0 ) out vec4 FragColor;
struct LightInfo{
    vec3 Position;
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
uniform float Shininess;
vec3 ads(int lightIndex,vec3 position,vec3 norm){
    vec3 s;
//    if(Lights[lightIndex].Position.w==0.0)//directional light source
//        s = normalize( vec3(Lights[lightIndex].Position) );
//    else
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
//            return I * ( Ka +spotFactor*Kd * max( dot(s, norm), 0.0 ) +/*Ks * pow( max( dot(r,v), 0.0 ), Shininess )*/spotFactor*Ks * pow( max( dot(h,norm), 0.0 ), Shininess ));
            return vec3(1.0f);
        }else{
            return vec3(0.0f);
        }
    }else{
//        return I * ( Ka +Kd * max( dot(s, norm), 0.0 ) +/*Ks * pow( max( dot(r,v), 0.0 ), Shininess )*/Ks * pow( max( dot(h,norm), 0.0 ), Shininess ));
        return vec3(0.5f);
    }
}
void main() {
    vec3 Color=vec3(0.0);
    //Color=vec3(1.0,0.0,0.0);
//    for(int i=0;i<5;i++){
//        Color+=ads(i,position,norm);
//    }
    Color=ads(0,positioN,norM);
    FragColor = vec4(Color, 1.0f);
}
