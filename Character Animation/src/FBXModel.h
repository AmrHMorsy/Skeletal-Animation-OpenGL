#include "Animator.h"




class FBXModel {
    
private:
    
    bool lock ;
    int current_animation ;
    float position ;
    bool step ;
    Parser * parser ;
    Skybox * skybox ;
    Shader * shader ;
    Animator * animator ;
    float time, elapsed_time ;
    std::vector<Mesh*> meshes ;
    glm::vec3 direction ;
    glm::mat4 model, projection, view ;
    std::vector<std::string> file_paths ;
    unsigned int shader_program, num_meshes ;
    glm::vec3 light_color, camera, light_position ;
    std::string model_filepath, vs_filepath, fs_filepath ;
    std::vector<unsigned int> textures, vao, vbo, ebo, num_indices ;

public:
    
    FBXModel( std::string modelfilepath, std::vector<std::string> texturesfilepaths, glm::mat4 v, glm::mat4 proj, glm::vec3 c, std::string vsfilepath, std::string fsfilepath ) ;
    void initialize_variables() ;
    void load_FBXModel() ;
    void load_shaders() ;
    void set( Skybox * skybox ) ;
    void generate_buffers() ;
    void bind_vertices() ;
    void bind_indices() ;
    void animate() ;
    void render() ;
    void update_time() ;
    void load_textures() ;
    void set_uniform_variables() ;
    void update_view( glm::vec3 camera, glm::mat4 v ) ;
    void activate_textures() ;
    void set_animation( std::string animation ) ;
    void set_animation( int animation, float requested_steer_angle ) ;
    void generate_error( std::string msg ) ;
    ~FBXModel() ;
    
} ;
