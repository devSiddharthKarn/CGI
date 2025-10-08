#pragma once


#include "cgi_data_types.hpp"


namespace cgi{
    namespace system{
        namespace display{

            long int get_width(){
                return GetSystemMetrics(SM_CXSCREEN);
            }
            
            long int get_height(){
                return GetSystemMetrics(SM_CYSCREEN);
            }

            long long int get_pixel_count(){
                return get_height()*get_width();
            }

            double get_size(){
                return sqrt(pow(get_height(),2)+pow(get_width(),2));
            }

            long long int get_area(){
                return get_width()*get_height();
            }
        }

        namespace keyboard{

            //ASCII keys vary from 32 to 126
            //we can manage only them
            //as they are standard inputs
            inline bool is_ASCII_key_pressed(char key){
                if(key>=97 && key<=122){
                    key = (char)(key-32);
                }

                if(GetAsyncKeyState(key) & 0x8000){
                    return true;
                }
                return false;
            }

            inline std::vector<char> which_ASCII_keys_pressed(){
                std::vector<char> keys;
                for(int i=32;i<=126;i++){
                    if(i>=97 && i<=122){
                        continue;
                    }else{

                        
                        if(cgi::system::keyboard::is_ASCII_key_pressed((char)i)){
                            keys.push_back(i);
                            keys.push_back(i+=32);
                        }
                    }
                }


                return keys;
                    
            }

        }

        namespace cursor{

            std::string type;

            void set_visible(bool __visibility=true){
                ShowCursor(__visibility);
                // true;
            }

            int get_x_pos(){
                POINT pnt={};
                GetCursorPos(&pnt);
                return pnt.x;
            }

            int get_y_pos(){
                POINT pnt={};
                GetCursorPos(&pnt);
                return pnt.y;
            }

            bool is_left_button_pressed(){
                if(GetAsyncKeyState(VK_LBUTTON) & 0x8000){
                    return true;
                }

                return false;
            }

            bool is_right_button_pressed(){
                if(GetAsyncKeyState(VK_RBUTTON) & 0x8000){
                    return true;
                }
                return false;
            }


            

            // bool set_image(cgi::type::cursor_t cursor){
                
            //     if(cursor==nullptr){
            //         std::cerr<<"error loading cursor from file "<<std::endl;
            //         return false;
            //     }


            //     SetCursor(cursor);
            //     return true;
            // }

            // bool set_image(LPCSTR cursor){
            //     cgi::type::cursor_t cur= LoadCursorA(nullptr,cursor);
            //     if(cur==nullptr){
            //         std::cout<<"no such cursor type found "<<std::endl;
            //         return false;
            //     }

            //     SetCursor(cur);
            //     return true;
            // }

           

            void set_pos(int x_pos,int y_pos){
                SetCursorPos(x_pos,y_pos);
                return;
            }

            
        }

        

        
    }

    
}