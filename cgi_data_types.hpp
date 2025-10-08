#ifndef DATA_TYPES_HPP
#define DATA_TYPES_HPP

#include "cgi_includes.hpp"

namespace cgi
{
    namespace type
    {
        
        using refresh_t = cgi::values::REFRESH_TYPE;

        using color_t = COLORREF;
        using cursor_t = HCURSOR;
        using rgb_t = cgi::type::color_t;
    }

    namespace color
    {
        inline static cgi::type::color_t rgb(int r, int g, int b)
        {
            return RGB(r, g, b);
        }

        inline static std::vector<int> parse_rgb(cgi::type::color_t color)
        {
            std::vector<int> rgb_vec;

            rgb_vec.push_back(GetRValue(color));
            rgb_vec.push_back(GetGValue(color));
            rgb_vec.push_back(GetBValue(color));

            return rgb_vec;
        }

        inline int parse_red(cgi::type::color_t color)
        {
            return GetRValue(color);
        }

        inline int parse_blue(cgi::type::color_t color)
        {
            return GetBValue(color);
        }

        inline int parse_green(cgi::type::color_t color)
        {
            return GetGValue(color);
        }
    }
}

namespace cgi
{
    namespace type
    {
        using buf_color_t = std::vector<cgi::type::color_t>;
        using buf2_color_t = std::vector<cgi::type::buf_color_t>;
        using base_map_t = char;
        using map_t = std::vector<cgi::type::base_map_t>;
        using map2_t = std::vector<std::vector<char>>;
        

        class rgba_t
        {
            int r = 0;
            int g = 0;
            int b = 0;
            float a = 1;

        public:
            rgba_t() : r(0), g(0), b(0), a(0) {};

            rgba_t(int red, int green, int blue, float alpha = 1)
            {
                if (alpha > 1)
                {
                    alpha = 1;
                }
                this->r = red;
                this->g = green;
                this->b = blue;
                this->a = alpha;
                return;
            }

            rgba_t(cgi::type::color_t color, float alpha = 1)
            {
                if (alpha > 1)
                {
                    alpha = 1;
                }
                std::vector<int> vec = cgi::color::parse_rgb(color);
                this->r = vec[0];
                this->g = vec[1];
                this->b = vec[2];
                this->a = 1;
                return;
            }

            const int &red() const
            {
                return this->r;
            }

            const int &green() const
            {
                return this->g;
            }

            const int &blue() const
            {
                return this->b;
            }

            const float &alpha()
            {
                return this->a;
            }

            inline cgi::type::color_t value()
            {
                return RGB(r * a, g * a, b * a);
            }
        };

        using buf_rgba_t = std::vector<cgi::type::rgba_t>;
        using buf2_rgba_t = std::vector<cgi::type::buf_rgba_t>;
    }

}


#endif
