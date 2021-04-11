// fog.fs
// 
varying vec3 N; // interpolated surface normal vector
varying vec3 L[ 3 ]; // up to three interpolated light vectors

void main( void )
{
   gl_FragColor = gl_Color; // primary color copying
}
