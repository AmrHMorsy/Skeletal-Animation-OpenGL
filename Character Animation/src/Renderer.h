#include "Camera.h"




class Renderer{
    
private:
    
    Ground * g ; 
    FBXModel * man ;
    Light * light ;
    Skybox * skybox ;
    Camera * camera ;
    Model * house, * ground, * sofa, * table, * carpet, * tv, * remote ; 
    GLFWwindow * window ;
    int fps, frameNumber ;
    float fps_time, time ;

public:
    
    Renderer( GLFWwindow * window_ ) ;
    void set( cl_context context, cl_kernel FA, cl_kernel IFT, cl_command_queue queue ) ;
    void start_game_loop() ;
    void render_scene() ; 
    void handle_events() ;
    void update_view() ;
    void calculate_fps() ;
    void generate_error( std::string error_msg ) ;
    ~Renderer() ;
} ;
