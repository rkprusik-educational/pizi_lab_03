// diffuse.fs
// 
varying vec3 N; // interpolated surface normal vector
varying vec3 L[ 3 ]; // up to three interpolated light vectors

void main( void )
{
   gl_FragColor = gl_Color; // primary color copying

   //Oblicz jasnoœæ float intensity jako maksimum (max()) z 0.0 oraz iloczynu skalarnego (dot())
   //znormalizowanego (normalize()) wektora normalnej i znormalizowanego wektora œwiat³a nr 0
   //(L[0]). Uwaga! Brak normalizacji parametrów typu varying prowadzi do nieprawid³owych, choæ czêsto
   //ciekawych efektów.
   float intensity = max(0.0, dot(normalize(N), normalize(L[0])));

   //Wyznacz kolor fragmentu (gl_FragColor) jako interpolowanego koloru z jednostki VS (gl_Color) –
   //wystarczy podstawienie – interpolacja wykonywana jest automatycznie podczas rasteryzacji.
   gl_FragColor = gl_Color;

   //Ustalenie ostatecznej jasnoœci koloru fragmentu (tylko pola .rgb) poprzez jego przemno¿enie przez
   //wyznaczon¹ wczeœniej intensywnoœæ. 
   gl_FragColor.xyz *= intensity;
}
