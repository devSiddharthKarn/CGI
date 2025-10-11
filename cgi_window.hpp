// =============================================================
//  CGI - C++ Graphics Ingine
//  Simple. Effective. Elegant.
//  Copyright (c) 2025 Siddharth Karn
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file in the project root for full license information.
// =============================================================




#include "cgi_data_types.hpp"
#include "cgi_std_font_loader.hpp"
#include "cgi_system_utils.hpp"
#include <chrono>
#include <thread>
#include "cgi_console.hpp"

namespace cgi
{

    // inline cgi::type::color_t rgb(int red, int green, int blue)
    // {
    //     return RGB(red, green, blue);
    // }

    /// @brief common window_structure used for managing windows internals. May not need unless trying to utilize the system directly
    struct cgi_window_struct
    {
        const char *class_name = "main_class";
        int x_pos = 0;
        int y_pos = 0;
        int width = 0;
        int height = 0;
        cgi::type::color_t color = cgi::type::rgba_t(255, 255, 255).value();
        cgi::type::buf_color_t buffer;

        WNDCLASSA wc = {};
        HWND hwnd = nullptr;
        HDC window_dc = nullptr;
        HDC window_mem_dc = nullptr;
        BITMAPINFO bmi = {};
        HBITMAP hbmi = nullptr;
        void *pixels_1D = nullptr;
        DWORD *pixel = nullptr;
        PAINTSTRUCT ps;

        float scroll_x = 0;
        float scroll_y = 0;
        double threshold_frame_period = 0;
        std::chrono::steady_clock::time_point last_frame_time;
        double frame_period = 0;

        uint32_t style = 0;
        uint32_t extended_style = 0;


    };

    /// @brief common windows procedure directly pointing to the interal message handler.
    /// @param  HWND for the window
    /// @param  UINT say message from the window
    /// @param  WPARAM received from window
    /// @param  LPARAM received from window
    /// @return returns windows LRESULT
    LRESULT CALLBACK cgi_windows_procedure(HWND, UINT, WPARAM, LPARAM);

    // void make_bmi(long int, long int);
    // inline void load_view();
    // inline void display_view(HDC);
    // void cleanup();


    
    /// @brief use this class to create a window . write cgi::window window_name for window creation
    class window
    {
    private:
        const char *name;

        

        // flags
        bool open = false;
        bool created = false;
        bool have_binded_console = false;
        bool manual_flags = false;

        bool first_log = true;

        bool resized = false;
        public:
        cgi_window_struct details;
        private:
        // cgi::window * parent_window=nullptr;

        /// @brief functions
        inline void val_reset()
        {
            /// @brief here values that fluctuates per frame are reset. Like the logic if the window is resized or not or window is being scrolled or not is reset every frame as these messages are instantaneous in nature.

            this->details.scroll_x = 0;
            this->details.scroll_y = 0;

            this->resized = false;
            // this->first_log=false;

            return;
        }

        void make_bmi(long int width, long int height)
        {
            cleanup();

            this->details.height = height;
            this->details.width = width;

            // making bmi
            this->details.bmi = {};
            this->details.bmi.bmiHeader.biBitCount = 32;
            this->details.bmi.bmiHeader.biCompression = BI_RGB;
            this->details.bmi.bmiHeader.biHeight = -this->details.height;
            this->details.bmi.bmiHeader.biPlanes = 1;
            this->details.bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            this->details.bmi.bmiHeader.biWidth = this->details.width;

            this->details.window_dc = GetDC(this->details.hwnd);
            this->details.pixels_1D = nullptr;

            this->details.hbmi = CreateDIBSection(this->details.window_dc, &this->details.bmi, DIB_RGB_COLORS, &this->details.pixels_1D, nullptr, 0);

            this->details.pixel = (DWORD *)this->details.pixels_1D;

            this->details.window_mem_dc = CreateCompatibleDC(this->details.window_dc);
            SelectObject(this->details.window_mem_dc, this->details.hbmi);

            return;
        }

        inline void load_view() noexcept
        {
            // load

            cgi::type::color_t color;
            for (int i = 0; i < this->get_buffer_height(); i++)
            {
                for (int j = 0; j < this->get_buffer_width(); j++)
                {
                    color = this->details.buffer[i * this->details.width + j];
                    this->details.pixel[i * this->details.width + j] = cgi::color::rgb(cgi::color::parse_blue(color), cgi::color::parse_green(color), cgi::color::parse_red(color));
                }
            }
        }

        inline void display_view(HDC draw_dc) noexcept
        {
            // display

            BitBlt(draw_dc, 0, 0, this->details.width, this->details.height, this->details.window_mem_dc, 0, 0, SRCCOPY);
        }

        void cleanup() noexcept
        {
            if (this->details.hbmi)
            {
                DeleteObject(this->details.hbmi);
                this->details.hbmi = nullptr;
            }

            if (this->details.hwnd && this->details.window_dc)
            {
                ReleaseDC(this->details.hwnd, this->details.window_dc);
                this->details.window_dc = nullptr;
            }

            if (this->details.window_mem_dc)
            {
                DeleteDC(this->details.window_mem_dc);
                this->details.window_mem_dc = nullptr;
            }

            return;
        }

    public:
        window(const char* name, int x_pos, int y_pos, int width, int height, cgi::type::color_t color)
        {
            this->name = name;
            this->details.x_pos = x_pos;
            this->details.y_pos = y_pos;
            this->details.width = width;
            this->details.height = height;
            this->details.color = color;

            // this->details.buffer.clear();
            // this->details.buffer.resize(this->details.height,std::vector<cgi::type::color_t>(this->details.width,this->details.color));

            details.wc = {};
            details.wc.hInstance = GetModuleHandle(nullptr);
            details.wc.lpfnWndProc = cgi::cgi_windows_procedure;
            details.wc.lpszClassName = details.class_name;
            details.wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
            details.wc.style = CS_HREDRAW | CS_VREDRAW;

            RegisterClassA(&details.wc);
            return;
        }

        /// @brief checks if a window is open
        /// @return gives true or false according to the present state
        inline bool is_open() noexcept
        {
            return this->open;
        }

        /// @brief used to close a running windows created using cgi::window
        void close() noexcept
        {
            if (!this->open)
            {
                std::cout << "window is not open . Cannot close it " << std::endl;
            }
            this->open = false;
            if (this->details.hwnd)
            {
                SendMessage(this->details.hwnd, WM_CLOSE, 0, 0);

                // SendMessage(this->details.hwnd,WM_DESTROY,0,0);
            }
            return;
        }

        /// @brief used to get the base color (say the color the window was created with)
        /// @return returns the initial color the window was created with (type: cgi::type::color_t)
        inline cgi::type::color_t base_color() noexcept
        {
            return this->details.color;
        }

        /// @brief used to get the width of window
        /// @return return current width of window in pixels
        inline long int get_width() noexcept
        {
            RECT rect = {};
            GetWindowRect(this->details.hwnd, &rect);
            return (rect.right - rect.left);
        }

        /// @brief used to set the width of window i.e. the whole window width (including borders added by windowsOS)
        /// @param width takes width in pixels as a only param
        /// @return returns true if  success and false if not
        inline bool set_width(long int width) noexcept {
            if(this->created  || this->open){
                std::cout<<"cannot set width after window is created"<<std::endl;
                return false;
            }else{
                this->details.width = width;
                return true;
            }
        }

        /// @brief gets the buffer width of the window i.e. the client area width
        /// @return return the width of the client area or buffer area of window in pixels
        inline long int get_buffer_width() noexcept
        {
            RECT rect_drawable = {};
            GetClientRect(this->details.hwnd, &rect_drawable);
            return (rect_drawable.right - rect_drawable.left);
        }

        /// @brief gets the total height of window including the default borders added by windows OS
        /// @return returns the height of the window including the borders in pixels
        inline long int get_height() noexcept
        {
            RECT rect = {};
            GetWindowRect(this->details.hwnd, &rect);
            return (rect.bottom - rect.top);
        }

        /// @brief used to set the height of the window 
        /// @param height takes height in pixel as param
        /// @return true if success false if not success to set height
        inline bool set_height(long int height) noexcept {

            if(this->created || this->open){
                std::cout<<"cannot set height after window is created"<<std::endl;
                return false;
            }else{
                this->details.height= height;
                return true;
            }    
        }



            

        /// @brief used to get the height of the client area of buffer area of window
        /// @return returns the height of the buffer area or client area in pixels
        inline long int get_buffer_height() noexcept
        {
            RECT rect_drawable = {};
            GetClientRect(this->details.hwnd, &rect_drawable);
            return (rect_drawable.bottom - rect_drawable.top);
        }

        /// @brief direectly return the pointer to the internal buffer of the window. address pointed by the pointer changes on some events like resizing  so always check before using
        /// @return nullptr if no buffer present or the address  if present
        inline cgi::type::buf_color_t *get_buffer() noexcept
        {
            if (this->is_open())
            {
                return &this->details.buffer;
            }
            else
            {
                return nullptr;
            }
        }

        /// @brief used to get the x position of windows in pixels from the top left corner of screen
        /// @return returns the x position of window in pixels from your top left cornet
        inline const int &get_x_pos() noexcept
        {
            return this->details.x_pos;
        }

        /// @brief used to get the y position of windows in pixels from the top left corner of screen
        /// @return returns the y position of window in pixels from your top left cornet
        inline const int &get_y_pos() noexcept
        {
            return this->details.y_pos;
        }

        /// @brief makes ready for the window to show for the first time or first log
        inline void show() noexcept
        {

            if (!this->created)
            {
                std::cout << "cannot show an uncreated window" << this->name << " Please create one " << std::endl;
                return;
            }

            if (this->open)
            {
                std::cout << "window " << this->name << ' ' << "is already being shown " << std::endl;
                return;
            }

            ShowWindow(this->details.hwnd, SW_SHOW);

            this->details.buffer.clear();
            this->details.buffer.resize(this->get_buffer_height() * this->get_buffer_width(), this->details.color);

            UpdateWindow(this->details.hwnd);
            // std::cout<<"here";

            this->open = true;
            this->first_log = false;
        }

        /// @brief used to manually set some of the custom window styles before creation and before calling .create() on cgi::window object, if skipped the cgi library directly adds a custom default style to the window
        /// @param resizable true if window should be resizable or false if not
        /// @param title true if window have title or false if not (some windows still show title after passing false which is commonly a windows OS issue)
        /// @param sys_menu true if it need to have the system menu like the close button maximize button or minimize button otherwise false
        /// @param minimizable true if windows can be minimizable in sys_menu otherwise false
        /// @param maximizable true if windows can be maximizavle in sys_menu otherwise false
        /// @param border true if window should have a border otherwise false(may or may not show border,due to windows OS issue on some systems)
        inline void set_flags(bool resizable, bool title, bool sys_menu, bool minimizable, bool maximizable, bool border)
        {

            // uint32_t composition = 0;

            this->manual_flags=true;

            if (resizable)
                this->details.style |= WS_THICKFRAME;
            if (title)
                this->details.style |= WS_CAPTION;
            if (sys_menu)
                this->details.style |= WS_SYSMENU;
            if (minimizable)
                this->details.style |= WS_MINIMIZEBOX;  
            if (maximizable)
                this->details.style |= WS_MAXIMIZEBOX; 
            if (border)
                this->details.style |= WS_BORDER;
        
            


        }

        /// @brief call when everything is properly set up and window is  now ready to be created
        /// @param __require_console_window true if you need console window binded with your cgi::window object,false if not
        void create(bool __require_console_window = true)
        {
            if (__require_console_window && !this->have_binded_console)
            {
                cgi::get_console();
            }

            if (this->details.hwnd != nullptr && this->open)
            {
                std::cout << "cannot create same window more than one time  " << std::endl;
                return;
            }

            if(manual_flags)
            this->details.hwnd = CreateWindowExA(this->details.extended_style, details.class_name, this->name, this->details.style, details.x_pos, details.y_pos, details.width, details.height, nullptr, nullptr, details.wc.hInstance, this);
            else 
            this->details.hwnd = CreateWindowExA(this->details.extended_style, details.class_name, this->name, WS_OVERLAPPEDWINDOW, details.x_pos, details.y_pos, details.width, details.height, nullptr, nullptr, details.wc.hInstance, this);


            if (!this->details.hwnd)
            {
                std::cout << "error creating window " << std::endl;
                return;
            }

            // this->open = true;
            this->created = true;
            return;
        }

        /// @brief call when you want to clear the window with a particular color(does not change the base color)
        /// @param clear_color the cgi::type::color_t value for the clear_color
        /// @return returns true if properly cleared the window otherwise false
        inline bool clear(cgi::type::color_t clear_color)
        {
            try
            {

                std::fill(this->details.buffer.begin(), this->details.buffer.end(), clear_color);

                return true;
            }
            catch (...)
            {
                return false;
            }
            // return;
        }

        /// @brief call when you want to clear the window with the default base color of window
        /// @return returns true if properly cleared the window otherwise false
        inline bool clear() 
        {

            try
            {

                std::fill(this->details.buffer.begin(), this->details.buffer.end(), this->details.color);
                return true;
            }
            catch (...)
            {
                return false;
            }
        }

        /// @brief used to manually set or manipulate the pixel color in window
        /// @param x_pos x position where you want to set from top left corner of your screen
        /// @param y_pos y position where you want to set from top left corner of your screen
        /// @param color cgi::type::rgba_t value that you want to set
        inline void set_pixel(int x_pos, int y_pos, cgi::type::rgba_t color)
        {

            if (x_pos < 0 || x_pos >= this->details.width || y_pos < 0 || y_pos >= this->details.height)
            {
                return;
            }

            cgi::type::color_t pix = this->get_pixel(x_pos, y_pos);

            // std::vector<int> pix_val = cgi::color::parse_rgb(this->get_pixel(x_pos, y_pos));

            int r = color.red() * color.alpha() + cgi::color::parse_red(pix) * (1 - color.alpha());

            int g = color.green() * color.alpha() + cgi::color::parse_green(pix) * (1 - color.alpha());
            int b = color.blue() * color.alpha() + cgi::color::parse_blue(pix) * (1 - color.alpha());

            this->details.buffer[y_pos * this->get_buffer_width() + x_pos] = cgi::color::rgb(r, g, b);

            // std::cout<<r<<' '<<g<<' '<<b<<'\n';

            return;
        }

        /// @brief used to set the pixel of window to a particular color
        /// @param x_pos x position where you want to set from top left corner of your screen
        /// @param y_pos y position where you want to set from top left corner of your screen
        /// @param color_rgb color in rgb format with type cgi::type::color_t
        /// @param alpha alpha channel value for the opacity of color from 0 to 1
        inline void set_pixel(int x_pos, int y_pos, cgi::type::color_t color_rgb,float alpha=1.0)
        {

            if(x_pos<0 || x_pos>=this->details.width || y_pos<0 || y_pos>=this->details.height)
            return;
            

            cgi::type::color_t pix = this->get_pixel(x_pos, y_pos);

            int r = cgi::color::parse_red(color_rgb) * alpha + cgi::color::parse_red(pix) * (1 - alpha);
            int g = cgi::color::parse_green(color_rgb) * alpha + cgi::color::parse_green(pix) * (1 - alpha);
            int b = cgi::color::parse_blue(color_rgb) * alpha + cgi::color::parse_blue(pix) * (1 - alpha);

            this->details.buffer[y_pos * this->get_buffer_width() + x_pos] = cgi::color::rgb(r, g, b);


            // set_pixel(x_pos, y_pos, cgi::type::rgba_t(color_rgb, 1));

            return;
        }

        /// @brief used to get the pixel of color at a definite point of the window
        /// @param x_pos x position where you want to get the pixel from top left corner of your screen
        /// @param y_pos y position where you want to get the pixel from top left corner of your screen
        /// @return 
        inline cgi::type::color_t get_pixel(int x_pos, int y_pos)
        {
            if (y_pos < 0 || y_pos >= this->details.height || x_pos < 0 || x_pos >= this->details.width)
            {
                return this->details.color;
            }

            return this->details.buffer[y_pos * this->get_buffer_width() + x_pos];
        }

        /// @brief used to draw a cgi::type::buf2_color_t object in window
        /// @param x_pos x position from where the drawing should begin with respect to window's top left corner
        /// @param y_pos y position from where the drawing should begin with respect to window's top left corner
        /// @param buffer cgi::type::buf2_color_t object that you want to draw
        /// @param alpha alpha channel for opacity from 0 to 1
        inline void draw_buf2_color_t(int x_pos, int y_pos, const cgi::type::buf2_color_t &buffer, float alpha = 1.0)
        {

            int r, g, b;
            int py, px;

            for (int i = 0; i < buffer.size(); i++)
            {
                py = y_pos + i;

                if (py < 0 || py >= this->details.height)
                    continue;

                for (int j = 0; j < buffer[i].size(); j++)
                {
                    px = x_pos + j;

                    if (px < 0 || px >= this->details.width)
                        continue;

                    r = cgi::color::parse_red(buffer[i][j]);
                    g = cgi::color::parse_green(buffer[i][j]);
                    b = cgi::color::parse_blue(buffer[i][j]);

                    set_pixel(px, py, cgi::type::rgba_t(r, g, b, alpha));
                }
            }

            // InvalidateRect(this->details.hwnd,nullptr,TRUE);
            // UpdateWindow(this->details.hwnd);

            return;
        }

        /// @brief used to draw an object of type cgi::type::map2_t to the window directly
        /// @param x_pos x position from where the drawing should begin with respect to window's top left corner
        /// @param y_pos  position from where the drawing should begin with respect to window's top left corner
        /// @param map cgi::type::map2_t object that you want to draw
        /// @param color color to set where there is value
        /// @param bg_color color to set where there is not value(optional)
        inline void draw_map2_t(int x_pos, int y_pos, const cgi::type::map2_t &map, cgi::type::rgba_t color, std::optional<cgi::type::rgba_t> bg_color = std::nullopt)
        {

            int size_i = map.size();
            int py, px;
            for (int i = 0; i < size_i; i++)
            {
                py = y_pos + i;

                if (py < 0 || py >= this->details.height)
                    continue;

                int size_j = map[i].size();

                for (int j = 0; j < size_j; j++)
                {
                    px = x_pos + j;

                    if (px < 0 || px >= this->details.width)
                        continue;

                    if (map[i][j] == '1')
                    {
                        // std::vector<int> pix = cgi::color::parse_rgb(get_pixel(px,py));

                        set_pixel(px, py, color);
                    }

                    if (map[i][j] == '0')
                    {
                        if (bg_color.has_value())
                        {
                            set_pixel(px, py, bg_color.value());
                        }

                        continue;
                    }
                }
            }

            return;
        }

        /// @brief used to draw an object of type cgi::type::buf2_rgba_t directly to window
        /// @param x_pos x position from where the drawing should begin with respect to window's top left corner
        /// @param y_pos y position from where the drawing should begin with respect to window's top left corner
        /// @param rgba_buffer cgi::type::buf2_rgba_t object that you want to draw 
        inline void draw_buf2_rgba_t(int x_pos, int y_pos, const cgi::type::buf2_rgba_t &rgba_buffer)
        {

            int size_i = rgba_buffer.size();
            int py, px;

            for (int i = 0; i < size_i; i++)
            {
                py = y_pos + i;

                if (py < 0 || py >= this->details.height)
                {
                    continue;
                }

                int size_j = rgba_buffer[i].size();

                for (int j = 0; j < size_j; j++)
                {
                    px = x_pos + j;

                    if (px < 0 || px >= this->details.width)
                        continue;

                    this->set_pixel(px, py, rgba_buffer[i][j]);
                }
            }

            return;
        }

        // inline void draw_buf2_rgba_t(int x_pos,int y_pos,const cgi::type::buf2_rgba_t rgba_buffer,int scale_x=1,int scale_y=1){

        //     for(int i=0;i<rgba_buffer.size();i++){
        //         for(int sy=0;sy<scale_y;sy++){
        //             int py ;

        //             if(py<0 || py>=this->details.height) continue;

        //             for(int j=0;j<rgba_buffer[i].size();j++){
        //                 const cgi::type::rgba_t color = rgba_buffer[i][j];

        //                 for(int sx=0;sx<scale_x;sx++){
        //                     int px=x_pos+;

        //                     if(px<0 || px>=this->details.width ) continue;

        //                 }
        //             }
        //         }
        //     }

        // }

        // inline cgi::type::map2_t write(std::string text, const pen &p, cgi::type::rgba_t color, int x_pos, int y_pos, int scale_x = 1, int scale_y = 1, int space = 1, std::optional<cgi::type::rgba_t> bg_color = std::nullopt)
        // {

        //     cgi::type::map2_t map;

        //     if (scale_x < 0)
        //         scale_x = 0;
        //     if (scale_y < 0)
        //         scale_y = 0;
        //     if (space < 0)
        //         space = 0;

        //     int row_at = 0;

        //     map.resize(scale_y * p.get_style_height());
        //     for (int i = 0; i < p.get_style_height(); i++)
        //     {
        //         for (int s = 0; s < scale_y; s++)
        //         {

        //             for (int j = 0; j < text.size(); j++)
        //             {
        //                 std::vector<cgi::type::map_t> line = p.get_style(text[j]);

        //                 for (char c : line[i])
        //                 {
        //                     for (int k = 0; k < scale_x; k++)
        //                     {
        //                         map[row_at].push_back(c);
        //                     }
        //                 }

        //                 if (j != text.size() - 1)
        //                 {
        //                     for (int k = 0; k < space; k++)
        //                     {
        //                         map[row_at].push_back('0');
        //                     }
        //                 }
        //             }

        //             row_at++;
        //         }
        //     }

        //     // displaying final map for debugging
        //     //  for (int i = 0; i < map.size(); i++)
        //     //  {
        //     //      for (int j = 0; j < map[i].size(); j++)
        //     //      {
        //     //          std::cout << map[i][j];
        //     //      }
        //     //      std::cout << '\n';
        //     //  }

        //     char ch;
        //     for (int i = 0; i < map.size(); i++)
        //     {
        //         for (int j = 0; j < map[i].size(); j++)
        //         {

        //             ch = map[i][j];

        //             if (ch == '1')
        //             {
        //                 this->set_pixel(j + x_pos, i + y_pos, color);
        //             }
        //             else if (ch == '0')
        //             {
        //                 if (bg_color.has_value())
        //                 {
        //                     this->set_pixel(j + x_pos, i + y_pos, bg_color.value());
        //                 }
        //             }
        //         }
        //     }

        //     return map;
        // }

        /// @brief used to check if a window is in focus
        /// @return returns true if focused or false
        inline bool is_focused() noexcept
        {
            if (GetForegroundWindow() == this->details.hwnd)
            {
                return true;
            }
            return false;
        }

        /// @brief used to check if an event of horizontal scrolling is catched
        /// @return true if scrolled horizonatlly otherwise false
        inline bool is_scrolled_x() noexcept
        {
            if (this->details.scroll_x != 0)
                return true;
            return false;
        }

        /// @brief used to check if an event of vertical scrolling is catched
        /// @return true if scrolled vertical otherwise false
        inline bool is_scrolled_y() noexcept
        {
            if (this->details.scroll_y != 0)
                return true;
            return false;
        }

        /// @brief used to get the number of times horizontal scrolling has happened continously
        /// @return count number to indicate how many horizontal scrolling has happened continously
        inline float get_scroll_x()  noexcept
        {
            return (float)this->details.scroll_x / 120;
        }

        /// @brief used to get the number of times vertical scrolling has happened continously
        /// @return  count number to indicate how many vertical scrolling has happened continously
        inline float get_scroll_y() noexcept
        {
            return (float)this->details.scroll_y / 120;
        }

        /// @brief used to get the x position of window client area or window buffer area in pixels with respect to top left corner of screen
        /// @return x position in pixels 
        inline int get_buffer_x_pos() noexcept
        {
            POINT p = {0, 0};
            ClientToScreen(this->details.hwnd, &p);
            return p.x;
        }

        /// @brief used to get the y position of window client area or window buffer area in pixels with respect to top left corner of screen
        /// @return y position in pixels
        inline int get_buffer_y_pos() noexcept
        {
            POINT p = {0, 0};
            ClientToScreen(this->details.hwnd, &p);
            return p.y;
        }

        /// @brief used to get the x position of cursor in pixels with respect to the window client or buffer area
        /// @return x position in pixels
        int get_cursor_x() noexcept
        {
            return (cgi::system::cursor::get_x_pos() - this->get_buffer_x_pos());
        }

        /// @brief used to get the y position of cursor in pixels with respect to the window client or buffer area
        /// @return y position in pixels
        int get_cursor_y() noexcept
        {
            return (cgi::system::cursor::get_y_pos() - this->get_buffer_y_pos());
        }

        /// @brief used when trying to pass a custom start function for window
        /// @param start_function pointer to a function that initializes the window
        void start_as(void (*start_function)()) noexcept
        {
            start_function();
        }

        /// @brief refreshes or repaints the client area of buffer area of the window
        inline void buffer_refresh() noexcept
        {
            InvalidateRect(this->details.hwnd, nullptr, TRUE);
            UpdateWindow(this->details.hwnd);
        }

        /// @brief used to pass a custom execution lopp for windows as how to run it each frame
        /// @param update_function pointer the function of execution loop
        /// @param refresh_rate refresh rate (default =30fps)
        void run_as(void (*update_function)(), double refresh_rate = 30)
        {

            if (!this->is_open())
            {
                std::cout << "cannot execute(run) an unopen window . Make sure you have open one " << std::endl;
                return;
            }

            this->details.threshold_frame_period = (double)1e9 / refresh_rate;

            this->details.last_frame_time = std::chrono::steady_clock::now();

            while (this->is_open())
            {

                auto now_time = std::chrono::steady_clock::now();

                MSG msg = {};
                while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);

                    if (msg.message == WM_QUIT)
                    {
                        this->open = false;
                    }
                }

                update_function();

                // InvalidateRect(this->details.hwnd, nullptr, TRUE);
                // UpdateWindow(this->details.hwnd);

                this->buffer_refresh();

                auto target_end_time = now_time + std::chrono::nanoseconds((int64_t)this->details.threshold_frame_period);

                auto before_sleep = std::chrono::steady_clock::now();

                if (before_sleep < target_end_time)
                {
                    std::this_thread::sleep_until(target_end_time);
                }

                auto end = std::chrono::steady_clock::now();
                this->details.frame_period = std::chrono::duration_cast<std::chrono::nanoseconds>(end - now_time).count();

                this->val_reset();
                this->details.last_frame_time = end;
            }
        }

        /// @brief used to get the time period between each frames of the window
        /// @return gets the time period between the frame in seconds each frame
        inline double frame_period() noexcept
        {

            return (double)this->details.frame_period / 1e9;
        }

        /// @brief used to get the fps of the window
        /// @return return the fps of the window each second
        inline double fps() noexcept
        {
            if (this->details.frame_period == 0)
            {
                return 0;
            }

            return (double)1 / frame_period();
        }

        /// @brief used to check if a window have been resized
        /// @return return a constant refernce to the bool to check if resized or not
        inline bool &is_resized() noexcept
        {

            return this->resized;
        }

        /// @brief custom message handler for window created using cgi::window
        /// @param hwnd handle to the window
        /// @param msg message received
        /// @param wp wparam for window
        /// @param lp lparam for window
        /// @return LRESULT of windows
        inline LRESULT handle_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
        {
            switch (msg)
            {

            case WM_CREATE:
            {
                make_bmi(this->get_buffer_width(), this->get_buffer_height());
                break;
            }

            case WM_PAINT:
            {
                this->details.ps = {};
                HDC hdc = BeginPaint(hwnd, &this->details.ps);

                load_view();
                // std::cout<<"here";
                display_view(hdc);
                EndPaint(hwnd, &this->details.ps);

                break;
            }

            case WM_ERASEBKGND:
            {
                return 1;
            }

            case WM_MOVE:
            {
                RECT rect = {};
                GetWindowRect(hwnd, &rect);

                this->details.x_pos = rect.left;
                this->details.y_pos = rect.top;

                // std::cout<<this->details.y_pos<<' '<<this->details.x_pos<<'\n';

                // InvalidateRect(hwnd,nullptr,TRUE);
                // UpdateWindow(hwnd);
                // SendMessage(hwnd,WM_PAINT,0,0);

                break;
            }

            case WM_SIZE:
            {

                if (!this->first_log)
                {
                    this->resized = true;
                }

                cleanup();

                // RECT rect = {};
                // GetClientRect(hwnd, &rect);
                // this->details.height = rect.bottom - rect.top;
                // this->details.width = rect.right - rect.left;

                this->details.height = this->get_buffer_height();
                this->details.width = this->get_buffer_width();

                this->details.buffer.clear();
                this->details.buffer.resize(this->get_buffer_height() * this->get_buffer_width(), this->details.color);

                make_bmi(this->details.width, this->details.height);

                break;
            }

            case WM_MOUSEWHEEL:
            {
                this->details.scroll_y = (float)GET_WHEEL_DELTA_WPARAM(wp);

                break;
            }

            case WM_MOUSEHWHEEL:
            {
                this->details.scroll_x = (float)GET_WHEEL_DELTA_WPARAM(wp);
                break;
            }

            case WM_CLOSE:
            {
                cleanup();
                DestroyWindow(hwnd);
                break;
            }

            case WM_DESTROY:
            {
                /// if main window then only postquit mmessage and unregisterclassa
                PostQuitMessage(0);
                // UnregisterClassA(this->details.class_name, this->details.wc.hInstance);
                break;
            }

                /// @brief error in closing window may have a fix soon

                // case WM_NCDESTROY:{
                //     UnregisterClassA(this->details.class_name,this->details.wc.hInstance);
                //     break;
                // }

            default:
                return DefWindowProc(hwnd, msg, wp, lp);
            }

            return 0;
        }

        /// @brief check for message and handle message only for one frame
        inline void refresh()
        {

            if (!this->is_open())
            {
                std::cout << "cannot refresh an uncreated window . Make sure you have created one " << std::endl;
                return;
            }

            this->val_reset();

            MSG msg = {};
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                if (msg.message == WM_QUIT)
                {
                    this->open = false;
                }
            }

            // InvalidateRect(this->details.hwnd, nullptr, TRUE);
            // UpdateWindow(this->details.hwnd);

            this->buffer_refresh();
        }
    };

    
    LRESULT CALLBACK cgi_windows_procedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        cgi::window *win = nullptr;

        if (msg == WM_NCCREATE)
        {
            CREATESTRUCT *cs = (CREATESTRUCT *)lp;
            win = (window *)cs->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(win));
        }
        else
        {
            win = (window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (win)
        {
            return win->handle_message(hwnd, msg, wp, lp);
        }
        else
        {
            if (msg == WM_NCCREATE)
            {
                std::cerr << "library error. Couldnot receive the win ptr from GWLP_USERDATA" << std::endl;
            }
        }

        return DefWindowProc(hwnd, msg, wp, lp);
    }

}