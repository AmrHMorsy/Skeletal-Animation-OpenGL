#include "Includes.h"




class Parser {
    
private:
        
    int b, rig_index ;
    Bone * root_bone, * rig ;
    std::vector<Mesh*> meshes ;
    std::vector<bool> is_precomputed ;
    std::vector<float> num_keyframes ;
    std::map<std::string, Bone*> bone_mapping ;
    std::map<std::string, int> animation_mapping ;
    
public:

    Parser() ;
    void parse( std::string file_name ) ;
    void parse_fbx( std::string file_name ) ;
    void find_rig_index( const aiScene * scene ) ;
    void parse_animations( const aiScene * scene ) ;
    void parse_animation( const aiScene * scene, unsigned int i ) ;
    void parse_channel( const aiScene * scene, unsigned int i, unsigned int j ) ;
    void parse_keyframe( Bone * bone, const aiScene * scene, unsigned int i, unsigned int j, glm::mat4 offset ) ;
    void parse_precomputed_animations( Bone * bone, const aiScene * scene, unsigned int i, unsigned int j, glm::mat4 offset ) ;
    glm::mat4 extract_transformation( aiNodeAnim * channel, int keyframe ) ;
    void parse_scene_graph( const aiScene * scene ) ;
    void parse_bone_heirarchy( aiNode * node, const aiScene * scene, Bone * parent ) ;
    void traverse_meshes( aiNode * node, const aiScene * scene, aiNode * parent ) ;
    Mesh * parse_mesh( aiMesh * mesh, const aiScene * scene ) ;
    void extract_vertices( aiMesh * mesh, Mesh * m ) ;
    void extract_indices( aiMesh * mesh, Mesh * m ) ;
    void extract_albedo( aiMesh * mesh, const aiScene * scene, Mesh * m ) ;
    void extract_bones( aiMesh * mesh, Mesh * m ) ;
    Bone * retrieve_bone( std::string bone_name ) ;
    std::vector<unsigned int> get_affected_vertices( aiBone * bone, Bone * b, Mesh * m ) ;
    void register_bone_vertex_dependency( Mesh * m, Bone * bone, unsigned int vertex_index, float weight ) ;
    Bone * search_bone( Bone * node, std::string bone_name ) ;
    glm::mat4 convert_to_GLM( const aiMatrix4x4& aiMat ) ;
    void normalize_weights() ;
    bool isprecomputed( std::string &animation_name ) ;
    std::map<std::string, int> get_animation_mapping() ;
    std::vector<Mesh*> get_meshes() ;
    std::map<std::string, Bone*> get_bone_mapping() ;
    Bone * get_root_bone() ;
    std::vector<float> get_num_keyframes() ;
    std::vector<bool> get_precomputed_animations() ;
    void generate_error( std::string error_msg ) ;
    ~Parser() ;
    
} ;
