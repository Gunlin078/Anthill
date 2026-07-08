#ifndef CONFIGS_H
#define CONFIGS_H

struct Config{
    inline static int SCENE_HEIGHT = 500;
    inline static int SCENE_WIDTH = 500;
    inline static int WALL_THICKNESS = 10;
    inline static int RESOURCE_COUNT = 2;
    inline static int NEST_ENTRANCE_RADIUS = 40;

    inline static int ANT_RADIUS = 5;
    inline static int ANT_COUNT = 3;

    inline static int FPS = 20;
    inline static float DELTA_TIME = 1.0f/FPS;

};

#endif // CONFIGS_H
