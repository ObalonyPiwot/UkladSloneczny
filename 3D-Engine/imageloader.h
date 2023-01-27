#ifndef IMAGE_LOADER_H_INCLUDED
#define IMAGE_LOADER_H_INCLUDED

/**
 * @class Image
 * Klasa reprezentuj�ca obraz.
 */
class Image {
public:
	/**
	 * @brief Konstruktor klasy Image
	 * @param ps - wska�nik na tablic� reprezentuj�c� obraz w formacie RGB
	 * @param w - szeroko�� obrazu
	 * @param h - wysoko�� obrazu
	 */
	Image(char* ps, int w, int h);
	/**
	 * @brief Destruktor klasy Image
	 */
	~Image();

	/**
 * @file
 * Plik zawiera zmienn� przechowuj�c� tablic� RGB reprezentuj�c� obraz oraz zmienne przechowuj�ce szeroko�� i wysoko�� obrazu.
 * @var pixels
 * Tablica w postaci (R1, G1, B1, R2, G2, B2, ...) okre�laj�ca kolor ka�dego piksela obrazu. Sk�adowe koloru zawarte s� w zakresie od 0 do 255.
 * Tablica rozpoczyna si� od lewego dolnego rogu, nast�pnie przesuwa si� w prawo do ko�ca rz�du, nast�pnie przesuwa si� w g�r� do nast�pnej kolumny i tak dalej. Format ten jest lubiany przez OpenGL.
 * @var width
 * Zmienna przechowuj�ca szeroko�� obrazu.
 * @var height
 * Zmienna przechowuj�ca wysoko�� obrazu.
 */
	char* pixels;
	int width;
	int height;

};

/**
 * @brief Funkcja wczytuj�ca obraz w formacie BMP z pliku.
 * @param filename - nazwa pliku z obrazem
 * @return Wska�nik na obiekt klasy Image reprezentuj�cy wczytany obraz
 */
Image* loadBMP(const char* filename);


#endif
