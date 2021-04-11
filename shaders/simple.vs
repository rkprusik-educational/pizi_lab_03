// simple.vs
// core functionality of the fixed rendering pipeline

uniform vec3 lightPos[ 3 ]; // up to three light positions
varying vec3 N; // interpolated surface normal vector
varying vec3 L[ 3 ]; // up to three interpolated light vectors

void main( void )
{
   // vertex transformation according to the current model view matrix and projection
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

   // primary color copying
   gl_FrontColor = gl_Color;
}
