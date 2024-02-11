#include <loader.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

namespace ArenaLoader {
    BgObj parse_bg(std::istringstream& iss) {
        BgObj object;
        char draw_type;
        iss >> draw_type;
        if (draw_type == 'C') {
            object.texture=false;
            unsigned int r,g,b,a;
            iss >> r >> g >> b >> a;
            object.color = (Color){(unsigned char)r,(unsigned char)g,(unsigned char)b,(unsigned char)a};
        }
        else if (draw_type == 'T') {
            object.texture=true;
            unsigned int texture_id;
            iss >> texture_id;
            object.texture_id = texture_id;
        }
        return object;
    }

    Object parse_draw(std::istringstream& iss) {
        Object object;
        char form_type;

        iss >> form_type;
        if (form_type == 'R') {
            float x,y,w,h;
            bool blocking;
            char draw_type;
            iss >> x >> y >> w >> h >> draw_type;
            object.rect = (Rectangle){x,y,w,h};
            if (draw_type=='C') {
                object.texture=false;
                unsigned int r,g,b,a;
                iss >> r >> g >> b >> a;
                object.color = (Color){(unsigned char)r,(unsigned char)g,(unsigned char)b,(unsigned char)a};
            }
            else if (draw_type == 'T') {
                object.texture=true;
                unsigned int texture_id;
                iss >> texture_id;
                object.texture_id = texture_id;
            }
            std::string bword;
            iss >> bword;
            blocking = (bword == "true" ? true : false);
            object.blocking=blocking;
        }
        std::cout << object.rect.x << " " << object.rect.y << " " << object.rect.width << " " << object.rect.height << " " << object.blocking << std::endl;
        return object;
    }

    PlayerObj parse_player(std::istringstream& iss) {
        PlayerObj player;
        float x,y,w,h,speed,jump;
        iss >> x >> y >> w >> h >> speed >> jump;
        player.pos_size = (Rectangle){x,y,w,h};
        player.speed = speed;
        player.jump = jump;

        char draw_type;
        iss >> draw_type;
        if (draw_type == 'C') {
            player.texture=false;
            unsigned int r,g,b,a;
            iss >> r >> g >> b >> a;
            player.color = (Color){(unsigned char)r,(unsigned char)g,(unsigned char)b,(unsigned char)a};
        }
        else if (draw_type == 'T') {
            player.texture=true;
            unsigned int texture_id;
            iss >> texture_id;
            player.texture_id = texture_id;
        }
        return player;
    }

    Level load_level(const char *path) {
        std::ifstream file_h(path);

        BgObj background = {
            .texture=false,
            .color=(Color){0, 0, 0, 0},
        };
        PlayerObj player = {
            .pos_size = (Rectangle){0, 0, 20, 20},
            .speed = 0.0f,
            .texture=false,
            .color=(Color){0, 0, 0, 0}
        };
        std::vector<Object> env;
        std::vector<int> textures;

        std::string line;
        while (getline(file_h, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;
            if (type == "BG") {
                background = parse_bg(iss);
                if (background.texture) {
                    textures.push_back(background.texture_id);
                }
            } else if (type == "DRAW") {
                Object obj = parse_draw(iss);
                if (obj.texture) {
                    textures.push_back(obj.texture_id);
                }
                env.push_back(
                    obj
                );
            } else if (type == "PLAYER") {
                player = parse_player(iss);
                if (player.texture) {
                    textures.push_back(player.texture_id);
                }
            }
        }
        return (Level){player, background, env, textures};
    }

    std::map<int, std::string> load_mapping(const char *path) {
        std::map<int, std::string> texture_map;

        std::ifstream file_h(path);
        std::string line;
        while (getline(file_h, line)) {
            std::istringstream iss(line);
            int index;
            char separator;
            std::string path;
            iss >> index >> separator >> path;
            texture_map.insert(
                std::pair<int, std::string>(index, path)
            );
        }

        return texture_map;
    }

}