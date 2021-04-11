// specular.fs
// 
varying vec3 N; // interpolated surface normal vector
varying vec3 L[ 3 ]; // up to three interpolated light vectors

//Zdefiniuj w sta�ej const float specularExp warto�ci wyk�adnika odbicia lustrzanego (128.0).
//Mniejsza warto�� oznacz� szerszy k�t odbicie zwierciadlanego.
const float specularExp = 128;

void main( void )
{
   gl_FragColor = gl_Color; // primary color copying

   //Zapisz w zmiennej vec3 NN znormalizowany wektor normalny N.
   vec3 NN = normalize(N);
   //Zapisz w zmiennej vec3 NL znormalizowany wektora �wiat�a nr 0
   vec3 NL = normalize(L[0]);
   //Wylicz w zmiennej vec3 NH znormalizowan� sum� wektora NL oraz wektora (0.0, 0.0, 1.0)
   vec3 NH = normalize(NL + (0.0, 0.0, 1.0));
   //Oblicz sk�adow� rozproszon� o�wietlenia (float intensity) podobnie jak w 32. i zapisz j� w zmiennej
   //tymczasowej vec3 diffuse (przypisanie do zmiennej gl_FragColor odb�dzie si� p�niej).
   float intensity = max(0.0, dot(NN, NL));
   vec3 diffuse = intensity;
   //Oblicz sk�adow� odbicia lustrzanego vec3 specular. Je�eli intensywno�� intensity jest mniejsza
   //lub r�wna 0.0, sk�adowa specular jest r�wna wektorowi zerowemu. W przeciwnym wypadku, wylicz
   //intensywno�� float specularIntensity, podobnie jak dla sk�adowej rozproszonej, jednak korzystaj�c z
   //wektora NH zamiast NL. Nast�pnie wylicz sk�adow� odbicia lustrzanego specular jako
   //specularIntensity do pot�gi (pow()) zdefiniowanego wcze�niej wyk�adnika specularExp,
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

    //Obliczenie koloru fragmentu gl_FragColor (tylko pola .rgb) jako sumy sk�adowej �wiat�a
    //rozproszonego (diffuse) i odbicia lustrzanego (specular).
    gl_FragColor.rgb *= diffuse + specular;
    //Skopiowanie przezroczysto�ci fragmentu (.a) z przezroczysto�ci koloru interpolowanego (gl_Color).
    gl_FragColor.a = gl_Color.a;
}
