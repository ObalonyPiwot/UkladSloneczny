#ifndef IMAGE_LOADER_H_INCLUDED
#define IMAGE_LOADER_H_INCLUDED

/**
 * @class Image
 * Klasa reprezentuj¹ca obraz.
 */
class Image {
public:
	/**
	 * @brief Konstruktor klasy Image
	 * @param ps - wskaŸnik na tablicê reprezentuj¹c¹ obraz w formacie RGB
	 * @param w - szerokoœæ obrazu
	 * @param h - wysokoœæ obrazu
	 */
	Image(char* ps, int w, int h);
	/**
	 * @brief Destruktor klasy Image
	 */
	~Image();

	/**
 * @file
 * Plik zawiera zmienn¹ przechowuj¹c¹ tablicê RGB reprezentuj¹c¹ obraz oraz zmienne przechowuj¹ce szerokoœæ i wysokoœæ obrazu.
 * @var pixels
 * Tablica w postaci (R1, G1, B1, R2, G2, B2, ...) okreœlaj¹ca kolor ka¿dego piksela obrazu. Sk³adowe koloru zawarte s¹ w zakresie od 0 do 255.
 * Tablica rozpoczyna siê od lewego dolnego rogu, nastêpnie przesuwa siê w prawo do koñca rzêdu, nastêpnie przesuwa siê w górê do nastêpnej kolumny i tak dalej. Format ten jest lubiany przez OpenGL.
 * @var width
 * Zmienna przechowuj¹ca szerokoœæ obrazu.
 * @var height
 * Zmienna przechowuj¹ca wysokoœæ obrazu.
 */
	char* pixels;
	int width;
	int height;

};

/**
 * @brief Funkcja wczytuj¹ca obraz w formacie BMP z pliku.
 * @param filename - nazwa pliku z obrazem
 * @return WskaŸnik na obiekt klasy Image reprezentuj¹cy wczytany obraz
 */
Image* loadBMP(const char* filename);


#endif
