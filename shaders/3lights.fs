// 3lights.fs
// 
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory œwiat³a
const float specularExp = 128;
void main( void )
{
    vec3 lightCol[3];
    lightCol[0] = (1.0, 0.0, 0.0);
    lightCol[1] = (0.0, 1.0, 0.0);
    lightCol[2] = (0.0, 0.0, 1.0);

   gl_FragColor = gl_Color; // skopiowanie g³ównego koloru (primary color)
   //Wyznacz znormalizowany wektor NN oraz wyzeruj kolor wynikowy fragmentu (gl_FragColor);
   vec3 NN = normalize(N);
   float intensity[3];
   float specularIntensity[3];
   vec3 NL[3];
   vec3 NH[3];
   vec3 diffuse[3];
   vec3 specular[3];

   //gl_FragColor.xyz = (0.0, 0.0, 0.0);
   for(int i=0; i < 3; i++)
   {
       //initializing values
       NL[i] = NH[i] = specular[i] = diffuse[i] = (0.0, 0.0, 0.0);
       intensity[i] = specularIntensity[i] = 0.0;

       NL[i] = normalize(L[i]);
       NH[i] = normalize(NL[i] + (0.0, 0.0, 1.0));
       intensity[i] = max(0.0, dot(NN, NL[i]));
       diffuse[i] = intensity[i];
       if(intensity[i] > 0.0)
       {
           specularIntensity[i] = max(0.0, dot(NN, NH[i]));
           specular[i] = (vec3) pow(specularIntensity[i], specularExp);
       }
       else
           specular[i] = (0.0, 0.0, 0.0);

       gl_FragColor.rgb += (diffuse[i] + specular[i]) * lightCol[i];
   }
   gl_FragColor.a = gl_Color.a;
}
