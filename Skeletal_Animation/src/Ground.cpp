#include "Ground.h"
#include "../external/Include/stb_image.h"
#define EPSILON 1.8





Ground::Ground( std::string modelfilepath, std::vector<std::string> texturesfilepaths, glm::mat4 v, glm::mat4 proj, glm::vec3 c, std::string vsfilepath, std::string tcsfilepath, std::string tesfilepath, std::string fsfilepath, glm::mat4 lightview, glm::mat4 lightproj, glm::vec3 lightpos, glm::vec3 lightcol ): view(v), projection(proj), camera(c), file_paths( texturesfilepaths ), light_view(lightview), light_projection(lightproj), light_position(lightpos), light_color(lightcol), model_filepath(modelfilepath), vs_filepath(vsfilepath), tcs_filepath(tcsfilepath), tes_filepath(tesfilepath), fs_filepath(fsfilepath)
{
    load_model() ;
    load_shaders() ;
    generate_buffers() ;
    load_textures() ;
    set_models() ;
    bind_vertices( parser->get_vertices() ) ;
    bind_models() ;
    bind_indices( parser->get_indices() ) ;
    set_uniform_variables() ;
}

void Ground::load_model()
{
    parser = new Parser() ;
    parser->parse( model_filepath ) ;
    glPatchParameteri( GL_PATCH_VERTICES, 3 ) ;
}

void Ground::load_shaders()
{
    shader = new Shader( vs_filepath, tcs_filepath, tes_filepath, fs_filepath ) ;
    shader_program = shader->get_shader_program() ;
    glUseProgram(shader_program) ;
}

void Ground::set( Skybox * skybox )
{
    this->skybox = skybox ;
}

void Ground::generate_buffers()
{
    textures.resize(file_paths.size()) ;
    glGenTextures(file_paths.size(),textures.data()) ;
    glGenVertexArrays(1, &vao) ;
    glGenBuffers(1, &vbo) ;
    glGenBuffers(1, &ebo) ;
    glGenBuffers(1, &model_vbo);
    glGenBuffers(1, &patch_indices_vbo);
}

void Ground::render( Shadow * shadow )
{
    glUseProgram( shader_program ) ;
    activate_textures() ;
    shadow->activate_texture() ;
    glBindVertexArray(vao) ;
    glDrawElementsInstanced( GL_PATCHES, num_indices, GL_UNSIGNED_INT, 0, num_patches ) ;
    glBindVertexArray(0) ;
}

void Ground::render( unsigned int shadow_shader_program )
{
    for( int i = 0 ; i < NUM_PATCHES ; i++ ){
        glUniformMatrix4fv(glGetUniformLocation(shadow_shader_program, "model"), 1, GL_FALSE, &models[i][0][0]) ;
        glBindVertexArray(vao) ;
        glDrawElementsInstanced( GL_PATCHES, num_indices, GL_UNSIGNED_INT, 0, num_patches ) ;
        glBindVertexArray(0) ;
    }
}

void Ground::bind_vertices( std::vector<float> vertices )
{
    glBindVertexArray(vao) ;
    glBindBuffer(GL_ARRAY_BUFFER, vbo) ;
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)0) ;
    glEnableVertexAttribArray(0) ;
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Ground::bind_models()
{
    glBindVertexArray(vao) ;
    glBindBuffer(GL_ARRAY_BUFFER, patch_indices_vbo);
    glBufferData(GL_ARRAY_BUFFER, num_patches * sizeof(float), patch_indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);
    glBindBuffer(GL_ARRAY_BUFFER, model_vbo);
    glBufferData(GL_ARRAY_BUFFER, num_patches * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW) ;
    for (int i = 0; i < 4; i++){
        glVertexAttribPointer(4+i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i)) ;
        glEnableVertexAttribArray(4+i) ;
        glVertexAttribDivisor(4+i, 1) ;
    }
    glBindVertexArray(0) ;
}

void Ground::update_time()
{
    float deltaTime = glfwGetTime() - time ;
    time += deltaTime ;
}

void Ground::bind_indices( std::vector<unsigned int> indices )
{
    glBindVertexArray(vao) ;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo) ;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), indices.data(), GL_STATIC_DRAW) ;
    glBindVertexArray(0) ;
    num_indices = indices.size() ;
}

void Ground::set_uniform_variables()
{
    glUniform1i(glGetUniformLocation(shader_program, "albedo_map"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "ao_map"), 1);
    glUniform1i(glGetUniformLocation(shader_program, "normal_map"), 2);
    glUniform1i(glGetUniformLocation(shader_program, "roughness_map"), 3);
    glUniform1i(glGetUniformLocation(shader_program, "displacement_map"), 4);
    glUniform1i(glGetUniformLocation(shader_program, "irradiance_map"), 5) ;
    glUniform1i(glGetUniformLocation(shader_program, "prefilter_map"), 6) ;
    glUniform1i(glGetUniformLocation(shader_program, "brdf_integration_map"), 7) ;
    glUniform1i( glGetUniformLocation(shader_program, "metallic_map"), 8) ;
    glUniform1i( glGetUniformLocation(shader_program, "opacity_map"), 9) ;
    glUniform1i( glGetUniformLocation(shader_program, "shadow_map"), 10) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, &projection[0][0]) ;
    glUniform3fv(glGetUniformLocation(shader_program, "light_color"), 1, &light_color[0]) ;
    glUniform3fv(glGetUniformLocation(shader_program, "light_position"), 1, &light_position[0]);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "light_view"), 1, GL_FALSE, &light_view[0][0]) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "light_projection"), 1, GL_FALSE, &light_projection[0][0]) ;
    glUniform3fv(glGetUniformLocation(shader_program, "camera"), 1, &camera[0]) ;
}

void Ground::set_models()
{
    patch_indices.clear() ;
    num_patches = 0 ;
    float x, z ;
    int s = std::sqrt(NUM_PATCHES) ;
    for( int i = 0; i < s; i++ ) {
        z = (i - s / 2) * 6.0f ;
        for( int j = 0; j < s; j++ ) {
            x = (j - s / 2) * 6.0f ;
            models[num_patches] = glm::translate(glm::mat4(1.0), glm::vec3(x, -0.3f, z)) * glm::scale( glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 3.0f) ) ;
            patch_indices.push_back(compute_index(i,j)) ;
            num_patches++ ;
        }
    }
}

float Ground::compute_index(int i, int j)
{
    if( i % 2 == 0 ){
        if( j % 2 == 0 )
            return 0.0f ;
        else
            return 1.0f ;
    }
    else{
        if( j % 2 == 0 )
            return 2.0f ;
        else
            return 3.0f ;
    }
}

void Ground::update_view( glm::vec3 camera, glm::mat4 v )
{
    this->camera = camera ;
    glUseProgram(shader_program) ;
    glUniform3fv(glGetUniformLocation(shader_program, "camera"), 1, &camera[0]) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, &v[0][0]) ;
}

void Ground::load_textures()
{
    GLenum format ;
    int w, h, nc ;
    unsigned char * data ;
    for( int i = 0 ; i < file_paths.size() ; i++ ){
        glBindTexture(GL_TEXTURE_2D, textures[i]) ;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT) ;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT) ;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) ;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) ;
        stbi_set_flip_vertically_on_load(true) ;
        data = stbi_load(file_paths[i].c_str(), &w, &h, &nc, 0);
        format = (nc==1)? GL_RED : (nc==3)? GL_RGB : (nc==4) ? GL_RGBA : GL_RGB ;
        if (data){
            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data) ;
            glGenerateMipmap(GL_TEXTURE_2D) ;
        }
        else
            generate_error("Failure To Load Texture") ;
        stbi_image_free(data);
    }
}

void Ground::activate_textures()
{
    for( int i = 0 ; i < textures.size() ; i++ ){
        glActiveTexture(GL_TEXTURE0+i) ;
        glBindTexture(GL_TEXTURE_2D, textures[i]) ;
    }
    skybox->activate_irradiance() ;
}

void Ground::generate_error( std::string msg )
{
    std::cout << msg << std::endl ;
    exit(1) ;
}

void Ground::activate()
{
    glUseProgram(shader_program) ;
}

Ground::~Ground()
{
    delete shader ;
    shader = NULL ;
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(textures.size(), textures.data());
}
