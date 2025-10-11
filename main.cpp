#include "cgi_window.hpp"

cgi::window window("mywindowhello", 50, 50, 400, 400, cgi::color::rgb(90, 89, 78));

cgi::type::map2_t rectangle;
void start()
{

    rectangle.resize(400, cgi::type::map_t(600, '1'));

    window.create();
    window.show();
}

void update()
{
    window.clear();
    std::cout << window.fps() << '\n';
    window.draw_map2_t(window.get_cursor_x(), window.get_cursor_y(), rectangle, cgi::color::rgb(255, 0, 0));
}

int main()
{

    window.start_as(start);
    window.run_as(update, 10000000000000000);
}