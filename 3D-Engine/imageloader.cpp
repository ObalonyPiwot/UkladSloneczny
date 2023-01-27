#include <assert.h>
#include <fstream>
#include "imageloader.h"

using namespace std;

Image::Image(char* ps, int w, int h) : pixels(ps), width(w), height(h) {

}

Image::~Image() {
	delete[] pixels;
}

namespace {
	/**
	* @brief Funkcja zamieniaj�ca tablic� bajt�w na liczb� typu int
	* @param bytes - tablica bajt�w do zamiany
	* @return Liczba typu int
	*/
	int toInt(const char* bytes) {
		return (int)(((unsigned char)bytes[3] << 24) |
			((unsigned char)bytes[2] << 16) |
			((unsigned char)bytes[1] << 8) |
			(unsigned char)bytes[0]);
	}

	/**
	 * @brief Funkcja zamieniaj�ca tablic� bajt�w na liczb� typu short
	 * @param bytes - tablica bajt�w do zamiany
	 * @return Liczba typu short
	 */
	short toShort(const char* bytes) {
		return (short)(((unsigned char)bytes[1] << 8) |
			(unsigned char)bytes[0]);
	}

	/**
	 * @brief Funkcja odczytuj�ca liczb� typu int z strumienia danych
	 * @param input - strumie� danych
	 * @return Odczytana liczba typu int
	 */
	int readInt(ifstream& input) {
		char buffer[4];
		input.read(buffer, 4);
		return toInt(buffer);
	}

	/**
	 * @brief Funkcja odczytuj�ca liczb� typu short z strumienia danych
	 * @param input - strumie� danych
	 * @return Odczytana liczba typu short
	 */
	short readShort(ifstream& input) {
		char buffer[2];
		input.read(buffer, 2);
		return toShort(buffer);
	}

	/**
 * @brief Klasa auto_array jest narz�dziem do automatycznego zarz�dzania tablic�.
 *
 * Klasa ta pozwala na automatyczne zarz�dzanie tablic�, tzn. gdy obiekt tej klasy jest usuwany,
 * tablica jest automatycznie usuwana.
 *
 * @tparam T typ element�w tablicy
 */
	template<class T>
	class auto_array {
	private:
		T* array;
		mutable bool isReleased;
	public:
		/**
		 * @brief Konstruktor
		 *
		 * Tworzy obiekt klasy auto_array z tablic� podan� jako parametr.
		 *
		 * @param array_ wska�nik na tablic�, kt�r� ma zarz�dza� obiekt
		 */
		explicit auto_array(T* array_ = NULL) :
			array(array_), isReleased(false) {
		}

		/**
		 * @brief Konstruktor kopiuj�cy
		 *
		 * Tworzy kopi� obiektu klasy auto_array.
		 *
		 * @param aarray obiekt, kt�ry ma zosta� skopiowany
		 */
		auto_array(const auto_array<T>& aarray) {
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}

		/**
		 * @brief Destruktor
		 *
		 * Usuwa tablic�, je�li obiekt nie zosta� ju� wcze�niej zwolniony.
		 */
		~auto_array() {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
		}

		/**
		 * @brief Zwraca wska�nik na tablic�
		 *
		 * @return wska�nik na tablic�
		 */
		T* get() const {
			return array;
		}

		/**
		 * @brief Dereferencja
		 *
		 * @return referencja do pierwszego elementu tablicy
		 */
		T& operator*() const {
			return *array;
		}

		/**
	 * @brief Operator przypisania
	 *
	 * Przypisuje tablic� z obiektu aarray do obiektu, na kt�rym jest wywo�ywany.
	 *
	 * @param aarray Obiekt auto_array, z kt�rego tablica ma zosta� przypisana
	 */
		void operator=(const auto_array<T>& aarray) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}

		/**
		 * @brief Operator strza�ki
		 *
		 * @return T* Wska�nik na tablic�
		 */
		T* operator->() const {
			return array;
		}

		/**
 * @brief Uwalnia pami�� przydzielon� dla tablicy
 *
 * Ustawia flag� isReleased na true, co oznacza, �e pami�� zosta�a ju� zwolniona.
 *
 * @return Wska�nik na tablic�
 */
		T* release() {
			isReleased = true;
			return array;
		}

		/**
		 * @brief Resetuje tablic� do warto�ci pocz�tkowej
		 *
		 * Je�eli flaga isReleased jest ustawiona na false oraz wska�nik array jest r�ny od NULL,
		 * to zwalnia pami�� przydzielon� dla tablicy.
		 * Nast�pnie ustawia wska�nik array na podany argument lub na NULL.
		 *
		 * @param array_ Wska�nik na tablic� lub NULL
		 */
		void reset(T* array_ = NULL) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = array_;
		}

		/**
		 * @brief Operator dodawania indeksu
		 *
		 * Zwraca wska�nik na element tablicy o indeksie array+i.
		 *
		 * @param i Indeks elementu
		 * @return Wska�nik na element tablicy o indeksie array+i
		 */
		T* operator+(int i) {
			return array + i;
		}

		/**
		 * @brief Operator indeksowania
		 *
		 * Zwraca referencj� do elementu tablicy o indeksie i.
		 *
		 * @param i Indeks elementu
		 * @return Referencja do elementu tablicy o indeksie i
		 */
		T& operator[](int i) {
			return array[i];
		}
	};
}

/**
 * @brief Wczytuje plik BMP z podanej �cie�ki
 *
 * @param filename �cie�ka do pliku BMP
 * @return Image* wska�nik na obiekt zawieraj�cy dane obrazu
 */
Image* loadBMP(const char* filename) {
	ifstream input;
	input.open(filename, ifstream::binary);
	assert(!input.fail() || !"Could not find file");
	char buffer[2];
	input.read(buffer, 2);
	assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Not a bitmap file");
	input.ignore(8);
	int dataOffset = readInt(input);


	int headerSize = readInt(input);
	int width;
	int height;
	switch (headerSize) {
	case 40:
		//V3
		width = readInt(input);
		height = readInt(input);
		input.ignore(2);
		assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
		assert(readShort(input) == 0 || !"Image is compressed");
		break;
	case 12:
		//OS/2 V1
		width = readShort(input);
		height = readShort(input);
		input.ignore(2);
		assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
		break;
	case 64:
		//OS/2 V2
		assert(!"Can't load OS/2 V2 bitmaps");
		break;
	case 108:
		//Windows V4
		assert(!"Can't load Windows V4 bitmaps");
		break;
	case 124:
		//Windows V5
		assert(!"Can't load Windows V5 bitmaps");
		break;
	default:
		assert(!"Unknown bitmap format");
	}


	int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
	int size = bytesPerRow * height;
	auto_array<char> pixels(new char[size]);
	input.seekg(dataOffset, ios_base::beg);
	input.read(pixels.get(), size);


	auto_array<char> pixels2(new char[width * height * 3]);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < 3; c++) {
				pixels2[3 * (width * y + x) + c] =
					pixels[bytesPerRow * y + 3 * x + (2 - c)];
			}
		}
	}

	input.close();
	return new Image(pixels2.release(), width, height);
}