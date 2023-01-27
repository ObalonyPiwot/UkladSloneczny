#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include "imageloader.h"
#include <vector>
#pragma once

/**
 * @brief Klasa reprezentuj�ca ksi�yc
 */
class Ksiezyc
{
public:
    /**
     * @brief Promie� ksi�yca
     */
    float radius;
    /**
     * @brief Odleg�o�� ksi�yca od planety
     */
    float distance;
    /**
     * @brief K�t obrotu ksi�yca wok� planety
     */
    float orbit;
    /**
     * @brief Pr�dko�� obrotu ksi�yca wok� planety
     */
    float orbitSpeed;
    /**
     * @brief K�t obrotu ksi�yca wok� w�asnej osi
     */
    float axisAni;

    /**
     * @brief Inicjalizuje parametry ksi�yca
     *
     * @param _radius Promie� ksi�yca
     * @param _distance Odleg�o�� ksi�yca od planety
     * @param _orbit K�t obrotu ksi�yca wok� planety
     * @param _orbitSpeed Pr�dko�� obrotu ksi�yca wok� planety
     */
    void initKsiezyc(float _radius, float _distance, float _orbit, float _orbitSpeed) {
        radius = _radius;
        distance = _distance;
        orbit = _orbit;
        orbitSpeed = _orbitSpeed;
        axisAni = 0;
    }

    /**
     * @brief Rysuje kszta�t ksi�yca
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
     * @brief Rysuje ma�� orbit� ksi�yca
     */
    void drawSmallOrbit() {
        glPushMatrix();
        glColor3ub(255, 255, 255);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        glutWireTorus(0.001, distance, 100.0, 100.0);
        glPopMatrix();
    }

    /**
     * @brief Animuje ksi�yc
     */
    void animate()
    {
        orbit += orbitSpeed;
        if (orbit >= 360)
            orbit = 0;
    }
};


