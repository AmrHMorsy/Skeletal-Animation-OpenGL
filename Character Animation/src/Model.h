#include "Ground.h"






class Model{
    
private:
    
    float time ;
    Parser * parser ;
    Skybox * skybox ;
    Shader * shader ;
    std::string model_filepath, vs_filepath, fs_filepath ; 
    std::vector<Mesh*> meshes ;
    glm::mat4 model, projection, view ;
    std::vector<std::string> file_paths ;
    unsigned int shader_program, num_meshes ;
    glm::vec3 light_color, camera, light_position ;
    std::vector<unsigned int> textures, vao, vbo, ebo, num_indices ;

public:
    
    Model( std::string modelfilepath, std::vector<std::string> texturesfilepaths, glm::mat4 v, glm::mat4 proj, glm::vec3 c, std::string vsfile_path, std::string fsfile_path ) ;
    void initialize_variables() ;
    void load_FBXModel() ;
    void load_shaders() ;
    void set( Skybox * skybox ) ;
    void generate_buffers() ;
    void bind_vertices() ;
    void bind_indices() ;
    void render() ;
    void update_time() ;
    void load_textures() ;
    void set_uniform_variables() ;
    void update_view( glm::vec3 camera, glm::mat4 v ) ;
    void activate_textures() ;
    void set_animation( int animation ) ;
    void generate_error( std::string msg ) ;
    ~Model() ;
    
} ;
