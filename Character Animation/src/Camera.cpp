#include "Camera.h"

// -9.84, 1.97342, 1.38273

Camera::Camera(): fov(60.0f), lookaround_speed(0.05f), angle(0.0f), projection_width(1500), projection_height(1000), up(glm::vec3(0.0f, 1.0f, 0.0f)), front(glm::normalize(glm::vec3(0.998104, 0.00558746, 0.0613405))), right(glm::normalize(glm::cross(front,up))), near_plane(0.1f), far_plane(10000000.0f), camera_position(glm::vec3(13.1947, 18.3567, -12.4115)), rotation(glm::mat4(1.0f))
{
    set_projection() ;
    set_view() ;
}

void Camera::look_around( bool rightwards, bool leftwards, bool upwards, bool downwards )
{
    if( rightwards ){
        rotation = rotation * glm::rotate(glm::mat4(1.0f), lookaround_speed, up) ;
        lookaround_rotation = glm::rotate(glm::mat4(1.0f), lookaround_speed, up) ;
    }
    else if( leftwards ){
        rotation = rotation * glm::rotate(glm::mat4(1.0f), -lookaround_speed, up) ;
        lookaround_rotation = glm::rotate(glm::mat4(1.0f), -lookaround_speed, up) ;
    }
    else if( upwards ){
        angle += 0.005f ;
        if( angle > glm::radians(89.0f) ){
            angle = glm::radians(89.0f) ;
            return ;
        }
        else{
            rotation = rotation * glm::rotate(glm::mat4(1.0f), lookaround_speed, right) ;
            lookaround_rotation = glm::rotate(glm::mat4(1.0f), lookaround_speed, right) ;
        }
    }
    else if( downwards ){
        angle -= 0.005f ;
        if( angle < glm::radians(-89.0f) ){
            angle = glm::radians(-89.0f) ;
            return  ;
        }
        else{
            rotation = rotation * glm::rotate(glm::mat4(1.0f), -lookaround_speed, right) ;
            lookaround_rotation = glm::rotate(glm::mat4(1.0f), -lookaround_speed, right) ;
        }
    }
    front = glm::vec3(lookaround_rotation * glm::vec4(front, 1.0f)) ;
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f,1.0f,0.0f))) ;
    up = glm::normalize(glm::cross(right, front)) ;
    set_view() ;
}

void Camera::move()
{
    camera_position = camera_position + (0.1f*front) ;
    set_view() ;
}

void Camera::update( glm::vec3 boat_position, glm::vec3 boat_direction )
{
    camera_position = boat_position - boat_direction * 0.9f ;
    camera_position += glm::vec3(0.0f, 0.5f, 0.0f) ;
    front = front + (0.03f * (boat_direction-front) ) ;
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f,1.0f,0.0f))) ;
    up = glm::normalize(glm::cross(right, front)) ;
    set_view() ;
}

void Camera::set_projection()
{
    projection = glm::perspective(glm::radians(fov), static_cast<float>(1500)/static_cast<float>(1000), near_plane, far_plane) ;
}

void Camera::set_view()
{
    view = glm::lookAt(camera_position, camera_position+front, up ) ;
}

float Camera::get_fov()
{
    return fov ;
}

glm::vec3 Camera::get_camera_front()
{
    return front ;
}

glm::vec3 Camera::get_camera_position()
{
    return camera_position ;
}

glm::mat4 Camera::get_view()
{
    return view ;
}

glm::mat4 Camera::get_projection()
{
    return projection ;
}

Camera::~Camera(){}
