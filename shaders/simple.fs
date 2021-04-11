// simple.fs
// core functionality of the fixed rendering pipeline
varying vec3 N; // interpolated surface normal vector
varying vec3 L[ 3 ]; // up to three interpolated light vectors

void main( void )
{
   gl_FragColor = gl_Color * 0.5f; // primary color copying
}
