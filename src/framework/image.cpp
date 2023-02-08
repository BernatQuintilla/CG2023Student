#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GL/glew.h"
#include "../extra/picopng.h"
#include "image.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"

Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

// Copy constructor
Image::Image(const Image& c)
{
	pixels = NULL;
	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
}

// Assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;

	if(c.pixels)
	{
		pixels = new Color[width*height*bytes_per_pixel];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}

void Image::Render()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(width, height, bytes_per_pixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

// Change image size (the old one will remain in the top-left corner)
void Image::Resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = GetPixel(x,y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

// Change image size and scale the content
void Image::Scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = GetPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++x)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.SetPixel( x, y, GetPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::FlipY()
{
	int row_size = bytes_per_pixel * width;
	Uint8* temp_row = new Uint8[row_size];
#pragma omp simd
	for (int y = 0; y < height * 0.5; y += 1)
	{
		Uint8* pos = (Uint8*)pixels + y * row_size;
		memcpy(temp_row, pos, row_size);
		Uint8* pos2 = (Uint8*)pixels + (height - y - 1) * row_size;
		memcpy(pos, pos2, row_size);
		memcpy(pos2, temp_row, row_size);
	}
	delete[] temp_row;
}

bool Image::LoadPNG(const char* filename, bool flip_y)
{
	std::string sfullPath = absResPath(filename);
	std::ifstream file(sfullPath, std::ios::in | std::ios::binary | std::ios::ate);

	// Get filesize
	std::streamsize size = 0;
	if (file.seekg(0, std::ios::end).good()) size = file.tellg();
	if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	if (!size)
		return false;

	std::vector<unsigned char> buffer;

	// Read contents of the file into the vector
	if (size > 0)
	{
		buffer.resize((size_t)size);
		file.read((char*)(&buffer[0]), size);
	}
	else
		buffer.clear();

	std::vector<unsigned char> out_image;

	if (decodePNG(out_image, width, height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size(), true) != 0)
		return false;

	size_t bufferSize = out_image.size();
	unsigned int originalBytesPerPixel = (unsigned int)bufferSize / (width * height);
	
	// Force 3 channels
	bytes_per_pixel = 3;

	if (originalBytesPerPixel == 3) {
		pixels = new Color[bufferSize];
		memcpy(pixels, &out_image[0], bufferSize);
	}
	else if (originalBytesPerPixel == 4) {

		unsigned int newBufferSize = width * height * bytes_per_pixel;
		pixels = new Color[newBufferSize];

		unsigned int k = 0;
		for (unsigned int i = 0; i < bufferSize; i += originalBytesPerPixel) {
			pixels[k] = Color(out_image[i], out_image[i + 1], out_image[i + 2]);
			k++;
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	return true;
}

// Loads an image from a TGA file
bool Image::LoadTGA(const char* filename, bool flip_y)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int imageSize;
	unsigned int bytesPerPixel;

    std::string sfullPath = absResPath( filename );

	FILE * file = fopen( sfullPath.c_str(), "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << sfullPath.c_str() << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		fclose(file);
		delete tgainfo;
		return NULL;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	// Save info in image
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	// Convert to float all pixels
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			// Make sure we don't access out of memory
			if( (pos < imageSize) && (pos + 1 < imageSize) && (pos + 2 < imageSize))
				SetPixel(x, height - y - 1, Color(tgainfo->data[pos + 2], tgainfo->data[pos + 1], tgainfo->data[pos]));
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::SaveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	std::string fullPath = absResPath(filename);
	FILE *file = fopen(fullPath.c_str(), "wb");
	if ( file == NULL )
	{
		perror("Failed to open file: ");
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	// Convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[y*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);

	return true;
}

void Image::DrawRect(int x, int y, int w, int h, const Color& c)
{
	for (int i = 0; i < w; ++i) {
		SetPixel(x + i, y, c);
		SetPixel(x + i, y + h, c);
	}

	for (int j = 0; j < h; ++j) {
		SetPixel(x, y + j, c);
		SetPixel(x + w, y + j, c);
	}
}

#ifndef IGNORE_LAMBDAS

// You can apply and algorithm for two images and store the result in the first one
// ForEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void ForEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif

FloatImage::FloatImage(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new float[width * height];
	memset(pixels, 0, width * height * sizeof(float));
}

// Copy constructor
FloatImage::FloatImage(const FloatImage& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
}

// Assign operator
FloatImage& FloatImage::operator = (const FloatImage& c)
{
	if (pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height * sizeof(float)];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
	return *this;
}

FloatImage::~FloatImage()
{
	if (pixels)
		delete pixels;
}

// Change image size (the old one will remain in the top-left corner)
void FloatImage::Resize(unsigned int width, unsigned int height)
{
	float* new_pixels = new float[width * height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for (unsigned int x = 0; x < min_width; ++x)
		for (unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * width + x] = GetPixel(x, y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}
void Image::DrawLineDDA(int x0, int y0, int x1, int y1, const Color& c)
{
	float dx, dy;
	dx = x1 - x0;
	dy = y1 - y0;
	float d = std::max(abs(dx), abs(dy));
	float vy = dx / d;
	float vx = dy / d;
	float x = x0;
	float y = y0;
	for (int i = 0; i < d; i++) {
		SetPixel(floor(x), floor(y), c);
		x = x + vx;
		y = y + vy;
	}
}
void Image::DrawLineBresenham(int x0, int y0, int x1, int y1, const Color& c)
{
	float inc_E, inc_NE, dx, dy, d, ay;
	int x, y, sx, sy, fx, fy;
	sx = x0;
	sy = y0;
	fx = x1;
	fy = y1;
	if (x0 > x1) {
		sx = x1;
		sy = y1;
		fx = x0;
		fy = y0;
	}
	dx = fx - sx;
	dy = fy - sy;
	x = sx;
	y = sy;
	ay = dy;
	ay = abs(ay);
	SetPixel(x, y, c);
	if (dx > ay) {
		inc_E = 2 * ay;
		inc_NE = 2 * (ay - dx);
		d = 2 * ay - dx;
		while (fx > x) {
			if (d <= 0) {
				d = d + inc_E;
				x++;
			}
			else {
				d = d + inc_NE;
				x++;
				if (dy < 0) {
					y--;
				}
				else {
					y++;
				}
			}
			SetPixel(x, y, c);
		}
	}
	else {
		inc_E = 2 * dx;
		inc_NE = 2 * (dx - ay);
		d = 2 * (dx - ay);
		while (y != fy) {
			if (d > 0) {
				d = d - inc_E;
				if (dy < 0) {
					y--;
				}
				else {
					y++;
				}
			}
			else {
				d = d - inc_NE;
				x++;
				if (dy < 0) {
					y--;
				}
				else {
					y++;
				}
			}
			SetPixel(x, y, c);
		}
	}

	//first octant
	/*int dx, dy, inc_E, inc_NE, d, x, y, ye, xv, yv, xve;
	dx = x1 - x0;
	dy = y1 - y0;
	inc_E = 2 * dy;
	inc_NE = 2 * (dy - dx);
	d = 2 * dy - dx;
	x = x0;
	y = y0;
	ye = y0;
	xv = x0;
	yv = y0;
	xve = x0;
	SetPixel(x, y, c);
	while (x < x1) {
		if (d <= 0) { //1
			d = d + inc_E;
			x = x + 1;
			yv = yv + 1;//2
			ye = ye - 1;//8
			xve = xve - 1;//3
		}
		else {
			d = d + inc_NE;
			x = x + 1;
			y = y + 1;
			yv = yv + 1;
			xv = xv + 1;
		}

		SetPixel(x, y, c);//1
		SetPixel(x, ye, c);//8
		SetPixel(xv, yv, c);//2
		SetPixel(xve, yv, c);//3
		
	}
	x1 = x1-2*dx;
	x = x0;
	y = y0;
	ye = y0;
	xv = x0;
	yv = y0;
	xve = x0;
	d= 2 * dy - dx;
	while (x > x1) {
		if (d <= 0) { //4
			d = d + inc_E;
			x = x - 1;
			yv = yv - 1;//7
			ye = ye - 1; //5
			xve = xve - 1;
		}
		else {
			d = d + inc_NE;
			x = x - 1;
			y = y + 1;
			yv = yv - 1;
			xv = xv + 1;
		}
		SetPixel(x, y, c);//4
		SetPixel(x, ye, c);//5
		SetPixel(xv, yv, c);//7
		SetPixel(xve, yv, c);//6
	}*/
}
void Image::DrawCircle(int x0, int y0, int r, const Color& c, bool fill) {
	int x = 0, y = r;
	int v = 1 - r;
	while (y >= x) {
		SetPixel(x0 + x, y0 + y, c);
		SetPixel(x0 + y, y0 + x, c);
		SetPixel(x0 - x, y0 + y, c);
		SetPixel(x0 - y, y0 + x, c);
		SetPixel(x0 + x, y0 - y, c);
		SetPixel(x0 + y, y0 - x, c);
		SetPixel(x0 - x, y0 - y, c);
		SetPixel(x0 - y, y0 - x, c);
		if (v < 0) {
			v += 2 * x + 3;
		}
		else {
			v += 2 * (x - y) + 5;
			y--;
		}
		x++;
		if (fill == true) {
			for (int i = x0 - x;i <= x0 + x;i++){
				SetPixel(i, y0 + y, c);
				SetPixel(i, y0 - y, c);
			}
			for (int i = x0 - y;i <= x0 + y;i++) {
				SetPixel(i, y0 + x, c);
				SetPixel(i, y0 - x, c);
			}
			for (int i = x0 - r;i <= x0 + r;i++) { //Arreglo para pintar l�nea horizontal central del c�rculo
				SetPixel(i, y0, c);
			}
		}
	}
}
void Image::DrawImagePixels(const Image& image, bool top) {
	//pasar pixeles de im a framebuffer
	Image im = image;
	Color c;
	int cont;
	int x_max = im.width;
	int y_max = im.height;
	if (top == true) {
		for (int i = 0;i < x_max;i++) {
			cont = 0;
			for (int j = y_max;j > 0;j--) {
				c = im.GetPixel(i, j);
				SetPixel(i, height - cont, c);
				cont++;
			}
		}
	}
	else {
		for (int i = 0; i < x_max; i++) {
			for (int j = 0; j < y_max; j++) {
				c = im.GetPixel(i, j);
				SetPixel(i, j, c);
			}
		}
	}
}