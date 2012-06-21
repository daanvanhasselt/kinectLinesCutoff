// uniform          ==  a variable you set as a configuration, for example 'blurSize'
// attribute        ==  a variable that can change for every vertex. for example 'pointSize'
// varying          ==  a variable that can be set for every vertex and is passed on to the fragment shader, but interpolated. for example: texCoords

varying vec2 texCoords;

void main(){
    texCoords = gl_MultiTexCoord0.xy;  // incoming texture coordinates (normalized: 0 - 1) (not normalized: 0 - texture width)
    
//    gl_Position = ftransform();     // default transform

    
    // this is a shortcut for:
    vec4 pos = gl_Vertex;
    pos.z += pos.y;
    gl_Position = gl_ModelViewProjectionMatrix * pos;
    
    // gl_Vertex = the incoming vertex
    
}