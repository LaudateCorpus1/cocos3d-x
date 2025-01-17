/*
 * Cocos3D-X 1.0.0
 * Author: Bill Hollings
 * Copyright (c) 2010-2014 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Copyright (c) 2014-2015 Jason Wang
 * http://www.cocos3dx.org/
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * http://en.wikipedia.org/wiki/MIT_License
 */
#ifndef _CC3_POD_RESOURCE_H_
#define _CC3_POD_RESOURCE_H_
#include "CC3PVRTModelPOD.h"

NS_COCOS3D_BEGIN
/**
 * CC3PODResource is a CC3NodesResource that wraps a PVR POD data structure loaded from a file.
 * It handles loading object data from POD files, and creating CC3Nodes from that data. This
 * class is the cornerstone of POD file management, and is typically one of only two POD-based
 * classes that your application needs to be aware of, the other being CC3PODResourceNode,
 * which is a CC3ResourceNode that, in turn, wraps an instance of this class. 
 *
 * CC3PODResource includes many properties and methods geared towards extracing object data
 * from the underlying complex POD resource structure. However, most of the properties and
 * methods exist as template methods to support internal behaviour and for overriding in
 * subclasses that might customize object creation from the POD data.
 *
 * Basic use of this class is straightforward:
 *   -# Allocate and initialize the CC3PODResource instance and load a POD file into the
 *      internal structures. This action also builds all the objects from the resource
 *      data structures (depending on the initializer used, loading can be triggered from
 *      the initializer, or can be performed separately).
 *   -# Access the nodes property to retrieve the fully-built node assembly.
 *
 * The array of nodes accessible via the nodes property are the root nodes of a hierarchical
 * structure of nodes. The loading step takes care of assembling this structural assembly.
 *
 * If this resource contains soft-body components such as skinned meshes, the corresponding
 * skinned mesh nodes and skeleton bone nodes are collected together and wrapped in a single
 * soft body node that appears in the nodes array.
 * 
 * In addition to this core functionality, this class includes many methods for accessing
 * data structures within the resource, and extracting object content from those data
 * structures, should the application have the need to do so. However, in almost all cases,
 * the basic two-step process of loading and retrieving the node assembly is all that is needed.
 *
 * Much of the building of the node assembly from the underlying data strucutres is handled
 * in template methods that are identified here in the interface for ease of overriding in
 * a customized subclass. Although not necessary, some applications may find it necessary
 * or convenient to override one or more of these template methods to modify the objects that
 * are extracted from the underlying file data, perhaps customizing them for the application,
 * or correcting idiosyncracies that might have been exported into the POD file from a 3D
 * editor. This capability can be useful if you are using a POD file of a 3D model that you
 * did not create yourself, and cannot edit.
 *
 * When customizing a subclass to change the properties of the objects returned, you will
 * most likely override one or more of the ...Class properties or build...AtIndex: methods:
 *
 * In most cases, the overridden method can simply invoke the superclass implementation
 * on this class, and then change the properties of the extracted object. In other cases
 * you may want to extract and return a customized subclass of the object of interest.
 */
class CC3PODResource : public CC3NodesResource 
{
	DECLARE_SUPER( CC3NodesResource );
public:
	CC3PODResource();
	~CC3PODResource();
	/**
	 * The underlying C++ CPVRTModelPOD class. It is defined here as a generic pointer
	 * so that it can be imported into header files without the need for the including
	 * file to support C++ This must be cast to a pointer to CPVRTModelPOD before accessing
	 * any elements within the class.
	 */
	PODClassPtr					getPvrtModel();

	/**
	 * The total number of nodes of all types in the POD file.
	 *
	 * This is a transient property that returns a valid value only during node building.
	 * Once node building is complete, this property will return zero.
	 */
	GLuint						getNodeCount();

	/**
	 * A collection of all of the nodes extracted from the POD file.
	 * This is the equivalent of flattening the nodes array.
	 */
	CCArray*					getAllNodes();

	/**
	 * The number of mesh nodes in the POD file.
	 *
	 * This is a transient property that returns a valid value only during node building.
	 * Once node building is complete, this property will return zero.
	 */
	GLuint						getMeshNodeCount();

	/**
	 * The number of lights in the POD file.
	 *
	 * This is a transient property that returns a valid value only during node building.
	 * Once node building is complete, this property will return zero.
	 */
	GLuint						getLightCount();

	/**
	 * The number of cameras in the POD file.
	 *
	 * This is a transient property that returns a valid value only during node building.
	 * Once node building is complete, this property will return zero.
	 */
	GLuint						getCameraCount();

	/**
	 * The number of meshes in the POD file.
	 *
	 * This is a transient property that returns a valid value only during node building.
	 * Once node building is complete, this property will return zero.
	 */
	GLuint						getMeshCount();

	/** A collection of the CC3Meshs extracted from  the POD file. */
	CCArray*					getMeshes();

	/**
	 * The number of materials in the POD file.
	 *
	 * This is a transient property that returns a valid value only during node building.
	 * Once node building is complete, this property will return zero.
	 */
	GLuint						getMaterialCount();

	/** A collection of the CC3Materials extracted from  the POD file. */
	CCArray*					getMaterials();

	/**
	 * The number of textures in the POD file.
	 *
	 * This is a transient property that returns a valid value only during node building.
	 * Once node building is complete, this property will return zero.
	 */
	GLuint						getTextureCount();

	/** A collection of the CC3Textures extracted from  the POD file. */
	CCArray*					getTextures();

	/** The number of frames of animation in the POD file. */
	GLuint						getAnimationFrameCount();

	/** The frame rate of animation in the POD file, in frames per second. */
	GLfloat						getAnimationFrameRate();

	/** The color of the ambient light in the scene. */
	ccColor4F					getAmbientLight();

	/** The background color of the scene. */
	ccColor4F					getBackgroundColor();

	/**
	 * Indicates whether the build method should be invoked automatically when the file is loaded.
	 *
	 * The initial value of this property is YES. This property must be set before the loadFromFile:
	 * method is invoked. Be aware that the loadFromFile: method is automatically invoked automatically
	 * by several instance initializers. To use this property, initialize this instance with an
	 * initializer method that does not invoke the loadFromFile: method.
	 */
	bool						shouldAutoBuild();
	void						setShouldAutoBuild( bool autoBuild );

	/**
	 * Template method that extracts and builds all components. This is automatically invoked from
	 * the loadFromFile: method if the POD file was successfully loaded, and the shouldAutoBuild
	 * property is set to YES. Autobuilding is the default behaviour, and usually, the application
	 * should not need to invoke this method directly.
	 * 
	 * The order of component extraction and building is:
	 *   - textures, by invoking the buildTextures template method
	 *   - materials, by invoking the buildMaterials template method
	 *   - mesh models, by invoking the buildMeshes template method
	 *   - nodes, by invoking the buildNodes template method
	 *   - a soft body node if needed
	 *
	 * This template method can be overridden in a subclass if specialized processing is required.
	 */
	void						build();

	/**
	 * Saves the content of this resource to the file at the specified file path and returns whether
	 * the saving was successful.
	 *
	 * The specified file path may be either an absolute path, or a path relative to the application
	 * resource directory. If the file is located directly in the application resources directory,
	 * the specified file path can simply be the name of the file.
	 *
	 * The build method releases loaded POD content from memory once the file content has been extracted
	 * and into component objects. As a result, content may not be saved back to file after the build
	 * method has been invoked, and this method will raise an assertion error if this method is invoked
	 * after content has been released.
	 *
	 * The build method is invoked automatically from the loadFromFile: method and several initializer
	 * methods that invoke the loadFromFile: method if the shouldAutoBuild property is set to its default
	 * YES value. To use this method, initialize this instance with an initializer method that does not
	 * invoke the loadFromFile: method, set the shouldAutoBuild property to NO. Then, invoke the
	 * loadFromFile: method, make any changes, and invoke this method to save the content back to a file.
	 * Once saved, the build method can then be invoked to extract the content into component objects.
	 */
	bool						saveToFile( const std::string& filePath );

	/**
	 * Saves the animation content of this resource to the file at the specified file path and
	 * returns whether the saving was successful. Animation content includes the nodes that have
	 * animation defined. All other content, including meshes, materials and textures are stripped
	 * from the POD resource that is saved. The POD content in this instance is not affected.
	 *
	 * The specified file path may be either an absolute path, or a path relative to the application
	 * resource directory. If the file is located directly in the application resources directory,
	 * the specified file path can simply be the name of the file.
	 *
	 * The build method releases loaded POD content from memory once the file content has been extracted
	 * and into component objects. As a result, content may not be saved back to file after the build
	 * method has been invoked, and this method will raise an assertion error if this method is invoked
	 * after content has been released.
	 *
	 * The build method is invoked automatically from the loadFromFile: method and several initializer
	 * methods that invoke the loadFromFile: method if the shouldAutoBuild property is set to its default
	 * YES value. To use this method, initialize this instance with an initializer method that does not
	 * invoke the loadFromFile: method, set the shouldAutoBuild property to NO. Then, invoke the
	 * loadFromFile: method, make any changes, and invoke this method to save the content back to a file.
	 * Once saved, the build method can then be invoked to extract the content into component objects.
	 */
	bool						saveAnimationToFile( const std::string& filePath );

	/** Returns the node at the specified index in the allNodes array. */
	CC3Node*					getNodeAtIndex( GLuint nodeIndex );

	/** Returns the node with the specified name from the allNodes array. */
	CC3Node*					getNodeNamed( const std::string& aName );
	 
	/**
	 * Template method that extracts and sets the scene info, including the following properties:
	 *   - animationFrameCount
	 *   - animationFrameRate
	 *   - ambientLight
	 *   - backgroundColor
	 *
	 * This template method can be overridden in a subclass if specialized processing is required.
	 */
	void						buildSceneInfo();

	/**
	 * Template method that extracts and builds the nodes from the underlying data.
	 * This is automatically invoked from the build method.
	 * The application should not invoke this method directly.
	 *
	 * This template method can be overridden in a subclass if specialized processing is required.
	 */
	void						buildNodes();

	/**
	 * Builds the node at the specified index. Checks the type of node, and invokes one
	 * of the following template methods:
	 *   - buildMeshNodeAtIndex:
	 *   - buildLightAtIndex:
	 *   - buildCameraAtIndex:
	 *   - buildStructuralNodeAtIndex:
	 *
	 * This is automatically invoked from the buildNodes method.
	 * The application should not invoke this method directly.
	 *
	 * This template method can be overridden in a subclass if specialized processing is required.
	 */
	CC3Node*					buildNodeAtIndex( GLuint nodeIndex );

	/**
	 * Builds the structural node at the specified index.
	 * 
	 * This is automatically invoked from the buildNodeAtIndex: method.
	 * The application should not invoke this method directly.
	 *
	 * This template method can be overridden in a subclass to adjust the properties
	 * of the new node. The subclass can invoke this superclass method, and then change
	 * properties as required.
	 */
	CC3Node*					buildStructuralNodeAtIndex( GLuint nodeIndex );

	/**
	 * Returns the underlying SPODNode data structure from the POD file, for the SPODNode
	 * at the specified index.
	 *
	 * The returned pointer must be cast to SPODNode before accessing any internals of
	 * the data structure.
	 */
	PODStructPtr				getNodePODStructAtIndex( GLuint nodeIndex );

	/**
	 * Returns whether the specified node index is an ancestor of the specified
	 * child node index. If it is, once the nodes are assembled into their structural
	 * hierarchy, the node with the specified child index will be a descendant of the
	 * specified node index.
	 */
	bool						isNodeIndex( int aNodeIndex, int childIndex );

	/**
	 * Returns whether the specified node index represents a bone node that is part
	 * of a skeleton node assembly that will be used to control vertex skinning.
	 */
	bool						isBoneNode( GLuint nodeIndex );

	/**
	 * If this resource contains soft-body components such as skinned meshes, the corresponding
	 * skinned mesh nodes and skeleton bone nodes are collected together and wrapped in a single
	 * soft body node.
	 */
	void						buildSoftBodyNode();

	/**
	 * Returns the meshIndex'th mesh node.
	 * Note that meshIndex is an ordinal number indicating the rank of the mesh node,
	 * and is not a direct index into the allNodes array.
	 */
	CC3MeshNode*				getMeshNodeAtIndex( GLuint meshIndex );

	/**
	 * Builds the meshIndex'th mesh node.
	 * Note that meshIndex is an ordinal number indicating the rank of the mesh node.
	 * 
	 * This is automatically invoked from the buildNodeAtIndex: method.
	 * The application should not invoke this method directly.
	 *
	 * This template method can be overridden in a subclass to adjust the properties
	 * of the new node. The subclass can invoke this superclass method, and then change
	 * properties as required.
	 */
	CC3MeshNode*				buildMeshNodeAtIndex( GLuint meshIndex );

	/**
	 * Returns the SPODNode structure of the meshIndex'th mesh node.
	 * Note that meshIndex is an ordinal number indicating the rank of the mesh node.
	 *
	 * The returned pointer must be cast to SPODNode before accessing any internals of
	 * the data structure.
	 */
	PODStructPtr				getMeshNodePODStructAtIndex( GLuint meshIndex );

	/**
	 * Returns the meshIndex'th mesh.
	 * Note that meshIndex is an ordinal number indicating the rank of the mesh.
	 */
	CC3Mesh*					getMeshAtIndex( GLuint meshIndex );

	/**
	 * Template method that extracts and builds the meshes from the underlying data.
	 * This is automatically invoked from the build method.
	 * The application should not invoke this method directly.
	 *
	 * This template method can be overridden in a subclass if specialized processing is required.
	 */
	void						buildMeshes();

	/**
	 * Builds the meshIndex'th mesh.
	 * Note that meshIndex is an ordinal number indicating the rank of the mesh.
	 */
	CC3Mesh*					buildMeshAtIndex( GLuint meshIndex );

	/**
	 * Returns meshIndex'th SPODMesh structure from the data structures.
	 * Note that meshIndex is an ordinal number indicating the rank of the mesh.
	 *
	 * The returned pointer must be cast to SPODMesh before accessing any internals of
	 * the data structure.
	 */
	PODStructPtr				getMeshPODStructAtIndex( GLuint meshIndex );

	/**
	 * Returns the lightIndex'th light node.
	 * Note that lightIndex is an ordinal number indicating the rank of the light node,
	 * and is not a direct index into the allNodes array.
	 */
	CC3Light*					getLightAtIndex( GLuint lightIndex );

	/**
	 * Builds the lightIndex'th light node.
	 * Note that lightIndex is an ordinal number indicating the rank of the light node.
	 * 
	 * This is automatically invoked from the buildNodeAtIndex: method.
	 * The application should not invoke this method directly.
	 *
	 * This template method can be overridden in a subclass to adjust the properties
	 * of the new node. The subclass can invoke this superclass method, and then change
	 * properties as required.
	 */
	virtual CC3Light*			buildLightAtIndex( GLuint lightIndex );

	/**
	 * Returns the SPODNode structure of the lightIndex'th light node.
	 * Note that lightIndex is an ordinal number indicating the rank of the light node.
	 *
	 * The returned pointer must be cast to SPODNode before accessing any internals of
	 * the data structure.
	 */
	PODStructPtr				getLightNodePODStructAtIndex( GLuint lightIndex );

	/**
	 * Returns lightIndex'th SPODLight structure from the data structures.
	 *
	 * The returned pointer must be cast to SPODLight before accessing any internals of
	 * the data structure.
	 */
	PODStructPtr				getLightPODStructAtIndex( GLuint lightIndex );

	/**
	 * Returns the cameraIndex'th camera node.
	 * Note that cameraIndex is an ordinal number indicating the rank of the camera node,
	 * and is not a direct index into the allNodes array.
	 */
	CC3Camera*					getCameraAtIndex( GLuint cameraIndex );

	/**
	 * Builds the cameraIndex'th camera node.
	 * Note that cameraIndex is an ordinal number indicating the rank of the camera node.
	 * 
	 * This is automatically invoked from the buildNodeAtIndex: method.
	 * The application should not invoke this method directly.
	 *
	 * This template method can be overridden in a subclass to adjust the properties
	 * of the new node. The subclass can invoke this superclass method, and then change
	 * properties as required.
	 */
	CC3Camera*					buildCameraAtIndex( GLuint cameraIndex );

	/**
	 * Returns the SPODNode structure of the cameraIndex'th light node.
	 * Note that cameraIndex is an ordinal number indicating the rank of the camera node.
	 *
	 * The returned pointer must be cast to SPODNode before accessing any internals of
	 * the data structure.
	 */
	PODStructPtr				getCameraNodePODStructAtIndex( GLuint cameraIndex );

	/**
	 * Returns cameraIndex'th SPODCamera structure from the data structures.
	 *
	 * The returned pointer must be cast to SPODCamera before accessing any internals of
	 * the data structure.
	 */
	PODStructPtr				getCameraPODStructAtIndex( GLuint cameraIndex );

	/**
	 * Returns the materialIndex'th material.
	 * Note that materialIndex is an ordinal number indicating the rank of the material.
	 */
	CC3Material*				getMaterialAtIndex( GLuint materialIndex );

	/** Returns the material with the specified name from the materials array. */
	CC3Material*				getMaterialNamed( const std::string& aName );

	/**
	 * Template method that extracts and builds the materials from the underlying data.
	 * This is automatically invoked from the build method.
	 * The application should not invoke this method directly.
	 *
	 * This template method can be overridden in a subclass if specialized processing is required.
	 */
	void						buildMaterials();

	/**
	 * Builds the materialIndex'th material.
	 * Note that materialIndex is an ordinal number indicating the rank of the material.
	 * 
	 * This is automatically invoked from the buildMaterials method.
	 * The application should not invoke this method directly.
	 *
	 * This template method can be overridden in a subclass to adjust the properties
	 * of the new node. The subclass can invoke this superclass method, and then change
	 * properties as required.
	 */
	virtual CC3Material*		buildMaterialAtIndex( GLuint materialIndex );

	/**
	 * Returns materialIndex'th SPODMaterial structure from the data structures.
	 * Note that materialIndex is an ordinal number indicating the rank of the material.
	 *
	 * The returned pointer must be cast to SPODMaterial before accessing any internals of
	 * the data structure.
	 */
	PODStructPtr				getMaterialPODStructAtIndex( GLuint materialIndex );

	/**
	 * Returns the textureIndex'th texture.
	 * Note that textureIndex is an ordinal number indicating the rank of the texture.
	 */
	CC3Texture*					getTextureAtIndex( GLuint textureIndex );

	/**
	 * Template method that extracts and builds the textures from the underlying data.
	 * This is automatically invoked from the build method.
	 * The application should not invoke this method directly.
	 *
	 * This template method can be overridden in a subclass if specialized processing is required.
	 */
	void						buildTextures();

	/**
	 * Builds the textureIndex'th texture.
	 * Note that textureIndex is an ordinal number indicating the rank of the texture.
	 * 
	 * This is automatically invoked from the buildTextures method.
	 * The application should not invoke this method directly.
	 *
	 * This template method can be overridden in a subclass to adjust the properties
	 * of the new node. The subclass can invoke this superclass method, and then change
	 * properties as required.
	 */
	CC3Texture*					buildTextureAtIndex( GLuint textureIndex );

	/**
	 * Returns textureIndex'th SPODTexture structure from the data structures.
	 * Note that textureIndex is an ordinal number indicating the rank of the texture.
	 *
	 * The returned pointer must be cast to SPODTexture before accessing any internals of
	 * the data structure.
	 */
	PODStructPtr				getTexturePODStructAtIndex( GLuint textureIndex );

	void						deleteCPVRTModelPOD();
	CPVRTModelPOD*				getPvrtModelImpl();
	void						createCPVRTModelPOD();
	virtual bool				init();
	bool						processFile( const std::string& anAbsoluteFilePath );
	std::string					fullDescription();
	static CC3PODResource*		resourceFromFile( const std::string& filePath );
    
private:
    int                         getNodeType( GLuint podIndex );
    void                        linkToPODNodes( CC3Node* pNode, int parentIndex, int targetIndex, CCArray* nodeArray );

protected:
	PODClassPtr					_pvrtModel;
	CCArray*					_allNodes;
	CCArray*					_meshes;
	CCArray*					_materials;
	CCArray*					_textures;
	ccTexParams					_textureParameters;
	ccColor4F					_ambientLight;
	ccColor4F					_backgroundColor;
	GLuint						_animationFrameCount;
	GLfloat						_animationFrameRate;
	bool						_shouldAutoBuild : 1;
};


NS_COCOS3D_END

#endif
