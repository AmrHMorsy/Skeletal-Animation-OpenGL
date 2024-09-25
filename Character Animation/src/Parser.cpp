#include "Parser.h"



Parser::Parser()
{
    rig_index = -1 ;
}

void Parser::parse( std::string file_name )
{
    Assimp::Importer importer ;
    const aiScene * scene = importer.ReadFile( file_name, aiProcess_Triangulate | aiProcess_GenNormals ) ;
    if( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
        generate_error( importer.GetErrorString() ) ;
    
    traverse_meshes( scene->mRootNode, scene, NULL ) ;
}

void Parser::parse_fbx( std::string file_name )
{
    Assimp::Importer importer ;
    const aiScene * scene = importer.ReadFile( file_name, aiProcess_Triangulate ) ;
    if( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
        generate_error( importer.GetErrorString() ) ;
    
    find_rig_index( scene ) ;
    parse_bone_heirarchy( scene->mRootNode->mChildren[rig_index] , scene, NULL ) ;
    parse_scene_graph( scene ) ;
    normalize_weights() ;
    parse_animations( scene ) ;
}

void Parser::find_rig_index( const aiScene * scene )
{
    for( int i = 0; i < scene->mRootNode->mNumChildren; i++) {
        if( std::string(scene->mRootNode->mChildren[i]->mName.C_Str()) == "Rig")
            rig_index = i ;
    }
    if( rig_index == -1 )
        generate_error( "Could Not Find Rig" ) ;
}

void Parser::parse_animations( const aiScene * scene )
{
    is_precomputed.resize( scene->mNumAnimations ) ;
    num_keyframes.resize( scene->mNumAnimations ) ;
    for( unsigned int i = 0 ; i < scene->mNumAnimations ; i++ )
        parse_animation( scene, i ) ;
}

void Parser::parse_animation( const aiScene * scene, unsigned int i )
{
    std::string animation_name = scene->mAnimations[i]->mName.C_Str() ;
    if( std::string(animation_name) == "Rig|Stand" )
        b = i ;
    is_precomputed[i] = isprecomputed( animation_name ) ;
    animation_mapping[ animation_name ] = i ;
    num_keyframes[i] = scene->mAnimations[i]->mDuration + 1 ;
    
    for( unsigned int j = 0 ; j < scene->mAnimations[i]->mNumChannels ; j++ )
        parse_channel( scene, i, j ) ;
}

void Parser::parse_channel( const aiScene * scene, unsigned int i, unsigned int j )
{
    std::string bone_name = scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str() ;
    Bone * bone = bone_mapping[ bone_name ] ;
    if( !bone )
        generate_error( "Could Not Find Channel " + bone_name ) ;
    glm::mat4 offset = extract_transformation( scene->mAnimations[b]->mChannels[j], 0 ) ;
    offset = glm::inverse( offset ) ;
    parse_keyframe( bone, scene, i, j, offset ) ;
}

void Parser::parse_keyframe( Bone * bone, const aiScene * scene, unsigned int i, unsigned int j, glm::mat4 offset )
{
    if( !is_precomputed[i] ){
        bone->active[i] = std::vector<bool>(num_keyframes[i], true) ;
        bone->animations[i] = offset * extract_transformation( scene->mAnimations[i]->mChannels[j], scene->mAnimations[i]->mChannels[j]->mNumPositionKeys - 1 ) ;
    }
    else
        parse_precomputed_animations( bone, scene, i, j, offset ) ;
}

void Parser::parse_precomputed_animations( Bone * bone, const aiScene * scene, unsigned int i, unsigned int j, glm::mat4 offset )
{
    bone->active[i] = std::vector<bool>( num_keyframes[i], false ) ;
    bone->precomputed_animations[i].resize( num_keyframes[i] ) ;
    for( unsigned int k = 1 ; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys ; k++ ){
        bone->precomputed_animations[i][k] = offset * extract_transformation(scene->mAnimations[i]->mChannels[j], k) ;
        bone->active[i][k] = true ;
    }
    for( unsigned int k = scene->mAnimations[i]->mChannels[j]->mNumPositionKeys ; k < num_keyframes[i] ; k++ )
        bone->precomputed_animations[i][k] = bone->precomputed_animations[i][scene->mAnimations[i]->mChannels[j]->mNumPositionKeys-1] ;
}

glm::mat4 Parser::extract_transformation( aiNodeAnim * channel, int keyframe )
{
    aiVector3D t = channel->mPositionKeys[keyframe].mValue ;
    glm::vec3 translate( t.x, t.y, t.z ) ;
    glm::mat4 translation = glm::translate( glm::mat4(1.0f), translate ) ;
    
    aiQuaternion q = channel->mRotationKeys[keyframe].mValue ;
    glm::quat quaternion( q.w, q.x, q.y, q.z ) ;
    glm::mat4 rotation = glm::toMat4( quaternion ) ;
    
    aiVector3D s = channel->mScalingKeys[keyframe].mValue ;
    glm::vec3 scale( s.x, s.y, s.z ) ;
    glm::mat4 scaling = glm::scale( glm::mat4(1.0f), scale ) ;
    
    return translation * rotation * scaling ;
}

void Parser::parse_scene_graph( const aiScene * scene )
{
    for( int i = 0  ; i < scene->mRootNode->mNumChildren ; i++ ){
        if( (std::string(scene->mRootNode->mChildren[i]->mName.C_Str()) == "Camera") || (std::string(scene->mRootNode->mChildren[i]->mName.C_Str()) == "Light") )
            continue ;
        traverse_meshes( scene->mRootNode->mChildren[i], scene, NULL ) ;
    }
    rig = root_bone ;
    root_bone = root_bone->children[0] ;
    bone_mapping[scene->mRootNode->mChildren[rig_index]->mChildren[0]->mName.C_Str()] = root_bone;
    bone_mapping[scene->mRootNode->mChildren[rig_index]->mName.C_Str()] = rig ;
}

void Parser::parse_bone_heirarchy( aiNode * node, const aiScene * scene, Bone * parent )
{
    Bone * bone = new Bone() ;
    
    if( parent ){
        bone->parent = parent ;
        parent->children.push_back( bone ) ;
    }
    else{
        root_bone = bone ;
        bone->parent = NULL ;
    }
    bone->name = node->mName.C_Str() ;
    bone->transformation = glm::mat4(1.0f) ;
    bone->local_transformation = glm::mat4( 1.0f ) ;
    bone->animations.resize(scene->mNumAnimations) ;
    bone->active.resize(scene->mNumAnimations) ;
    bone->precomputed_animations.resize( scene->mNumAnimations ) ;
    for( int i = 0 ; i < scene->mNumAnimations ; i++ )
        bone->animations[i] = glm::mat4(1.0f) ;
    bone->offset = glm::mat4( 1.0f ) ;
    bone->inverse_offset = glm::mat4( 1.0f ) ;
    for( unsigned int i = 0 ; i < node->mNumChildren ; i++ )
        parse_bone_heirarchy( node->mChildren[i], scene, bone ) ;
}

void Parser::traverse_meshes( aiNode * node, const aiScene * scene, aiNode * parent )
{
    for(unsigned int i = 0 ; i < node->mNumMeshes; i++)
        meshes.push_back( parse_mesh( scene->mMeshes[node->mMeshes[i]], scene ) ) ;

    for (unsigned int i = 0 ; i < node->mNumChildren; i++)
        traverse_meshes( node->mChildren[i], scene, node ) ;
}

Mesh * Parser::parse_mesh( aiMesh * mesh, const aiScene * scene )
{
    Mesh * m = new Mesh() ;
    
    extract_vertices( mesh, m ) ;
    
    extract_indices( mesh, m ) ;
    
    extract_albedo( mesh, scene, m ) ;

    extract_bones( mesh, m ) ;
    
    return m ;
}

void Parser::extract_vertices( aiMesh * mesh, Mesh * m )
{
    m->bones.resize( mesh->mNumVertices ) ;
    m->weights.resize( mesh->mNumVertices ) ;

    for( unsigned int j = 0 ; j < mesh->mNumVertices ; j++ ){
        
        m->vertices.push_back( mesh->mVertices[j].x ) ;
        m->vertices.push_back( mesh->mVertices[j].y ) ;
        m->vertices.push_back( mesh->mVertices[j].z ) ;
        if( mesh->mTextureCoords[0] ){
            m->vertices.push_back( mesh->mTextureCoords[0][j].x ) ;
            m->vertices.push_back( mesh->mTextureCoords[0][j].y ) ;
        }
        m->vertices.push_back( mesh->mNormals[j].x ) ;
        m->vertices.push_back( mesh->mNormals[j].y ) ;
        m->vertices.push_back( mesh->mNormals[j].z ) ;
    }
    
    m->original_vertices = m->vertices ;
}

void Parser::extract_indices( aiMesh * mesh, Mesh * m )
{
    for( unsigned int j = 0 ; j < mesh->mNumFaces ; j++ ) {
        for( unsigned int k = 0 ; k < mesh->mFaces[j].mNumIndices ; k++ )
            m->indices.push_back( mesh->mFaces[j].mIndices[k] ) ;
    }
}

void Parser::extract_albedo( aiMesh * mesh, const aiScene * scene, Mesh * m )
{
    if( scene->mMaterials[mesh->mMaterialIndex] ){
        float roughness, metallic ;
        aiColor3D color(0.0f, 0.0f, 0.0f) ;
        aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex] ;
        if( material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS )
            m->albedo = glm::vec3(color[0], color[1], color[2]) ;
        if (material->Get("ROUGHNESS_FACTOR", 0, 0, roughness) != AI_SUCCESS)
            m->roughness = 1.0f ;
        if( material->Get("METALLIC_FACTOR", 0, 0, metallic) != AI_SUCCESS )
            m->metallic = 0.0f ;
        m->metallic = metallic ;
        m->roughness = roughness ; 
    }
}

void Parser::extract_bones( aiMesh * mesh, Mesh * m )
{
    for( unsigned int j = 0 ; j < mesh->mNumBones ; j++ ) {
        std::string bone_name = mesh->mBones[j]->mName.data ;
        Bone * bone = retrieve_bone( bone_name ) ;
        if( bone ){
            bone->offset = convert_to_GLM( mesh->mBones[j]->mOffsetMatrix ) ;
            bone->inverse_offset = glm::inverse( bone->offset ) ;
            bone->local_transformation = glm::mat4( 1.0f ) ;
            bone->transformation = glm::mat4( 1.0f ) ;
            bone->affected_vertices.push_back( get_affected_vertices(mesh->mBones[j], bone, m) ) ;
            bone_mapping[ bone_name ] = bone ;
        }
        else
            generate_error( "Could Not Find Bone " + bone_name ) ;
    }
}

Bone * Parser::retrieve_bone( std::string bone_name )
{
    std::map<std::string, Bone*>::iterator it = bone_mapping.find( bone_name ) ;
    
    if( it != bone_mapping.end() )
        return it->second ;
    else
        return search_bone( root_bone, bone_name ) ;
}

std::vector<unsigned int> Parser::get_affected_vertices( aiBone * bone, Bone * b, Mesh * m )
{
    std::vector<unsigned int> affected_vertices ;
        
    for( unsigned int k = 0 ; k < bone->mNumWeights ; k++ ){
        affected_vertices.push_back( bone->mWeights[k].mVertexId ) ;
        register_bone_vertex_dependency( m, b, bone->mWeights[k].mVertexId, bone->mWeights[k].mWeight ) ;
    }
    
    return affected_vertices ;
}

void Parser::register_bone_vertex_dependency( Mesh * m, Bone * bone, unsigned int vertex_index, float weight )
{
    m->bones[vertex_index].push_back( bone ) ;
    m->weights[vertex_index].push_back( weight ) ;
}

Bone * Parser::search_bone( Bone * node, std::string bone_name )
{
    if( node->name == bone_name )
        return node ;
    
    for( int i = 0 ; i < node->children.size() ; i++ ){
        Bone * bone = search_bone( node->children[i], bone_name ) ;
        if( bone )
            return bone ;
    }
    
    return NULL ;
}

glm::mat4 Parser::convert_to_GLM( const aiMatrix4x4& aiMat )
{
    return glm::mat4(
        aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
        aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
        aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
        aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
    ) ;
}

void Parser::normalize_weights()
{
    for( int i = 0 ; i < meshes.size() ; i++ ){
        for( int j = 0 ; j < meshes[i]->weights.size() ; j++ ){
            float total = 0 ;
            for( int k = 0 ; k < meshes[i]->weights[j].size() ; k++ )
                total += meshes[i]->weights[j][k] ;
            for( int k = 0 ; k < meshes[i]->weights[j].size() ; k++ )
                meshes[i]->weights[j][k] /= total ;
        }
    }
}

bool Parser::isprecomputed( std::string &animation_name )
{
    if( animation_name.back() == '*' ){
        animation_name.pop_back() ;
        return true ;
    }
    else
        return false ;
}

std::map<std::string, int> Parser::get_animation_mapping()
{
    return animation_mapping ;
}

std::vector<Mesh*> Parser::get_meshes()
{
    return meshes ;
}

std::map<std::string, Bone*> Parser::get_bone_mapping()
{
    return bone_mapping ;
}

Bone * Parser::get_root_bone()
{
    return root_bone ;
}

std::vector<float> Parser::get_num_keyframes()
{
    return num_keyframes ; 
}

std::vector<bool> Parser::get_precomputed_animations()
{
    return is_precomputed ;
}

void Parser::generate_error( std::string error_msg )
{
    std::cerr << "ERROR -- " << error_msg << std::endl ;
    
    exit(1) ; 
}

Parser::~Parser(){}
