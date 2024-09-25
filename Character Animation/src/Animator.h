#include "Model.h"





class Animator{
    
private:
    
    bool update ; 
    glm::mat4 model ;
    float keyframe, angle, steering_angle ;
    Bone * root_bone ;
    int active_animation ;
    std::vector<Mesh*> meshes ;
    glm::vec3 direction, position ;
    std::queue<int> animation_queue ;
    std::vector<bool> is_precomputed ;
    std::vector<float> num_keyframes ;
    std::map<std::string, int> animation_mapping ;
    
public:
    
    Animator( Bone * rootbone, std::vector<Mesh*> mshes, std::vector<float> numkeyframes, std::vector<bool> isprecomputed, std::map<std::string, int> animationmapping ) ;
    void set_direction( float requested_steer_angle ) ;
    void set_animation( std::string animation ) ;
    bool animate() ;
    void move() ;
    void execute_animation() ;
    void apply_animations( Bone * bone ) ;
    void apply_transformation( Bone * bone ) ;
    void reset_vertices() ;
    void update_vertices() ;
    void update_vertex( int mesh_index, int vertex_index ) ;
    glm::mat4 compute_animations( Bone * bone, int active_animation, float ratio ) ;
    DecomposedTransformation interpolate( DecomposedTransformation dt1, DecomposedTransformation dt2, float factor ) ;
    DecomposedTransformation decompose( glm::mat4 transformation ) ;
    glm::mat4 recompose( DecomposedTransformation dt ) ;
    float easeInOut( float t ) ;
    bool reset_animation() ;
    glm::mat4 get_model() ; 
    ~Animator() ;
    
} ;
