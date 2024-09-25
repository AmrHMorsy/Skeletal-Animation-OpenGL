#include "Skybox.h"
#define NUMBER_OCEAN_PATCHES 1024





class Ground{
    
private:
    
    GLint location ;
    Parser * parser ;
    Skybox * skybox ;
    Shader * shader ;
    float time, fov ;
    std::vector<float> patch_indices ;
    std::vector<unsigned int> textures ;
    std::vector<std::string> file_paths ;
    int num_progress_bars, number_ocean_patches ;
    glm::mat4 models[NUMBER_OCEAN_PATCHES], projection, view, light_proj_view_matrix, light_projection, light_view ;
    glm::vec3 light_color, camera, light_position, camera_front ;
    unsigned int vao, vbo, ebo, shader_program, number_of_indices, model_vbo, patch_indices_vbo ;
    unsigned int row, row2 ;
    int index, current_i, index2 ;
    unsigned int col, col2 ;
    int cindex, current_j, cindex2 ;

public:
    
    Ground( glm::mat4 view, glm::mat4 projection, glm::vec3 c, glm::vec3 front, float fv ) ;
    void set( Skybox * skybox ) ;
    void generate_buffers() ;
    void render() ;
    void bind_vertices( std::vector<float> vertices ) ;
    void update_time() ;
    void bind_indices( std::vector<unsigned int> indices ) ;
    void set_uniform_variables() ;
    void bind_models() ;
    void reorganize_mesh_grid() ;
    void set_models() ;
    float compute_index(int i, int j) ;
    bool is_in_view( glm::vec3 ocean_position ) ;
    void update_view( glm::vec3 camera, glm::mat4 v, glm::vec3 front ) ;
    void update_light( glm::mat4 view ) ;
    void load_textures() ;
    void activate() ;
    void activate_textures() ;
    void generate_error( std::string msg ) ;
    glm::mat4 get_model_matrix( glm::vec3 boatPosition ) ;
    ~Ground() ;
    
    
    
    
    void expand_ocean() ;
} ;
