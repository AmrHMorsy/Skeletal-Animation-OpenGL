#include "Renderer.h"
#include "../external/include/stb_image_write.h"






Renderer::Renderer( GLFWwindow * window_): fps(0), fps_time(0), window(window_), frameNumber(0)
{
    camera = new Camera() ;
    light = new Light(window) ;
    remote = new Model( "../assets/remote/remote.obj", { "../assets/remote/Albedo.png", "../assets/remote/AO.png", "../assets/remote/Normal.png", "../assets/remote/Roughness.png", "../assets/remote/Displacement.png", "../assets/remote/Metallic.png", "../assets/remote/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/Shaders/Man/Man_vs.vs", "../external/Shaders/Man/Man_fs.fs") ;
    man = new FBXModel( "../assets/Man/Man5.fbx", { "../assets/Man/Albedo.png", "../assets/Man/AO.png", "../assets/Man/Normal.png", "../assets/Man/Roughness.png", "../assets/Man/Displacement.png", "../assets/Man/Metallic.png", "../assets/Man/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/Shaders/Man/Man_vs.vs", "../external/Shaders/Man/Man_fs.fs") ;
    house = new Model( "../assets/House2/House2.obj", { "../assets/House2/Albedo.png", "../assets/House2/AO.png", "../assets/House2/Normal.png", "../assets/House2/Roughness.png", "../assets/House2/Displacement.png", "../assets/House2/Metallic.png", "../assets/House2/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/Shaders/Object/Object_vs.vs", "../external/Shaders/Object/Object_fs.fs" ) ;
    table = new Model( "../assets/Table/Table.obj", { "../assets/Table/Albedo.png", "../assets/Table/AO.png", "../assets/Table/Normal.png", "../assets/Table/Roughness.png", "../assets/Table/Displacement.png", "../assets/Table/Metallic.png", "../assets/Table/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/Shaders/Object/Object_vs.vs", "../external/Shaders/Object/Object_fs.fs" ) ;
    carpet = new Model( "../assets/Carpet/Carpet.obj", { "../assets/Carpet/Albedo.png", "../assets/Carpet/AO.png", "../assets/Carpet/Normal.png", "../assets/Carpet/Roughness.png", "../assets/Carpet/Displacement.png", "../assets/Carpet/Metallic.png", "../assets/Carpet/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/Shaders/Object/Object_vs.vs", "../external/Shaders/Object/Object_fs.fs" ) ;
    tv = new Model( "../assets/tv/tv.obj", { "../assets/tv/Albedo.png", "../assets/tv/AO.png", "../assets/tv/Normal.png", "../assets/tv/Roughness.png", "../assets/tv/Displacement.png", "../assets/tv/Metallic.png", "../assets/tv/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/Shaders/Object/Object_vs.vs", "../external/Shaders/Object/Object_fs.fs" ) ;
    sofa = new Model( "../assets/Sofa/Sofa.obj", { "../assets/Sofa/Albedo.png", "../assets/Sofa/AO.png", "../assets/Sofa/Normal.png", "../assets/Sofa/Roughness.png", "../assets/Sofa/Displacement.png", "../assets/Sofa/Metallic.png", "../assets/Sofa/Opacity.png" }, camera->get_view(), camera->get_projection(), camera->get_camera_position(), "../external/Shaders/Object/Object_vs.vs", "../external/Shaders/Object/Object_fs.fs" ) ;
    skybox = new Skybox( camera->get_view(), camera->get_projection(), camera->get_camera_position()) ;
}

void Renderer::set( cl_context context, cl_kernel FA, cl_kernel IFT, cl_command_queue queue )
{
    man->set(skybox) ;
    remote->set(skybox) ;
    sofa->set( skybox ) ;
    house->set(skybox) ;
    table->set(skybox) ;
    carpet->set(skybox) ;
    tv->set(skybox) ;
}

void Renderer::start_game_loop()
{
    time = glfwGetTime() ;
    update_view() ;
    while( !glfwWindowShouldClose(window) ){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
        glClearColor(0.0f, 0.0f, 0.0f,1.0f) ;
        skybox->activate() ;
        skybox->render() ;
        man->render() ;
        house->render() ;
        sofa->render() ;
        table->render() ;
        tv->render() ;
        carpet->render() ;
        remote->render() ;
        glfwSwapBuffers(window) ;
        handle_events() ;
        calculate_fps() ;
    }
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
    if( glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS ){
        man->set_animation( "Rig|Stand" ) ;
        update_view() ;
    }
    if( glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ){
        man->set_animation( "Rig|Walk" ) ;
        update_view() ;
    }
    if( glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS ){
        man->set_animation( "Rig|Watch_TV" ) ;
        update_view() ;
    }
}

void Renderer::update_view()
{
    remote->update_view( camera->get_camera_position(), camera->get_view() ) ;
    man->update_view( camera->get_camera_position(), camera->get_view() ) ;
    skybox->update_view( camera->get_view(), camera->get_camera_position() ) ;
    house->update_view( camera->get_camera_position(), camera->get_view() ) ;
    sofa->update_view( camera->get_camera_position(), camera->get_view() ) ;
    table->update_view( camera->get_camera_position(), camera->get_view() ) ;
    carpet->update_view( camera->get_camera_position(), camera->get_view() ) ;
    tv->update_view( camera->get_camera_position(), camera->get_view() ) ;
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
    delete remote ;
    remote = NULL ;
    delete tv ;
    tv = NULL ;
    delete carpet ;
    carpet = NULL ;
    delete table ;
    table = NULL ;
    delete house ;
    house = NULL ; 
    delete light ;
    light = NULL ;
    delete man ;
    man = NULL ;
    delete camera ;
    camera = NULL ;
    delete skybox ;
    skybox = NULL ;
}
