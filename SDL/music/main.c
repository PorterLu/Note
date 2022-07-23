#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

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

SDL_Color textColor = {0,0,0};

Mix_Music *music = NULL;

Mix_Chunk *scratch = NULL;
Mix_Chunk *high = NULL;
Mix_Chunk *med = NULL;
Mix_Chunk *low = NULL;

SDL_Surface *load_image(char *filename)
{
    SDL_Surface *loadedImage = NULL;

    SDL_Surface *optimizedImage = NULL;

    loadedImage = IMG_Load( filename );

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );

        SDL_FreeSurface( loadedImage );

        if( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format,0,0xff,0xff));


        }
    }

    return optimizedImage;
}

void apply_surface( int x,int y, SDL_Surface* source, SDL_Surface *destination, SDL_Rect *clip)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, clip, destination, &offset);
}

bool init()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

    if( screen == NULL)
    {
        return false;
    }

    if( TTF_Init() == -1 )
    {
        return false;
    }

    /*the first parameter of this function is the sound frequency we use,
    in this lesson, it is set to 22050,which is also recommended value.The 
    second parameter is sound format.We use the default format.The third para-
    meter is the number of channel we plan to use, we set it to 2 to get stereo
    sound.If it is set to 0, there will be only mono sound.The last parameter is
    the sample size,which is set to 4096.
    */

    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        return false;
    }

    SDL_WM_SetCaption( "Monitor Music", NULL);

    return true;
}

bool load_files()
{
    background = load_image( "background.png" );

    font = TTF_OpenFont( "lazy.ttf", 30 );

    if( background == NULL )
    {
        return false;
    }

    if( font == NULL )
    {
        return false;
    }

    music = Mix_LoadMUS( "beat.wav" );

    if(music == NULL )
    {
        return false;
    }

    scratch = Mix_LoadWAV( "scratch.wav" );
    high = Mix_LoadWAV( "high.wav" );
    med = Mix_LoadWAV( "medium.wav" );
    low = Mix_LoadWAV( "low.wav" );

    if( (scratch == NULL) || (high == NULL )|| (med == NULL)|| (low == NULL))
    {
        return false;
    }

    return true;
}

void clean_up()
{
    SDL_FreeSurface ( background );
    Mix_FreeChunk( scratch );
    Mix_FreeChunk( high );
    Mix_FreeChunk( med );
    Mix_FreeChunk( low );

    Mix_FreeMusic( music );

    TTF_CloseFont( font );

    Mix_CloseAudio();

    TTF_Quit();

    SDL_Quit();
}

int main(int argc, char* argv[])
{
    bool quit = false;

    if( init() == false )
    {
        return 1;
    }

    if( load_files() == false )
    {
        return 1;
    }

    apply_surface( 0, 0, background, screen, NULL);

    message = TTF_RenderText_Solid( font, "Press 1,2,3, or 4 to play a sound effect", textColor);

    if( message == NULL )
    {
        return 1;
    }

    apply_surface( ( SCREEN_WIDTH - message->w ) / 2, 200, message, screen, NULL);

    SDL_FreeSurface( message );

    message = TTF_RenderText_Solid( font, "Press 0 to Stop", textColor);

    if( message == NULL )
    {
        return 1;
    }

    apply_surface( ( SCREEN_WIDTH - message->w ) / 2, 300, message, screen, NULL);

    SDL_FreeSurface( message );

    if( SDL_Flip( screen ) == -1 )
    {
        return 1;
    }


    /* the first parameter of Mix_PlayChannel is the mix Channel where we are going to play the sound. Since it is set to -1, this
        function will automatically find first available channel and play the sound. The second parameter is the mix_sound to be played.
        the third parameter is the number of times the sound will be repeated. Since it is set to 0, it will play only once.
    */
    while( quit == false )
    {
        while( SDL_PollEvent( &event ))
        {
            if( event.type == SDL_KEYDOWN )
            {
                if( event.key.keysym.sym == SDLK_1 )
                {
                    if( Mix_PlayChannel(-1, scratch, 0) == -1 )
                    {
                        return 1;
                    }
                }
                
            }
            else if( event.key.keysym.sym == SDLK_2 )
            {
                //Play the high hit effect
                if( Mix_PlayChannel( -1, high, 0 ) == -1 )
                {
                    return 1;
                }
            }
            //If 3 was pressed
            else if( event.key.keysym.sym == SDLK_3 )
            {
                //Play the medium hit effect
                if( Mix_PlayChannel( -1, med, 0 ) == -1 )
                {
                    return 1;
                }
            }
            //If 4 was pressed
            else if( event.key.keysym.sym == SDLK_4 )
            {
                //Play the low hit effect
                if( Mix_PlayChannel( -1, low, 0 ) == -1 )
                {
                    return 1;
                }
            }
            //If 9 was pressed
            else if( event.key.keysym.sym == SDLK_9 )
            {
                //If there is no music playing
                if( Mix_PlayingMusic() == 0 )
                {
                    //Play the music
                    if( Mix_PlayMusic( music, -1 ) == -1 )
                    {
                        return 1;
                    }
                }
                //If music is being played
                else
                {
                    //If the music is paused
                    if( Mix_PausedMusic() == 1 )
                    {
                        //Resume the music
                        Mix_ResumeMusic();
                    }
                    //If the music is playing
                    else
                    {
                        //Pause the music
                        Mix_PauseMusic();
                    }
                }
            }
            else if( event.key.keysym.sym == SDLK_0 )
            {
                //Stop the music
                Mix_HaltMusic();
            }

        }

        if( event.type == SDL_QUIT )
        {
            quit = true;
        }
    }

    clean_up();

    return 0;
}
