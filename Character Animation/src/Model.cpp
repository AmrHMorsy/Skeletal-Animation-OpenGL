#include "Model.h"
#include "../external/Include/stb_image.h"
#define EPSILON 1.8






Model::Model( std::string modelfilepath, std::vector<std::string> texturesfilepaths, glm::mat4 v, glm::mat4 proj, glm::vec3 c, std::string vsfilepath, std::string fsfilepath): view(v), projection(proj), camera(c), model_filepath(modelfilepath), file_paths(texturesfilepaths), vs_filepath( vsfilepath), fs_filepath( fsfilepath )
{
    initialize_variables() ;
    load_FBXModel() ;
    load_shaders() ;
    generate_buffers() ;
    load_textures() ;
    bind_vertices() ;
    bind_indices() ;
    set_uniform_variables() ;
}

void Model::initialize_variables()
{
    time = 0.0f ;
    light_position = glm::vec3( 13.9123, 15.3301, -3.75414 ) ;
    light_color = glm::vec3( 10.0f, 10.0f, 10.0f ) ;
    model = glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, -0.3f, 0.0f) ) * glm::scale( glm::mat4(1.0f), glm::vec3( 3.0f, 3.0f, 3.0f) ) ;
}

void Model::load_FBXModel()
{
    parser = new Parser() ;
    parser->parse( model_filepath ) ;
    meshes = parser->get_meshes() ;
    num_meshes = meshes.size() ;
}

void Model::load_shaders()
{
    shader = new Shader( vs_filepath, fs_filepath ) ;
    shader_program = shader->get_shader_program() ;
    glUseProgram(shader_program) ;
}

void Model::set( Skybox * skybox )
{
    this->skybox = skybox ;
}

void Model::generate_buffers()
{
    num_indices.resize( num_meshes ) ;
    textures.resize( file_paths.size() ) ;
    vao.resize( num_meshes ) ;
    vbo.resize( num_meshes ) ;
    ebo.resize( num_meshes ) ;
    glGenTextures( file_paths.size(), textures.data() ) ;
    glGenVertexArrays( num_meshes, vao.data() ) ;
    glGenBuffers( num_meshes, vbo.data() ) ;
    glGenBuffers( num_meshes, ebo.data() ) ;
}

void Model::bind_vertices()
{
    for( int i = 0 ; i < num_meshes ; i++ ){
        glBindVertexArray( vao[i] ) ;
        glBindBuffer( GL_ARRAY_BUFFER, vbo[i] ) ;
        glBufferData( GL_ARRAY_BUFFER, meshes[i]->vertices.size() * sizeof(float), meshes[i]->vertices.data(), GL_STATIC_DRAW ) ;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)0) ;
        glEnableVertexAttribArray(0) ;
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindVertexArray(0) ;
    }
}

void Model::bind_indices()
{
    for( int i = 0 ; i < num_meshes ; i++ ){
        glBindVertexArray( vao[i] ) ;
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo[i] ) ;
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, meshes[i]->indices.size()*sizeof(int), meshes[i]->indices.data(), GL_DYNAMIC_DRAW ) ;
        glBindVertexArray(0) ;
        num_indices[i] = meshes[i]->indices.size() ;
    }
}

void Model::render()
{
    glUseProgram(shader_program) ;
    activate_textures() ;
    for( int i = 0 ; i < num_meshes ; i++ ){
        glBindVertexArray( vao[i] ) ;
        glDrawElements( GL_TRIANGLES, num_indices[i], GL_UNSIGNED_INT, 0 ) ;
        glBindVertexArray( 0 ) ;
    }
}

void Model::update_time()
{
    float deltaTime = glfwGetTime() - time ;
    time += deltaTime ;
    glUniform1f(glGetUniformLocation(shader_program, "time"), time) ;
}

void Model::load_textures()
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

void Model::set_uniform_variables()
{
    glUniform1i( glGetUniformLocation(shader_program, "albedo_map"), 0);
    glUniform1i( glGetUniformLocation(shader_program, "ao_map"), 1);
    glUniform1i( glGetUniformLocation(shader_program, "normal_map"), 2);
    glUniform1i( glGetUniformLocation(shader_program, "roughness_map"), 3);
    glUniform1i( glGetUniformLocation(shader_program, "displacement_map"), 4) ;
    glUniform1i( glGetUniformLocation(shader_program, "irradiance_map"), 5) ;
    glUniform1i( glGetUniformLocation(shader_program, "prefilter_map"), 6) ;
    glUniform1i( glGetUniformLocation(shader_program, "brdf_integration_map"), 7) ;
    glUniform1i( glGetUniformLocation(shader_program, "metallic_map"), 8) ;
    glUniform1i( glGetUniformLocation(shader_program, "opacity_map"), 9) ;
    glUniformMatrix4fv( glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, &projection[0][0]) ;
    glUniformMatrix4fv( glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &model[0][0]);
    glUniform3fv( glGetUniformLocation(shader_program, "light_color"), 1, &light_color[0] );
    glUniform3fv( glGetUniformLocation(shader_program, "light_position"), 1, &light_position[0] );
    glUniform3fv( glGetUniformLocation(shader_program, "camera"), 1, &camera[0] ) ;
    glUniform1f( glGetUniformLocation(shader_program, "time"), time ) ;
}

void Model::update_view( glm::vec3 camera, glm::mat4 v )
{
    this->camera = camera ;
    glUseProgram(shader_program) ;
    glUniform3fv(glGetUniformLocation(shader_program, "camera"), 1, &camera[0]) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, &v[0][0]) ;
}

void Model::activate_textures()
{
    for( int i = 0 ; i < textures.size()-2 ; i++ ){
        glActiveTexture(GL_TEXTURE0+i) ;
        glBindTexture(GL_TEXTURE_2D, textures[i]) ;
    }
    skybox->activate_irradiance() ;
    glActiveTexture(GL_TEXTURE8) ;
    glBindTexture(GL_TEXTURE_2D, textures[textures.size()-2]) ;
    glActiveTexture(GL_TEXTURE9) ;
    glBindTexture(GL_TEXTURE_2D, textures[textures.size()-1]) ;
}

void Model::generate_error( std::string msg )
{
    std::cout << msg << std::endl ;
    exit(1) ;
}

Model::~Model()
{
    delete shader ;
    shader = NULL ;
    glDeleteVertexArrays(num_meshes, vao.data());
    glDeleteBuffers(num_meshes, vbo.data());
    glDeleteBuffers(num_meshes, ebo.data());
    glDeleteBuffers(textures.size(), textures.data());
}
