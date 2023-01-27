#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include "imageloader.h"
#include <vector>
#pragma once

/**
 * @brief Klasa reprezentuj¹ca ksiê¿yc
 */
class Ksiezyc
{
public:
    /**
     * @brief Promieñ ksiê¿yca
     */
    float radius;
    /**
     * @brief Odleg³oœæ ksiê¿yca od planety
     */
    float distance;
    /**
     * @brief K¹t obrotu ksiê¿yca wokó³ planety
     */
    float orbit;
    /**
     * @brief Prêdkoœæ obrotu ksiê¿yca wokó³ planety
     */
    float orbitSpeed;
    /**
     * @brief K¹t obrotu ksiê¿yca wokó³ w³asnej osi
     */
    float axisAni;

    /**
     * @brief Inicjalizuje parametry ksiê¿yca
     *
     * @param _radius Promieñ ksiê¿yca
     * @param _distance Odleg³oœæ ksiê¿yca od planety
     * @param _orbit K¹t obrotu ksiê¿yca wokó³ planety
     * @param _orbitSpeed Prêdkoœæ obrotu ksiê¿yca wokó³ planety
     */
    void initKsiezyc(float _radius, float _distance, float _orbit, float _orbitSpeed) {
        radius = _radius;
        distance = _distance;
        orbit = _orbit;
        orbitSpeed = _orbitSpeed;
        axisAni = 0;
    }

    /**
     * @brief Rysuje kszta³t ksiê¿yca
     */
    void drawMoon() {
        GLUquadricObj* quadric;
        quadric = gluNewQuadric();
        glPushMatrix();
        glColor3ub(255, 255, 255);
        glRotatef(orbit, 0.0, 1.0, 0.0);
        glTranslatef(distance, 0.0, 0.0);
        gluSphere(quadric, radius, 20.0, 20.0);
        glPopMatrix();
    }
    /**
     * @brief Rysuje ma³¹ orbitê ksiê¿yca
     */
    void drawSmallOrbit() {
        glPushMatrix();
        glColor3ub(255, 255, 255);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        glutWireTorus(0.001, distance, 100.0, 100.0);
        glPopMatrix();
    }

    /**
     * @brief Animuje ksiê¿yc
     */
    void animate()
    {
        orbit += orbitSpeed;
        if (orbit >= 360)
            orbit = 0;
    }
};


