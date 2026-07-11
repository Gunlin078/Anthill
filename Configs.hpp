#ifndef CONFIGS_H
#define CONFIGS_H

// Если определен флаг релиза, то макрос станет заглушкой
#ifdef NDEBUG
#define LOG_MSG(x)
#else
#define LOG_MSG(x) qDebug() << x
#endif

struct Config{
    inline static int SCENE_HEIGHT = 1640;
    inline static int SCENE_WIDTH = 2600;
    inline static int WALL_THICKNESS = 40;
    inline static int RESOURCE_COUNT = 2;
    inline static int CREATION_INTERVAL = 20;
    inline static int NEST_ENTRANCE_RADIUS = 40; //нужен фронтенд

    inline static int ANT_WARRIOR_COUNT = 0;
    inline static int ANT_WORKER_COUNT = 3;
    inline static int ANT_SCOUT_COUNT = 0;

    inline static int FPS = 60;
    inline static float DELTA_TIME = 1.0f/FPS;

};

#endif // CONFIGS_H
