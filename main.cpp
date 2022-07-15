#define _CRT_SECURE_NO_DEPRECATE
#include <GL/glut.h>
#include <stdio.h>
#include <ctype.h>

int bigOrbitActive = 1;//Órbitas maiores
int changeCamera = 0;//Mudança de câmera
int zoom = 80;//Proximidade da câmera
static float factor = 1;//Fator de velocidade


bool luz0 = true;
bool luz1 = true;

GLdouble Angle = 80.0;
GLdouble a = 0.0;
GLdouble b = 0.0;
GLdouble c = 0.0;

//=========================================================================================================================


int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}
GLuint LoadTexture(const char* filename)
{

	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint  texture_ID = 0;


	FILE* pFile = fopen(filename, "rb");
	if (pFile == 0)
		return 0;

	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, 54, SEEK_SET);


	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}


	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}


	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}


	// The is for the compatibility with old version of opengl
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width)
			|| !power_of_two(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 512;
			const GLint new_height = 512; // set the picture to 512*512
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;


			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;


			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}


			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);


			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}


	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	free(pixels);
	return texture_ID;

}


//==========================================================================================================================

class Planet {
public:
	float radius, distance, orbit, orbitSpeed, axisTilt, axisAni;
	Planet(float _radius, float _distance, float _orbit, float _orbitSpeed, float _axisTilt, float _axisAni) {
		radius = _radius;
		distance = _distance;
		orbit = _orbit;
		orbitSpeed = _orbitSpeed;
		axisTilt = _axisTilt;
		axisAni = _axisAni;
	}
	//Linhas da órbita
	void drawSmallOrbit(void) {
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glutWireTorus(0.001, distance, 100.0, 100.0);
		glPopMatrix();
	}
	//Gera o satélite natural do planeta
	void drawMoon(void) {
		GLUquadricObj* quadric;
		quadric = gluNewQuadric();
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(orbit, 0.0, 1.0, 0.0);
		glTranslatef(distance, 0.0, 0.0);
		gluSphere(quadric, radius, 20.0, 20.0);
		glPopMatrix();
	}

};

Planet sun(5.0, 0, 0, 0, 0, 0);				//Sol
Planet mer(1.0, 7, 0, 4.74, 02.11, 0);		//Mercurio
Planet ven(1.5, 11, 0, 3.50, 177.0, 0);		//Venus
Planet ear(2.0, 16, 0, 2.98, 23.44, 0);		//Terra
Planet mar(1.2, 21, 0, 2.41, 25.00, 0);		//Marte
Planet jup(3.5, 28, 0, 1.31, 03.13, 0);		//Jupiter
Planet sat(3.0, 37, 0, 0.97, 26.70, 0);		//Saturno
Planet ura(2.5, 45.5, 0, 0.68, 97.77, 0);	//Urano
Planet nep(2.3, 53.6, 0, 0.54, 28.32, 0);	//Netuno
Planet lun(.40, 3, 0, 5.40, 0, 0);			//Lua

//Desenha a trajetória das órbitas
void orbitalTrails(void) {
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.001, mer.distance, 100.0, 100.0);
	glutWireTorus(0.001, ven.distance, 100.0, 100.0);
	glutWireTorus(0.001, ear.distance, 100.0, 100.0);
	glutWireTorus(0.001, mar.distance, 100.0, 100.0);
	glutWireTorus(0.001, jup.distance, 100.0, 100.0);
	glutWireTorus(0.001, sat.distance, 100.0, 100.0);
	glutWireTorus(0.001, ura.distance, 100.0, 100.0);
	glutWireTorus(0.001, nep.distance, 100.0, 100.0);
	glPopMatrix();
}

GLuint sunTexture, merTexture, venTexture, earTexture, marTexture, jupTexture, satTexture, uraTexture, nepTexture, staTexture;

void setup(void) {
	staTexture = LoadTexture("stars.bmp");
	sunTexture = LoadTexture("sun.bmp");
	merTexture = LoadTexture("mercury.bmp");
	venTexture = LoadTexture("venus.bmp");
	earTexture = LoadTexture("earth.bmp");
	marTexture = LoadTexture("mars.bmp");
	jupTexture = LoadTexture("jupiter.bmp");
	satTexture = LoadTexture("saturn.bmp");
	uraTexture = LoadTexture("uranus.bmp");
	nepTexture = LoadTexture("neptune.bmp");
}

//Redimensionamento de janela
void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

//legenda
void renderbitmap(float x, float y, void* font, char* string) {
	char* c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

//legenda
void introscreen() {
	glColor3f(1.f, 1.f, 1.f);
	char buf[40] = { 0 };

	sprintf_s(buf, "Iluminacao: [I]");
	renderbitmap(-500, -2400, GLUT_BITMAP_TIMES_ROMAN_24, buf);  //1280 x 720 padrao
	sprintf_s(buf, "Zoom: - MOUSE(1)   + MOUSE(2)");
	renderbitmap(150, -2400, GLUT_BITMAP_TIMES_ROMAN_24, buf);  
	sprintf_s(buf, "Cameras: [1] [2] [3]");
	renderbitmap(300, -800, GLUT_BITMAP_TIMES_ROMAN_24, buf);  
	sprintf_s(buf, "Movimentacao: [W] [A] [S] [D]");
	renderbitmap(-500, -800, GLUT_BITMAP_TIMES_ROMAN_24, buf); 
	sprintf_s(buf, "Velocidades: [L]ow [F]ast");
	renderbitmap(-100, -800, GLUT_BITMAP_TIMES_ROMAN_24, buf); 

}


//Captar algumas entradas do teclado
void keyInput(unsigned char key, int x, int y) {
	switch (toupper(key)) {		//toupper para ativar key maiusculo/minusculo -> necessario ctype.h 
	case 27: exit(0); break;//Pressionar esc para fechar a janela
	case 'o': if (bigOrbitActive) bigOrbitActive = 0; else bigOrbitActive = 1; glutPostRedisplay(); break; //'o' ativa as órbitas 
	case '1': changeCamera = 0;  glutPostRedisplay(); break;//'1' câmera em perspectiva
	case '2': changeCamera = 1; glutPostRedisplay(); break;//'2' câmera lateral
	case '3': changeCamera = 2; glutPostRedisplay(); break;//'3' câmera superior
	case 'F': factor *= 2; glutPostRedisplay; break;//'f' dobra a velocidade de translação dos planetas
	case 'L': factor /= 2; glutPostRedisplay; break;//'s' divide por 2 a velocidade de translação dos planetas
	case 'I': luz1 = !luz1; glutPostRedisplay; break; //Ativa/Desativa luz 1
	case 'D': a += 5; glutPostRedisplay; break; // movimenta para a direita
	case 'A': a -= 5; glutPostRedisplay; break; // movimenta para a esquerda
	case 'W': b += 5; glutPostRedisplay; break; // movimenta para cima
	case 'S': b -= 5; glutPostRedisplay; break; // movimenta para baixo



	}
}

//Captar entradas do mouse
void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//Botão esquerdo do mouse aumenta o zoom
		zoom += 10;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)//Botão direito do mouse diminui o zoom
		zoom -= 10;
	glutPostRedisplay();
}






void get_bg()
{

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, staTexture);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex2d(-1.0, -1.0);
	glTexCoord2d(1.0, 0.0); glVertex2d(+1.0, -1.0);
	glTexCoord2d(1.0, 1.0); glVertex2d(+1.0, +1.0);
	glTexCoord2d(0.0, 1.0); glVertex2d(-1.0, +1.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
} 
void get_sun()
{
	glPushMatrix();
	glRotatef(mer.orbit / 25.0 * 360, 0.0, 0.0, -1.0);
	{
		GLfloat sun_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

		glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);
		if (luz0)
			glEnable(GL_LIGHT0);
		if (luz1)
			glEnable(GL_LIGHTING);
	}
	{
		GLfloat sun_mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat sun_mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat sun_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_mat_emission[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat sun_mat_shininess = 0.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);
	}


	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	gluQuadricDrawStyle(sphere, GLU_FILL);
	glBindTexture(GL_TEXTURE_2D, sunTexture);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, sun.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void get_mercu()
{
	glPushMatrix();
	glRotatef(mer.orbit, 0.0, 1.0, 0.0);
	glTranslatef(mer.distance, 0.0f, 0.0f);
	glPushMatrix();
	glRotatef(mer.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(mer.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	{
		GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_shininess = 0.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	}
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	gluQuadricDrawStyle(sphere, GLU_FILL);

	glBindTexture(GL_TEXTURE_2D, merTexture);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, mer.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

}
void get_ven()
{

	glPushMatrix();
	glTranslatef(2, 0.0f, 0.0f);
	glRotatef(1.5, 0.0f, 0.0f, -1.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(ven.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ven.distance, 0.0f, 0.0f);
	{
		GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_shininess = 0.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	}
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	gluQuadricDrawStyle(sphere, GLU_FILL);

	glBindTexture(GL_TEXTURE_2D, venTexture);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, ven.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}
void get_earth()
{

	glPushMatrix();
	glRotatef(ear.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ear.distance, 0.0f, 0.0f);
	glPushMatrix();
	glRotatef(ear.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ear.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	{
		GLfloat earth_mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat earth_mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat earth_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat earth_mat_shininess = 0.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);

	}
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	gluQuadricDrawStyle(sphere, GLU_FILL);

	glBindTexture(GL_TEXTURE_2D, earTexture);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, ear.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	lun.drawMoon(); //Desenha a lua
	if (!luz0)
		glDisable(GL_LIGHT0);
	if (!luz1)
		glDisable(GL_LIGHTING);
	glPopMatrix();
}
void get_mars()
{

	glPushMatrix();
	glTranslatef(2, 0.0f, 0.0f);
	glRotatef(1.5, 0.0f, 0.0f, -1.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(mar.orbit, 0.0, 1.0, 0.0);
	glTranslatef(mar.distance, 0.0f, 0.0f);
	{
		GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_shininess = 0.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	}
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	gluQuadricDrawStyle(sphere, GLU_FILL);

	glBindTexture(GL_TEXTURE_2D, marTexture);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, mar.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}
void get_jup()
{
	glPushMatrix();
	glTranslatef(2, 0.0f, 0.0f);
	glRotatef(180, 0.0f, 0.0f, -1.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(jup.orbit, 0.0, 1.0, 0.0);
	glTranslatef(jup.distance, 0.0f, 0.0f);

	{
		GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_shininess = 0.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	}
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	gluQuadricDrawStyle(sphere, GLU_FILL);

	glBindTexture(GL_TEXTURE_2D, jupTexture);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, jup.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}
void get_sat()
{
	glPushMatrix();
	glTranslatef(2, 0.0f, 0.0f);
	glRotatef(180, 0.0f, 0.0f, -1.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(sat.orbit, 0.0, 1.0, 0.0);
	glTranslatef(sat.distance, 0.0f, 0.0f);

	{
		GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_shininess = 0.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	}
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	gluQuadricDrawStyle(sphere, GLU_FILL);

	glBindTexture(GL_TEXTURE_2D, satTexture);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, sat.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glColor3ub(158, 145, 137);
	glRotatef(-63.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.2, 6.0, 30.0, 30.0);//Anéis de saturno
	glutWireTorus(0.4, 5.0, 30.0, 30.0);//Anéis de saturno
	glPopMatrix();
	glPopMatrix();

}
void get_ura()
{
	glPushMatrix();
	glTranslatef(2, 0.0f, 0.0f);
	glRotatef(180, 0.0f, 0.0f, -1.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(ura.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ura.distance, 0.0f, 0.0f);

	{
		GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_shininess = 0.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	}
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	gluQuadricDrawStyle(sphere, GLU_FILL);

	glBindTexture(GL_TEXTURE_2D, uraTexture);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, ura.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}
void get_nep()
{
	glPushMatrix();
	glTranslatef(2, 0.0f, 0.0f);
	glRotatef(120, 0.0f, 0.0f, -1.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(nep.orbit, 0.0, 1.0, 0.0);
	glTranslatef(nep.distance, 0.0f, 0.0f);

	{
		GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_shininess = 0.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	}
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	gluQuadricDrawStyle(sphere, GLU_FILL);

	glBindTexture(GL_TEXTURE_2D, nepTexture);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, nep.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

//Display Function
void myDisplay(void)

{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	get_bg();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(Angle, 1, 1, 10000000000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Diferentes posições da câmera
	if (changeCamera == 0)gluLookAt(0.0, zoom, 50.0, a, b, c, 0.0, 1.0, 0.0);
	if (changeCamera == 1)gluLookAt(0.0, 0.0, zoom, a, b, c, 0.0, 1.0, 0.0);
	if (changeCamera == 2)gluLookAt(0.0, zoom, 2, a, b, c, 0.0, 1.0, 0.0);
	if (bigOrbitActive == 1) orbitalTrails();
	get_sun();
	get_mercu();
	get_ven();
	get_earth();
	get_mars();
	get_jup();
	get_sat();
	get_ura();
	get_nep();

	//legenda - mostrar objetos apenas na camera1
	if (changeCamera == 0) {
		introscreen();
	} 

	glFlush();
	glutSwapBuffers();
}

//Translação dos planetas
void TimerFunction(int value) {
	mer.orbit += mer.orbitSpeed * factor;
	if (mer.orbit >= 360) mer.orbit = 0;
	ven.orbit += ven.orbitSpeed * factor;
	if (ven.orbit >= 360) ven.orbit = 0;
	ear.orbit += ear.orbitSpeed * factor;
	if (ear.orbit >= 360) ear.orbit = 0;
	mar.orbit += mar.orbitSpeed * factor;
	if (mar.orbit >= 360) mar.orbit = 0;
	jup.orbit += jup.orbitSpeed * factor;
	if (jup.orbit >= 360) jup.orbit = 0;
	sat.orbit += sat.orbitSpeed * factor;
	if (sat.orbit >= 360) sat.orbit = 0;
	ura.orbit += ura.orbitSpeed * factor;
	if (ura.orbit >= 360) ura.orbit = 0;
	nep.orbit += nep.orbitSpeed * factor;
	if (nep.orbit >= 360) nep.orbit = 0;
	lun.orbit += lun.orbitSpeed * factor;
	if (lun.orbit >= 360) lun.orbit = 0;

	mer.axisAni += 10.0;
	if (mer.axisAni > 360.0) mer.axisAni -= 360.0;
	ven.axisAni += 10.0;
	if (ven.axisAni > 360.0) ven.axisAni -= 360.0;
	ear.axisAni += 10.0;
	if (ear.axisAni > 360.0) ear.axisAni -= 360.0;
	mar.axisAni += 10.0;
	if (mar.axisAni > 360.0) mar.axisAni -= 360.0;
	jup.axisAni += 10.0;
	if (jup.axisAni > 360.0) jup.axisAni -= 360.0;
	sat.axisAni += 10.0;
	if (sat.axisAni > 360.0) sat.axisAni -= 360.0;
	ura.axisAni += 10.0;
	if (ura.axisAni > 360.0) ura.axisAni -= 360.0;
	nep.axisAni += 10.0;
	if (nep.axisAni > 360.0) nep.axisAni -= 360.0;

	glutPostRedisplay();
	glutTimerFunc(33, TimerFunction, 1);
}

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("Sistema Solar");
	setup();	
	glutDisplayFunc(&myDisplay);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseControl);
	glutKeyboardFunc(keyInput);
	glutTimerFunc(100, TimerFunction, 0);
	glutMainLoop();
}