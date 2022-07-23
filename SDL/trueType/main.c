#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_ttf.h>

#define bool int
#define true 1
#define false 0


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_Surface *background = NULL;
SDL_Surface *message = NULL;
SDL_Surface *screen = NULL;

SDL_Event event;

TTF_Font *font = NULL;

SDL_Color textColor = {255, 255, 255};

SDL_Surface *load_image( char* filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}

	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	if(screen == NULL)
	{
		return false;
	}

	if( TTF_Init() == -1)
	{
		return false;
	}

	SDL_WM_SetCaption( "TTF Test", NULL);

	return true;

}

bool load_files()
{
	background = load_image("background.png");

	font = TTF_OpenFont( "lazy.ttf",28);

	if( background == NULL || font == NULL)
	{
		return false;
	}

	return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( background );
    SDL_FreeSurface( message );

    //Close the font that was used
    TTF_CloseFont( font );

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}


int main(void)
{

	init();
	load_files();
	message = TTF_RenderText_Solid( font ,"The quick brownfox jumps over the lazy dog", textColor);

	apply_surface( 0, 0, background, screen, NULL);
	apply_surface( 0, 150, message, screen, NULL);

	SDL_Flip(screen);

	bool quit = false;

    	while( quit == false )
    	{
        //While there's events to handle
    	    while( SDL_PollEvent( &event ) )
        	{
            	//If the user has Xed out the window
            	if( event.type == SDL_QUIT )
            	{
                	//Quit the program
                	quit = true;
            	}
        	}
    	}

    //Free surfaces and font then quit SDL_ttf and SDL
    	clean_up();

    	return 0;
}
