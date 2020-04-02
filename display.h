#ifndef DISPLAY_H
#define DISPLAY_H
#include <string>
#include <SDL2/SDL.h>

class Display
{
    public:
        Display(int width, int height, const std::string& title);

        void Clear(float r, float g, float b, float a);
        void Update();
        bool isClosed();
        virtual ~Display();

    protected:

    private:
    Display(const Display& other){}
    void operator=(const Display& other){}
    // our window
    SDL_Window* m_window;
    // our context
    SDL_GLContext m_glContext;
    // to check event (polling)
    bool m_isClosed;
};

#endif // DISPLAY_H
