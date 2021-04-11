// specular.fs
// 
varying vec3 N; // interpolated surface normal vector
varying vec3 L[ 3 ]; // up to three interpolated light vectors

//Zdefiniuj w sta³ej const float specularExp wartoœci wyk³adnika odbicia lustrzanego (128.0).
//Mniejsza wartoœæ oznacz¹ szerszy k¹t odbicie zwierciadlanego.
const float specularExp = 128;

void main( void )
{
   gl_FragColor = gl_Color; // primary color copying

   //Zapisz w zmiennej vec3 NN znormalizowany wektor normalny N.
   vec3 NN = normalize(N);
   //Zapisz w zmiennej vec3 NL znormalizowany wektora œwiat³a nr 0
   vec3 NL = normalize(L[0]);
   //Wylicz w zmiennej vec3 NH znormalizowan¹ sumê wektora NL oraz wektora (0.0, 0.0, 1.0)
   vec3 NH = normalize(NL + (0.0, 0.0, 1.0));
   //Oblicz sk³adow¹ rozproszon¹ oœwietlenia (float intensity) podobnie jak w 32. i zapisz j¹ w zmiennej
   //tymczasowej vec3 diffuse (przypisanie do zmiennej gl_FragColor odbêdzie siê póŸniej).
   float intensity = max(0.0, dot(NN, NL));
   vec3 diffuse = intensity;
   //Oblicz sk³adow¹ odbicia lustrzanego vec3 specular. Je¿eli intensywnoœæ intensity jest mniejsza
   //lub równa 0.0, sk³adowa specular jest równa wektorowi zerowemu. W przeciwnym wypadku, wylicz
   //intensywnoœæ float specularIntensity, podobnie jak dla sk³adowej rozproszonej, jednak korzystaj¹c z
   //wektora NH zamiast NL. Nastêpnie wylicz sk³adow¹ odbicia lustrzanego specular jako
   //specularIntensity do potêgi (pow()) zdefiniowanego wczeœniej wyk³adnika specularExp,
   //zrzutowana na wektor vec3.
   vec3 specular;
   float specularIntensity;
   if(intensity > 0.0)
   {
        specularIntensity = max(0.0, dot(NN, NH));
        specular = (vec3) pow(specularIntensity, specularExp);
   }
   else
        specular = (0.0, 0.0, 0.0);

    //Obliczenie koloru fragmentu gl_FragColor (tylko pola .rgb) jako sumy sk³adowej œwiat³a
    //rozproszonego (diffuse) i odbicia lustrzanego (specular).
    gl_FragColor.rgb *= diffuse + specular;
    //Skopiowanie przezroczystoœci fragmentu (.a) z przezroczystoœci koloru interpolowanego (gl_Color).
    gl_FragColor.a = gl_Color.a;
}
