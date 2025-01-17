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
#ifndef _CCL_CC3MESH_H_
#define _CCL_CC3MESH_H_

NS_COCOS3D_BEGIN

/**
 * A CC3Mesh holds the 3D mesh for a CC3MeshNode.
 *
 * In 3D models, the mesh generally remains fixed, and transformations such as translation,
 * rotation, and scaling are applied at the node level. A single CC3Mesh instance, which
 * typically contains a large set of data points, can be used by many nodes simultaneously,
 * and each node instance can be transformed, colored, and textured independently.
 *
 * With this in mind, and following best practices to consevere memory and processing time,
 * you should strive to create only one CC3Mesh instance for each distinct mesh in your
 * application, and assign that single CC3Mesh instance to any number of separate CC3MeshNode
 * instances that make use of it.

 *
 * The vertex content in this mesh is kept in a set of CC3VertexArrays instances. Each of
 * the contained CC3VertexArray instances manages the data for one aspect of the vertices
 * (locations, normals, colors, texture mapping, bone weights...).
 *
 * Vertex data can be interleaved into a single underlying memory buffer that is shared
 * between the contained CC3VertexArrays, or it can be separated into distinct memory
 * buffers for each vertex aspect.
 *
 * The only vertex array that is required is the vertexLocations property. The others are
 * optional, depending on the nature of the mesh. If the vertexIndices property is provided,
 * it will be used during drawing. If ther vertexIndices property is nil, the vertices will
 * be drawn in linear order as they appear in the vertexLocations property.
 *
 * This class supports multi-texturing. In most situations, the mesh will use the same texture
 * mapping for all texture units. In this case, the single texture coordinates array in the
 * vertexTexureCoordinates property will be applied to all textures in the material applied
 * to a CC3MeshNode using this mesh.
 *
 * If multi-texturing is used, and separate texture coordinate mapping is required for each texture
 * unit, additional texture coordinate arrays can be added using the addTextureCoordinates: method.
 *
 * For consistency, the addTextureCoordinates:, removeTextureCoordinates:, and
 * getTextureCoordinatesNamed: methods all interact with the vertexTextureCoordinates
 * property. If that property has not been set, the first texture coordinate array that
 * is added via addTextureCoordinates: will be set into the vertexTextureCoordinates
 * array. And the removeTextureCoordinates:, and getTextureCoordinatesNamed: methods
 * each check the vertexTextureCoordinates property as well as the overlayTextureCoordinates
 * collection. This design can simplify configurations in that all texture coordinate arrays
 * can be treated the same.
 *
 * If there are more textures applied to a node than there are texture coordinate arrays
 * in the mesh (including the vertexTextureCoordinates and the those in the
 * overlayTextureCoordinates collection), the last texture coordinate array is reused.
 *
 * This class supports covering the mesh with a repeating texture through the repeatTexture: method.
 *
 * This class also supports covering the mesh with only a fractional part of the texture
 * through the use of the textureRectangle property, effectlivly permitting sprite-sheet
 * textures to be used with 3D meshes.
 *
 * When a copy is made of a CC3Mesh instance, copies are not made of the vertex arrays. Instead,
 * they are retained by reference and shared between both the original mesh, and the new copy.
 *
 * CC3Mesh manages data for one contiguous set of vertices that can be drawn with a single call
 * to the GL engine, or a single set of draw-strip calls to the GL engine, using the same material
 * properties. To assemble a large, complex mesh containing several distinct vertex groups, assign
 * each vertex group to its own CC3Mesh instance, wrap each mesh instance in a CC3MeshNode instance,
 * and create an structural assembly of the nodes. See the class notes for CC3MeshNode for more
 * information on assembling mesh nodes.
 *
 * When drawing the mesh to the GL engine, this class remembers which mesh was last drawn
 * and only binds the mesh data to the GL engine when a different mesh is drawn. This allows
 * the application to organize the CC3MeshNodes within the CC3Scene so that nodes using the
 * same mesh are drawn together, before moving on to other mesh models. This strategy
 * can minimize the number of mesh switches in the GL engine, which improves performance. 
 *
 * CC3Mesh is an abstract class. Subclasses can be created for loading and managing
 * meshes from different sources and third-party libraries.
 */

class CC3VertexLocations;
class CC3VertexNormals;
class CC3VertexTangents;
class CC3VertexColors;
class CC3VertexTextureCoordinates;
class CC3VertexBoneWeights;
class CC3VertexBoneIndices;
class CC3VertexPointSizes;
class CC3VertexIndices;
class CC3FaceArray;

class CC3Mesh : public CC3Identifiable
{
	DECLARE_SUPER( CC3Identifiable );
public:
	CC3Mesh();
	virtual ~CC3Mesh();

	virtual void				setName( const std::string& name );
	virtual std::string			getNameSuffix();

	/** The vertex array instance managing the positional content for the vertices. */
	CC3VertexLocations*			getVertexLocations();
	void						setVertexLocations( CC3VertexLocations* locations );

	/** Returns whether this mesh contains content for vertex locations. */
	bool						hasVertexLocations();

	/**
	 * The vertex array instance managing the normal content for the vertices.
	 *
	 * Setting this property is optional. Not all meshes require normals.
	 */
	CC3VertexNormals*			getVertexNormals();
	void						setVertexNormals( CC3VertexNormals* normals );

	/** Returns whether this mesh contains content for vertex normals. */
	bool						hasVertexNormals();

	/** Reverses the direction of all of the normals in this mesh. */
	void						flipNormals();

	/**
	 * The vertex array instance managing the tangent content for the vertices.
	 *
	 * Setting this property is optional. Not all meshes require tangents.
	 */
	CC3VertexTangents*			getVertexTangents();
	void						setVertexTangents( CC3VertexTangents* tagents );

	/** Returns whether this mesh contains content for vertex tangents. */
	bool						hasVertexTangents();

	/**
	 * The vertex array instance managing the bitangent content for the vertices.
	 *
	 * Setting this property is optional. Not all meshes require bitangents.
	 */
	CC3VertexTangents*			getVertexBitangents();
	void						setVertexBitangents( CC3VertexTangents* tagents );

	/** Returns whether this mesh contains content for vertex bitangents. */
	bool						hasVertexBitangents();

	/**
	 * The vertex array instance managing the per-vertex color content for the vertices.
	 *
	 * Setting this property is optional. Many meshes do not require per-vertex coloring.
	 */
	CC3VertexColors*			getVertexColors();
	void						setVertexColors( CC3VertexColors* colors );

	/** Returns whether this mesh contains content for vertex colors.  */
	bool						hasVertexColors();

	/**
	 * Returns the symbolic content type of the vertex color, which indicates the range of values
	 * stored for each vertex color.
	 *
	 * This property will return one of the values: GL_FLOAT, GL_UNSIGNED_BYTE, or GL_FIXED,
	 * or will return GL_FALSE if this mesh does not support individual vertex colors.
	 *
	 * You can use the value returned by this property to select whether to access individual vertex
	 * color content as bytes or floats, in order to retain accuracy and avoid unnecessary type conversions.
	 */
	GLenum						getVertexColorType();

	/**
	 * The vertex array that manages the indices of the bones that influence each vertex.
	 *
	 * Each element of the vertex array in this property is a small set of index values that
	 * reference a set of bones that influence the location of that vertex.
	 *
	 * The elementSize property of the vertex arrays in the vertexBoneWeights and vertexBoneIndices
	 * properties must be the same, and under OpenGL ES 1.1, the elementSize value must also not 
	 * be larger than the maximum number of available bone influences allowed by the platform, 
	 * which can be retreived from CC3OpenGL.sharedGL.maxNumberOfBoneInfluencesPerVertex.
	 */
	CC3VertexBoneIndices*		getVertexBoneIndices();
	void						setVertexBoneIndices( CC3VertexBoneIndices* indices );

	/** Indicates whether this mesh contains content for vertex bone indices. */
	bool						hasVertexBoneIndices();

	/**
	 * The vertex array that manages the weighting that each bone has in influencing each vertex.
	 *
	 * Each element of the vertex array in this property contains a small set of weighting values
	 * that determine the relative influence that each of the bones identified for that vertex in
	 * the vertexBoneIndices property has on transforming the location of the vertex.
	 *
	 * The elementSize property of the vertex arrays in the vertexBoneWeights and vertexBoneIndices
	 * properties must be the same, and under OpenGL ES 1.1, the elementSize value must also not
	 * be larger than the maximum number of available bone influences allowed by the platform,
	 * which can be retreived from CC3OpenGL.sharedGL.maxNumberOfBoneInfluencesPerVertex.
	 */
	CC3VertexBoneWeights*		getVertexBoneWeights();
	void						setVertexBoneWeights( CC3VertexBoneWeights* weights );

	/** Indicates whether this mesh contains content for vertex bone weights. */
	bool						hasVertexBoneWeights();

	/**
	 * The vertex array instance managing a point size for each vertex.
	 *
	 * Setting this property is optional. It is used for point particle systems, and even then,
	 * particle systems often do not require individual sizing for each particle.
	 */
	CC3VertexPointSizes*		getVertexPointSizes();
	void						setVertexPointSizes( CC3VertexPointSizes* sizes );

	/** Indicates whether this mesh contains content for vertex point sizes. */
	bool						hasVertexPointSizes();

	/**
	 * The vertex array instance managing the index content for the vertices.
	 *
	 * Setting this property is optional. If vertex index data is not provided, the vertices
	 * will be drawn in linear order as they appear in the vertexLocations property.
	 */
	CC3VertexIndices*			getVertexIndices();
	void						setVertexIndices( CC3VertexIndices* indices );

	/** Returns whether this mesh uses index vertices. */
	bool						hasVertexIndices();

	/**
	 * The vertex array instance managing the texture mapping content for the vertices.
	 *
	 * Setting this property is optional. Not all meshes use textures.
	 *
	 * If multi-texturing is used, and separate texture coordinate mapping is required
	 * for each texture unit, additional texture coordinate arrays can be added using
	 * the addTextureCoordinates: method. If this property has not been set already,
	 * the first texture coordinate array that is added via addTextureCoordinates:
	 * will be placed in this property. This can simplify configurations in that all
	 * texture coordinate arrays can be treated the same.
	 */
	CC3VertexTextureCoordinates* getVertexTextureCoordinates();
	void						setVertexTextureCoordinates( CC3VertexTextureCoordinates* cooridnates );

	/**
	 * Returns the number of texture coordinate arrays used by this mesh, regardless of whether
	 * the texture coordinates were attached using the vertexTextureCoordinates property or the
	 * addTextureCoordinates: method.
	 */
	GLuint						getTextureCoordinatesArrayCount();

	/**
	 * This class supports multi-texturing. In most situations, the mesh will use the same
	 * texture mapping for all texture units. In such a case, the single texture coordinates
	 * array in the vertexTexureCoordinates property will be applied to all texture units.
	 *
	 * However, if multi-texturing is used, and separate texture coordinate mapping is required
	 * for each texture unit, additional texture coordinate arrays can be added using this method.
	 *
	 * If the vertexTextureCoordinates property has not been set already, the first
	 * texture coordinate array that is added via this method will be placed in the
	 * vertexTextureCoordinates property. This can simplify configurations in that all
	 * texture coordinate arrays can be treated the same.
	 *
	 * If there are more textures applied to a node than there are texture coordinate arrays
	 * in the mesh (including the vertexTextureCoordinates and the those in the
	 * overlayTextureCoordinates collection), the last texture coordinate array is reused.
	 */
	void						addTextureCoordinates( CC3VertexTextureCoordinates* aTexCoord );

	/**
	 * Removes the specified texture coordinate array from either the vertexTextureCoordinates
	 * property or from the overlayTextureCoordinates collection.
	 */
	void						removeTextureCoordinates( CC3VertexTextureCoordinates* aTexCoord );

	/**
	 * Removes all texture coordinates arrays from the the vertexTextureCoordinates
	 * property and from the overlayTextureCoordinates collection.
	 */
	void						removeAllTextureCoordinates();

	/**
	 * Returns the overlay texture coordinate array with the specified name,
	 * or nil if it cannot be found. This checks both the vertexTextureCoordinates
	 * property and the overlayTextureCoordinates collection.
	 */
	CC3VertexTextureCoordinates* getTextureCoordinatesNamed( const std::string& aName );

	/**
	 * Returns the texture coordinate array that will be processed by the texture unit
	 * with the specified index.
	 *
	 * If the specified texture unit index is equal to or larger than the number of
	 * texture coordinates arrays, as indicated by the value of the
	 * textureCoordinatesArrayCount property, the texture coordinate array with the
	 * highest index is returned.
	 *
	 * This design reuses the texture coordinate array with the highest index
	 * for all texture units higher than that index.
	 *
	 * The value returned will be nil if there are no texture coordinates.
	 */
	CC3VertexTextureCoordinates* getTextureCoordinatesForTextureUnit( GLuint texUnit );

	/**
	 * Sets the texture coordinates array that will be processed by the texture unit with
	 * the specified index, which should be a number between zero, and the value of the
	 * textureCoordinatesArrayCount property.
	 *
	 * If the specified index is less than the number of texture units added already, the
	 * specified texture coordinates array will replace the one assigned to that texture unit.
	 * Otherwise, this implementation will invoke the addTextureCoordinates: method to add
	 * the texture to this material.
	 *
	 * If the specified texture unit index is zero, the value of the vertexTextureCoordinates
	 * property will be changed to the specified texture.
	 */
	void						setTextureCoordinates( CC3VertexTextureCoordinates* aTexture, GLuint texUnit );

	/** Returns whether this mesh contains data for vertex texture coordinates. */
	bool						hasVertexTextureCoordinates();

	/**
	 * Returns the contained vertex array that contains the vertex content for the specified semantic
	 * and semantic meaning.
	 *
	 * Based on the value of the specified semantic, this method returns the following:
	 *   - 
	 *   - kCC3SemanticVertexLocation:		self.vertexLocations
	 *   - kCC3SemanticVertexNormal:		self.vertexNormals
	 *   - kCC3SemanticVertexTangent:		self.vertexTangents
	 *   - kCC3SemanticVertexBitangent:		self.vertexBitangents
	 *   - kCC3SemanticVertexColor:			self.vertexColors
	 *   - kCC3SemanticVertexBoneWeights:	self.vertexBoneWeights
	 *   - kCC3SemanticVertexBoneIndices:	self.vertexBoneIndices
	 *   - kCC3SemanticVertexPointSize:		self.vertexPointSizes
	 *   - kCC3SemanticVertexTexture:		[self textureCoordinatesForTextureUnit: semanticIndex]
	 */
	CC3VertexArray*				getVertexArrayForSemantic( GLenum semantic, GLuint semanticIndex );

	/**
	 * Indicates whether the vertex content should be interleaved, or separated by type.
	 *
	 * If the vertex content is interleaved, each of the contained CC3VertexArray instances will
	 * reference the same underlying memory buffer through their individual vertices property.
	 *
	 * Interleaving vertex content is recommended, as it improves the GPU's ability to optimize
	 * throughput.
	 *
	 * The value of this property should be set before the values of the vertexContentTypes and
	 * allocatedVertexCapacity are set.
	 *
	 * The initial value is YES, indicating that the vertex content will be interleaved.
	 */
	bool						shouldInterleaveVertices();
	void						setShouldInterleaveVertices( bool shouldInterLeave );

	/**
	 * Indicates the types of content contained in each vertex of this mesh.
	 *
	 * Each vertex can contain several types of content, optionally including location, normal,
	 * color, texture coordinates, along with other specialized content for certain specialized
	 * meshes. To identify this various content, this property is a bitwise-OR of flags that
	 * enumerate the types of content contained in each vertex of this mesh.
	 *
	 * Valid component flags of this property include:
	 *   - kCC3VertexContentLocation
	 *   - kCC3VertexContentNormal
	 *   - kCC3VertexContentTangent
	 *   - kCC3VertexContentBitangent
	 *   - kCC3VertexContentColor
	 *   - kCC3VertexContentTextureCoordinates
	 *   - kCC3VertexContentBoneWeights
	 *   - kCC3VertexContentBoneIndices
	 *   - kCC3VertexContentPointSize
	 *
	 * To indicate that this mesh should contain particular vertex content, construct a
	 * bitwise-OR combination of one or more of the component types listed above, and set
	 * this property to that combined value.
	 *
	 * Setting each bitwise-OR component in this property instructs this instance to
	 * automatically construct the appropriate type of contained vertex array:
	 *   - kCC3VertexContentLocation - automatically constructs a CC3VertexLocations instance in the
	 *     vertexLocations property, that holds 3D vertex locations, in one CC3Vector structure per vertex.
	 *     This component is optional, as the vertexLocations property will be constructed regardless.
	 *   - kCC3VertexContentNormal - automatically constructs a CC3VertexNormals instance in the
	 *     vertexNormals property, that holds 3D vertex normals, in one CC3Vector structure per vertex.
	 *   - kCC3VertexContentTangent - automatically constructs a CC3VertexTangents instance in the
	 *     vertexTangents property, that holds 3D vertex tangents, in one CC3Vector structure per vertex.
	 *   - kCC3VertexContentBitangent - automatically constructs a CC3VertexTangents instance in the
	 *     vertexBitangents property, that holds 3D vertex tangents, in one CC3Vector structure per vertex.
	 *   - kCC3VertexContentColor - automatically constructs a CC3VertexColors instance in the vertexColors
	 *     property, that holds RGBA colors with GLubyte components, in one ccColor4B structure per vertex.
	 *   - kCC3VertexContentTextureCoordinates - automatically constructs a CC3VertexTextureCoordinates
	 *     instance in the vertexTextureCoordinates property, that holds 2D texture coordinates, in one
	 *     ccTex2F structure per vertex.
	 *   - kCC3VertexContentBoneWeights - automatically constructs a CC3VertexBoneWeights instance
	 *     in the vertexBoneWeights property, that holds several GLfloat values per vertex.
	 *   - kCC3VertexContentBoneIndices - automatically constructs a CC3VertexBoneIndices instance
	 *     in the vertexBoneIndices property, that holds several GLubyte or GLushort values per vertex.
	 *   - kCC3VertexContentPointSize - automatically constructs a CC3VertexPointSizes
	 *     instance in the vertexPointSizes property, that holds one GLfloat per vertex.
	 *
	 * This property is a convenience property. Instead of using this property, you can create the
	 * appropriate vertex arrays in those properties directly.
	 *
	 * The vertex arrays constructed by this property will be configured to use interleaved data
	 * if the shouldInterleaveVertices property is set to YES. You should ensure the value of the
	 * shouldInterleaveVertices property to the desired value before setting the value of this property.
	 * The initial value of the shouldInterleaveVertices property is YES.
	 *
	 * If the content is interleaved, for each vertex, the content is held in the structures identified in
	 * the list above, in the order that they appear in the list. You can use this consistent organization
	 * to create an enclosing structure to access all data for a single vertex, if it makes it easier to
	 * access vertex content that way. If vertex content is not specified, it is simply absent, and the content
	 * from the following type will be concatenated directly to the content from the previous type.
	 *
	 * For instance, in a typical textured and illuminated mesh, you might not require per-vertex
	 * color, tangent and bitangent content. You would therefore omit the kCC3VertexContentColor,
	 * kCC3VertexContentTangent and kCC3VertexContentBitangent values in the bitmask when setting
	 * this property, and the resulting structure for each vertex would be a location CC3Vector,
	 * followed by a normal CC3Vector, followed immediately by a texture coordinate ccTex2F.
	 * You can then define an enclosing structure to hold and manage all content for a single vertex.
	 * In this particular example, this is already done for you with the CC3TexturedVertex structure.
	 *
	 * You can declare and use such a custom vertex structure even if you have constructed the vertex
	 * arrays directly, without using this property. The structure of the content of a single vertex
	 * is the same in either case.
	 *
	 * The vertex arrays created by this property cover the most common use cases and data formats.
	 * If you require more customized vertex arrays, you can use this property to create the typical
	 * vertex arrays, and then customize them, by accessing the vertex arrays individually through
	 * their respective properties. After doing so, if the vertex content is interleaved, you should
	 * invoke the updateVertexStride method on this instance to automatically align the elementOffset
	 * and vertexStride properties of all of the contained vertex arrays. After setting this property,
	 * you do not need to invoke the updateVertexStride method unless you subsequently make changes
	 * to the constructed vertex arrays.
	 *
	 * It is safe to set this property more than once. Doing so will remove any existing vertex arrays
	 * and replace them with those indicated by this property.
	 *
	 * When reading this property, the appropriate bitwise-OR values are returned, corresponding
	 * to the contained vertex arrays, even if those arrays were constructed directly, instead
	 * of by setting this property. If this mesh contains no vertex arrays, this property will
	 * return kCC3VertexContentNone.
	 */
	CC3VertexContent			getVertexContentTypes();
	void						setVertexContentTypes( CC3VertexContent contentTypes );

	/**
	 * If the shouldInterleaveVertices property is set to YES, updates the elementOffset and vertexStride
	 * properties of each enclosed vertex array to correctly align them for interleaved data.
	 *
	 * After constructing the vertex arrays in this mesh, and setting the shouldInterleaveVertices property
	 * is set to YES, you can invoke this method to align the vertex arrays for interleaved vertex content.
	 *
	 * If the shouldInterleaveVertices property is set to NO, this method has no effect.
	 *
	 * If you used the vertexContentTypes property to construct the vertex arrays, you do not need to
	 * invoke this method. However, if you subsequently adjusted the elementType or elementSize of any
	 * of the vertex arrays, or if you added additional texture coordinate overlay vertex arrays, you
	 * can invoke this method to align the vertex arrays correctly again.
	 *
	 * The element offsets of the vertex arrays are aligned in the order documented in the notes of the
	 * vertexContentTypes property, even if the vertex arrays were created directly, instead of by setting
	 * the vertexContentTypes property.
	 *
	 * Returns the number of bytes used by the all of the content of one vertex. This value is
	 * calculated and returned regardless of the value of the shouldInterleaveVertices property
	 */
	GLuint						updateVertexStride();

	/**
	 * Allocates, reallocates, or deallocates underlying memory for the specified number of
	 * vertices, taking into consideration the amount of memory required by each vertex.
	 *
	 * Setting this property affects the value of the vertexCount property. After setting this
	 * property, the vertexCount property will be set to the same value as this property. After
	 * setting this property, if you will not be using all of the allocated vertices immediately,
	 * you should set the value of the vertexCount property to the actual number of vertices in use.
	 *
	 * Use of this property is not required if the vertex content has already been loaded into
	 * memory by a file loader, or defined by a static array. In that situation, you should
	 * set the vertexCount property directly, and avoid using this property.
	 *
	 * If the vertex content consists only of vertex locations, you can set this property without
	 * having to define any content, and the CC3VertexLocations instance in the vertexLocations
	 * property will automatically be created.
	 *
	 * However, if the vertex content contains more than just location data, since memory allocation
	 * is dependent on the content required by each vertex, you should set this property only after
	 * the contained vertex arrays have been constructed and configured, either directly, or via the
	 * the vertexContentTypes property, the shouldInterleaveVertices property has been set, and the
	 * udpateVertexStride method has been invoked, if needed.
	 *
	 * If adding vertex arrays directly, in general, the order of operations is:
	 *   -# set the shouldInterleaveVertices property appropriately
	 *   -# add vertex arrays directly
	 *   -# invoke the updateVertexStride method (if shouldInterleaveVertices set to YES)
	 *   -# set the allocatedVertexCapacity property to allocate memory
	 *   -# populate the vertex content with your data
	 *
	 * If using the vertexContentTypes property to automatically construct the vertex arrays,
	 * the order of operations is:
	 *   -# set the shouldInterleaveVertices property appropriately
	 *   -# set the vertexContentTypes property
	 *   -# set the allocatedVertexCapacity property to allocate memory
	 *   -# populate the vertex content with your data
	 *
	 * This property may be set repeatedly to manage the underlying mesh vertex content as a
	 * dynamically-sized array, growing and shrinking the allocated memory as needed.
	 *
	 * In addition, you can set this property to zero to safely deallocate all memory used
	 * by the vertex content of this mesh. After setting this property to zero, the value of
	 * the vertexCount property will be zero.
	 *
	 * When setting the value of this property to a new non-zero value, all current vertex
	 * content, up to the lesser of the new and old values of this property, will be preserved.
	 *
	 * If the value of this property is increased (including from zero on the first assignement),
	 * vertex content for those vertices beyond the old value of this property will be undefined,
	 * and must be populated by the application before attempting to draw that vertex content.
	 *
	 * If you are not ready to populate the newly allocated vertex content yet, after setting the value
	 * of this property, you can set the value of the vertexCount property to a value less than the
	 * value of this property (including to zero) to stop such undefined vertex content from being drawn.
	 */
	GLuint						getAllocatedVertexCapacity();
	void						setAllocatedVertexCapacity( GLuint capacity );

	/**
	 * Checks to see if the previously-allocated, underlying vertex capacity is large enough to hold
	 * the specified number of vertices, and if not, expands the memory allocations accordingly.
	 *
	 * If exansion is required, vertex capacity is expanded to hold the specified number of vertices,
	 * multiplied by the capacityExpansionFactor property, to provide a buffer for future requirements.
	 *
	 * Returns whether the underlying vertex memory had to be expanded. The application
	 * can use this response value to determine whether or not to reset GL buffers, etc.
	 */
	bool						ensureVertexCapacity( GLuint vtxCount );

	/**
	 * A factor that is used to provide additional vertex capacity when increasing the
	 * allocated vertex capacity via the ensureVertexCapacity: method.
	 *
	 * The initial value of this property is 1.25, providing a buffer of 25% whenever
	 * vertex capacity is expanded.
	 */
	GLfloat						getCapacityExpansionFactor();
	void						setCapacityExpansionFactor( GLfloat capacity );

	/**
	 * The number of bytes used by the content of each vertex.
	 *
	 * The value of this property is calculated each time it is read, by accumulating the
	 * values of the elementLength property of each enclosed vertex array. If this instance
	 * contains no vertex arrays, this property will return zero.
	 *
	 * If the shouldInterleaveVertices property is set to YES, setting this property will set
	 * the same value in all enclosed vertex arrays. If the shouldInterleaveVertices property
	 * is set to NO, setting this property has no effect.
	 *
	 * The initial value of this property is the same as the value of the elementLength property.
	 */
	GLuint						getVertexStride();
	void						setVertexStride( GLuint stride );

	/**
	 * If the shouldInterleaveVertices is set to YES, returns a pointer to the interleaved vertex
	 * content of this mesh. If the shouldInterleaveVertices is set to NO, returns a NULL pointer.
	 *
	 * You must set the allocatedVertexCapacity property, or directly attach vertex storage
	 * to the vertex arrays, prior to accessing this property.
	 *
	 * When populating the interleaved vertex content for this mesh, you can use this pointer
	 * as a starting point to iterate through the vertex content. You can cast the returned
	 * pointer to a custom structure that you declare that matches the content structure of a
	 * single interleaved vertex. The form of that structure depends on the content components
	 * defined for the vertices, is described in the documentation for the vertexContentTypes
	 * property. An enumeration of the vertex content components is available through the
	 * vertexContentTypes property.
	 */
	GLvoid*						interleavedVertices();

	/**
	 * Allocates, reallocates, or deallocates underlying memory for the specified number of
	 * vertex indices, taking into consideration the amount of memory required by each index.
	 *
	 * Setting this property affects the value of the vertexIndexCount property. After setting this
	 * property, the vertexIndexCount property will be set to the same value as this property. After
	 * setting this property, if you will not be using all of the allocated vertex indices immediately,
	 * you should set the value of the vertexIndexCount property to the actual number of vertices in use.
	 *
	 * Use of this property is not required if the vertex content has already been loaded into
	 * memory by a file loader, or defined by a static array. In that situation, you should
	 * set the vertexIndexCount property directly, and avoid using this property.
	 *
	 * This property may be set repeatedly to manage the underlying mesh vertex index data as
	 * a dynamically-sized array, growing and shrinking the allocated memory as needed.
	 *
	 * In addition, you can set this property to zero to safely deallocate all memory used
	 * by the vertex indices of this mesh. After setting this property to zero, the value of
	 * the vertexIndexCount property will be zero.
	 *
	 * When setting the value of this property to a new non-zero value, all current vertex
	 * indices, up to the lesser of the new and old values of this property, will be preserved.
	 *
	 * If the value of this property is increased (including from zero on the first assignement),
	 * those vertex indices beyond the old value of this property will be undefined, and must be
	 * populated by the application before attempting to draw that vertex content.
	 *
	 * If you are not ready to populate the newly allocated vertex indices yet, after setting the value
	 * of this property, you can set the value of the vertexIndexCount property to a value less than the
	 * value of this property (including to zero) to stop such undefined vertices from being drawn.
	 */
	GLuint						getAllocatedVertexIndexCapacity();
	void						setAllocatedVertexIndexCapacity( GLuint capacity );

	/**
	 * Copies vertex content for the specified number of vertices from memory starting at the
	 * specified source vertex index to memory starting at the specified destination vertex index.
	 *
	 * You can use this method to copy data from one area in the mesh to another.
	 */
	void						copyVertices( GLuint vtxCount, GLuint srcIdx, GLuint dstIdx );

	/**
	 * Copies vertex content for the specified number of vertices from memory starting at the
	 * specified source vertex index, in the specified source mesh, to memory starting at the
	 * specified destination vertex index in this mesh.
	 *
	 * You can use this method to copy data from another mesh to this mesh.
	 */
	void						copyVertices( GLuint vtxCount, GLuint srcIdx, CC3Mesh* srcMesh, GLuint dstIdx );

	/**
	 * Copies the vertex content at the specified vertex index in the specified mesh to
	 * this mesh at the specified vertex index.
	 *
	 * It is permissible for the two meshes to have different vertex content types. Only the vertex
	 * content applicable to this mesh will be copied over. If this mesh has vertex content that is
	 * not available in the source mesh, default content is applied to the vertex in this mesh.
	 */
	void						copyVertexAt( GLuint srcIdx, CC3Mesh* srcMesh, GLuint dstIdx );

	/**
	 * Copies vertex indices for the specified number of vertices from memory starting at the specified
	 * source vertex index to memory starting at the specified destination vertex index, and offsets
	 * each value by the specified offset amount. The value at the destination vertex will be that of
	 * the source vertex, plus the specified offset.
	 *
	 * You can use this method to copy content from one area in the vertex indices array to another area,
	 * while adjusting for movement of the underlying vertex content pointed to by these vertex indices.
	 *
	 * If this mesh has no vertex indices, this method does nothing.
	 */
	void						copyVertexIndices( GLuint vtxCount, GLuint srcIdx, GLuint dstIdx, GLint offset );

	/**
	 * Copies vertex content for the specified number of vertices from memory starting at the specified
	 * source vertex index, in the specified source mesh, to memory starting at the specified destination
	 * vertex index in this mesh.
	 *
	 * You can use this method to copy vertex indices from another mesh to this mesh, while adjusting
	 * for differences in where the vertex content lies in each mesh. This method compensates correctly
	 * if the vertex indices in the source mesh are of a different type (GL_UNSIGNED_BYTE or
	 * GL_UNSIGNED_SHORT) than the vertex indices of this mesh.
	 *
	 * If this mesh has no vertex indices, this method does nothing. If the source mesh has no vertex
	 * indices, the specified offset is taken as the starting index of the vertex content in this mesh,
	 * and vertex indices are manufactured automatically to simply point directly to the corresponding
	 * vertex content, in a 1:1 relationship.
	 */
	void						copyVertexIndices( GLuint vtxCount, GLuint srcIdx, CC3Mesh* srcMesh, GLuint dstIdx, GLint offset );

	/**
	 * Indicates the number of vertices in this mesh.
	 *
	 * Usually, you should treat this property as read-only. However, there may be
	 * occasions with meshes that contain dynamic content, such as particle systems,
	 * where it may be appropriate to set the value of this property.
	 *
	 * Setting the value of this property changes the amount of vertex content that
	 * will be submitted to the GL engine during drawing.
	 *
	 * When setting this property, care should be taken to ensure that the value is
	 * not set larger than the number of vertices that were allocated for this mesh.
	 */
	GLuint						getVertexCount();
	void						setVertexCount( GLuint vertexCount );

	/**
	* If indexed drawing is used by this mesh, indicates the number of vertex
	* indices in the mesh.
	*
	* If indexed drawing is not used by this mesh, this property has no effect,
	* and reading it will return zero.
	*
	* Usually, you should treat this property as read-only. However, there may be
	* occasions with meshes that contain dynamic content, such as particle systems,
	* where it may be appropriate to set the value of this property.
	 *
	 * Setting the value of this property changes the amount of vertex content that
	 * will be submitted to the GL engine during drawing.
	 *
	 * When setting this property, care should be taken to ensure that the value is
	 * not set larger than the number of vertices that were allocated for this mesh.
	 */
	GLuint						getVertexIndexCount();
	void						setVertexIndexCount( GLuint indexCount );

	/**
	 * Returns the location element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * This implementation takes into consideration the dimensionality of the underlying
	 * vertex content. If the dimensionality is 2, the returned vector will contain zero in
	 * the Z component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	CC3Vector					getVertexLocationAt( GLuint index );

	/**
	 * Sets the location element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * This implementation takes into consideration the dimensionality of the underlying
	 * vertex content. If the dimensionality is 2, the Z component of the specified vector
	 * will be ignored. If the dimensionality is 4, the specified vector will be converted
	 * to a 4D vector, with the W component set to one, before storing.
	 * 
	 * If this mesh is being used by any mesh nodes, be sure to invoke the markBoundingVolumeDirty
	 * method on all nodes that use this mesh, to ensure that the boundingVolume is recalculated
	 * using the new location values.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexLocationsGLBuffer method to ensure that the GL VBO
	 * that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexLocation( const CC3Vector& aLocation, GLuint index );

	/**
	 * Returns the location element at the specified index in the underlying vertex content,
	 * as a four-dimensional location in the 4D homogeneous coordinate space.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * This implementation takes into consideration the elementSize property. If the
	 * value of the elementSize property is 3, the returned vector will contain one
	 * in the W component. If the value of the elementSize property is 2, the returned
	 * vector will contain zero in the Z component and one in the W component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	CC3Vector4					getVertexHomogeneousLocationAt( GLuint index );

	/**
	 * Sets the location element at the specified index in the underlying vertex content to
	 * the specified four-dimensional location in the 4D homogeneous coordinate space.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * This implementation takes into consideration the dimensionality of the underlying
	 * data. If the dimensionality is 3, the W component of the specified vector will be
	 * ignored. If the dimensionality is 2, both the W and Z components of the specified
	 * vector will be ignored.
	 * 
	 * If this mesh is being used by any mesh nodes, be sure to invoke the markBoundingVolumeDirty
	 * method on all nodes that use this mesh, to ensure that the boundingVolume is recalculated
	 * using the new location values.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexLocationsGLBuffer method to ensure that the GL VBO
	 * that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexHomogeneousLocation( const CC3Vector4& aLocation, GLuint index );

	/**
	 * Returns the normal element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	CC3Vector					getVertexNormalAt( GLuint index );

	/**
	 * Sets the normal element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexNormalsGLBuffer method to ensure that the GL VBO
	 * that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexNormal( const CC3Vector& aNormal, GLuint index );

	/**
	 * Returns the tangent element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	CC3Vector					getVertexTangentAt( GLuint index );

	/**
	 * Sets the tangent element at the specified index in the vertex content to the specified value.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexTangentsGLBuffer method to ensure that the GL VBO
	 * that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexTangent( const CC3Vector& aTangent, GLuint index );

	/**
	 * Returns the bitangent element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	CC3Vector					getVertexBitangentAt( GLuint index );

	/**
	 * Sets the bitangent element at the specified index in the vertex content to the specified value.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexBitangentsGLBuffer method to ensure that the GL VBO
	 * that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexBitangent( const CC3Vector& aTangent, GLuint index );

	/**
	 * Returns the color element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	ccColor4F					getVertexColor4FAt( GLuint index );

	/**
	 * Sets the color element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexColorsGLBuffer method to ensure that the GL VBO
	 * that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexColor4F( const ccColor4F& aColor, GLuint index );

	/**
	 * Returns the color element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	ccColor4B					getVertexColor4BAt( GLuint index );

	/**
	 * Sets the color element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexColorsGLBuffer method to ensure that the GL VBO
	 * that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexColor4B( const ccColor4B& aColor, GLuint index );

	/**
	 * Returns the number of bones that influence each vertex in this mesh. This value defines
	 * the number of bone weights and bone indices that are attached to each vertex.
	 */
	GLuint						getVertexBoneCount();

	/**
	 * Returns the weight value, for the specified influence index within the vertex, for the
	 * vertex at the specified index within the underlying vertex content.
	 *
	 * The weight indicates how much a particular bone influences the movement of the particular
	 * vertex. Several weights are stored for each vertex, one for each bone that influences the
	 * movement of that vertex. The specified influenceIndex parameter must be between zero, and
	 * the vertexBoneCount property (inclusive/exclusive respectively).
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	GLfloat						getVertexWeightForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex );

	/**
	 * Sets the weight value, for the specified influence index within the vertex, for the
	 * vertex at the specified index within the underlying vertex content.
	 *
	 * The weight indicates how much a particular bone influences the movement of the particular
	 * vertex. Several weights are stored for each vertex, one for each bone that influences the
	 * movement of that vertex. The specified influenceIndex parameter must be between zero, and
	 * the vertexBoneCount property (inclusive/exclusive respectively).
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the updateVertexBoneWeightsGLBuffer
	 * method to ensure that the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexWeight( GLfloat weight, GLuint influenceIndex, GLuint vtxIndex );

	/**
	 * Returns the weights of all of the bones that influence the movement of the vertex at the
	 * specified index within the underlying vertex content.
	 *
	 * Several weights are stored for each vertex, one for each bone that influences the movement
	 * of the vertex. The number of elements in the returned array is the same for each vertex
	 * in this vertex array, as defined by the vertexBoneCount property.
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	GLfloat*					getVertexBoneWeightsAt( GLuint vtxIndex );

	/**
	 * Sets the weights of all of the bones that influence the movement of the vertex at the
	 * specified index within the underlying vertex content.
	 *
	 * Several weights are stored for each vertex, one for each bone that influences the movement
	 * of the vertex. The number of elements in the specified input array must therefore be at
	 * least as large as the value of the vertexBoneCount property.
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the updateVertexBoneWeightsGLBuffer
	 * method to ensure that the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexBoneWeights( GLfloat* weights, GLuint vtxIndex );

	/**
	 * Returns the index of the bone, that provides the influence at the specified influence index
	 * within a vertex, for the vertex at the specified index within the underlying vertex content.
	 *
	 * The bone index indicates which bone provides the particular influence for the movement of
	 * the particular vertex. Several bone indices are stored for each vertex, one for each bone
	 * that influences the movement of that vertex. The specified influenceIndex parameter must
	 * be between zero, and the vertexBoneCount property (inclusive/exclusive respectively).
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	GLuint						getVertexBoneIndexForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex );

	/**
	 * Sets the index of the bone, that provides the influence at the specified influence index
	 * within a vertex, for the vertex at the specified index within the underlying vertex content.
	 *
	 * The bone index indicates which bone provides the particular influence for the movement of
	 * the particular vertex. Several bone indices are stored for each vertex, one for each bone
	 * that influences the movement of that vertex. The specified influenceIndex parameter must
	 * be between zero, and the vertexBoneCount property (inclusive/exclusive respectively).
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the updateVertexBoneIndicesGLBuffer
	 * method to ensure that the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexBoneIndex( GLuint boneIndex, GLuint influenceIndex, GLuint vtxIndex );

	/**
	 * Returns the indices of all of the bones that influence the movement of the vertex at the
	 * specified index within the underlying vertex content.
	 *
	 * Several indices are stored for each vertex, one for each bone that influences the movement
	 * of the vertex. The number of elements in the returned array is the same for each vertex
	 * in this vertex array, as defined by the vertexBoneCount property.
	 *
	 * The bone indices can be stored in each vertex as either type GLushort or type GLubyte.
	 * The returned array will be of the type of index stored by the verties in this mesh, and it
	 * is up to the application to know which type will be returned, and cast the returned array
	 * accordingly. The type can be determined by the vertexBoneIndexType property of this mesh,
	 * which will return one of GL_UNSIGNED_SHORT or GL_UNSIGNED_BYTE, respectively.
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	GLvoid*						getVertexBoneIndicesAt( GLuint vtxIndex );

	/**
	 * Sets the indices of all of the bones that influence the movement of the vertex at the
	 * specified index within the underlying vertex content.
	 *
	 * Several indices are stored for each vertex, one for each bone that influences the movement
	 * of the vertex. The number of elements in the specified input array must therefore be at
	 * least as large as the value of the vertexBoneCount property.
	 *
	 * The bone indices can be stored in each vertx as either type GLushort or type GLubyte.
	 * The specified array must be of the type of index stored by the verties in this mesh, and
	 * it is up to the application to know which type is required, and provide that type of array
	 * accordingly. The type can be determined by the vertexBoneIndexType property of this mesh,
	 * which will return one of GL_UNSIGNED_SHORT or GL_UNSIGNED_BYTE, respectively.
	 *
	 * To avoid checking the elementType altogether, you can use the setVertxBoneIndex:forBoneInfluence:at:
	 * method, which sets the bone index values one at a time, and automatically converts the input type to
	 * the correct stored type.
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the updateVertexBoneIndicesGLBuffer
	 * method to ensure that the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexBoneIndices( GLvoid* boneIndices, GLuint vtxIndex );

	/**
	 * Returns the type of data element used to store each bone index.
	 *
	 * The value returned by this property will be either GL_UNSIGNED_SHORT or GL_UNSIGNED_BYTE,
	 * corresponding to each bone index being stored in either a type GLushort or type GLubyte,
	 * respectively.
	 *
	 * You can use the value of this property to determine how to cast the data arrays used by
	 * the vertexBoneIndicesAt: and setVertexBoneIndices:at: methods.
	 */
	GLenum						getVertexBoneIndexType();

	/**
	 * Returns the point size element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	GLfloat						getVertexPointSizeAt( GLuint vtxIndex );

	/**
	 * Sets the point size element at the specified index in the vertex content to the specified value.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * When all vertex changes have been made, be sure to invoke the updatePointSizesGLBuffer
	 * method to ensure that the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexPointSize( GLfloat aSize, GLuint vtxIndex );

	/**
	 * Returns the texture coordinate element at the specified index from the vertex content
	 * at the specified texture unit index.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	ccTex2F						getVertexTexCoord2FForTextureUnit( GLuint texUnit, GLuint index );

	/**
	 * Sets the texture coordinate element at the specified index in the vertex content,
	 * at the specified texture unit index, to the specified texture coordinate value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexTextureCoordinatesGLBufferForTextureUnit: method
	 * to ensure that the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexTexCoord2F( const ccTex2F& aTex2F, GLuint texUnit, GLuint index );

	/**
	 * Returns the texture coordinate element at the specified index from the vertex content
	 * at the commonly used texture unit zero.
	 *
	 * This is a convenience method that is equivalent to invoking the
	 * vertexTexCoord2FForTextureUnit:at: method, with zero as the texture unit index.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	ccTex2F						vertexTexCoord2FAt( GLuint index );

	/**
	 * Sets the texture coordinate element at the specified index in the vertex content,
	 * at the commonly used texture unit zero, to the specified texture coordinate value.
	 *
	 * This is a convenience method that delegates to the setVertexTexCoord2F:forTextureUnit:at:
	 * method, passing in zero for the texture unit index.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexTextureCoordinatesGLBuffer method to ensure that
	 * the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexTexCoord2F( const ccTex2F& aTex2F, GLuint index );

	/**
	 * Returns the index element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	GLuint						getVertexIndexAt( GLuint index );

	/**
	 * Sets the index element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexIndicesGLBuffer method to ensure that the GL VBO
	 * that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setVertexIndex( GLuint vertexIndex, GLuint index );

	/**
	 * Additional information about the faces in the mesh.
	 *
	 * This property does not contain vertex information for the faces. That is contained
	 * within the mesh itself. As such, most meshes do not require this additional information
	 * about the faces of the mesh. This property provides additional information about the
	 * faces that can be used in certain customized lighting and shadowing effects.
	 *
	 * If this property is not set directly, it will be lazily initialized on first access.
	 *
	 * Since the face array contains static information about a mesh, when copying a mesh,
	 * the face array is not itself copied by default. This avoids duplication of data that
	 * does not change between two copies of the same mesh object. Instead, both mesh copies
	 * will share a reference to the same face array instance. If you need to create separate
	 * copies of the faces array when copying a mesh, you must explicitly create a copy.
	 */
	CC3FaceArray*				getFaces();
	void						setFaces( CC3FaceArray* faces );

	/**
	 * Indicates whether information about the faces of this mesh should be cached.
	 *
	 * If this property is set to NO, accessing information about the faces through the
	 * methods faceAt:, faceIndicesAt:, faceCenterAt:, faceNormalAt:, or facePlaneAt:,
	 * will be calculated dynamically from the mesh data.
	 *
	 * If such data will be accessed frequently, this repeated dynamic calculation may
	 * cause a noticable impact to performance. In such a case, this property can be
	 * set to YES to cause the data to be calculated once and cached, improving the
	 * performance of subsequent accesses to information about the faces.
	 *
	 * However, caching information about the faces will increase the amount of memory
	 * required by the mesh, sometimes significantly. To avoid this additional memory
	 * overhead, in general, you should leave this property set to NO, unless intensive
	 * access to face information is causing a performance impact.
	 *
	 * An example of a situation where the use of this property may be noticable,
	 * is when adding shadow volumes to nodes. Shadow volumes make intense use of
	 * accessing face information about the mesh that is casting the shadow.
	 *
	 * When the value of this property is set to NO, any data cached during previous
	 * access through the indicesAt:, centerAt:, normalAt:, or planeAt:, methods will
	 * be cleared.
	 *
	 * The initial value of this property is NO.
	 */
	bool						shouldCacheFaces();
	void						setShouldCacheFaces( bool shouldCache );

	/**
	 * Returns the number of faces in this mesh.
	 *
	 * This is calculated from the number of vertices, taking into
	 * consideration the type of primitives that this mesh is drawing.
	 */
	GLuint						getFaceCount();

	/**
	 * Returns the number of faces to be drawn from the specified number of
	 * vertex indices, based on the type of primitives that this mesh is drawing.
	 */ 
	GLuint						getFaceCountFromVertexIndexCount( GLuint vc );

	/**
	 * Returns the number of vertex indices required to draw the specified number
	 * of faces, based on the type of primitives that this mesh is drawing.
	 */ 
	GLuint						getVertexIndexCountFromFaceCount( GLuint fc );

	/**
	 * Returns the face from the mesh at the specified index.
	 *
	 * The specified faceIndex value refers to the index of the face, not the vertices
	 * themselves. So, a value of 5 will retrieve the three vertices that make up the
	 * fifth triangular face in this mesh. The specified index must be between zero,
	 * inclusive, and the value of the faceCount property, exclusive.
	 *
	 * The returned face structure contains only the locations of the vertices. If the vertex
	 * locations are interleaved with other vertex content, such as color or texture coordinates,
	 * or other padding, that data will not appear in the returned face structure. For that
	 * remaining vertex content, you can use the faceIndicesAt: method to retrieve the indices
	 * of the vertex content, and then use the vertex accessor methods to retrieve the individual
	 * vertex content components.
	 *
	 * If you will be invoking this method frequently, you can optionally set the
	 * shouldCacheFaces property to YES to speed access, and possibly improve performance.
	 * However, be aware that setting the shouldCacheFaces property to YES can significantly
	 * increase the amount of memory used by the mesh.
	 */
	CC3Face						getFaceAt( GLuint faceIndex );

	/**
	 * Returns the mesh face that is made up of the three vertices at the three indices
	 * within the specified face indices structure.
	 *
	 * The returned face structure contains only the locations of the vertices. If the vertex
	 * locations are interleaved with other vertex content, such as color or texture coordinates,
	 * or other padding, that data will not appear in the returned face structure. For that
	 * remaining vertex content, you can use the faceIndicesAt: method to retrieve the indices
	 * of the vertex content, and then use the vertex accessor methods to retrieve the individual
	 * vertex content components.
	 */
	CC3Face						getFaceFromIndices( const CC3FaceIndices& faceIndices );

	CC3FaceIndices				getUncachedFaceIndicesAt( GLuint faceIndex );

	/**
	 * Returns the face from the mesh at the specified index, as indices into the mesh vertices.
	 *
	 * The specified faceIndex value refers to the index of the face, not the vertices
	 * themselves. So, a value of 5 will retrieve the three vertices that make up the
	 * fifth triangular face in this mesh. The specified index must be between zero,
	 * inclusive, and the value of the faceCount property, exclusive.
	 *
	 * The returned structure reference contains the indices of the three vertices that
	 * make up the triangular face. These indices index into the actual vertex content within
	 * the layout of the mesh.
	 *
	 * This method takes into consideration any padding (stride) between the vertex indices.
	 *
	 * If you will be invoking this method frequently, you can optionally set the
	 * shouldCacheFaces property to YES to speed access, and possibly improve performance.
	 * However, be aware that setting the shouldCacheFaces property to YES can significantly
	 * increase the amount of memory used by the mesh.
	 */
	CC3FaceIndices				getFaceIndicesAt( GLuint faceIndex );

	/**
	 * Returns the center of the mesh face at the specified index.
	 *
	 * If you will be invoking this method frequently, you can optionally set the
	 * shouldCacheFaces property to YES to speed access, and possibly improve performance.
	 * However, be aware that setting the shouldCacheFaces property to YES can significantly
	 * increase the amount of memory used by the mesh.
	 */
	CC3Vector					getFaceCenterAt( GLuint faceIndex );

	/**
	 * Returns the normal of the mesh face at the specified index.
	 *
	 * If you will be invoking this method frequently, you can optionally set the
	 * shouldCacheFaces property to YES to speed access, and possibly improve performance.
	 * However, be aware that setting the shouldCacheFaces property to YES can significantly
	 * increase the amount of memory used by the mesh.
	 */
	CC3Vector					getFaceNormalAt( GLuint faceIndex );

	/**
	 * Returns the plane of the mesh face at the specified index.
	 *
	 * If you will be invoking this method frequently, you can optionally set the
	 * shouldCacheFaces property to YES to speed access, and possibly improve performance.
	 * However, be aware that setting the shouldCacheFaces property to YES can significantly
	 * increase the amount of memory used by the mesh.
	 */
	CC3Plane					getFacePlaneAt( GLuint faceIndex );

	/** Returns the indices of the neighbours of the mesh face at the specified index. */
	CC3FaceNeighbours			getFaceNeighboursAt( GLuint faceIndex );

	/**
	 * Populates the specified array with information about the intersections of the specified ray
	 * and this mesh, up to the specified maximum number of intersections.
	 *
	 * This method returns the actual number of intersections found (up to the specified maximum).
	 * This value indicates how many of the elements of the specifed intesections array were populated
	 * during the execution of this method. The contents of elements beyond that number are undefined.
	 *
	 * Each of the populated elements of the intersections array contains information about the face
	 * on which the intersection occurred, the location of the intersection, and the distance from the
	 * ray startLocation where the intersection occurred. The location and distance components are
	 * specified in the local coordinates system of this mesh.
	 *
	 * The intersections array is not sorted in any way. In particular, when the array contains multiple
	 * entries, the first element in the array does not necessily contain the closest intersection.
	 * If you need to determine the closest intersection, you can iterate the intersections array and
	 * compare the values of the location element of each intersection.
	 *
	 * To use this method, allocate an array of CC3MeshIntersection structures, pass a reference to it
	 * in the intersections parameter, and indicate the size of that array in the maxHitCount parameter.
	 *
	 * The method iterates through the faces in the mesh until the indicated number of intersections are
	 * found, or until all the faces in the mesh have been inspected. Therefore, to keep performance high,
	 * you should set the maxHitCount parameter no higher than the number of intersections that are useful
	 * to you. For example, specifiying a value of one for the maxHitCount parameter will cause this method
	 * to return as soon as the first intersection is found. In most cases, this is all that is needed.
	 * 
	 * The allowBackFaces parameter is used to indicate whether to include intersections where the ray
	 * pierces a face from its back face. Typically, this means that the ray has intersected the face
	 * as the ray exits on the far side of the mesh. In most cases you will interested only where the
	 * ray intersects the near side of the mesh, in which case you can set this parameter to NO.
	 *
	 * The allowBehind parameter is used to indicate whether to include intersections that occur behind
	 * the startLocation of the ray, in the direction opposite to the direction of the ray. Typically,
	 * this might mean the mesh is located behind the ray startLocation, or it might mean the ray starts
	 * inside the mesh. Again,in most cases, you will be interested only in intersections that occur in
	 * the direction the ray is pointing, and can ususally set this parameter to NO.
	 */
	GLuint						findFirst( GLuint maxHitCount, CC3MeshIntersection* intersections, 
		const CC3Ray& aRay, bool acceptBackFaces, bool acceptBehind );

	/**
	 * Convenience method to create GL buffers for all vertex arrays used by this mesh.
	 *
	 * This method may safely be called more than once, or on more than one mesh that shares
	 * vertex arrays, since vertex array GL buffers are only created if they don't already exist.
	 */
	void						createGLBuffers();

	/**
	 * Convenience method to delete any GL buffers for all vertex arrays used by this mesh.
	 * The arrays may continue to be used, and the arrays will be passed from the client during
	 * each draw instead of bound to the GL server as a vertex buffer.
	 *
	 * This is a convenience method. Because vertex arrays may be shared between arrays, this
	 * method should likely be used when it is known that this mesh is the only user of the array,
	 * or to clear GL memory for any rarely used meshes. A more general design is to simply release
	 * the vertex array. The GL buffer will be deleted when the vertex array is deallocated.
	 *
	 * This method may safely be called more than once, or on more than one mesh that shares
	 * vertex arrays, since vertex array GL buffers are only deleted if they exist.
	 */
	void						deleteGLBuffers();

	/**
	 * Returns whether the underlying vertex content has been loaded into GL engine vertex
	 * buffer objects. Vertex buffer objects are engaged via the createGLBuffers method.
	 */
	bool						isUsingGLBuffers();

	/**
	 * Once the vertex content has been buffered into a GL vertex buffer object (VBO)
	 * within the GL engine, via the createGLBuffer method, this method can be used
	 * to release the data in main memory that is now redundant.
	 *
	 * Typically, this method is not invoked directly by the application. Instead,
	 * consider using the same method on a node assembly in order to release as much
	 * memory as possible in one simply method invocation.
	 */
	void						releaseRedundantContent();

	/**
	 * Convenience method to cause all vertex content to be retained in application
	 * memory when releaseRedundantContent is invoked, even if it has been buffered to a GL VBO.
	 *
	 * All vertex content, such as location, normal, color, texture coordinates, point size,
	 * bone weights, and bone indices will be retained.
	 *
	 * Invoking this method does NOT cause vertex index data to be retained. To retain vertex
	 * index data, use the retainVertexIndices method.
	 */
	void						retainVertexContent();

	/**
	 * Convenience method to cause the vertex location content to be retained in application
	 * memory when releaseRedundantContent is invoked, even if it has been buffered to a GL VBO.
	 *
	 * Only the vertex locations will be retained. Any other vertex content, such as normals,
	 * or texture coordinates, that has been buffered to GL VBO's, will be released from
	 * application memory when releaseRedundantContent is invoked.
	 */
	void						retainVertexLocations();

	/**
	 * Convenience method to cause the vertex normal content to be retained in application
	 * memory when releaseRedundantContent is invoked, even if it has been buffered to a GL VBO.
	 *
	 * Only the vertex normals will be retained. Any other vertex content, such as locations,
	 * or texture coordinates, that has been buffered to GL VBO's, will be released from
	 * application memory when releaseRedundantContent is invoked.
	 */
	void						retainVertexNormals();

	/**
	 * Convenience method to cause the vertex tangent content to be retained in application
	 * memory when releaseRedundantContent is invoked, even if it has been buffered to a GL VBO.
	 *
	 * Only the vertex tangents will be retained. Any other vertex content, such as locations,
	 * or texture coordinates, that has been buffered to GL VBO's, will be released from
	 * application memory when releaseRedundantContent is invoked.
	 */
	void						retainVertexTangents();

	/**
	 * Convenience method to cause the vertex bitangent content to be retained in application
	 * memory when releaseRedundantContent is invoked, even if it has been buffered to a GL VBO.
	 *
	 * Only the vertex tangents will be retained. Any other vertex content, such as locations,
	 * or texture coordinates, that has been buffered to GL VBO's, will be released from
	 * application memory when releaseRedundantContent is invoked.
	 */
	void						retainVertexBitangents();

	/**
	 * Convenience method to cause the vertex color content to be retained in application
	 * memory when releaseRedundantContent is invoked, even if it has been buffered to a GL VBO.
	 *
	 * Only the vertex colors will be retained. Any other vertex content, such as locations,
	 * or texture coordinates, that has been buffered to GL VBO's, will be released from
	 * application memory when releaseRedundantContent is invoked.
	 */
	void						retainVertexColors();

	/**
	 * Convenience method to cause the vertex bone weight content to be retained in application
	 * memory when releaseRedundantContent is invoked, even if it has been buffered to a GL VBO.
	 *
	 * Only the vertex bone weights will be retained. Any other vertex content, such as locations,
	 * or texture coordinates, that has been buffered to GL VBO's, will be released from
	 * application memory when releaseRedundantContent is invoked.
	 */
	void						retainVertexBoneWeights();

	/**
	 * Convenience method to cause the vertex bone index content to be retained in application
	 * memory when releaseRedundantContent is invoked, even if it has been buffered to a GL VBO.
	 *
	 * Only the vertex bone indices will be retained. Any other vertex content, such as locations,
	 * or texture coordinates, that has been buffered to GL VBO's, will be released from
	 * application memory when releaseRedundantContent is invoked.
	 */
	void						retainVertexBoneIndices();

	/**
	 * Convenience method to cause the vertex point size content to be retained in application
	 * memory when releaseRedundantContent is invoked, even if it has been buffered to a GL VBO.
	 *
	 * Only the vertex point sizes will be retained. Any other vertex content, such as locations,
	 * or texture coordinates, that has been buffered to GL VBO's, will be released from
	 * application memory when releaseRedundantContent is invoked.
	 */
	void						retainVertexPointSizes();

	/**
	 * Convenience method to cause the vertex texture coordinate content for all texture units
	 * used by this mesh to be retained in application memory when releaseRedundantContent is
	 * invoked, even if it has been buffered to a GL VBO.
	 *
	 * Only the vertex texture coordinates will be retained. Any other vertex content, such as
	 * locations, or normals, that has been buffered to GL VBO's, will be released from
	 * application memory when releaseRedundantContent is invoked.
	 */
	void						retainVertexTextureCoordinates();

	/**
	 * Convenience method to cause the vertex index content to be retained in application
	 * memory when releaseRedundantContent is invoked, even if it has been buffered to a GL VBO.
	 *
	 * Only the vertex indices will be retained. Any other vertex content, such as locations,
	 * or texture coordinates, that has been buffered to GL VBO's, will be released from
	 * application memory when releaseRedundantContent is invoked.
	 */
	void						retainVertexIndices();

	/**
	 * Convenience method to cause all vertex content to be skipped when createGLBuffers is invoked.
	 * The vertex content is not buffered to a a GL VBO, is retained in application memory, and is
	 * submitted to the GL engine on each frame render.
	 *
	 * This method causes the vertex content to be retained in application memory, so, if you have
	 * invoked this method, you do NOT also need to invoke the retainVertexContent method.
	 */
	void						doNotBufferVertexContent();

	/**
	 * Convenience method to cause the vertex location content to be skipped when createGLBuffers
	 * is invoked. The vertex content is not buffered to a a GL VBO, is retained in application
	 * memory, and is submitted to the GL engine on each frame render.
	 *
	 * Only the vertex locations will not be buffered to a GL VBO. Any other vertex data, such as
	 * normals, or texture coordinates, will be buffered to a GL VBO when createGLBuffers is invoked.
	 *
	 * This method causes the vertex content to be retained in application memory, so, if you have
	 * invoked this method, you do NOT also need to invoke the retainVertexLocations method.
	 */
	void						doNotBufferVertexLocations();

	/**
	 * Convenience method to cause the vertex normal content to be skipped when createGLBuffers
	 * is invoked. The vertex content is not buffered to a a GL VBO, is retained in application
	 * memory, and is submitted to the GL engine on each frame render.
	 *
	 * Only the vertex normals will not be buffered to a GL VBO. Any other vertex data, such as
	 * locations, or texture coordinates, will be buffered to a GL VBO when createGLBuffers is invoked.
	 *
	 * This method causes the vertex content to be retained in application memory, so, if you have
	 * invoked this method, you do NOT also need to invoke the retainVertexNormals method.
	 */
	void						doNotBufferVertexNormals();

	/**
	 * Convenience method to cause the vertex tangent content to be skipped when createGLBuffers
	 * is invoked. The vertex content is not buffered to a a GL VBO, is retained in application
	 * memory, and is submitted to the GL engine on each frame render.
	 *
	 * Only the vertex normals will not be buffered to a GL VBO. Any other vertex data, such as
	 * locations, or texture coordinates, will be buffered to a GL VBO when createGLBuffers is invoked.
	 *
	 * This method causes the vertex content to be retained in application memory, so, if you have
	 * invoked this method, you do NOT also need to invoke the retainVertexTangents method.
	 */
	void						doNotBufferVertexTangents();

	/**
	 * Convenience method to cause the vertex bitangent content to be skipped when createGLBuffers
	 * is invoked. The vertex content is not buffered to a a GL VBO, is retained in application
	 * memory, and is submitted to the GL engine on each frame render.
	 *
	 * Only the vertex normals will not be buffered to a GL VBO. Any other vertex data, such as
	 * locations, or texture coordinates, will be buffered to a GL VBO when createGLBuffers is invoked.
	 *
	 * This method causes the vertex content to be retained in application memory, so, if you have
	 * invoked this method, you do NOT also need to invoke the retainVertexBitangents method.
	 */
	void						doNotBufferVertexBitangents();

	/**
	 * Convenience method to cause the vertex color content to be skipped when createGLBuffers
	 * is invoked. The vertex content is not buffered to a a GL VBO, is retained in application
	 * memory, and is submitted to the GL engine on each frame render.
	 *
	 * Only the vertex colors will not be buffered to a GL VBO. Any other vertex data, such as
	 * locations, or texture coordinates, will be buffered to a GL VBO when createGLBuffers is invoked.
	 *
	 * This method causes the vertex content to be retained in application memory, so, if you have
	 * invoked this method, you do NOT also need to invoke the retainVertexColors method.
	 */
	void						doNotBufferVertexColors();

	/**
	 * Convenience method to cause the vertex bone weight content to be skipped when createGLBuffers
	 * is invoked. The vertex content is not buffered to a GL VBO, is retained in application
	 * memory, and is submitted to the GL engine on each frame render.
	 *
	 * Only the vertex bone weight will not be buffered to a GL VBO. Any other vertex content, such as
	 * locations, or texture coordinates, will be buffered to a GL VBO when createGLBuffers is invoked.
	 *
	 * This method causes the vertex content to be retained in application memory, so, if you have
	 * invoked this method, you do NOT also need to invoke the retainVertexBoneWeights method.
	 */
	void						doNotBufferVertexBoneWeights();

	/**
	 * Convenience method to cause the vertex bone index content to be skipped when createGLBuffers
	 * is invoked. The vertex content is not buffered to a GL VBO, is retained in application memory,
	 * and is submitted to the GL engine on each frame render.
	 *
	 * Only the vertex bone index will not be buffered to a GL VBO. Any other vertex content, such as
	 * locations, or texture coordinates, will be buffered to a GL VBO when createGLBuffers is invoked.
	 *
	 * This method causes the vertex content to be retained in application memory, so, if you have
	 * invoked this method, you do NOT also need to invoke the retainVertexBoneIndices method.
	 */
	void						doNotBufferVertexBoneIndices();

	/**
	 * Convenience method to cause the vertex point size content to be skipped when createGLBuffers
	 * is invoked. The vertex content is not buffered to a GL VBO, is retained in application memory,
	 * and is submitted to the GL engine on each frame render.
	 *
	 * Only the vertex point sizes will not be buffered to a GL VBO. Any other vertex content, such as
	 * locations, or texture coordinates, will be buffered to a GL VBO when createGLBuffers is invoked.
	 *
	 * This method causes the vertex content to be retained in application memory, so, if you have
	 * invoked this method, you do NOT also need to invoke the retainVertexPointSizes method.
	 */
	void						doNotBufferVertexPointSizes();

	/**
	 * Convenience method to cause the vertex texture coordinate content for all texture units used
	 * by this mesh to be skipped when createGLBuffers is invoked. The vertex content is not buffered
	 * to a a GL VBO, is retained in application memory, and is submitted to the GL engine on each
	 * frame render.
	 *
	 * Only the vertex texture coordinates will not be buffered to a GL VBO. Any other vertex content,
	 * such as locations, or texture coordinates, will be buffered to a GL VBO when createGLBuffers
	 * is invoked.
	 *
	 * This method causes the vertex content to be retained in application memory, so, if you have
	 * invoked this method, you do NOT also need to invoke the retainVertexTextureCoordinates method.
	 */
	void						doNotBufferVertexTextureCoordinates();

	/**
	 * Convenience method to cause the vertex index content to be skipped when createGLBuffers
	 * is invoked. The vertex content is not buffered to a a GL VBO, is retained in application
	 * memory, and is submitted to the GL engine on each frame render.
	 *
	 * Only the vertex indices will not be buffered to a GL VBO. Any other vertex data, such as
	 * locations, or texture coordinates, will be buffered to a GL VBO when createGLBuffers is invoked.
	 *
	 * This method causes the vertex content to be retained in application memory, so, if you have
	 * invoked this method, you do NOT also need to invoke the retainVertexColors method.
	 */
	void						doNotBufferVertexIndices();

	/** Updates the GL engine buffer with the vertex location content in this mesh. */
	void						updateVertexLocationsGLBuffer();

	/** Updates the GL engine buffer with the vertex normal content in this mesh. */
	void						updateVertexNormalsGLBuffer();

	/** Updates the GL engine buffer with the vertex tangent content in this mesh. */
	void 						updateVertexTangentsGLBuffer();

	/** Updates the GL engine buffer with the vertex bitangent content in this mesh. */
	void 						updateVertexBitangentsGLBuffer();

	/** Updates the GL engine buffer with the vertex color content in this mesh. */
	void 						updateVertexColorsGLBuffer();

	/** Updates the GL engine buffer with the vertex bone weight content in this mesh. */
	void						updateVertexBoneWeightsGLBuffer();

	/** Updates the GL engine buffer with the vertex bone indices content in this mesh. */
	void 						updateVertexBoneIndicesGLBuffer();

	/** Updates the GL engine buffer with the point size content in this mesh. */
	void 						updatePointSizesGLBuffer();

	ccTex2F						getVertexTexCoord2FAt( GLuint index );

	/**
	 * Updates the GL engine buffer with the vertex texture coord content from the
	 * specified texture unit in this mesh.
	 */
	void 						updateVertexTextureCoordinatesGLBufferForTextureUnit( GLuint texUnit );

	/**
	 * Updates the GL engine buffer with the vertex texture coord content from
	 * texture unit zero in this mesh.
	 */
	void 						updateVertexTextureCoordinatesGLBuffer();

	/**
	 * Convenience method to update the GL engine buffers with the vertex content in this mesh.
	 *
	 * This updates the content of each vertex. It does not update the vertex indices. To update
	 * the vertex index data to the GL engine, use the updateVertexIndicesGLBuffer method.
	 */
	void 						updateGLBuffers();

	/**
	 * Convenience method to update GL buffers for all vertex arrays used by this mesh, except
	 * vertexIndices, starting at the vertex at the specified offsetIndex, and extending for
	 * the specified number of vertices.
	 */
	void						updateGLBuffersStartingAt( GLuint offsetIndex, GLuint vertexCount );

	/** Updates the GL engine buffer with the vertex index data in this mesh. */
	void 						updateVertexIndicesGLBuffer();

	/** The center of geometry of this mesh. */
	CC3Vector					getCenterOfGeometry();

	/** Returns the the smallest axis-aligned-bounding-box (AABB) that surrounds the mesh. */
	CC3Box						getBoundingBox();

	/**
	 * Returns the radius of a spherical boundary, centered on the centerOfGeometry,
	 * that encompasses all the vertices of this mesh.
	 */
	GLfloat						getRadius();

	/**
	 * Changes the mesh vertices so that the origin of the mesh is at the specified location.
	 *
	 * The origin of the mesh is the location (0,0,0) in the local coordinate system, and is the
	 * location around which all transforms are performed.
	 *
	 * This method can be used to adjust the mesh structure to make it easier to apply transformations,
	 * by moving the origin of the transformations to a more convenient location in the mesh.
	 *
	 * This method changes the location component of every vertex in the mesh. This can be quite costly,
	 * and should only be performed once, to adjust a mesh so that it is easier to manipulate. As an
	 * alternate, you should consider changing the origin of the mesh at development time using a 3D editor.
	 *
	 * Do not use this method to move your model around. Instead, use the transform properties (location,
	 * rotation and scale) of the CC3MeshNode that contains this mesh, and let the GL engine do the heavy
	 * lifting of transforming the mesh vertices.
	 *
	 * If this mesh is being used by any mesh nodes, be sure to invoke the markBoundingVolumeDirty method
	 * on all nodes that use this mesh, to ensure that the boundingVolume is recalculated using the new
	 * location values. Invoking this method on the CC3MeshNode instead will automatically invoke the
	 * markBoundingVolumeDirty method.
	 *
	 * This method ensures that the GL VBO that holds the vertex content is updated.
	 */
	void						moveMeshOriginTo( const CC3Vector& aLocation );

	/**
	 * Changes the mesh vertices so that the origin of the mesh is at the center of geometry of the mesh.
	 *
	 * The origin of the mesh is the location (0,0,0) in the local coordinate system, and is the
	 * location around which all transforms are performed.
	 *
	 * This method can be used to adjust the mesh structure to make it easier to apply transformations,
	 * by moving the origin of the transformations to the center of the mesh.
	 *
	 * This method changes the location component of every vertex in the mesh. This can be quite costly,
	 * and should only be performed once, to adjust a mesh so that it is easier to manipulate. As an
	 * alternate, you should consider changing the origin of the mesh at development time using a 3D editor.
	 *
	 * Do not use this method to move your model around. Instead, use the transform properties (location,
	 * rotation and scale) of the CC3MeshNode that contains this mesh, and let the GL engine do the heavy
	 * lifting of transforming the mesh vertices.
	 *
	 * If this mesh is being used by any mesh nodes, be sure to invoke the markBoundingVolumeDirty method
	 * on all nodes that use this mesh, to ensure that the boundingVolume is recalculated using the new
	 * location values. Invoking this method on the CC3MeshNode instead will automatically invoke the
	 * markBoundingVolumeDirty method.
	 *
	 * This method ensures that the GL VBO that holds the vertex content is updated.
	 */
	void 						moveMeshOriginToCenterOfGeometry();

	/**
	 * The color of the vertices of this mesh.
	 *
	 * Querying this property returns the RGB components of the first vertex in this mesh, or ccBLACK
	 * if this mesh contains no per-vertex color content.
	 *
	 * When setting this property, if this mesh contains per-vertex color content, the RGB values of each
	 * vertex in this mesh are set to the specified color, without affecting the opacity value of each
	 * individual vertex. If the vertex color content of this mesh has been copied to a GL buffer, that
	 * GL buffer is automatically updated.
	 */
	CCColorRef					getColor();
	void						setColor( const CCColorRef& color );

	/**
	 * The opacity of the vertices in this mesh.
	 *
	 * Querying this property returns the alpha component of the first vertex in this mesh, or 
	 * full opacity if this mesh contains no per-vertex color content.
	 *
	 * When setting this property, if this mesh contains per-vertex color content, the alpha values of
	 * each vertex in this mesh is set to the specified opacity, without affecting the RGB color value
	 * of each individual vertex. If the vertex color content of this mesh has been copied to a GL buffer,
	 * that GL buffer is automatically updated.
	 */
	CCOpacity					getOpacity();
	void						setOpacity( CCOpacity opacity );

	/**
	 * Indicates whether the texture coordinates of this mesh expects that the texture
	 * was flipped upside-down during texture loading.
	 *
	 * The vertical axis of the coordinate system of OpenGL is inverted relative to the
	 * CoreGraphics view coordinate system. As a result, some texture file formats may be
	 * loaded upside down. Most common file formats, including JPG, PNG & PVR are loaded
	 * right-way up, but using proprietary texture formats developed for other platforms
	 * may result in textures being loaded upside-down.
	 *
	 * The value of this property is used in combination with the value of the
	 * isUpsideDown property of a texture to determine whether the texture
	 * will be oriented correctly when displayed using these texture coordinates.
	 *
	 * The alignTextureUnit:withTexture: method compares the value of this property
	 * with the isUpsideDown property of the texture to automatically determine
	 * whether these texture coordinates need to be flipped vertically in order to
	 * display the texture correctly, and will do so if needed. As part of that inversion,
	 * the value of this property will also be flipped, to indicate that the texture
	 * coordinates are now aligned differently.
	 *
	 * The alignTextureUnit:withTexture: method is invoked automatically when a
	 * texture is assigned to cover this mesh in the mesh node. If you need to
	 * adjust the value of this property, you sould do so before setting a texture
	 * or material into the mesh node.
	 *
	 * When multi-texturing is being used on this mesh, you can use the
	 * expectsVerticallyFlippedTexture:inTextureUnit: method for finer control
	 * of orienting textures for each texture unit. When multi-texturing is
	 * being used, setting this value of this property will invoke the
	 * expectsVerticallyFlippedTexture:inTextureUnit: method to set the same
	 * value for each texture unit.
	 *
	 * Reading the value of this property will return YES if the property-reading
	 * method expectsVerticallyFlippedTextureInTextureUnit: returns YES for
	 * any texture unit, otherwise this property will return NO.
	 *
	 * The initial value of this property is set when the underlying mesh texture
	 * coordinates are built or loaded. See the expectsVerticallyFlippedTextures
	 * property on the CC3NodesResource class to understand how this property is set
	 * during mesh resource loading.
	 *
	 * When building meshes programmatically, you should endeavour to design the
	 * mesh so that this property will be YES if you will be using vertically-flipped
	 * textures (all texture file formats except PVR).
	 */
	bool						expectsVerticallyFlippedTextures();
	void						setExpectsVerticallyFlippedTextures( bool expectsFlippedTexture );

	/**
	 * Returns whether the texture coordinates for the specfied texture unit expects
	 * that the texture was flipped upside-down during texture loading.
	 *
	 * The vertical axis of the coordinate system of OpenGL is inverted relative to the
	 * CoreGraphics view coordinate system. As a result, some texture file formats may be
	 * loaded upside down. Most common file formats, including JPG, PNG & PVR are loaded
	 * right-way up, but using proprietary texture formats developed for other platforms
	 * may result in textures being loaded upside-down.
	 *
	 * The value of this property is used in combination with the value of the
	 * isUpsideDown property of a texture to determine whether the texture
	 * will be oriented correctly when displayed using these texture coordinates.
	 *
	 * The alignTextureUnit:withTexture: method compares the value of this property
	 * with the isUpsideDown property of the texture to automatically determine
	 * whether these texture coordinates need to be flipped vertically in order to
	 * display the texture correctly, and will do so if needed. As part of that inversion,
	 * the value of this property for the specified texture unit will also be flipped,
	 * to indicate that the texture coordinates are now aligned differently.
	 *
	 * The alignTextureUnit:withTexture: method is invoked automatically when a
	 * texture is assigned to cover this mesh in the mesh node. If you need to
	 * adjust the value of this property, you sould do so before setting a texture
	 * or material into the mesh node.
	 *
	 * The initial value of this property is set when the underlying mesh texture
	 * coordinates are built or loaded. See the expectsVerticallyFlippedTextures
	 * property on the CC3NodesResource class to understand how this property is set
	 * during mesh resource loading.
	 *
	 * When building meshes programmatically, you should endeavour to design the
	 * mesh so that this property will be YES if you will be using vertically-flipped
	 * textures (all texture file formats except PVR).
	 */
	bool						expectsVerticallyFlippedTextureInTextureUnit( GLuint texUnit );

	/**
	 * Sets whether the texture coordinates for the specified texture unit expects
	 * that the texture was flipped upside-down during texture loading.
	 *
	 * See the notes of the expectsVerticallyFlippedTextureInTextureUnit: method
	 * for a discussion of texture coordinate orientation.
	 *
	 * Setting the value of this property will change the way the texture coordinates
	 * are aligned when a texture is assigned to cover this texture unit for this mesh.
	 */
	void						setExpectsVerticallyFlippedTexture( bool expectsFlipped, GLuint texUnit );

	/**
	 * Aligns the texture coordinates of the specified texture unit to the specified texture.
	 *
	 * Under iOS, textures that do not have dimensions that are a power-of-two, will
	 * be padded to dimensions of a power-of-two on loading. The result is that the
	 * texture will be physically larger than is expected by these texture coordinates.
	 *
	 * The usable area of the texture is indicated by its mapSize property, and invoking this
	 * method will align these texture coordinates with the usable size of the specified texture.
	 *
	 * If the value of the expectsVerticallyFlippedTexture:InTextureUnit: property is different
	 * than the value of the isUpsideDown property of the specified texture, the texture coordinates
	 * are not oriented vertically for the texture. If so, this method also flips the texture
	 * coordinates to align with the texture.
	 *
	 * This method is invoked automatically when a texture is assigned to cover this mesh in the
	 * mesh node. Normally, the application has no need to invoke this method directly. However,
	 * you can invoke this method manually if you have changed the texture coordinate alignment
	 * using the expectsVerticallyFlippedTexture:inTextureUnit: method.
	 *
	 * To avoid updating the texture coordinates when no change has occurred, if the coordinates
	 * do not need to be flipped vertically, and the specified texture has the same usable area
	 * as the texture used on the previous invocation (or has a full usable area on the first
	 * invocation), this method does nothing.
	 *
	 * If the number of texture coordinate arrays is less than the number of textures, the last
	 * texture coordinate array will be used by all remaining texture units. In this case, it
	 * will only be aligned for the texture in the same texture unit. Subsequent textures will
	 * use that same alignment.
	 *
	 * Care should be taken when using this method, as it changes the actual vertex content.
	 * This may cause mapping conflicts if the same vertex content is shared by other
	 * CC3MeshNodes that use different textures.
	 */
	void						alignTextureUnit( GLuint texUnit, CC3Texture* aTexture );

	/**
	 * Convenience method that flips the texture coordinate mapping vertically
	 * for the specified texture channels. This has the effect of flipping the
	 * texture for that texture channel vertically on the model. and can be
	 * useful for creating interesting effects, or mirror images.
	 *
	 * This implementation flips correctly if the mesh is mapped
	 * to only a section of the texture (a texture atlas).
	 */
	void						flipVerticallyTextureUnit( GLuint texUnit );

	/**
	 * Convenience method that flips the texture coordinate mapping vertically
	 * for all texture units. This has the effect of flipping the textures
	 * vertically on the model. and can be useful for creating interesting
	 * effects, or mirror images.
	 *
	 * This implementation flips correctly if the mesh is mapped
	 * to only a section of the texture (a texture atlas).
	 *
	 * This has the same effect as invoking the flipVerticallyTextureUnit:
	 * method for all texture units.
	 */
	void						flipTexturesVertically();

	/**
	 * Convenience method that flips the texture coordinate mapping horizontally
	 * for the specified texture channels. This has the effect of flipping the
	 * texture for that texture channel horizontally on the model. and can be
	 * useful for creating interesting effects, or mirror images.
	 *
	 * This implementation flips correctly if the mesh is mapped
	 * to only a section of the texture (a texture atlas).
	 */
	void						flipHorizontallyTextureUnit( GLuint texUnit );

	/**
	 * Convenience method that flips the texture coordinate mapping horizontally
	 * for all texture units. This has the effect of flipping the textures
	 * horizontally on the model. and can be useful for creating interesting
	 * effects, or mirror images.
	 *
	 * This implementation flips correctly if the mesh is mapped
	 * to only a section of the texture (a texture atlas).
	 *
	 * This has the same effect as invoking the flipHorizontallyTextureUnit:
	 * method for all texture units.
	 */
	void						flipTexturesHorizontally();

	/**
	 * Configures the mesh so that a texture applied to the specified texture unit will
	 * be repeated the specified number of times across the mesh, in each dimension.
	 * The repeatFactor argument contains two numbers, corresponding to how many times
	 * in each dimension the texture should be repeated.
	 *
	 * As an example, a value of (1, 2) for the repeatValue indicates that the texture
	 * should repeat twice vertically, but not repeat horizontally.
	 *
	 * When a texture is repeated, the corresponding side of the texture covering this
	 * mesh must have a length that is a power-of-two, otherwise the padding added by
	 * iOS to convert it to a power-of-two length internally will be visible in the
	 * repeating pattern across the mesh.
	 *
	 * For a side that is not repeating, the corresponding side of the texture covering
	 * this mesh does not require a length that is a power-of-two.
	 *
	 * The textureParameters property of any texture covering this mesh should include
	 * the GL_REPEAT setting in each of its texture wrap components that correspond to
	 * a repeatFactor greater than one. The GL_REPEAT setting is the default setting
	 * for CC3Texture.
	 *
	 * For example, if you want to repeat your texture twice in one dimension, but only
	 * once in the other, then you would use a repeatFactor of (1, 2) or (2, 1). For the
	 * side that is repeating twice, the length of that side of the texture must be a
	 * power-of-two. But the other side may have any dimension. The textureParameters
	 * property of the CC3Texture should include the GL_REPEAT setting for the
	 * corresponding texture dimension.
	 *
	 * You can specify a fractional value for either of the components of the repeatFactor
	 * to expand the texture in that dimension so that only part of the texture appears
	 * in that dimension, while potentially repeating multiple times in the other dimension.
	 */
	void						repeatTexture( const ccTex2F& repeatFactor, GLuint texUnit );

	/**
	 * Configures the mesh so that the textures in all texture units will be repeated the
	 * specified number of times across the mesh, in each dimension. The repeatFactor
	 * argument contains two numbers, corresponding to how many times in each dimension
	 * the texture should be repeated.
	 *
	 * This has the same effect as invoking the repeatTexture:forTextureUnit: method
	 * for each texture unit.
	 */
	void						repeatTexture( const ccTex2F& repeatFactor );

	/**
	 * Defines the rectangular area of the textures, for all texture units, that should
	 * be mapped to this mesh.
	 *
	 * This property facilitates the use of sprite-sheets, where the mesh is covered
	 * by a small fraction of a larger texture. This technique has many uses, including
	 * animating a texture onto a mesh, where each section of the full texture is really
	 * a different frame of a texture animation, or simply loading one larger texture
	 * and using parts of it to texture many different meshes.
	 *
	 * The dimensions of this rectangle are taken as fractional portions of the full
	 * area of the texture. Therefore, a rectangle with zero origin, and unit size
	 * ((0.0, 0.0), (1.0, 1.0)) indicates that the mesh should be covered with the
	 * complete texture.
	 *
	 * A rectangle of smaller size, and/or a non-zero origin, indicates that the mesh
	 * should be covered by a fractional area of the texture. For example, a rectangular
	 * value for this property with origin at (0.5, 0.5), and size of (0.5, 0.5) indicates
	 * that only the top-right quarter of the texture will be used to cover this mesh.
	 *
	 * The bounds of the texture rectangle must fit within a unit rectangle. Both the
	 * bottom-left and top-right corners must lie between zero and one in both the
	 * X and Y directions.
	 *
	 * The dimensions of the rectangle in this property are independent of adjustments
	 * made by the  alignWithTexturesIn: and alignWithInvertedTexturesIn: methods.
	 * A unit rectangle value for this property will automatically take into
	 * consideration the adjustment made to the mesh by those methods, and will display
	 * only the part of the texture defined by them. Rectangular values for this property
	 * that are smaller than the unit rectangle will be relative to the displayable area
	 * defined by alignWithTexturesIn: and alignWithInvertedTexturesIn:.
	 *
	 * As an example, if the alignWithTexturesIn: method was used to limit the mesh
	 * to using only 80% of the texture (perhaps when using a non-POT texture), and this
	 * property was set to a rectangle with origin at (0.5, 0.0) and size (0.5, 0.5),
	 * the mesh will be covered by the bottom-right quarter of the usable 80% of the
	 * overall texture.
	 *
	 * This property affects all texture units used by this mesh, to query or change
	 * this property for a single texture unit only, use the textureRectangleForTextureUnit:
	 * and setTextureRectangle:forTextureUnit: methods.
	 *
	 * The initial value of this property is a rectangle with origin at zero, and unit
	 * size, indicating that the mesh will be covered with the complete usable area of
	 * the texture.
	 */
	CCRect						getTextureRectangle();
	void						setTextureRectangle( const CCRect& rect );

	/**
	 * Returns the textureRectangle property from the texture coordinates that are
	 * mapping the specified texture unit index.
	 *
	 * See the notes for the textureRectangle property of this class for an explanation
	 * of the use of this property.
	 */
	CCRect						getTextureRectangleForTextureUnit( GLuint texUnit );

	/**
	 * Sets the textureRectangle property from the texture coordinates that are
	 * mapping the specified texture unit index.
	 *
	 * See the notes for the textureRectangle property of this class for an explanation
	 * of the use of this property.
	 */
	void						setTextureRectangle( const CCRect& aRect, GLuint texUnit );

	/**
	 * The drawing mode indicating how the vertices are connected (points, lines, triangles...).
	 *
	 * This must be set with a valid GL drawing mode enumeration.
	 * The default value is GL_TRIANGLES.
	 */
	GLenum						getDrawingMode();
	void						setDrawingMode( GLenum drawingMode );

	/**
	 * Binds the mesh data to the GL engine without drawing. The specified visitor encapsulates
	 * the currently active camera, and certain drawing options.
	 *
	 * If this mesh is different than the last mesh drawn, this method binds this mesh data to the
	 * GL engine. Otherwise, if this mesh is the same as the mesh already bound, it is not bound again.
	 *
	 * Most drawing operations will use the drawWithVisitor: method instead of this method. This
	 * method can be used for those situations where the binding and drawing operations are manged
	 * separately, such as with vertex skinning.
	 */
	void						bindWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Binds the mesh data to the GL engine and draws the mesh data. The specified visitor
	 * encapsulates the currently active camera, and certain drawing options.
	 *
	 * If this mesh is different than the last mesh drawn, this method binds this mesh data to
	 * the GL engine by invoking the bindWithVisitor: method. Otherwise, if this mesh is the same
	 * as the mesh already bound, it is not bound again,
	 *
	 * Once binding is complete, this method then performs the GL draw operations.
	 *
	 * This is invoked automatically from the draw method of the CC3MeshNode instance that is
	 * using this mesh. Usually, the application never needs to invoke this method directly.
	 */
	void						drawWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Binds the mesh data to the GL engine, and draws a portion of the mesh data, starting
	 * at the vertex at the specified index, and drawing the specified number of vertices. The
	 * specified visitor encapsulates the currently active camera, and certain drawing options.
	 *
	 * If this mesh is different than the last mesh drawn, this method binds this mesh data to
	 * the GL engine by invoking the bindWithVisitor: method. Otherwise, if this mesh is the same
	 * as the mesh already bound, it is not bound again,
	 *
	 * Once binding is complete, this method then performs the GL draw operations.
	 *
	 * This is invoked automatically from the draw method of the CC3MeshNode instance that is
	 * using this mesh. Usually, the application never needs to invoke this method directly.
	 */
	void						drawFrom( GLuint vertexIndex, GLuint vertexCount, CC3NodeDrawingVisitor* visitor );

	/**
	 * Allocates and initializes an autoreleased unnamed instance with an automatically
	 * generated unique tag value. The tag value is generated using a call to nextTag.
	 */
	static CC3Mesh*				mesh();

	/** Allocates and initializes an unnamed autoreleased instance with the specified tag. */
	static CC3Mesh*				meshWithTag( GLuint aTag );

	/**
	 * Allocates and initializes an autoreleased instance with the specified name and an
	 * automatically generated unique tag value. The tag value is generated using a call to nextTag.
	 */
	static CC3Mesh*				meshWithName( const std::string& aName );

	/** Allocates and initializes an autoreleased instance with the specified tag and name. */
	static CC3Mesh*				meshWithTag( GLuint aTag, const std::string& aName );		

	void						createVertexContent( CC3VertexContent vtxContentTypes );
	bool						ensureCapacity( GLuint vtxCount );

	/**
	 * Populates any shader program uniform variables that have draw scope,
	 * and then draws the mesh vertices to the GL engine.
	 *
	 * If the vertexIndices property is not nil, the draw method is invoked on that
	 * CC3VertexIndices instance. Otherwise, the draw method is invoked on the
	 * CC3VertexLocations instance in the vertexLocations property.
	 */
	void						drawVerticesWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Populates any shader program uniform variables that have draw scope,
	 * and then draws the specified range of mesh vertices to the GL engine.
	 *
	 * If the vertexIndices property is not nil, the draw method is invoked on that
	 * CC3VertexIndices instance. Otherwise, the draw method is invoked on the
	 * CC3VertexLocations instance in the vertexLocations property.
	 */
	void						drawVerticesFrom( GLuint vertexIndex, GLuint vertexCount, CC3NodeDrawingVisitor* visitor );

	void						initWithTag( GLuint aTag, const std::string& aName );
	void						initWithTag( GLuint aTag );

	void						populateFrom( CC3Mesh* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	GLuint						nextTag();
	void						resetTagAllocation();

	/**
	 * Ensures that this mesh has vertexContentType defined.
	 *
	 * This method is invoked by each of the populateAs... family of methods, prior to populating
	 * the mesh contents.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking any of the populateAs...
	 * family of methods, to define the content type for each vertex.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value
	 * of (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate).
	 * and the mesh will be populated with location, normal and texture coordinates for each vertex.
	 *
	 * If you do not need either of the normal or texture coordinates, set the vertexContentTypes
	 * property accordingly prior to invoking any of the populateAs... methods.
	 */
	void						ensureVertexContent();

	/**
	 * Populates this instance as a simple triangular mesh.
	 *
	 * The specified face defines the three vertices at the corners of the triangular mesh in 3D space.
	 * The vertices within the CC3Face structure are specified in the winding order of the triangular
	 * face. The winding order of the specified face determines the winding order of the vertices in
	 * the mesh, and the direction of the normal vector applied to each of the vertices. Since the
	 * resulting triangular mesh is flat, all vertices will have the same normal vector.
 
	 * Although the triangle can be created with the corners can be anywhere in 3D space, for simplicity
	 * of construction, it is common practice, when using this method, to specify the mesh in the X-Y
	 * plane (where all three corners have a zero Z-component), and then rotate the node containing this
	 * mesh to an orientation in 3D space.
	 *
	 * The texCoords parameter is an array of ccTex2F structures, providing the texture coordinates for
	 * the cooresponding vertices of the face. This array must have three elements, one for each vertex
	 * in the specified face. If the mesh will not be covered with a texture, you can pass in any values
	 * in the elements of this array.
	 *
	 * The tessellation property determines how the mesh will be tessellated into smaller faces. The
	 * specified tessellation value indicates how many divisions each side of the main triangle should
	 * be divided into. Each side of the triangular mesh is tessellated into the same number of divisions.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh is to be covered
	 * with a texture, use the texture property of this mesh to set the texture. If a solid color is
	 * desired, leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method, to define the
	 * content type for each vertex. Content types kCC3VertexContentLocation, kCC3VertexContentNormal, and
	 * kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value of
	 * (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate), and
	 * the mesh will be populated with location, normal and texture coordinates for each vertex.
	 */
	void						populateAsTriangle( const CC3Face& face, ccTex2F* texCoords, GLuint divsPerSide );

	/**
	 * Populates this instance as a simple rectangular mesh of the specified size, centered at the origin,
	 * and laid out on the X-Y plane.
	 *
	 * The rectangular mesh contains only one face with two triangles. The result is the same as invoking
	 * populateAsCenteredRectangleWithSize:andTessellation: with the divsPerAxis argument set to {1,1}.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh is to be covered
	 * with a texture, use the texture property of this mesh to set the texture. If a solid color is desired,
	 * leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method, to define the
	 * content type for each vertex. Content types kCC3VertexContentLocation, kCC3VertexContentNormal, and
	 * kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value of
	 * (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate), and
	 * the mesh will be populated with location, normal and texture coordinates for each vertex.
	 */
	void						populateAsCenteredRectangleWithSize( const CCSize& rectSize );

	/**
	 * Populates this instance as a simple rectangular mesh of the specified size, centered at the origin,
	 * and laid out on the X-Y plane.
	 *
	 * The large rectangle can be divided into many smaller divisions. Building a rectanglular surface
	 * from more than one division can dramatically improve realism when the surface is illuminated with
	 * specular lighting or a tightly focused spotlight, because increasing the face count increases the
	 * number of vertices that interact with the specular or spot lighting.
	 *
	 * The divsPerAxis argument indicates how to break this large rectangle into multiple faces. The X & Y
	 * elements of the divsPerAxis argument indicate how each axis if the rectangle should be divided into
	 * faces. The total number of faces in the rectangle will therefore be the multiplicative product of
	 * the X & Y elements of the divsPerAxis argument.
	 *
	 * For example, a value of {5,5} for the divsPerAxis argument will result in the rectangle being
	 * divided into 25 faces, arranged into a 5x5 grid.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh is to be covered
	 * with a texture, use the texture property of this mesh to set the texture. If a solid color is desired,
	 * leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method, to define the
	 * content type for each vertex. Content types kCC3VertexContentLocation, kCC3VertexContentNormal, and
	 * kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value of
	 * (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate), and
	 * the mesh will be populated with location, normal and texture coordinates for each vertex.
	 */
	void						populateAsCenteredRectangleWithSize( const CCSize& rectSize, const CC3Tessellation& divsPerAxis );

	/**
	 * Populates this instance as a simple rectangular mesh of the specified size, with the specified
	 * relative origin, and laid out on the X-Y plane.
	 *
	 * The rectangular mesh contains only one face with two triangles. The result is the same as invoking
	 * the populateAsRectangleWithSize:andRelativeOrigin:andTessellation: with the divsPerAxis argument
	 * set to {1,1}.
	 *
	 * The relative origin is a fractional point that is relative to the rectangle's extent, and indicates
	 * where the origin of the rectangular mesh is to be located. The mesh origin is the origin of the
	 * local coordinate system of the mesh, and is the basis for all transforms applied to the mesh
	 * (including the location and rotation properties).
	 *
	 * The specified relative origin should be a fractional value. If it is {0, 0}, the rectangle will
	 * be laid out so that the bottom-left corner is at the origin. If it is {1, 1}, the rectangle will
	 * be laid out so that the top-right corner of the rectangle is at the origin. If it is {0.5, 0.5},
	 * the rectangle will be laid out with the origin at the center, as in the
	 * populateAsCenteredRectangleWithSize: method.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh is to be covered
	 * with a texture, use the texture property of this mesh to set the texture. If a solid color is
	 * desired, leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method, to define
	 * the content type for each vertex. Content types kCC3VertexContentLocation, kCC3VertexContentNormal,
	 * and kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value of
	 * (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate), and
	 * the mesh will be populated with location, normal and texture coordinates for each vertex.
	 */
	void						populateAsRectangleWithSize( const CCSize& rectSize, const CCPoint& origin );

	/**
	 * Populates this instance as a simple rectangular mesh of the specified size, with the specified
	 * relative origin, and laid out on the X-Y plane.
	 *
	 * The large rectangle can be divided into many smaller divisions. Building a rectanglular surface
	 * from more than one division can dramatically improve realism when the surface is illuminated with
	 * specular lighting or a tightly focused spotlight, because increasing the face count increases the
	 * number of vertices that interact with the specular or spot lighting.
	 *
	 * The divsPerAxis argument indicates how to break this large rectangle into multiple faces. The X & Y
	 * elements of the divsPerAxis argument indicate how each axis if the rectangle should be divided into
	 * faces. The total number of faces in the rectangle will therefore be the multiplicative product of
	 * the X & Y elements of the divsPerAxis argument.
	 *
	 * For example, a value of {5,5} for the divsPerAxis argument will result in the rectangle being
	 * divided into 25 faces, arranged into a 5x5 grid.
	 *
	 * The relative origin is a fractional point that is relative to the rectangle's extent, and indicates
	 * where the origin of the rectangular mesh is to be located. The mesh origin is the origin of the
	 * local coordinate system of the mesh, and is the basis for all transforms applied to the mesh
	 * (including the location and rotation properties).
	 *
	 * The specified relative origin should be a fractional value. If it is {0, 0}, the rectangle will
	 * be laid out so that the bottom-left corner is at the origin. If it is {1, 1}, the rectangle will
	 * be laid out so that the top-right corner of the rectangle is at the origin. If it is {0.5, 0.5},
	 * the rectangle will be laid out with the origin at the center, as in the
	 * populateAsCenteredRectangleWithSize: method.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh is to be covered
	 * with a texture, use the texture property of this mesh to set the texture. If a solid color is desired,
	 * leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method, to define the
	 * content type for each vertex. Content types kCC3VertexContentLocation, kCC3VertexContentNormal, and
	 * kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value of
	 * (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate), and
	 * the mesh will be populated with location, normal and texture coordinates for each vertex.
	 */
	void						populateAsRectangleWithSize( const CCSize& rectSize, const CCPoint& origin, const CC3Tessellation& divsPerAxis );

	/**
	 * Populates this instance as a flat, single-sided circular disk mesh of the specified radius,
	 * centered at the origin, and laid out on the X-Y plane.
	 *
	 * The surface of the disk is divided into many smaller divisions, both in the radial and angular dimensions.
	 *
	 * The radialAndAngleDivs argument indicates how to divide the surface of the disks into divisions.
	 * The X element of the radialAndAngleDivs argument indicates how many radial divisions will occur
	 * from the center and the circuferential edge. A value of one means that the mesh will consist of
	 * a series of radial triangles from the center of the circle to the edge. A larger value for the
	 * X element of the radialAndAngleDivs argument will structure the mesh as a series of concentric
	 * rings. This value must be at least one.
	 * 
	 * The Y element of the radialAndAngleDivs argument indicates how many angular divisions will occur
	 * around the circumference. This value must be at least three, which will essentially render the
	 * circle as a triangle. But, typically, this value will be larger.
	 * 
	 * For example, a value of {4,24} for the radialAndAngleDivs argument will result in the disk being
	 * divided into four concentric rings, each divided into 24 segments around the circumference of the circle.
	 * 
	 * Each segement, except those in the innermost disk is trapezoidal, and will be constructed from two
	 * triangular mesh faces. Therefore, the number of triangles in the mesh will be (2X - 1) * Y, where
	 * X = radialAndAngleDivs.x and Y = radialAndAngleDivs.y.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh is to be covered
	 * with a texture, use the texture property of this mesh to set the texture. If a solid color is desired,
	 * leave the texture property unassigned.
	 *
	 * The texture is mapped to the tessellated disk as if a tagential square was overlaid over the circle,
	 * starting from the lower left corner, where both X and Y are at a minimum. The center of the disk
	 * maps to the center of the texture.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method, to define the
	 * content type for each vertex. Content types kCC3VertexContentLocation, kCC3VertexContentNormal, and
	 * kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value of
	 * (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate), and
	 * the mesh will be populated with location, normal and texture coordinates for each vertex.
	 */
	void						populateAsDiskWithRadius( GLfloat radius, const CC3Tessellation& radialAndAngleDivs );

	/**
	 * Populates this instance as a simple rectangular box mesh from the specified bounding box, which
	 * contains two of the diagonal corners of the box.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh is to be covered
	 * with a texture, use the texture property of this mesh to set the texture. If a solid color is desired,
	 * leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method, to define the
	 * content type for each vertex. Content types kCC3VertexContentLocation, kCC3VertexContentNormal, and
	 * kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value of
	 * (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate), and
	 * the mesh will be populated with location, normal and texture coordinates for each vertex.
	 *
	 * If a texture is to be wrapped around this mesh, since the single texture is wrapped around all six
	 * sides of the box, the texture will be mapped according to the layout illustrated in the texture
	 * file BoxTexture.png, included in the distribution.
	 *
	 * The "front" of the box is the side that faces towards the positive-Z axis, the "top" of the box is
	 * the side that faces towards the positive-Y axis, and the "right" side of the box is the side that
	 * faces towards the positive-X axis.
	 *
	 * For the purposes of wrapping a texture around the box, the texture will wrap uniformly around all
	 * sides, and the texture will not appear stretched between any two adjacent sides. This is useful when
	 * you are texturing the box with a simple rectangular repeating pattern and want the texture to appear
	 * consistent across the sides, for example, a brick pattern wrapping around all four sides of a house.
	 *
	 * Depending on the relative aspect of the height and width of the box, the texture may appear distorted
	 * horizontal or vertically. If you need to correct that, you can use the repeatTexture: method, and
	 * adjust one of the dimensions.
	 *
	 * For higher fidelity in applying textures to non-cube boxes, so that the texture will not be stretched
	 * to fit, use the populateAsSolidBox:withCorner: method.
	 *
	 * Thanks to Cocos3D user andyman for contributing the prototype code and texture template file for this method.
	 */
	void						populateAsSolidBox( const CC3Box& box );

	/**
	 * Populates this instance as a simple rectangular box mesh from the specified bounding box, which
	 * contains two of the diagonal corners of the box.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh is to be covered
	 * with a texture, use the texture property of this mesh to set the texture. If a solid color is desired,
	 * leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method, to define the
	 * content type for each vertex. Content types kCC3VertexContentLocation, kCC3VertexContentNormal, and
	 * kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value of
	 * (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate), and
	 * the mesh will be populated with location, normal and texture coordinates for each vertex.
	 *
	 * If a texture is to be wrapped around this mesh, since the single texture is wrapped around all six
	 * sides of the box, the texture will be mapped according to the layout illustrated in the texture
	 * file BoxTexture.png, included in the distribution.
	 *
	 * The "front" of the box is the side that faces towards the positive-Z axis, the "top" of the box
	 * is the side that faces towards the positive-Y axis, and the "right" side of the box is the side
	 * that faces towards the positive-X axis.
	 *
	 * For the purposes of wrapping the texture around the box, this method assumes that the texture is
	 * an unfolded cube. The box can be created with any relative dimensions, but if it is not a cube,
	 * the texture may appear stretched or shrunk on two or more sides. The texture will still fully wrap
	 * all six sides of the box, but the texture is stretched or shrunk to fit each side according to its
	 * dimension relative to the other sides. The appearance will be as if you had started with a textured
	 * cube and then pulled one or two of the dimensions out further.
	 *
	 * For higher fidelity in applying textures to non-cube boxes, so that the texture will not be stretched
	 * to fit, use either of the populateAsSolidBox: or populateAsSolidBox:withCorner: methods, with a
	 * texture whose layout is compatible with the aspect ratio of the box.
	 *
	 * Thanks to Cocos3D user andyman for contributing the prototype code and texture template file for this method.
	 */
	void						populateAsCubeMappedSolidBox( const CC3Box& box );

	/**
	 * Populates this instance as a simple rectangular box mesh from the specified bounding box, which
	 * contains two of the diagonal corners of the box, and configures the mesh texture coordinates so
	 * that the entire box can be wrapped in a single texture.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh is to be covered
	 * with a texture, use the texture property of this mesh to set the texture. If a solid color is desired,
	 * leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method, to define the
	 * content type for each vertex. Content types kCC3VertexContentLocation, kCC3VertexContentNormal, and
	 * kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value of
	 * (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate), and
	 * the mesh will be populated with location, normal and texture coordinates for each vertex.
	 *
	 * If a texture is to be wrapped around this mesh, since the single texture is wrapped around all six
	 * sides of the box, the texture will be mapped according to the layout illustrated in the texture file
	 * BoxTexture.png, included in the distribution.
	 *
	 * The "front" of the box is the side that faces towards the positive-Z axis, the "top" of the box
	 * is the side that faces towards the positive-Y axis, and the "right" side of the box is the side
	 * that faces towards the positive-X axis.
	 *
	 * For the purposes of wrapping the texture around the box, the corner argument specifies the relative
	 * point in the texture that will map to the corner of the box that is at the juncture of the "left",
	 * "front" and "bottom" sides (see the BoxTexture.png image for a better understanding of this point).
	 * The corner argument is specified as a fraction in each of the S & T dimensions of the texture.
	 * In the CGPoint that specifies the corner, the x & y elements of the CGPoint correspond to the S & T
	 * dimensions of this left-front-bottom corner mapping, with each value being between zero and one.
	 *
	 * Since, by definition, opposite sides of the box have the same dimensions, this single corner point
	 * identifies the S & T dimensions of all six of the sides of the box. A value of (1/4, 1/3) for the
	 * corner is used when the box is a cube. A smaller value for the x-element would move the corner to
	 * the left in the texture layout, indicating that the left and right sides are shallower than they
	 * are in a cube, and that the front and back are wider than in a cube, and vice-versa for a larger
	 * value in the x-element of the corner. Similarly for the y-element. A y-element that is smaller
	 * than 1/3, moves the corner point downwards on the texture, indicating that the bottom and top are
	 * shallower than they are in a cube, or that the front and back are higher than they are in a cube.
	 *
	 * The two axes defined by the corner are interrelated, because the sides need to be the same depth as
	 * the top and bottom. The best way to determine the values to use in the corner is to use the measure
	 * of this point (where the "left", "front", and "bottom" sides meet) from the layout of the texture.
	 * If the aspect of the corner on the texture does not align with the aspect of the width, height and
	 * depth of the box, the texture will appear stretched on one or two sides relative to the others.
	 *
	 * Thanks to Cocos3D user andyman for contributing the prototype code and texture template file for this method.
	 */
	void						populateAsSolidBox( const CC3Box& box, const CCPoint& corner );

	/**
	 * Populates this instance as a wire-frame box with the specified dimensions.
	 *
	 * This is a convenience method for creating a simple, but useful, shape.
	 */
	void						populateAsWireBox( const CC3Box& box );

	/**
	 * Populates this instance as a spherical mesh of the specified radius, centered at the origin.
	 *
	 * The surface of the sphere is divided into many smaller divisions, similar to latitude and
	 * longtitude divisions. The sphere mesh contains two poles, where the surface intersects the
	 * positive and negative Y-axis.
	 *
	 * The divsPerAxis argument indicates how to divide the surface of the sphere into divisions.
	 * The X element of the divsPerAxis argument indicates how many longtitude divisions will
	 * occur around one circumnavigation of the equator. The Y element of the divsPerAxis argument
	 * indicates how many latitude divisions will occur between the north pole and the south pole.
	 * 
	 * For example, a value of {12,8} for the divsPerAxis argument will result in the sphere being
	 * divided into twelve divisions of longtitude around the equator, and eight divisions of
	 * latitude between the north and south poles.
	 *
	 * Except at the poles, each division is roughly trapezoidal and is drawn as two triangles.
	 * At the poles, each division is a single triangle.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh
	 * is to be covered with a texture, use the texture property of this mesh to set
	 * the texture. If a solid color is desired, leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method,
	 * to define the content type for each vertex. Content types kCC3VertexContentLocation,
	 * kCC3VertexContentNormal, and kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value
	 * of (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate).
	 * and the mesh will be populated with location, normal and texture coordinates for each vertex.
	 *
	 * If a texture is applied to this mesh, it is mapped to the sphere with a simple cylindrical
	 * projection around the equator (similar to Mercator projection without the north-south stretching).
	 * This type of projection is typical of maps of the earth taken from space, and results in the
	 * smooth curving of any texture around the sphere from the equator to the poles. Texture wrapping
	 * begins at the negative Z-axis, so the center of the texture will be positioned at the point where
	 * the sphere intersects the positive Z-axis, and the conceptual seam (where the left and right
	 * edges of the texture are stitched together) will occur where the sphere intersects the plane
	 * (X = 0) along the negative-Z axis. This texture orientation means that the center of the texture
	 * will face the forwardDirection of the sphere node.
	 */
	void						populateAsSphereWithRadius( GLfloat radius, const CC3Tessellation& divsPerAxis );

	/**
	 * Populates this instance as a conical mesh of the specified radius and height.
	 *
	 * The mesh is constructed so that the base of the cone is centered on the origin of the X-Z plane,
	 * and the apex is on the positive Y-axis at the specified height. The cone is open and does not
	 * have a bottom.
	 *
	 * The surface of the cone is divided into many smaller divisions, as specified by the
	 * angleAndHeightsDivs parameter. The X-coordinate of this parameter indicates how many angular
	 * divisions are created around the circumference of the base, and the Y-coordinate of this
	 * parameter indicates how many vertical divisions are created between the base and the apex.
	 * 
	 * For example, a value of {12,8} for the angleAndHeightsDivs parameter will result in a cone with
	 * 12 divisions around the circumference of the base, and 8 divisions along the Y-axis to the apex.
	 *
	 * By reducing the number of angular divisions to 3 or 4, you can use this method to create a
	 * tetrahedron or square pyramid, respectively.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh
	 * is to be covered with a texture, use the texture property of this mesh to set
	 * the texture. If a solid color is desired, leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method,
	 * to define the content type for each vertex. Content types kCC3VertexContentLocation,
	 * kCC3VertexContentNormal, and kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value
	 * of (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate).
	 * and the mesh will be populated with location, normal and texture coordinates for each vertex.
	 *
	 * If a texture is applied to this mesh, it is mapped to the cone with a simple horizontal
	 * projection. Horizontal lines in the texture will remain parallel, but vertical lines will
	 * converge at the apex. Texture wrapping begins at the negative Z-axis, so the center of the
	 * texture will be positioned at the point where the cone intersects the positive Z-axis, and
	 * the conceptual seam (where the left and right edges of the texture are stitched together)
	 * will occur where the cone intersects the negative-Z axis. This texture orientation means
	 * that the center of the texture will face the forwardDirection of the cone node.
	 */
	void						populateAsHollowConeWithRadius( GLfloat radius, GLfloat height, const CC3Tessellation& angleAndHeightDivs );

	/**
	 * Populates this instance as a line strip with the specified number of vertex points.
	 * The data for the points that define the end-points of the lines are contained within the
	 * specified vertices array. The vertices array must contain at least vertexCount elements.
	 *
	 * The lines are specified and rendered as a strip, where each line is connected to the
	 * previous and following lines. Each line starts at the point where the previous line ended,
	 * and that point is defined only once in the vertices array. Therefore, the number of lines
	 * drawn is equal to one less than the specified vertexCount.
	 * 
	 * The shouldRetainVertices flag indicates whether the data in the vertices array should
	 * be retained by this instance. If this flag is set to YES, the data in the vertices array
	 * will be copied to an internal array that is managed by this instance. If this flag is
	 * set to NO, the data is not copied internally and, instead, a reference to the vertices
	 * data is established. In this case, it is up to you to manage the lifespan of the data
	 * contained in the vertices array.
	 *
	 * If you are defining the vertices data dynamically in another method, you may want to
	 * set this flag to YES to have this instance copy and manage the data. If the vertices
	 * array is a static array, you can set this flag to NO.
	 *
	 * This is a convenience method for creating a simple, but useful, shape.
	 */
	void						populateAsLineStripWith( GLuint vertexCount, CC3Vector* vertices, bool shouldRetainVertices );

protected:
	CC3VertexLocations*			m_vertexLocations;
	CC3VertexNormals*			m_vertexNormals;
	CC3VertexTangents*			m_vertexTangents;
	CC3VertexTangents*			m_vertexBitangents;
	CC3VertexColors*			m_vertexColors;
	CC3VertexTextureCoordinates* m_vertexTextureCoordinates;
	CCArray*					m_overlayTextureCoordinates;
	CC3VertexBoneWeights*		m_vertexBoneWeights;
	CC3VertexBoneIndices*		m_vertexBoneIndices;
	CC3VertexPointSizes*		m_vertexPointSizes;
	CC3VertexIndices*			m_vertexIndices;
	CC3FaceArray*				m_faces;
	GLfloat						m_capacityExpansionFactor;
	bool						m_shouldInterleaveVertices : 1;
};

	/**
	 * CC3FaceArray holds additional cached calculated information about mesh faces,
	 * such as the centers, normals, planes and neighbours of each face.
	 *
	 * The additional face data is maintained in separate internal arrays, and each
	 * type of data is lazily allocated and initialized when accessed the first time.
	 * So, requesting information about the center of a face will cause all of the
	 * face centers to be calculated and cached, but will not cause the face normals
	 * or planes to be calculated and cached. They will be calculated and cached when
	 * a face normal or plane is explicitly requested.
	 */
class CC3FaceArray : public CC3Identifiable 
{
	DECLARE_SUPER( CC3Identifiable );
public:
	CC3FaceArray();
	virtual ~CC3FaceArray();

	/**
	 * The mesh containing the vertices for which this face array is managing faces.
	 *
	 * Setting this property will cause the centers, normals, planes and neighbours
	 * properties to be deallocated and then re-built on the next access.
	 */
	CC3Mesh*					getMesh();
	void						setMesh( CC3Mesh* mesh );

	/**
	 * Indicates the number of faces in this array, as retrieved from the mesh.
	 *
	 * The value of this property will be zero until the mesh property is set.
	 */
	GLuint						getFaceCount();

	/**
	 * Indicates whether information about the faces of this mesh should be cached.
	 *
	 * If this property is set to NO, accessing information about the faces through
	 * the methods indicesAt:, centerAt:, normalAt:, or planeAt:, will be calculated
	 * dynamically from the mesh data.
	 *
	 * If such data will be accessed frequently, this repeated dynamic calculation may
	 * cause a noticable impact to performance. In such a case, this property can be
	 * set to YES to cause the data to be calculated once and cached, improving the
	 * performance of subsequent accesses to information about the faces.
	 *
	 * However, caching information about the faces will increase the amount of memory
	 * required by the mesh, sometimes significantly. To avoid this additional memory
	 * overhead, in general, you should leave this property set to NO, unless intensive
	 * access to face information is causing a performance impact.
	 *
	 * An example of a situation where the use of this property may be noticable,
	 * is when adding shadow volumes to nodes. Shadow volumes make intense use of
	 * accessing face information about the mesh that is casting the shadow.
	 *
	 * When the value of this property is set to NO, any data cached during previous
	 * access through the indicesAt:, centerAt:, normalAt:, or planeAt:, methods will
	 * be cleared.
	 *
	 * Because the face neighbour data returned by the neighboursAt: method is
	 * a function of the relationship between faces, that data is always cached,
	 * and is not affected by the setting of this property.
	 *
	 * The initial value of this property is NO.
	 */
	bool						shouldCacheFaces();
	void						setShouldCacheFaces( bool shouldCache );

	/**
	 * Allocates and initializes an autoreleased unnamed instance with an automatically
	 * generated unique tag value. The tag value is generated using a call to nextTag.
	 */
	static CC3FaceArray*		faceArray();

	/** Allocates and initializes an unnamed autoreleased instance with the specified tag. */
	static CC3FaceArray*		faceArrayWithTag( GLuint aTag );

	/**
	 * Allocates and initializes an autoreleased instance with the specified name and an
	 * automatically generated unique tag value. The tag value is generated using a call to nextTag.
	 */
	static CC3FaceArray*		faceArrayWithName( const std::string& aName );

	/** Allocates and initializes an autoreleased instance with the specified tag and name. */
	static CC3FaceArray*		faceArrayWithTag( GLuint aTag, const std::string& aName );

	/**
	 * An array containing the vertex indices of each face.
	 *
	 * This property will be lazily initialized on the first access after the mesh
	 * property has been set, by an automatic invocation of the populateIndices method.
	 * When created in this manner, the memory allocated to hold the data in the
	 * returned array will be managed by this instance.
	 *
	 * Alternately, this property may be set directly to an array that was created
	 * externally. In this case, the underlying data memory is not managed by this
	 * instance, and it is up to the application to manage the allocation and
	 * deallocation of the underlying data memory, and to ensure that the array is
	 * large enough to contain the number of CC3FaceIndices structures specified by
	 * the faceCount property.
	 */
	CC3FaceIndices*				getIndices();
	void						setIndices( CC3FaceIndices* indices );

	/**
	 * Returns the vertex indices of the face at the specified index,
	 * lazily initializing the indices property if needed.
	 */
	CC3FaceIndices				getIndicesAt( GLuint faceIndex );

	/**
	 * Populates the contents of the indices property from the associated mesh,
	 * automatically allocating memory for the property if needed.
	 *
	 * This method is invoked automatically on the first access of the indices
	 * property after the mesh property has been set. Usually, the application
	 * never needs to invoke this method directly.
	 *
	 * However, if the indices property has been set to an array created outside
	 * this instance, this method may be invoked to populate that array from the mesh.
	 */
	void						populateIndices();

	/**
	 * Allocates underlying memory for the indices property, and returns a pointer
	 * to the allocated memory.
	 *
	 * This method will allocate enough memory for the indices property to hold
	 * the number of CC3FaceIndices structures specified by the faceCount property.
	 *
	 * This method is invoked automatically by the populateIndices method.
	 * Usually, the application never needs to invoke this method directly.
	 *
	 * It is safe to invoke this method more than once, but understand that any 
	 * previously allocated memory will be safely released prior to the allocation
	 * of the new memory. The memory allocated earlier will therefore be lost and
	 * should not be referenced.
	 * 
	 * The memory allocated will automatically be released when this instance
	 * is deallocated.
	 */
	CC3FaceIndices*				allocateIndices();

	/**
	 * Deallocates the underlying memory that was previously allocated with the
	 * allocateIndices method. It is safe to invoke this method more than once,
	 * or even if the allocateIndices method was not previously invoked.
	 *
	 * This method is invoked automatically when allocateIndices is invoked,
	 * and when this instance is deallocated. Usually, the application never
	 * needs to invoke this method directly.
	 */
	void						deallocateIndices();

	/** Marks the indices data as dirty. It will be automatically repopulated on the next access. */
	void						markIndicesDirty();

	/**
	 * An array containing the location of the center of each face.
	 *
	 * This property will be lazily initialized on the first access after the mesh
	 * property has been set, by an automatic invocation of the populateCenters method.
	 * When created in this manner, the memory allocated to hold the data in the
	 * returned array will be managed by this instance.
	 *
	 * Alternately, this property may be set directly to an array that was created
	 * externally. In this case, the underlying data memory is not managed by this
	 * instance, and it is up to the application to manage the allocation and
	 * deallocation of the underlying data memory, and to ensure that the array is
	 * large enough to contain the number of CC3Vector structures specified by the
	 * faceCount property.
	 */
	CC3Vector*					getCenters();
	void						setCenters( CC3Vector* centers );

	/**
	 * Returns the center of the face at the specified index,
	 * lazily initializing the centers property if needed.
	 */
	CC3Vector					getCenterAt( GLuint faceIndex );

	/**
	 * Populates the contents of the centers property from the associated mesh,
	 * automatically allocating memory for the property if needed.
	 *
	 * This method is invoked automatically on the first access of the centers
	 * property after the mesh property has been set. Usually, the application
	 * never needs to invoke this method directly.
	 *
	 * However, if the centers property has been set to an array created outside
	 * this instance, this method may be invoked to populate that array from the mesh.
	 *
	 * In addition, if the mesh is deformed in some way, the application may invoke
	 * this method in order to recalculate updated face centers.
	 */
	void						populateCenters();

	/**
	 * Allocates underlying memory for the centers property, and returns a pointer
	 * to the allocated memory.
	 *
	 * This method will allocate enough memory for the centers property to hold
	 * the number of CC3Vector structures specified by the faceCount property.
	 *
	 * This method is invoked automatically by the populateCenters method.
	 * Usually, the application never needs to invoke this method directly.
	 *
	 * It is safe to invoke this method more than once, but understand that any 
	 * previously allocated memory will be safely released prior to the allocation
	 * of the new memory. The memory allocated earlier will therefore be lost and
	 * should not be referenced.
	 * 
	 * The memory allocated will automatically be released when this instance
	 * is deallocated.
	 */
	CC3Vector*					allocateCenters();

	/**
	 * Deallocates the underlying memory that was previously allocated with the
	 * allocateCenters method. It is safe to invoke this method more than once,
	 * or even if the allocateCenters method was not previously invoked.
	 *
	 * This method is invoked automatically when allocateCenters is invoked,
	 * and when this instance is deallocated. Usually, the application never
	 * needs to invoke this method directly.
	 */
	void						deallocateCenters();

	/** Marks the centers data as dirty. It will be automatically repopulated on the next access. */
	void						markCentersDirty();

	/**
	 * An array containing the normal vector for each face.
	 *
	 * This property will be lazily initialized on the first access after the mesh
	 * property has been set, by an automatic invocation of the populateNormals method.
	 * When created in this manner, the memory allocated to hold the data in the
	 * returned array will be managed by this instance.
	 *
	 * Alternately, this property may be set directly to an array that was created
	 * externally. In this case, the underlying data memory is not managed by this
	 * instance, and it is up to the application to manage the allocation and
	 * deallocation of the underlying data memory, and to ensure that the array is
	 * large enough to contain the number of CC3Vector structures specified by the
	 * faceCount property.
	 */
	CC3Vector*					getNormals();
	void						setNormals( CC3Vector* normals );

	/**
	 * Returns the normal of the face at the specified index,
	 * lazily initializing the normals property if needed.
	 */
	CC3Vector					getNormalAt( GLuint faceIndex );

	/**
	 * Populates the contents of the normals property from the associated mesh,
	 * automatically allocating memory for the property if needed.
	 *
	 * This method is invoked automatically on the first access of the normals
	 * property after the mesh property has been set, or from the ensureNormals
	 * method. Usually, the application never needs to invoke this method directly.
	 *
	 * However, if the normals property has been set to an array created outside
	 * this instance, this method may be invoked to populate that array from the mesh.
	 *
	 * In addition, if the mesh is deformed in some way, the application may invoke
	 * this method in order to recalculate updated face normals.
	 */
	void						populateNormals();

	/**
	 * Allocates underlying memory for the normals property, and returns a pointer
	 * to the allocated memory.
	 *
	 * This method will allocate enough memory for the normals property to hold
	 * the number of CC3Vector structures specified by the faceCount property.
	 *
	 * This method is invoked automatically by the populateNormals method.
	 * Usually, the application never needs to invoke this method directly.
	 *
	 * It is safe to invoke this method more than once, but understand that any 
	 * previously allocated memory will be safely released prior to the allocation
	 * of the new memory. The memory allocated earlier will therefore be lost and
	 * should not be referenced.
	 * 
	 * The memory allocated will automatically be released when this instance
	 * is deallocated.
	 */
	CC3Vector*					allocateNormals();

	/**
	 * Deallocates the underlying memory that was previously allocated with the
	 * allocateNormals method. It is safe to invoke this method more than once,
	 * or even if the allocateNormals method was not previously invoked.
	 *
	 * This method is invoked automatically when allocateNormals is invoked,
	 * and when this instance is deallocated. Usually, the application never
	 * needs to invoke this method directly.
	 */
	void						deallocateNormals();

	/** Marks the normals data as dirty. It will be automatically repopulated on the next access. */
	void						markNormalsDirty();

	/**
	 * An array containing the plane equation for each face.
	 *
	 * This property will be lazily initialized on the first access after the mesh
	 * property has been set, by an automatic invocation of the populatePlanes method.
	 * When created in this manner, the memory allocated to hold the data in the
	 * returned array will be managed by this instance.
	 *
	 * Alternately, this property may be set directly to an array that was created
	 * externally. In this case, the underlying data memory is not managed by this
	 * instance, and it is up to the application to manage the allocation and
	 * deallocation of the underlying data memory, and to ensure that the array is
	 * large enough to contain the number of CC3Plane structures specified by the
	 * faceCount property.
	 */
	CC3Plane*					getPlanes();
	void						setPlanes( CC3Plane* planes );

	/**
	 * Returns the plane of the face at the specified index,
	 * lazily initializing the planes property if needed.
	 */
	CC3Plane					getPlaneAt( GLuint faceIndex );
		
	/**
	 * Populates the contents of the planes property from the associated mesh,
	 * automatically allocating memory for the property if needed.
	 *
	 * This method is invoked automatically on the first access of the planes
	 * property after the mesh property has been set. Usually, the application
	 * never needs to invoke this method directly.
	 *
	 * However, if the planes property has been set to an array created outside
	 * this instance, this method may be invoked to populate that array from the mesh.
	 *
	 * In addition, if the mesh is deformed in some way, the application may invoke
	 * this method in order to recalculate updated face planes.
	 */
	void						populatePlanes();

	/**
	 * Allocates underlying memory for the planes property, and returns a pointer
	 * to the allocated memory.
	 *
	 * This method will allocate enough memory for the planes property to hold
	 * the number of CC3Plane structures specified by the faceCount property.
	 *
	 * This method is invoked automatically by the populatePlanes method.
	 * Usually, the application never needs to invoke this method directly.
	 *
	 * It is safe to invoke this method more than once, but understand that any 
	 * previously allocated memory will be safely released prior to the allocation
	 * of the new memory. The memory allocated earlier will therefore be lost and
	 * should not be referenced.
	 * 
	 * The memory allocated will automatically be released when this instance
	 * is deallocated.
	 */
	CC3Plane*					allocatePlanes();

	/**
	 * Deallocates the underlying memory that was previously allocated with the
	 * allocatePlanes method. It is safe to invoke this method more than once,
	 * or even if the allocatePlanes method was not previously invoked.
	 *
	 * This method is invoked automatically when allocatePlanes is invoked,
	 * and when this instance is deallocated. Usually, the application never
	 * needs to invoke this method directly.
	 */
	void						deallocatePlanes();

	/** Marks the planes data as dirty. It will be automatically repopulated on the next access. */
	void						markPlanesDirty();

	/**
	 * An array containing neighbour data for each face. Each element in this array
	 * contains a CC3FaceNeighbours strucutre which keeps track of the indices to
	 * the neighbouring faces of each face.
	 *
	 * This property will be lazily initialized on the first access after the mesh
	 * property has been set, by an automatic invocation of the populateNeighbours method.
	 * When created in this manner, the memory allocated to hold the data in the
	 * returned array will be managed by this instance.
	 *
	 * Alternately, this property may be set directly to an array that was created
	 * externally. In this case, the underlying data memory is not managed by this
	 * instance, and it is up to the application to manage the allocation and
	 * deallocation of the underlying data memory, and to ensure that the array is
	 * large enough to contain the number of CC3FaceNeighbours structures specified
	 * by the faceCount property.
	 */
	CC3FaceNeighbours*			getNeighbours();
	void						setNeighbours( CC3FaceNeighbours* neighbours );

	/**
	 * Returns the neighbours of the face at the specified index,
	 * lazily initializing the neighbours property if needed.
	 */
	CC3FaceNeighbours			getNeighboursAt( GLuint faceIndex );

	/**
	 * Populates the contents of the neighbours property from the associated mesh,
	 * automatically allocating memory for the property if needed.
	 *
	 * This method is invoked automatically on the first access of the neighbours
	 * property after the mesh property has been set. Usually, the application
	 * never needs to invoke this method directly.
	 *
	 * However, if the neighbours property has been set to an array created outside
	 * this instance, this method may be invoked to populate that array from the mesh.
	 */
	void						populateNeighbours();

	/**
	 * Allocates underlying memory for the neighbours property, and returns a pointer
	 * to the allocated memory.
	 *
	 * This method will allocate enough memory for the normals property to hold
	 * the number of CC3FaceNeighbours structures specified by the faceCount property.
	 *
	 * This method is invoked automatically by the populateNeighbours method.
	 * Usually, the application never needs to invoke this method directly.
	 *
	 * It is safe to invoke this method more than once, but understand that any 
	 * previously allocated memory will be safely released prior to the allocation
	 * of the new memory. The memory allocated earlier will therefore be lost and
	 * should not be referenced.
	 * 
	 * The memory allocated will automatically be released when this instance
	 * is deallocated.
	 */
	CC3FaceNeighbours*			allocateNeighbours();

	/**
	 * Deallocates the underlying memory that was previously allocated with the
	 * allocateNeighbours method. It is safe to invoke this method more than once,
	 * or even if the allocateNeighbours method was not previously invoked.
	 *
	 * This method is invoked automatically when allocateNeighbours is invoked,
	 * and when this instance is deallocated. Usually, the application never
	 * needs to invoke this method directly.
	 */
	void						deallocateNeighbours();

	/** Marks the neighbours data as dirty. It will be automatically repopulated on the next access. */
	void						markNeighboursDirty();

	CC3Face						getFaceAt( GLuint faceIndex );

	void						initWithTag( GLuint aTag, const std::string& aName );
	void						initWithTag( GLuint aTag );

	// Template method that populates this instance from the specified other instance.
	// This method is invoked automatically during object copying via the copyWithZone: method.
	void						populateFrom( CC3FaceArray* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	CC3FaceIndices				getUncachedIndicesAt( GLuint faceIndex );

protected:
	CC3Mesh*					m_pMesh;
	CC3FaceIndices*				m_indices;
	CC3Vector*					m_centers;
	CC3Vector*					m_normals;
	CC3Plane*					m_planes;
	CC3FaceNeighbours*			m_neighbours;
	bool						m_shouldCacheFaces;
	bool						m_indicesAreRetained;
	bool						m_centersAreRetained;
	bool						m_normalsAreRetained;
	bool						m_planesAreRetained;
	bool						m_neighboursAreRetained;
	bool						m_indicesAreDirty;
	bool						m_centersAreDirty;
	bool						m_normalsAreDirty;
	bool						m_planesAreDirty;
	bool						m_neighboursAreDirty;
};


NS_COCOS3D_END

#endif
