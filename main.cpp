#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <Magick++.h>

std::vector<double> getAverageBrightness(Magick::Image& image, int& columns, int& rows){
	std::vector<double> brightnessMap;
	brightnessMap.resize(columns*rows);

	double sum = 0;
	for(int i(0); i < columns; i++){
		for(int j(0); j < rows; j++){
			Magick::ColorRGB color = image.pixelColor(i, j);
			sum += color.red() + color.green() + color.blue();
			brightnessMap.at((columns*j + i)) = sum / 3;
			sum = 0;
		}
	} 
	return brightnessMap;
}

std::vector<char> getCharMap(std::vector<double> brightnessMap, int columns, int rows){
	std::vector<char> charMap;
	charMap.resize(columns*rows);

	for(int i(0); i < columns; i++){
		for(int j(0); j < rows; j++){
			if(brightnessMap[(j*columns + i)] <= 0.1) 	 charMap[(j*columns + i)] = '#';
			else if(brightnessMap[(j*columns + i)] <= 0.2) charMap[(j*columns + i)] = '@';
			else if(brightnessMap[(j*columns + i)] <= 0.3) charMap[(j*columns + i)] = '9';
			else if(brightnessMap[(j*columns + i)] <= 0.4) charMap[(j*columns + i)] = '&';
			else if(brightnessMap[(j*columns + i)] <= 0.5) charMap[(j*columns + i)] = '$';
			else if(brightnessMap[(j*columns + i)] <= 0.6) charMap[(j*columns + i)] = '7';
			else if(brightnessMap[(j*columns + i)] <= 0.7) charMap[(j*columns + i)] = '/';
			else if(brightnessMap[(j*columns + i)] <= 0.8) charMap[(j*columns + i)] = '=';
			else if(brightnessMap[(j*columns + i)] <= 0.8) charMap[(j*columns + i)] = '-';
			else if(brightnessMap[(j*columns + i)] <= 1.0) charMap[(j*columns + i)] = '.';
		}
	}
	return charMap;
}

void printArt(std::vector<char> charMap, int columns, int rows, std::string filename){
	std::ofstream fout;
	fout.open(std::string(filename+".txt"));
	for(int j(0); j < columns; j++){
		for(int i(0); i < rows; i++){
			fout << charMap[i*columns + j];
		}
		fout << std::endl;
	}
	fout.close();
}

int main(int argc, char** argv) {
	if(argc < 2){
		std::cout << "Please, enter filename" << std::endl;
		return 1;
	}
	Magick::InitializeMagick(*argv);
	std::string filename = argv[1]; // Getting filename

	Magick::Image image;
	image.read(filename);
	image.rotate(270);
	image.flip();
	int columns = image.columns();
	int rows = image.rows();

	auto brightnessMap = getAverageBrightness(image, columns, rows);
	auto charMap = getCharMap(brightnessMap, columns, rows);
	printArt(charMap, columns, rows, filename);

	return 0;
}