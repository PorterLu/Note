#include<string.h>
#include<SDL/SDL.h>


SDL_Surface *load_image(char* filename)
{
	SDL_Surface* loadedImage = NULL	;
	SDL_Surface* optimizedImage = NULL;
	loadedImage = SDL_LoadBMP(filename);//加载Bmp文件
	if(loadedImage != NULL)
	{
		optimizedImage = SDL_DisplayFormat(loadedImage);//创建一个与窗口有相同格式的新图像,使其粘贴到另一个表明不用再转换格式
		SDL_FreeSurface( loadedImage);
	}
	return optimizedImage;

}

void apply_surface( int x, int y, SDL_Surface *source, SDL_Surface *destination)
{
	SDL_Rect offset;//坐标信息
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(source,NULL,destination,&offset);
}

int main(void)
{
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	const int SCREEN_BPP = 32; //位像素
	
	SDL_Surface *message = NULL;
	SDL_Surface *background = NULL;
	SDL_Surface *screen = NULL;
	

	if(SDL_Init( SDL_INIT_EVERYTHING) == -1)
	{
		return -1;
	}

	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	if( screen == NULL )
	{
		return 1;
	}

	SDL_WM_SetCaption("Hello World", NULL);

	message = load_image("hello.bmp");
	background = load_image("background.bmp");
	//sdl的坐标原点在左上角
	apply_surface(0,0,background,screen);
	apply_surface(320,0,background,screen);
	apply_surface(320,240,background,screen);
	apply_surface(0,240,background,screen);

	apply_surface(160,120,message,screen);

	if(SDL_Flip(screen) == -1 )
	{
		return 1;
	}

	SDL_Delay(2000);
	SDL_FreeSurface( message );
	SDL_FreeSurface( background );

	SDL_Quit();
	return 0;
}
