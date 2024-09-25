#include "Light.h"




class Camera{
    
private:
    
    glm::vec3 camera_position, front, up, right ;
    glm::mat4 projection, view, model, rotation, lookaround_rotation ;
    float angle, fov, lookaround_speed, near_plane, far_plane, projection_width, projection_height ; 
    
public:
    
    Camera() ;
    void look_around( bool rightwards, bool leftwards, bool upwards, bool downwards ) ;
    void update( glm::vec3 boat_position, glm::vec3 boat_direction ) ;
    void set_projection() ;
    void set_view() ;
    float get_fov() ;
    void move() ; 
    void restore_camera() ;
    glm::vec3 get_camera_front() ;
    glm::vec3 get_camera_position() ;
    glm::mat4 get_view() ;
    glm::mat4 get_projection() ;
    ~Camera() ;    
} ;
