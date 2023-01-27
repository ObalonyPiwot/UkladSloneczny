#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include "imageloader.h"
#include <vector>
#include <string>
#include "Ksiezyc.h"
#pragma comment(lib, "glew32.lib")
#pragma once
#pragma warning(disable:4996)


using namespace std;

/**
 * @brief Klasa reprezentuj�ca planete
 */
class Planeta
{
public:
	/**
	* @brief Promie� planety
	*/
	float radius;
	/**
	 * @brief Odleg�o�� planety od centrum
	 */
	float distance;
	/**
	 * @brief K�t obrotu planety wok� centrum
	 */
	float orbit;
	/**
	 * @brief Pr�dko�� obrotu planety wok� centrum
	 */
	float orbitSpeed;
	/**
	 * @brief K�t obrotu planety wok� w�asnej osi
	 */
	float axisAni;
	/**
	 * @brief K�t Pochylenia osi planety wok� w�asnej osi
	 */
	float axisTilt;

	/**
	 * @brief Inicjalizuje parametry Planety
	 *
	 * @param _radius Promie� planety
	 * @param _distance Odleg�o�� planety od centrum
	 * @param _orbit K�t obrotu planety wok� centrum
	 * @param _orbitSpeed Pr�dko�� obrotu planety wok� centrum
	 * @param _axisTilt K�t Pochylenia osi planety wok� w�asnej osi
	 * @param _axisAni  K�t obrotu planety wok� w�asnej osi
	 * @param _nameOfPlanet Nazwa Planey
	 */
	void initPlaneta(float _radius, float _distance, float _orbit, float _orbitSpeed, float _axisTilt, float _axisAni, char* _nameOfPlanet) {
		radius = _radius;
		distance = _distance;
		orbit = _orbit;
		orbitSpeed = _orbitSpeed;
		axisTilt = _axisTilt;
		axisAni = _axisAni;
		nameOfPlanet = _nameOfPlanet;
		setTexture();
	}

	/**
	 * @brief Funkcja kt�ra na podstawie nazwy planety wczytuje teksture
	 *
	 */
	void setTexture(){
		string filename = nameOfPlanet;
		filename.append(".bmp");
		char* myCharArray = new char[filename.length() + 1];
		strcpy(myCharArray, filename.c_str());
        Image* Ista = loadBMP(myCharArray);		tex = loadTexture(Ista);		delete Ista;
    };

	/**
	 * @brief Ustawia ksi�yce planety
	 *
	 * @param _moons wektor z ksi�ycami planety
	 */
    void setMoons(vector<Ksiezyc*> _moons){ moons = _moons;}


	/**

	 *@brief Funkcja rysuj�ca tekst na ekranie
	 *Funkcja ta jest wykorzystywana do wy�wietlenia tekstu na ekranie.
	 *Przyjmuje ona jako argumenty: rodzaj czcionki, kt�ra ma by� u�yta oraz samego tekstu, kt�ry ma zosta� wy�wietlony.
	 *@param font Rodzaj czcionki, kt�ra ma by� u�yta
	 *@param string Tekst, kt�ry ma zosta� wy�wietlony
	*/
	void writeBitmapString(void* font, char* string)
	{
		char* c;
		for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
	}


	/**
	*
	*	@brief Funkcja rysuj�ca obiekt
	*	Funkcja rysuj�ca obiekt, kt�ry jest reprezentowany przez klas�.
	*	Rysowanie obejmuje rysowanie orbity, rysowanie tekstury oraz rysowanie ksi�yc�w.
	*	@return void
	*/
	void draw(void) {
		GLUquadricObj* quadric;
		quadric = gluNewQuadric();
		glPushMatrix();
		glRotatef(orbit, 0.0, 1.0, 0.0);
		glRotatef(orbit, 0.0, 1.0, 0.0);
		glTranslatef(distance, 0.0, 0.0);

		glRasterPos3f(-1.2, 7.0, 0.0);
		glColor3ub(255, 255, 255);

		writeBitmapString(GLUT_BITMAP_HELVETICA_12, nameOfPlanet);


		glPushMatrix();
		glRotatef(axisTilt, 1.0, 0.0, 0.0);
		glRotatef(axisAni, 0.0, 1.0, 0.0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluQuadricTexture(quadric, 1);
		gluSphere(quadric, radius, 20.0, 20.0);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

        for(int i=0; i< moons.size();i++)
        {
			//cout << nameOfPlanet << endl;
            moons[i]->drawSmallOrbit();
            moons[i]->drawMoon();
        }
            
		glPopMatrix();
	}

	/**
	 * @brief Funkcja odpowiedzialna za animowanie ruchu obiekt�w.
	 */
	void animate()
	{
		// zwi�kszenie k�ta orbity o warto�� orbitSpeed
		orbit += orbitSpeed;
		// je�li k�t orbity przekroczy 360 stopni, to jest on ustawiany na 0
		if (orbit >= 360)
			orbit = 0;
		// zwi�kszenie k�ta obrotu wzgl�dem osi
		axisAni += 10.0;
		// je�li k�t obrotu przekroczy 360 stopni, to jest on ustawiany na 0
		if (axisAni > 360.0)
			axisAni = 0;

		// iteracja po ksi�ycach i wywo�anie ich animowanie
		for (int i = 0; i < moons.size(); i++)
			moons[i]->animate();
	}

    private:
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
		 * @brief Tablica wska�nik�w na obiekty klasy Ksi�yc
		 */
		vector<Ksiezyc*> moons;

		/**
		 * @brief Nazwa planety
		 */
		char* nameOfPlanet;

		/**
		 * @brief Identyfikator tekstury
		 */
		GLuint tex;
};

