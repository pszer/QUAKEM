#include "Render.hpp"

void SetColor(const SDL_Color& c);
void SetColor(const int r, const int g, const int b, const int a);

void Clear(const SDL_Color c = {0x00,0x00,0x00,0xff});
void Update();
