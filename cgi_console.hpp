#pragma once
#ifndef CGI_CONSOLE_HPP
#define CGI_CONSOLE_HPP

#include "cgi_includes.hpp"

namespace cgi{

    bool get_console(std::string output_buffer="CONOUT$",std::string input_buffer="CONIN$"){
        try{
            AllocConsole();
            freopen(output_buffer.c_str(),"w",stdout);
            freopen(output_buffer.c_str(),"w",stderr);

            if(!freopen(input_buffer.c_str(),"r",stdin)){
                freopen(input_buffer.c_str(),"w",stdin);
            }

            
            return true;
        }catch(...){
            
            return false;
        }
    }
}

#endif
