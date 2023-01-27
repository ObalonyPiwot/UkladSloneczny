#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include "imageloader.h"
#include "Planeta.h"
#include "Ksiezyc.h"
#include <vector>
#pragma comment(lib, "glew32.lib")

using namespace std;


//Planety
/* Klasy planet*/
	Planeta sun;		//Sun
	Planeta mer;		//Mercury
	Planeta ven;		//Venus
	Planeta ear;		//Earth
	Planeta mar;		//Mars
	Planeta jup;		//Jupiter
	/* Klasy ksiezycow*/
	Ksiezyc lun;		//Moon     (Earth)
	Ksiezyc pho;		//Phobos   (Mars)
	Ksiezyc dei;		//Deimos   (Mars)
	Ksiezyc eur;		//Europa   (Jupiter)
	Ksiezyc gan;		//Ganymede (Jupiter)
	Ksiezyc cal;		//Callisto (Jupiter)
vector <Planeta*> ukladSloneczny;

int isAnimate = 0;
int bigOrbitActive = 1;
int smallOrbitActive = 1;
int moonsActive = 1;
int changeCamera = 0;
int frameCount = 0;

int zoom = 50;
int logoScene = 1;

float lightPos[] = { 0.0, 0.0, -75.0, 1.0 }; // Spotlight position.
static float spotAngle = 360; // Spotlight cone half-angle.
float spotDirection[] = { 1.0, 0.0, 0.0 }; // Spotlight direction.
static float spotExponent = 1.0; // Spotlight exponent = attenuation factor.

/**
* @brief zmienna przechowuj�ca teksture t�a
*/
GLuint  staTexture;

/**
	 * @brief Tworzy tekstur� z obrazu
	 * @param image Wska�nik na obiekt zawieraj�cy informacje o obrazie
	 * @return Identyfikator tekstury
	 */
GLuint loadTexture(Image* image) {
	GLuint textureId; // Identyfikator tekstury
	glGenTextures(1, &textureId); // Generowanie identyfikatora tekstury
	glBindTexture(GL_TEXTURE_2D, textureId); // Powi�zanie identyfikatora z aktualnie u�ywanym buforem tekstur
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels); // Tworzenie tekstury z obrazu
	return textureId;
};


/**
 * @brief Funkcja inicjuj�ca ustawienia graficzne oraz tworz�ca obiekty planet i ksi�yc�w.
 *
 * W tej funkcji ustawiana jest kolorystyka okna oraz w��czany jest test g��bi.
 * Nast�pnie w��czane s� tekstury oraz o�wietlenie.
 * Tworzone s� obiekty ksi�yc�w oraz planet, a nast�pnie dodawane do odpowiednich wektor�w.
 * Na ko�cu tworzony jest uk�ad s�oneczny z dodanymi planetami i ksi�ycami.
 */
void setup(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);


	//TEXUTRING SETUP
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	Image* sta = loadBMP("stars.bmp");        staTexture = loadTexture(sta);        delete sta;

	//LIGHTING SETUP
	glEnable(GL_LIGHTING);
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float globAmb[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);

	

	vector<Ksiezyc *> earthMoons;
	vector<Ksiezyc *> marsMoons;
	vector<Ksiezyc *> jupiterMoons;

	lun.initKsiezyc(.40, 3, 0, 5.44);	//Moon     (Earth)
	pho.initKsiezyc(.20, 1.8, 0,2.30);	//Phobos   (Mars)
	dei.initKsiezyc(.24, 2.4, 0, 3.6);	//Deimos   (Mars)
	eur.initKsiezyc(.24, 4, 0, 4.4);	//Europa   (Jupiter)
	gan.initKsiezyc(.24, 4.7, 0, 5.0);	//Ganymede (Jupiter)
	cal.initKsiezyc(.24, 5.3, 0, 2.3);	//Callisto (Jupiter)

	earthMoons.push_back(&lun);
	marsMoons.push_back(&pho);
	marsMoons.push_back(&dei);
	jupiterMoons.push_back(&eur);
	jupiterMoons.push_back(&gan);
	jupiterMoons.push_back(&cal);
	
	//Sun, Planets and Stars
	sun.initPlaneta(5.0, 0, 0, 0, 0, 0, "sun");				//Sun

	mer.initPlaneta(1.0, 7, 0, 1.74, 02.11, 0,  "mercury");		//Mercury
	ven.initPlaneta(1.5, 11, 0, 1.50, 177.0, 0,  "venus" );		//Venus

	ear.initPlaneta(2.0, 16, 0, 1.98, 23.44, 0,  "earth");
	ear.setMoons(earthMoons);		//Earth

	mar.initPlaneta(1.2, 21, 0, 1.41, 25.00, 0,  "mars");
	mar.setMoons(marsMoons);		//Mars

	jup.initPlaneta(3.5, 28, 0, 1.31, 03.13, 0,  "jupiter");
	jup.setMoons(jupiterMoons);		//Jupiter

	ukladSloneczny.push_back(&sun);
	ukladSloneczny.push_back(&mer);
	ukladSloneczny.push_back(&ven);
	ukladSloneczny.push_back(&ear);
	ukladSloneczny.push_back(&mar);
	ukladSloneczny.push_back(&jup);

}

/**
 * @brief Funkcja rysuj�ca �lady orbit planet w uk�adzie s�onecznym
 *
 * Funkcja rysuje �lady orbit planet w uk�adzie s�onecznym za pomoc� funkcji glutWireTorus.
 * Warto�ci przekazywane do tej funkcji: promie� wewn�trzny torusa (0.001), promie� zewn�trzny (odleg�o�� planety od S�o�ca), liczba punkt�w na torusie (100) oraz liczba segment�w torusa (100).
 * Przed rysowaniem �lad�w orbit, ustawiana jest kolor bia�y i wykonywane s� operacje translacji i rotacji.
 *
 */
void orbitalTrails(void){
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	for(int i = 0; i < ukladSloneczny.size();i++)
		if(i!=0)
			glutWireTorus(0.001, ukladSloneczny[i]->distance, 100.0, 100.0);
	glPopMatrix();
}


/**
 * @brief Rysowanie sceny
 *
 * Funkcja odpowiedzialna za rysowanie sceny, w kt�rej znajduj� si� obiekty Uk�adu S�onecznego.
 * W funkcji wywo�ywane s� kolejno:
 * - Czyszczenie bufor�w koloru i g��boko�ci
 * - Ustawienie macierzy jednostkowej
 * - Okre�lenie po�o�enia kamery zale�nie od zmiennej changeCamera
 * - Rysowanie �lad�w orbit zale�nie od zmiennej bigOrbitActive
 * - Rysowanie obiekt�w Uk�adu S�onecznego
 * - Zamiana bufor�w
 */
void drawScene(void){
	frameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (changeCamera == 0)gluLookAt(0.0, zoom, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if (changeCamera == 1)gluLookAt(0.0, 0.0, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if (changeCamera == 2)gluLookAt(0.0, zoom, 0.00001, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	if (bigOrbitActive == 1) orbitalTrails();

	GLUquadric *quadric;
	quadric = gluNewQuadric();


	for(int i = 0; i < ukladSloneczny.size(); i++)
	{
		if(i==0)
		{
			glDisable(GL_LIGHTING);
				ukladSloneczny[i]->draw();
			glEnable(GL_LIGHTING);
		}
		ukladSloneczny[i]->draw();
	}

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, staTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_POLYGON);
	glTexCoord2f(-1.0, 0.0); glVertex3f(-200, -200, -100);
	glTexCoord2f(2.0, 0.0); glVertex3f(200, -200, -100);
	glTexCoord2f(2.0, 2.0); glVertex3f(200, 200, -100);
	glTexCoord2f(-1.0, 2.0); glVertex3f(-200, 200, -100);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, staTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-200, -83, 200);
	glTexCoord2f(8.0, 0.0); glVertex3f(200, -83, 200);
	glTexCoord2f(8.0, 8.0); glVertex3f(200, -83, -200);
	glTexCoord2f(0.0, 8.0); glVertex3f(-200, -83, -200);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glutSwapBuffers();
}


/**
 * @brief Funkcja odpowiedzialna za zmian� rozmiaru okna
 *
 * Funkcja ta jest wywo�ywana przy zmianie rozmiaru okna. Ustawia widok na ca�e okno, ustawia tryb macierzy rzutowania na GL_PROJECTION,
 * wczytuje macierz jednostkow�, ustawia pola widzenia (frustum) i ustawia tryb macierzy modelowania na GL_MODELVIEW
 *
 * @param w szeroko�� okna
 * @param h wysoko�� okna
 */
void resize(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

/**
 * @brief Funkcja animuj�ca
 *
 * Funkcja ta jest wywo�ywana przez funkcj� glutTimerFunc,
 * i odpowiada za animacj� obiekt�w wirtualnego uk�adu s�onecznego.
 * Je�li zmienna isAnimate jest ustawiona na true, wywo�uje ona metod� animate()
 * dla ka�dego obiektu znajduj�cego si� w wektorze ukladSloneczny, a nast�pnie od�wie�a ekran.
 *
 * @param n parametr przekazywany przez funkcj� glutTimerFunc, zwykle jest to 1
 */
void animate(int n){

	if (isAnimate) {
		for(int i = 0; i < ukladSloneczny.size(); i++)
			ukladSloneczny[i]->animate();
		glutPostRedisplay();
		glutTimerFunc(30, animate, 1);
	}
}

/**
*  @brief  Funkcja obs�uguj�ca zdarzenie klikni�cia mysz�
*  @param  button - przycisk myszy, kt�ry zosta� klikni�ty
*  @param  state - stan przycisku myszy (wci�ni�ty/puszczony)
*  @param  x - pozycja x kursora myszy w momencie klikni�cia
*  @param  y - pozycja y kursora myszy w momencie klikni�cia
*/
void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		if (logoScene) logoScene = 0;

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) exit(0);

	glutPostRedisplay();
}

/**
 * @brief Funkcja obs�uguj�ca ruch k�kiem myszy
 *
 * @param wheel numer k�ka myszy
 * @param direction kierunek ruchu k�ka (1 - w g�r�, -1 - w d�)
 * @param x po�o�enie x kursora
 * @param y po�o�enie y kursora
 */
void mouseWheel(int wheel, int direction, int x, int y)
{
	if (direction > 0 && zoom < 100) zoom++;
	if (direction < 0 && zoom > -75) zoom--;
	glutPostRedisplay();
}

/**
 * @brief Funkcja obs�uguj�ca wciskanie przycisk�w na klawiaturze
 *
 * @param key - wci�ni�ty klawisz
 * @param x - pozycja kursora myszy w momencie wci�ni�cia klawisza na osi x
 * @param y - pozycja kursora myszy w momencie wci�ni�cia klawisza na osi y
 */
void keyInput(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // klawisz esc
		exit(0);
		break;
	case ' ': // spacja
		if (isAnimate)
			isAnimate = 0;
		else {
			isAnimate = 1;
			animate(1);
		}
		break;
	case 'o': // litera o
		if (bigOrbitActive)
			bigOrbitActive = 0;
		else
			bigOrbitActive = 1;
		glutPostRedisplay();
		break;
	case '1': changeCamera = 0; glutPostRedisplay(); break;
	case '2': changeCamera = 1; glutPostRedisplay(); break;
	case '3': changeCamera = 2; glutPostRedisplay(); break;
	}
}

int main(int argc, char **argv){
	glutInit(&argc, argv);

	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(500, 0);
	glutCreateWindow("Solar System");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutMouseFunc(mouseControl);
	glutKeyboardFunc(keyInput);
	glutMouseWheelFunc(mouseWheel);
	glewExperimental = GL_TRUE;
	glewInit();

	setup();
	glutMainLoop();
}