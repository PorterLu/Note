#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define true 1
#define false 0
#define bool int
const int SCREEN_HEIGHT = 640;
const int SCREEN_WIDTH = 480;
const int SCREEN_BPP = 32;

SDL_Surface *image = NULL ;
SDL_Surface *screen = NULL;
SDL_Event event;

SDL_Surface *load_image(char* filename)
{
        SDL_Surface* loadedImage = NULL ;
        SDL_Surface* optimizedImage = NULL;
        loadedImage = IMG_Load(filename);//加载Bmp文件
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
bool init()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}

	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	if(screen == NULL)
		return false;

	SDL_WM_SetCaption("Event",NULL);
	
	return true;

}

bool load_files()
{
	image = load_image("x.png");
	if(image == NULL)
	{
		return false;
	}

	return true;
}


void clean_up()
{
	SDL_FreeSurface( image );
	SDL_Quit();
}

int main(void)
{
	bool quit = false;
	if(init() == false )
		return 1;

	if(load_files() == false)
		return 1;

	apply_surface(0,0,image,screen);

	if(SDL_Flip(screen) == -1)
		return 1;

	while(quit == false)//SDL中每发生一个时间，它都会被置于事件池中，SDL_PollEvent()会从队列头取出一个时间
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				quit = true;
		}
	}

	clean_up();
	return 0;

}
