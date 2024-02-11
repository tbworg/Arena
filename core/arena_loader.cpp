#include <loader.hpp>
#include <fstream>
#include <iostream>
#include <vector>

namespace ArenaLoader {
    std::vector<Object> load_level(const char *path) {
        std::ifstream file_h(path);
        std::vector<Object> env;

        char type;
        float x,y,w,h;
        int r,g,b,a,blocking;
        while (file_h >> type >> x >> y >> w >> h >> r >> g >> b >> a >> blocking) {
            if (type == 'R') {
                env.push_back(
                    (Object){
                        {x,y,w,h},
                        blocking,
                        (Color){(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a}
                    }
                );
            }
        }
        
        return env;
    }
}