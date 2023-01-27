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
* @brief zmienna przechowuj¹ca teksture t³a
*/
GLuint  staTexture;

/**
	 * @brief Tworzy teksturê z obrazu
	 * @param image WskaŸnik na obiekt zawieraj¹cy informacje o obrazie
	 * @return Identyfikator tekstury
	 */
GLuint loadTexture(Image* image) {
	GLuint textureId; // Identyfikator tekstury
	glGenTextures(1, &textureId); // Generowanie identyfikatora tekstury
	glBindTexture(GL_TEXTURE_2D, textureId); // Powi¹zanie identyfikatora z aktualnie u¿ywanym buforem tekstur
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels); // Tworzenie tekstury z obrazu
	return textureId;
};


/**
 * @brief Funkcja inicjuj¹ca ustawienia graficzne oraz tworz¹ca obiekty planet i ksiê¿yców.
 *
 * W tej funkcji ustawiana jest kolorystyka okna oraz w³¹czany jest test g³êbi.
 * Nastêpnie w³¹czane s¹ tekstury oraz oœwietlenie.
 * Tworzone s¹ obiekty ksiê¿yców oraz planet, a nastêpnie dodawane do odpowiednich wektorów.
 * Na koñcu tworzony jest uk³ad s³oneczny z dodanymi planetami i ksiê¿ycami.
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
 * @brief Funkcja rysuj¹ca œlady orbit planet w uk³adzie s³onecznym
 *
 * Funkcja rysuje œlady orbit planet w uk³adzie s³onecznym za pomoc¹ funkcji glutWireTorus.
 * Wartoœci przekazywane do tej funkcji: promieñ wewnêtrzny torusa (0.001), promieñ zewnêtrzny (odleg³oœæ planety od S³oñca), liczba punktów na torusie (100) oraz liczba segmentów torusa (100).
 * Przed rysowaniem œladów orbit, ustawiana jest kolor bia³y i wykonywane s¹ operacje translacji i rotacji.
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
 * Funkcja odpowiedzialna za rysowanie sceny, w której znajduj¹ siê obiekty Uk³adu S³onecznego.
 * W funkcji wywo³ywane s¹ kolejno:
 * - Czyszczenie buforów koloru i g³êbokoœci
 * - Ustawienie macierzy jednostkowej
 * - Okreœlenie po³o¿enia kamery zale¿nie od zmiennej changeCamera
 * - Rysowanie œladów orbit zale¿nie od zmiennej bigOrbitActive
 * - Rysowanie obiektów Uk³adu S³onecznego
 * - Zamiana buforów
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
 * @brief Funkcja odpowiedzialna za zmianê rozmiaru okna
 *
 * Funkcja ta jest wywo³ywana przy zmianie rozmiaru okna. Ustawia widok na ca³e okno, ustawia tryb macierzy rzutowania na GL_PROJECTION,
 * wczytuje macierz jednostkow¹, ustawia pola widzenia (frustum) i ustawia tryb macierzy modelowania na GL_MODELVIEW
 *
 * @param w szerokoœæ okna
 * @param h wysokoœæ okna
 */
void resize(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

/**
 * @brief Funkcja animuj¹ca
 *
 * Funkcja ta jest wywo³ywana przez funkcjê glutTimerFunc,
 * i odpowiada za animacjê obiektów wirtualnego uk³adu s³onecznego.
 * Jeœli zmienna isAnimate jest ustawiona na true, wywo³uje ona metodê animate()
 * dla ka¿dego obiektu znajduj¹cego siê w wektorze ukladSloneczny, a nastêpnie odœwie¿a ekran.
 *
 * @param n parametr przekazywany przez funkcjê glutTimerFunc, zwykle jest to 1
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
*  @brief  Funkcja obs³uguj¹ca zdarzenie klikniêcia mysz¹
*  @param  button - przycisk myszy, który zosta³ klikniêty
*  @param  state - stan przycisku myszy (wciœniêty/puszczony)
*  @param  x - pozycja x kursora myszy w momencie klikniêcia
*  @param  y - pozycja y kursora myszy w momencie klikniêcia
*/
void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		if (logoScene) logoScene = 0;

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) exit(0);

	glutPostRedisplay();
}

/**
 * @brief Funkcja obs³uguj¹ca ruch kó³kiem myszy
 *
 * @param wheel numer kó³ka myszy
 * @param direction kierunek ruchu kó³ka (1 - w górê, -1 - w dó³)
 * @param x po³o¿enie x kursora
 * @param y po³o¿enie y kursora
 */
void mouseWheel(int wheel, int direction, int x, int y)
{
	if (direction > 0 && zoom < 100) zoom++;
	if (direction < 0 && zoom > -75) zoom--;
	glutPostRedisplay();
}

/**
 * @brief Funkcja obs³uguj¹ca wciskanie przycisków na klawiaturze
 *
 * @param key - wciœniêty klawisz
 * @param x - pozycja kursora myszy w momencie wciœniêcia klawisza na osi x
 * @param y - pozycja kursora myszy w momencie wciœniêcia klawisza na osi y
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