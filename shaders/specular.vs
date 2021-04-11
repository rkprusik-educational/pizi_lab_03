// specular.vs 

uniform vec3 lightPos[ 3 ]; // up to three light positions
varying vec3 N; // interpolated surface normal vector
varying vec3 L[ 3 ]; // up to three interpolated light vectors

void main( void )
{
   // vertex transformation according to the current model view matrix and projection
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

   // primary color copying
   gl_FrontColor = gl_Color;

	//Wyznacz wektor normalny N w przestrzeni kamery jako iloczyn macierzy gl_NormalMatrix oraz
	//wektora normalnego (gl_Normal). Wbudowana macierz gl_NormalMatrix jest odwróconą i
	//transponowaną lewą górną podmacierzą 33 ModelView.
	N = gl_NormalMatrix * gl_Normal;

	//Oblicz pomocniczy wektor vec4 V (wierzchołek w przestrzeni widoku modelu eye-space), jako iloczyn
	//macierzy widoku modelu (gl_ModelViewMatrix) i współrzędnych wierzchołka (gl_Vertex).
	vec4 V = gl_ModelViewMatrix * gl_Vertex;

	//Wyznacz wektoraświatła L[ 0 ] w przestrzenie widoku modelu eye-space jako różnicę pomiędzy pozycją
	//światła nr 0 (lightPos[ 0 ]) i wektorem V.xyz
	L[ 0 ] = lightPos[ 0 ] - V.xyz;
}
