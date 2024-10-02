#include "Skybox.h"
#define NUM_PATCHES 625





class Ground{
    
private:
    
    float time ; 
    Parser * parser ;
    Skybox * skybox ;
    Shader * shader ;
    std::vector<float> patch_indices ;
    std::vector<unsigned int> textures ;
    std::vector<std::string> file_paths ;
    glm::vec3 light_color, camera, light_position ;
    std::string model_filepath, vs_filepath, tcs_filepath, tes_filepath, fs_filepath ;
    glm::mat4 models[NUM_PATCHES], projection, view, light_projection, light_view ;
    unsigned int vao, vbo, ebo, shader_program, num_indices, model_vbo, patch_indices_vbo, num_patches ;

public:
    
    Ground( std::string modelfilepath, std::vector<std::string> texturesfilepaths, glm::mat4 v, glm::mat4 proj, glm::vec3 c, std::string vsfilepath, std::string tcsfilepath, std::string tesfilepath, std::string fsfilepath, glm::mat4 lightview, glm::mat4 lightproj, glm::vec3 lightpos, glm::vec3 lightcol ) ;
    void load_model() ;
    void load_shaders() ;
    void set( Skybox * skybox ) ;
    void generate_buffers() ;
    void render( Shadow * shadow ) ;
    void render( unsigned int shadow_shader_program ) ;
    void bind_vertices( std::vector<float> vertices ) ;
    void update_time() ;
    void bind_indices( std::vector<unsigned int> indices ) ;
    void set_uniform_variables() ;
    void bind_models() ;
    void set_models() ;
    float compute_index(int i, int j) ;
    void update_view( glm::vec3 camera, glm::mat4 v ) ;
    void update_light( glm::mat4 view ) ;
    void load_textures() ;
    void activate() ;
    void activate_textures() ;
    void generate_error( std::string msg ) ;
    ~Ground() ;
} ;
