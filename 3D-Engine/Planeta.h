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
 * @brief Klasa reprezentuj¹ca planete
 */
class Planeta
{
public:
	/**
	* @brief Promieñ planety
	*/
	float radius;
	/**
	 * @brief Odleg³oœæ planety od centrum
	 */
	float distance;
	/**
	 * @brief K¹t obrotu planety wokó³ centrum
	 */
	float orbit;
	/**
	 * @brief Prêdkoœæ obrotu planety wokó³ centrum
	 */
	float orbitSpeed;
	/**
	 * @brief K¹t obrotu planety wokó³ w³asnej osi
	 */
	float axisAni;
	/**
	 * @brief K¹t Pochylenia osi planety wokó³ w³asnej osi
	 */
	float axisTilt;

	/**
	 * @brief Inicjalizuje parametry Planety
	 *
	 * @param _radius Promieñ planety
	 * @param _distance Odleg³oœæ planety od centrum
	 * @param _orbit K¹t obrotu planety wokó³ centrum
	 * @param _orbitSpeed Prêdkoœæ obrotu planety wokó³ centrum
	 * @param _axisTilt K¹t Pochylenia osi planety wokó³ w³asnej osi
	 * @param _axisAni  K¹t obrotu planety wokó³ w³asnej osi
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
	 * @brief Funkcja która na podstawie nazwy planety wczytuje teksture
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
	 * @brief Ustawia ksiê¿yce planety
	 *
	 * @param _moons wektor z ksiê¿ycami planety
	 */
    void setMoons(vector<Ksiezyc*> _moons){ moons = _moons;}


	/**

	 *@brief Funkcja rysuj¹ca tekst na ekranie
	 *Funkcja ta jest wykorzystywana do wyœwietlenia tekstu na ekranie.
	 *Przyjmuje ona jako argumenty: rodzaj czcionki, która ma byæ u¿yta oraz samego tekstu, który ma zostaæ wyœwietlony.
	 *@param font Rodzaj czcionki, która ma byæ u¿yta
	 *@param string Tekst, który ma zostaæ wyœwietlony
	*/
	void writeBitmapString(void* font, char* string)
	{
		char* c;
		for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
	}


	/**
	*
	*	@brief Funkcja rysuj¹ca obiekt
	*	Funkcja rysuj¹ca obiekt, który jest reprezentowany przez klasê.
	*	Rysowanie obejmuje rysowanie orbity, rysowanie tekstury oraz rysowanie ksiê¿yców.
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
	 * @brief Funkcja odpowiedzialna za animowanie ruchu obiektów.
	 */
	void animate()
	{
		// zwiêkszenie k¹ta orbity o wartoœæ orbitSpeed
		orbit += orbitSpeed;
		// jeœli k¹t orbity przekroczy 360 stopni, to jest on ustawiany na 0
		if (orbit >= 360)
			orbit = 0;
		// zwiêkszenie k¹ta obrotu wzglêdem osi
		axisAni += 10.0;
		// jeœli k¹t obrotu przekroczy 360 stopni, to jest on ustawiany na 0
		if (axisAni > 360.0)
			axisAni = 0;

		// iteracja po ksiê¿ycach i wywo³anie ich animowanie
		for (int i = 0; i < moons.size(); i++)
			moons[i]->animate();
	}

    private:
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
		 * @brief Tablica wskaŸników na obiekty klasy Ksiê¿yc
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

