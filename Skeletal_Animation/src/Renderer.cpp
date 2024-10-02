#include "Renderer.h"
#include "../external/include/stb_image_write.h"





Renderer::Renderer( GLFWwindow * window_): fps(0), fps_time(0), window(window_), frameNumber(0)
{
    camera = new Camera() ;
    light = new Light(window) ;
    shadow = new Shadow( window, light->get_projection(), light->get_view() ) ;
    man = new FBXModel( "../assets/Man/Mann.fbx", { "../assets/Man/Albedo.png", "../assets/Man/AO.png", "../assets/Man/Normal.png", "../assets/Man/Roughness.png", "../assets/Man/Displacement.png", "../assets/Man/Metallic.png", "../assets/Man/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/shaders/Man/Man_vs.vs", "../external/shaders/Man/Man_fs.fs", light->get_view(), light->get_projection(), light->get_position(), light->get_color() ) ;
    house = new Model( "../assets/House/House.obj", { "../assets/House/Albedo.png", "../assets/House/AO.png", "../assets/House/Normal.png", "../assets/House/Roughness.png", "../assets/House/Displacement.png", "../assets/House/Metallic.png", "../assets/House/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/shaders/Object/Object_vs.vs", "../external/shaders/Object/Object_fs.fs", light->get_view(), light->get_projection(), light->get_position(), light->get_color() ) ;
    tower = new Model( "../assets/Tower/Tower.obj", { "../assets/Tower/Albedo.png", "../assets/Tower/AO.png", "../assets/Tower/Normal.png", "../assets/Tower/Roughness.png", "../assets/Tower/Displacement.png", "../assets/Tower/Metallic.png", "../assets/Tower/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/shaders/Object/Object_vs.vs", "../external/shaders/Object/Object_fs.fs", light->get_view(), light->get_projection(), light->get_position(), light->get_color() ) ;
    barrel = new Model( "../assets/Barrel/Barrel.obj", { "../assets/Barrel/Albedo.png", "../assets/Barrel/AO.png", "../assets/Barrel/Normal.png", "../assets/Barrel/Roughness.png", "../assets/Barrel/Displacement.png", "../assets/Barrel/Metallic.png", "../assets/Barrel/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/shaders/Object/Object_vs.vs", "../external/shaders/Object/Object_fs.fs", light->get_view(), light->get_projection(), light->get_position(), light->get_color() ) ;
    bottle = new Model( "../assets/Bottle/Bottle.obj", { "../assets/Bottle/Albedo.png", "../assets/Bottle/AO.png", "../assets/Bottle/Normal.png", "../assets/Bottle/Roughness.png", "../assets/Bottle/Displacement.png", "../assets/Bottle/Metallic.png", "../assets/Bottle/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/shaders/Object/Object_vs.vs", "../external/shaders/Object/Object_fs.fs", light->get_view(), light->get_projection(), light->get_position(), light->get_color() ) ;
    streetlamp2 = new Model( "../assets/StreetLamp/StreetLamp.obj", { "../assets/StreetLamp/Albedo.png", "../assets/StreetLamp/AO.png", "../assets/StreetLamp/Normal.png", "../assets/StreetLamp/Roughness.png", "../assets/StreetLamp/Displacement.png", "../assets/StreetLamp/Metallic.png", "../assets/StreetLamp/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/shaders/Object/Object_vs.vs", "../external/shaders/Object/Object_fs.fs", light->get_view(), light->get_projection(), light->get_position(), light->get_color() ) ;
    streetlamp = new Model( "../assets/StreetLamp2/StreetLamp2.obj", { "../assets/StreetLamp2/Albedo.png", "../assets/StreetLamp2/AO.png", "../assets/StreetLamp2/Normal.png", "../assets/StreetLamp2/Roughness.png", "../assets/StreetLamp2/Displacement.png", "../assets/StreetLamp2/Metallic.png", "../assets/StreetLamp2/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/shaders/Object/Object_vs.vs", "../external/shaders/Object/Object_fs.fs", light->get_view(), light->get_projection(), light->get_position(), light->get_color() ) ;
    box = new Model( "../assets/Box/Box.obj", { "../assets/Box/Albedo.png", "../assets/Box/AO.png", "../assets/Box/Normal.png", "../assets/Box/Roughness.png", "../assets/Box/Displacement.png", "../assets/Box/Metallic.png", "../assets/Box/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/shaders/Object/Object_vs.vs", "../external/shaders/Object/Object_fs.fs", light->get_view(), light->get_projection(), light->get_position(), light->get_color() ) ;
    box2 = new Model( "../assets/Box2/Box2.obj", { "../assets/Box2/Albedo.png", "../assets/Box2/AO.png", "../assets/Box2/Normal.png", "../assets/Box2/Roughness.png", "../assets/Box2/Displacement.png", "../assets/Box2/Metallic.png", "../assets/Box2/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/shaders/Object/Object_vs.vs", "../external/shaders/Object/Object_fs.fs", light->get_view(), light->get_projection(), light->get_position(), light->get_color() ) ;
    skybox = new Skybox( camera->get_view(), camera->get_projection(), camera->get_camera_position()) ;
    ground = new Ground( "../assets/Ground/Ground.obj", {"../assets/Ground/Albedo.png", "../assets/Ground/AO.png", "../assets/Ground/Normal.png", "../assets/Ground/Roughness.png", "../assets/Ground/Displacement.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/Shaders/Ground/Ground_vs.vs", "../external/Shaders/Ground/Ground_fs.fs", "../external/Shaders/Ground/Ground_tcs.tcs", "../external/Shaders/Ground/Ground_tes.tes", light->get_view(), light->get_projection(), light->get_position(), light->get_color() ) ;
}

void Renderer::set( cl_context context, cl_kernel FA, cl_kernel IFT, cl_command_queue queue )
{
    house->set( skybox ) ;
    tower->set( skybox ) ;
    barrel->set( skybox ) ;
    bottle->set( skybox ) ;
    ground->set( skybox ) ;
    man->set( skybox ) ;
    streetlamp->set( skybox ) ;
    streetlamp2->set( skybox ) ;
    box->set( skybox ) ;
    box2->set( skybox ) ;
}

void Renderer::start_game_loop()
{
    time = glfwGetTime() ;
    update_view() ;
    while( !glfwWindowShouldClose(window) ){
        render_shadow() ;
        render_scene() ;
        glfwSwapBuffers(window) ;
        handle_events() ;
        calculate_fps() ;
    }
}

void Renderer::render_shadow()
{
    shadow->render() ;
    house->render( shadow->get_shader_program() ) ;
    tower->render( shadow->get_shader_program() ) ;
    barrel->render( shadow->get_shader_program() ) ;
    bottle->render( shadow->get_shader_program() ) ;
    ground->render( shadow->get_shader_program() ) ;
    man->render( shadow->get_shader_program() ) ;
    streetlamp->render( shadow->get_shader_program() ) ;
    streetlamp2->render( shadow->get_shader_program() ) ;
    box->render( shadow->get_shader_program() ) ;
    box2->render( shadow->get_shader_program() ) ;
    shadow->reset() ;
}

void Renderer::render_scene()
{
    skybox->activate() ;
    skybox->render() ;
    house->render( shadow ) ;
    tower->render( shadow ) ;
    barrel->render( shadow ) ;
    ground->render( shadow ) ;
    man->render( shadow ) ;
    bottle->render( shadow ) ;
    streetlamp->render( shadow ) ;
    streetlamp2->render( shadow ) ;
    box->render( shadow ) ;
    box2->render( shadow ) ;
}

void Renderer::handle_events()
{
    glfwPollEvents() ;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        camera->look_around(false,false,true,false) ;
        update_view() ;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        camera->look_around(false,false,false,true) ;
        update_view() ;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        camera->look_around(true,false,false,false) ;
        update_view() ;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        camera->look_around(false,true,false,false) ;
        update_view() ;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
        camera->move() ;
        update_view() ;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        man->set_animation( "Rig|Stand" ) ;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        man->set_animation( "Rig|Walk" ) ;
    
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        man->set_animation( "Rig|Run" ) ;
}

void Renderer::update_view()
{
    ground->update_view( camera->get_camera_position(), camera->get_view() ) ;
    man->update_view( camera->get_camera_position(), camera->get_view() ) ;
    bottle->update_view( camera->get_camera_position(), camera->get_view() ) ;
    streetlamp->update_view( camera->get_camera_position(), camera->get_view() ) ;
    streetlamp2->update_view( camera->get_camera_position(), camera->get_view() ) ;
    box->update_view( camera->get_camera_position(), camera->get_view() ) ;
    box2->update_view( camera->get_camera_position(), camera->get_view() ) ;
    barrel->update_view( camera->get_camera_position(), camera->get_view() ) ;
    house->update_view( camera->get_camera_position(), camera->get_view() ) ;
    tower->update_view( camera->get_camera_position(), camera->get_view() ) ;
    skybox->update_view( camera->get_view(), camera->get_camera_position() ) ;
}

void Renderer::calculate_fps()
{
    fps++ ;
    if( glfwGetTime() - static_cast<float>(fps_time) >= 1.0 ){
        std::cout << "FPS: \033[1;31m" << fps << "\033[0m" << '\r' << std::flush ;
        fps = 0 ;
        fps_time += 1.0 ;
    }
}

void Renderer::generate_error( std::string error_msg )
{
    std::cout << "\033[1;31m" << error_msg << "\033[0m" << std::endl;
    exit(1) ;
}

Renderer::~Renderer()
{
    delete man ;
    man = NULL ; 
    delete shadow ;
    shadow = NULL ;
    delete bottle ;
    bottle = NULL ; 
    delete streetlamp ;
    streetlamp = NULL ;
    delete streetlamp2 ;
    streetlamp2 = NULL ;
    delete box ;
    box = NULL ;
    delete box2 ;
    box2 = NULL ;
    delete barrel ;
    barrel = NULL ; 
    delete ground ;
    ground = NULL ;
    delete tower ;
    tower = NULL ; 
    delete house ;
    house = NULL ; 
    delete light ;
    light = NULL ;
    delete camera ;
    camera = NULL ;
    delete skybox ;
    skybox = NULL ;
}
