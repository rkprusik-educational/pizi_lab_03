// lighting.c
// By M.Szwoch, Based on
// OpenGL SuperBible, Chapter 17
// Demonstrates vertex and fragment shaders together
// Program by Benjamin Lipchak

#include "../shared/gltools.h"   // OpenGL toolkit
#include "../shared/glFrame.h"   // Frame toolkit

#include <stdio.h>
#pragma comment(lib, "legacy_stdio_definitions.lib") // For backward compatibility
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; } // For backward compatibility

// 3. Add your first and last name to the title bar
// ...
const char *authorName = "Rados³aw Prusik";

enum ShaderTypes { SIMPLE, DIFFUSE, SPECULAR, THREELIGHTS, FOG, PARAMS, SHADERS_NUM };

GLuint vShader[ SHADERS_NUM ]; // vertex shader IDs (VS)
GLuint fShader[ SHADERS_NUM ]; // fragment shader IDs (FS)
GLuint progObj[ SHADERS_NUM ]; // shading program objects (VS+FS)
GLboolean needsValidation[ SHADERS_NUM ]; // validation information of the shading program
char *shaderNames[ SHADERS_NUM ] = 
{ // shader names
	"simple", 
	"diffuse", 
	"specular", 
	"3lights",
	"fog",
	"params"
};
// current shading program
ShaderTypes actualShader = SIMPLE;
// window size
GLint windowWidth = 1024;
GLint windowHeight = 768;

GLint mainMenu, shaderMenu; // menu handles

GLfloat cameraPos[] = { 100.0f, 75.0f, 150.0f, 1.0f };
GLdouble cameraZoom = 0.4;

GLfloat lightPos0[] = { 140.0f, 250.0f, 140.0f, 1.0f };
GLfloat lightPos1[] = { -140.0f, 250.0f, 140.0f, 1.0f };
GLfloat lightPos2[] = { 0.0f, 250.0f, -200.0f, 1.0f };

GLfloat lightRotation = 0.0f;

#define MAX_INFO_LOG_SIZE 2048
GLchar infoLog[ MAX_INFO_LOG_SIZE ]; // buffer for shader handling errors
void FatalError( char *errorKind, char *errorDescription, char *errorDetails )
{ // displaying error information and terminating the program ("radical" error handling)
	fprintf( stderr, "!!! %s ERROR !!!\n", errorKind );
	fprintf( stderr, " %s \"%s\"\n", errorDescription, errorDetails );
	Sleep( 5000 );
	exit( 0 );
}
GLchar *LoadShaderText( const char *fileName )
{ // Load the shader source code from a file as ASCIIZ text
	GLchar *shaderText = NULL;
	GLint shaderLength = 0;
	FILE *fp;
	fp = fopen( fileName, "r" );
	if ( fp != NULL )
	{
		while ( fgetc( fp ) != EOF )
			shaderLength++;
		rewind( fp );
		shaderText = (GLchar *)malloc( shaderLength + 1 );
		if ( shaderText != NULL )
			fread( shaderText, 1, shaderLength, fp );
		shaderText[ shaderLength ] = '\0';
		fclose( fp );
	}
	return shaderText;
}
void LoadAndCompileShader( char * fullFileName, GLuint shaderKind, GLuint *shaderNum )
{ // loading a shader file and compiling it
	// loading a shader file
	GLint success;
	GLchar *shString = LoadShaderText( fullFileName ); // loading the source code
	fprintf( stdout, "Loading '%s' shader\n", fullFileName );

	if ( !shString )
		FatalError( "LOADING", "Unable to load", fullFileName );

	// 5. Create a shader with glCreateShader()
	// The parameter is the type of the shader object (shaderKind)
	// The result of the call must be substituted into the object identifier (* shaderNum)
	// ...
	*shaderNum = glCreateShader(shaderKind);

	// 6. Specifying the source code of the shader with glShaderSource ()
	// The first parameter is the shader object ID, 
	// the second is the number of objects (1), 
	// the third is the pointer to the pointer (const GLchar **) & to the source code
	// the fourth is NULL
	// ...
	glShaderSource(*shaderNum, 1, (const GLchar**)&shString, NULL);

	free( shString );

	// 7. Compile a shader with glCompileShader ()
	// The parameter is the ID of the shader object 
	// ...
	glCompileShader(*shaderNum);


	// 8. Check compilation status with glGetShaderiv ()
	// The first parameter is the shader object ID, 
	// the second - type of checked error (GL_COMPILE_STATUS)
	// the third - variable address on the returned error code (success)
	// ...
	glGetShaderiv(*shaderNum, GL_COMPILE_STATUS, &success);

	// 9. Information about a possible error when success  == 0
	// The message text is retrieved with the glGetShaderInfoLog() function
	// The first parameter is the shader object ID, 
	// the second - buffer size (MAX_INFO_LOG_SIZE),
	// the thord is NULL,
	// the fourth - buffer (infoLog)
	// The error message should be output with the FatalError() function
	// ...
	if (success == 0)
	{
		glGetShaderInfoLog(*shaderNum, MAX_INFO_LOG_SIZE, NULL, infoLog);
		FatalError("COMPILATION STATUS UNSUCCESSFUL", "", infoLog);
	}
}
void PrepareShader( GLint shaderNum )
{ // Preparation of the shading program
	char fullFileName[ 255 ];
	GLint success;

	// Specifying the relative name of the file in fullFileNum with the vertex shader pointed by shaderNum
	sprintf( fullFileName, "./shaders/%s.vs", shaderNames[ shaderNum ] );

	// 11. Load and compile a vertex shader with LoadAndCompileShader ()
	// The first parameter is the name of the file,
	// the second is type of shader program (GL_VERTEX_SHADER) 
	// the third is the address of the corresponding field in the vShader ID table of shader objects
	// ...
	LoadAndCompileShader(fullFileName, GL_VERTEX_SHADER, &vShader[shaderNum]);

	// 12. Determining the relative name of the file with the indicated (shaderNum) fragment shader
	// The files have the "fs" extension
	// ...
	sprintf(fullFileName, "./shaders/%s.fs", shaderNames[shaderNum]);

	// 13. Load and compile the fragment shader: (GL_FRAGMENT_SHADER, fShader)
	// ...
	LoadAndCompileShader(fullFileName, GL_FRAGMENT_SHADER, &fShader[shaderNum]);

	// 14. Create a shader program object with glCreateProgram ()
	// assign the shader object identifier to the appropriate place in the progObj array
	// ...
	progObj[shaderNum] = glCreateProgram();

	// 15. Attach compiled VS code to the shader program object with glAttachShader ()
	// The first parameter is the shader program object ID, 
	// the second parameter is the VS object ID
	// ...
	glAttachShader(progObj[shaderNum], vShader[shaderNum]);

	// 16. Attach compiled FS code to a shader object
	// ...
	glAttachShader(progObj[shaderNum], fShader[shaderNum]);

	// 17. Linking a shader program with glLinkProgram ()
	// The parameter is the shader object ID
	// ...
	glLinkProgram(progObj[shaderNum]);

	// 18. Check connection status with glGetProgramiv ()
	// The first parameter is the shader program object ID, 
	// the second type of checked error (GL_LINK_STATUS)
	// the third variable address on the returned error code (success)
	// ...
	glGetProgramiv(progObj[shaderNum], GL_LINK_STATUS, &success);

	// 19. Information about a possible error in the case when success == 0
	// SHADER LINKING, "Unable to link"
	// ...
	if (success == 0)
	{
		FatalError("SHADER LINKING", "Unable to link", "");
	}

	// 20. Setting the needsValidation array in the appropriate place [] 
	// information about the need to validate the shader program (GL_TRUE)
	// ...
	needsValidation[shaderNum] = GL_TRUE;
}
// ------------------------------------------------------------------------
void DrawModels(void)
{ // drawing objects on the scene, setting Uniform parameters for shading units
	M3DVector3f lightPos0Eye, lightPos1Eye, lightPos2Eye;
	M3DMatrix44f mv;

	// transformation of light into camera space (eye space)
	glPushMatrix();
	glRotatef( lightRotation, 0.0, 1.0, 0.0 );
	glGetFloatv( GL_MODELVIEW_MATRIX, mv );
	m3dTransformVector3( lightPos0Eye, lightPos0, mv );


	if ( actualShader >= THREELIGHTS )
	{
		m3dTransformVector3( lightPos1Eye, lightPos1, mv );
		m3dTransformVector3( lightPos2Eye, lightPos2, mv );
	}

	glPopMatrix();
	GLint uniformLoc;

	// 29. Getting the identifier of the Uniform variable for the light source 0 with the use of glGetUniformLocation () 
	// The first parameter is the current shader object ID
	// the second parameter is the name of the Uniform variable "lightPos [0]"
	// Substitute the result into the variable uniformLoc
	// ...
	uniformLoc = glGetUniformLocation(progObj[actualShader], "lightPos[0]");

	// 30. If the returned identifier is different from -1, setting the current position of the light to 0 with glUniform3fv ()
	// The first parameter is the variable identifier, 
	// the second parameter is the number of arguments (1),
	// the third parameter is the vector of the lighting coordinates with (lightPos0Eye)
	// ...
	if (uniformLoc != -1)
		glUniform3fv(uniformLoc, 1, lightPos0Eye);


	// 37. The same parameter setting for light No. 1
	// ...
	uniformLoc = glGetUniformLocation(progObj[actualShader], "lightPos[1]");
	if (uniformLoc != -1)
		glUniform3fv(uniformLoc, 1, lightPos1Eye);



	// 37. The same parameter setting for light No. 2
	// ...
	uniformLoc = glGetUniformLocation(progObj[actualShader], "lightPos[2]");
	if (uniformLoc != -1)
		glUniform3fv(uniformLoc, 1, lightPos2Eye);



	// Draw the blue base of objects
	glColor3f( 0.0f, 0.0f, 0.90f );
	glNormal3f( 0.0f, 1.0f, 0.0f );
	glBegin( GL_QUADS );
		glVertex3f( -100.0f, -25.0f, -100.0f );
		glVertex3f( -100.0f, -25.0f, 100.0f );		
		glVertex3f( 100.0f,  -25.0f, 100.0f );
		glVertex3f( 100.0f,  -25.0f, -100.0f );
	glEnd();
	// red cube
	glColor3f( 1.0f, 0.0f, 0.0f );
	glutSolidCube( 48.0f );
	// green sphere
	glColor3f( 0.0f, 1.0f, 0.0f );
	glPushMatrix();
	glTranslatef( -60.0f, 0.0f, 0.0f );
	glutSolidSphere( 25.0f, 50, 50 );
	glPopMatrix();
	// maroon torus
	glColor3f( 1.0f, 0.0f, 1.0f );
	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, 60.0f );
	glutSolidTorus( 8.0f, 16.0f, 50, 50 );
	glPopMatrix();
	// yellow cone
	glColor3f( 1.0f, 1.0f, 0.0f );
	glPushMatrix();
	glRotatef( -90.0f, 1.0f, 0.0f, 0.0f );
	glTranslatef( 60.0f, 0.0f, -24.0f );
	glutSolidCone( 25.0f, 50.0f, 50, 50 );
	glPopMatrix();
	// sea blue teapot
	glColor3f( 0.0f, 1.0f, 1.0f );
	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, -60.0f );
	glutSolidTeapot( 25.0f );
	glPopMatrix();
}
void RenderScene( void )
{ // drawing a scene
	// perspective setting
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	if ( windowWidth > windowHeight )
	{
		GLdouble ar = (GLdouble)windowWidth / (GLdouble)windowHeight;
		glFrustum( -ar * cameraZoom, ar * cameraZoom, -cameraZoom, cameraZoom, 1.0, 1000.0 );
	}
	else
	{
		GLdouble ar = (GLdouble)windowHeight / (GLdouble)windowWidth;
		glFrustum( -cameraZoom, cameraZoom, -ar * cameraZoom, ar * cameraZoom, 1.0, 1000.0 );
	}
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( cameraPos[ 0 ], cameraPos[ 1 ], cameraPos[ 2 ], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
	glViewport( 0, 0, windowWidth, windowHeight );
	// clear the window
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	GLint success;
	// 20. Validation of the actual (actualShader) shader program. 
	// ONLY in case it needs validation needsValidation []
	// WARNING! actualShader is the index !!!!!
	// ...
	if ( needsValidation[ actualShader ] )
	{

		// 21. Validation of the current (actualShader) shader program with the glValidateProgram () function
		// The parameter is the current shader object ID
		// ...
		glValidateProgram(progObj[actualShader]);

		// 22. Check validation status with glGetProgramiv ()
		// The first parameter is the shader program object ID, 
		// the second - type of checked error (GL_VALIDATE_STATUS)
		// the third - variable address on the returned error code (success)
		// ...
		glGetProgramiv(progObj[actualShader], GL_VALIDATE_STATUS, &success);

		// 23. Information about a possible error in the case when success == 0
		// "SHADER VALIDATION", "Unable to validate"
		// ...
		if (success == 0)
		{
			FatalError("SHADER VALIDATION", "Unable to validate", "");
		}


		// 24. Reset information about the need to validate the current shader program
		// ...
		needsValidation[actualShader] = GL_FALSE;
	}
	// Drawing models 
	DrawModels();
	if ( glGetError() != GL_NO_ERROR )
		fprintf( stderr, "GL Error!\n" );
	// rendering a scene
	glutSwapBuffers();
}

// This function does any needed initialization on the rendering context
void SetupRC()
{ // scene initialization
	fprintf( stdout, "Masters of GLSL shaders (2020)\n\n" );
	// 4. Verification of compliance with the minimum requirements of the shadermodel version of the SM
	if ( !GLEE_VERSION_2_0 && ( !GLEE_ARB_fragment_shader || !GLEE_ARB_vertex_shader || 
				!GLEE_ARB_shader_objects || !GLEE_ARB_shading_language_100 ) )
		FatalError( "OpenGL version", "OpenGL version", "earlier then 2.0" );

	fprintf( stdout, "Controls:\n" );
	fprintf( stdout, "\tR/L arrows\t+/- rotate lights\n\n" );
	fprintf( stdout, "\tx/X\t\tMove +/- in x direction\n" );
	fprintf( stdout, "\ty/Y\t\tMove +/- in y direction\n" );
	fprintf( stdout, "\tz/Z\t\tMove +/- in z direction\n\n" );
	fprintf( stdout, "\tq, ESC\t\tExit demo\n\n" );

	// OpenGL settings
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ); // czarne t³o
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glShadeModel( GL_SMOOTH );

	// 25. Load and prepare all SHADERS_NUM shader programs with PrepareShader ()
	// ...
	for (int i = 0; i < SHADERS_NUM; i++)
		PrepareShader(i);


	// 26. Setting the actual (actualShader) shader program with the glUseProgram () function
	// The parameter is the identifier of the current shader program
	// WARNING! actualShader is the index !!!!!
	// ...
	glUseProgram(progObj[actualShader]);
}
char *SetTitle( void )
{
	static char buffer[ 200 ];
	sprintf( buffer, "Master of GLSL shaders by %s [%s]", authorName, shaderNames[ actualShader ] );
	return( buffer );
}
void KeyPressFunc( unsigned char key, int x, int y )
{
	switch ( key )
	{
		case ' ': // space
			actualShader = (ShaderTypes)( ( actualShader + 1 ) % SHADERS_NUM ); // naprzemienne w³¹czenie kolejnych programów cieniowania
			glutSetWindowTitle( SetTitle() );

			// 27. Setting a new (actualShader) shader program with the glUseProgram () function
			// The parameter is the identifier of the current shader program
			// ...
			glUseProgram(progObj[actualShader]);

			break;
		case 'x':
			cameraPos[ 0 ] += 5.0f;
			break;
		case 'X':
         cameraPos[0] -= 5.0f;
			break;
		case 'y':
			cameraPos[ 1 ] += 5.0f;
			break;
		case 'Y':
			cameraPos[ 1 ] -= 5.0f;
			break;
		case 'z':
			cameraPos[ 2 ] += 5.0f;
			break;
		case 'Z':
			cameraPos[2] -= 5.0f;
			break;
		case 'q':
		case 'Q':
		case 27 : /* ESC */
			glutDestroyWindow( glutGetWindow() );
			break;
	}
	glutPostRedisplay(); // refresh the window
}
void SpecialKeys(int key, int x, int y)
{
	switch ( key )
	{
		case GLUT_KEY_LEFT:
			lightRotation -= 5.0f;
			break;
		case GLUT_KEY_RIGHT:
			lightRotation += 5.0f;
			break;
		case GLUT_KEY_UP:
			cameraPos[ 1 ] += 5.0f;
			break;
		case GLUT_KEY_DOWN:
			cameraPos[1] -= 5.0f;
			break;
	}
	glutPostRedisplay(); // refresh the window
}
void ChangeSize( int w, int h )
{
	windowWidth = w;
	windowHeight = h;
}

int main( int argc, char* argv[] )
{
	GLint i;
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( windowWidth, windowHeight );
	glutCreateWindow( SetTitle() ); // create the window
	glutReshapeFunc( ChangeSize );
	glutKeyboardFunc( KeyPressFunc );
	glutSpecialFunc( SpecialKeys );
	glutDisplayFunc( RenderScene );
	SetupRC();
	glutMainLoop();

	if ( glDeleteProgram && glDeleteShader )
	{
		// 26. Free memory for all (SHADERS_NUM) shader program objects 
		// and VS and FS shader units with glDeleteProgram () and glDeleteShader ()
		// ...
		for ( i = 0; i < SHADERS_NUM; i++ )
		{
			glDeleteProgram( progObj[ i ] );
			glDeleteShader( vShader[ i ] );
			glDeleteShader( fShader[ i ] );
		}
	}
	return 0;
}
