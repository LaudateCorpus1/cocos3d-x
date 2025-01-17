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
#ifndef _CCL_CC3MATERIAL_H_
#define _CCL_CC3MATERIAL_H_

NS_COCOS3D_BEGIN

/** Default material color under ambient lighting. */
static const ccColor4F kCC3DefaultMaterialColorAmbient = { 0.2f, 0.2f, 0.2f, 1.0f };

/** Default material color under diffuse lighting. */
static const ccColor4F kCC3DefaultMaterialColorDiffuse = { 0.8f, 0.8f, 0.8f, 1.0f };

/** Default material color under specular lighting. */
static const ccColor4F kCC3DefaultMaterialColorSpecular = { 0.0f, 0.0f, 0.0f, 1.0f };

/** Default emissive material color. */
static const ccColor4F kCC3DefaultMaterialColorEmission = { 0.0f, 0.0f, 0.0f, 1.0f };

/** Default material shininess. */
static const GLfloat kCC3DefaultMaterialShininess = 0.0f;

/** Maximum material shininess allowed by OpenGL ES. */
static const GLfloat kCC3MaximumMaterialShininess = 128.0f;

/** Default material reflectivity. */
static const GLfloat kCC3DefaultMaterialReflectivity = 0.0f;

/** 
 * CC3Material manages information about a material that is used to cover one or more meshes.
 * This includes:
 *   - color
 *   - texture
 *   - interaction with lighting
 *   - opacity, translucency, and blending with background objects
 *
 * CC3Material supports two levels of control for blending and translucency:
 *   - To achieve the highest level of detail, accuracy and realism, you can individually
 *     set the explicit ambientColor, diffuseColor, specularColor, emissiveColor, shininess,
 *     sourceBlend, and destinationBlend properties. This suite of properties gives you the
 *     most complete control over the appearance of the material and its interaction with
 *     lighting conditions and the colors of the objects behind it, allowing you to generate
 *     rich visual effects. In addition, the isOpaque property sets the most commonly used
 *     blending combinations, and can be used to simplify your management of blending opaque
 *     or transparent materials, while still providing fine control of the ambient, diffuse
 *     and specular coloring.
 *   - At a simpler level, CC3Material also supports the Cocos2D <CCRGBAProtocol> protocol.
 *     You can use the color and opacity properties of this protocol to set the most commonly
 *     used coloring and blending characteristics simply and easily. Setting the color property
 *     changes both the ambient and diffuse colors of the material in tandem. Setting the
 *     opacity property also automatically sets the source and destination blend functions to
 *     appropriate values for the opacity level. By using the color and opacity properties,
 *     you will not be able to achieve the complexity and realism that you can by using the
 *     more detailed properties, but you can achieve good effect with much less effort.
 *     And by supporting the <CCRGBAProtocol> protocol, the coloring and translucency of nodes
 *     with materials can be changed using standard Cocos2D CCTint and CCFade actions, making
 *     it easier for you to add dynamic coloring effects to your nodes.
 * 
 * CC3Material also supports alpha testing, where the alpha value of each pixel can be
 * tested to determine whether or not it should be drawn. By default, alpha testing is
 * disabled, but alpha testing can sometimes be useful when drawing overlapping objects 
 * each contain transparency and it is not possible to rely only on drawing order and
 * depth testing to mediate whether a pixel should be drawn.
 *
 * Textures are optional. In some cases, if simple solid coloring is to be used, the material
 * may hold no texture at all. This solid coloring will still interact with lighting, creating
 * a realistic surface.
 *
 * More commonly, a material will hold a single instance of CC3Texture in the texture
 * property to provide a simple single-texture surface. This is the most common application
 * of textures to a material.
 *
 * For more sophisticated surfaces, materials also support multi-texturing, where more than
 * one instance of CC3Texture is added to the material using the addTexture: method. Using
 * multi-texturing, these textures can be combined in flexible, customized fashion,
 * permitting sophisticated surface effects.
 *
 * With OpenGL, multi-texturing is processed by a chain of texture units. The material's
 * first texture is processed by the first texture unit (texture unit zero), and subsequent
 * textures held in the material are processed by subsequent texture units, in the order in
 * which the textures were added to the material.
 * 
 * Each texture unit combines its texture with the output of the previous texture unit
 * in the chain. Combining textures is quite flexible under OpenGL, and there are many
 * ways that each texture can be combined with the output of the previous texture unit.
 *
 * Under fixed-pipeline rendering, such as with OpenGL ES 1.1, the way that a particular
 * texture combines with the previous textures is defined by an instance of CC3TextureUnit,
 * held in the textureUnit property of each texture that was added to the material.
 * To support a texture unit, the texture must be of type CC3TextureUnitTexture.
 *
 * For example, to configure a material for bump-mapping, add a CC3TextureUnitTexture
 * instance that contains a normal vector at each pixel instead of a color, and set the
 * textureUnit property of the texture to a CC3BumpMapTextureUnit. You can then combine 
 * the output of this bump-mapping with an additional texture that contains the image
 * that will be visible, to provide a detailed 3D bump-mapped surface. To do so, add that
 * second texture to the material, with a texture unit that defines how that addtional
 * texture is to be combined with the output of the bump-mapped texture.
 *
 * The maximum number of texture units is platform dependent, and can be read from the
 * CC3OpenGL.sharedGL.maxNumberOfTextureUnits property. This effectively defines how many
 * textures you can add to a material.
 *
 * You'll notice that there are two ways to assign textures to a material: through the
 * texture propety, and through the addTexture: method. The texture property exists for
 * the common case where only one texture is attached to a material. The addTexture:
 * method is used when more than one texture is to be added to the material. However,
 * for the first texture, the two mechanisms are synonomous. The texture property
 * corresponds to the first texture added using the addTexture: method, and for that
 * first texture, you can use either the texture property or the addTexture: method.
 * When multi-texturing, for consistency and simplicity, you would likely just use the
 * addTexture: method for all textures added to the material, including the first texture.
 * 
 * Each CC3MeshNode instance references an instance of CC3Material. Many CC3MeshNode
 * instances may reference the same instance of CC3Material, allowing many objects to
 * be covered by the same material.
 *
 * Once this material has been assigned to a mesh node, changing a texture to a new
 * texture should be performed through the mesh node itself, and not through the
 * material. This is to keep the mesh aligned with the orientation and usable area of
 * the textures since, under iOS, textures are padded to dimensions of a power-of-two
 * (POT), and most texture formats are loaded updside-down.
 *
 * When being drawn, the CC3MeshNode invokes the draw method on the CC3Material
 * instance prior to drawing the associated mesh.
 *
 * When drawing the material to the GL engine, this class remembers which material was
 * last drawn, and only binds the material data to the GL engine when a different material
 * is drawn. This allows the application to organize the CC3MeshNodes within the CC3Scene
 * so that nodes using the same material are drawn together, before moving on to other
 * materials. This strategy can minimize the number of mesh switches in the GL engine,
 * which improves performance. 
 */
class CC3Material : public CC3Identifiable
{
	DECLARE_SUPER( CC3Identifiable );
public:
	CC3Material();
	virtual ~CC3Material();

public:
	
	/**
	 * If this value is set to YES, current lighting conditions (from either lights or light probes)
	 * will be taken into consideration when drawing colors and textures.
	 *
	 * If this value is set to NO, lighting conditions will be ignored when drawing colors and
	 * textures, and the emissionColor will be applied to the mesh surface, without regard to
	 * lighting. Blending will still occur, but the other material aspects, including ambientColor,
	 * diffuseColor, specularColor, and shininess will be ignored. This is useful for a cartoon
	 * effect, where you want a pure color, or the natural colors of the texture, to be included
	 * in blending calculations, without having to arrange lighting, or if you want those colors
	 * to be displayed in their natural values despite current lighting conditions.
	 *
	 * Be aware that the initial value of the emissionColor property is normally black. If you
	 * find your node disappears or turns black when you set this property to NO, try changing
	 * the value of the emissionColor property.
	 *
	 * The initial value of this property is YES.
	 */
	bool						shouldUseLighting();
	void						setShouldUseLighting( bool shouldUse );

	/**
	 * The color of this material under ambient lighting.
	 * Initially set to kCC3DefaultMaterialColorAmbient.
	 *
	 * The value of this property is also affected by changes to the color and opacity
	 * properties. See the notes for those properties for more information.
	 */
	ccColor4F					getAmbientColor();
	void						setAmbientColor( const ccColor4F& color );

	/**
	 * The color of this material under ambient lighting.
	 * Initially set to kCC3DefaultMaterialColorDiffuse.
	 *
	 * The value of this property is also affected by changes to the color and opacity
	 * properties. See the notes for those properties for more information.
	 */
	ccColor4F					getDiffuseColor();
	void						setDiffuseColor( const ccColor4F& color );

	/**
	 * The color of this material under ambient lighting.
	 * Initially set to kCC3DefaultMaterialColorSpecular.
	 *
	 * The value of this property is also affected by changes to the opacity property.
	 * See the notes for the opacity property for more information.
	 */
	ccColor4F					getSpecularColor();
	void						setSpecularColor( const ccColor4F& color );

	/**
	 * The emission color of this material.
	 * Initially set to kCC3DefaultMaterialColorEmission.
	 *
	 * The value of this property is also affected by changes to the opacity property.
	 * See the notes for the opacity property for more information.
	 */
	ccColor4F					getEmissionColor();
	void						setEmissionColor( const ccColor4F& color );

	/**
	 * The ambient colour currently used by this material during rendering.
	 *
	 * The value of this property is influenced by the value of the shouldApplyOpacityToColor property.
	 * If the shouldApplyOpacityToColor property is set to YES, this property scales each of the red,
	 * green & blue componenents of the ambientColor property by its alpha component and returns the
	 * result. Otherwise, this property returns the unaltered value of the ambientColor property.
	 */
	ccColor4F					getEffectiveAmbientColor();

	/**
	 * The diffuse colour currently used by this material during rendering.
	 *
	 * The value of this property is influenced by the value of the shouldApplyOpacityToColor property.
	 * If the shouldApplyOpacityToColor property is set to YES, this property scales each of the red,
	 * green & blue componenents of the diffuseColor property by its alpha component and returns the
	 * result. Otherwise, this property returns the unaltered value of the diffuseColor property.
	 */
	ccColor4F					getEffectiveDiffuseColor();

	/**
	 * The specular colour currently used by this material during rendering.
	 *
	 * The value of this property is influenced by the value of the shouldApplyOpacityToColor property.
	 * If the shouldApplyOpacityToColor property is set to YES, this property scales each of the red,
	 * green & blue componenents of the specularColor property by its alpha component and returns the
	 * result. Otherwise, this property returns the unaltered value of the specularColor property.
	 */
	ccColor4F					getEffectiveSpecularColor();

	/**
	 * The emission colour currently used by this material during rendering.
	 *
	 * The value of this property is influenced by the value of the shouldApplyOpacityToColor property.
	 * If the shouldApplyOpacityToColor property is set to YES, this property scales each of the red,
	 * green & blue componenents of the emissionColor property by its alpha component and returns the
	 * result. Otherwise, this property returns the unaltered value of the emissionColor property.
	 */
	ccColor4F					getEffectiveEmissionColor();

	/**
	 * The shininess of this material.
	 * 
	 * The value of this property is clamped to between zero and kCC3MaximumMaterialShininess.
	 * The initial value of this property is kCC3DefaultMaterialShininess (zero).
	 */
	GLfloat						getShininess();
	void						setShininess( GLfloat shininess );

	/**
	 * The reflectivity of this material.
	 *
	 * This property can be used when the material is covered by an environmental reflection cube-map
	 * texture to indicate the weighting that should be applied to the reflection texture, relative to
	 * any other textures on the material. A value of zero indicates that the surface should be 
	 * completely unreflective, and a value of one indicates that the surface is entirely reflective.
	 *
	 * This property requires a programmable pipeline and has no effect when running OpenGL ES 1.1
	 * on iOS, or a fixed rendering pipeline when running OpenGL on OSX.
	 *
	 * The value of this property is clamped to between zero and one.
	 * The initial value of this property is kCC3DefaultMaterialReflectivity (zero).
	 */
	GLfloat						getReflectivity();
	void						setReflectivity( GLfloat reflectivity );

	/**
	 * The blending function to be applied to the source material (this material). This property must
	 * be set to one of the valid GL blending functions.
	 *
	 * The value in this property combines with the value in the destinationBlend property to determine
	 * the way that materials are combined when one (the source) is drawn over another (the destination).
	 * Features such as transparency can cause the two to blend together in various ways.
	 *
	 * Although you can set this property directly, you can also allow this material to manage the
	 * value of this property automatically, based on the values of the isOpaque and opacity properties.
	 * See the notes for those properties for more information.
	 *
	 * If this property is set to GL_ONE, and the hasPremultipliedAlpha property returns YES, the red,
	 * green and blue components of all material color properties will be blended with their alpha
	 * components prior to being applied to the GL engine. This enables correct fading of materials
	 * containing a texture with pre-multiplied alpha.
	 *
	 * If you want the source to completely cover the destination, set sourceBlend to GL_ONE.
	 *
	 * If you want to have the destination show through the source, either by setting the diffuse
	 * alpha below one, or by covering this material with a texture that contains transparency, set
	 * the sourceBlend to GL_ONE_MINUS_SRC_ALPHA.
	 *
	 * However, watch out for textures with a pre-multiplied alpha channel. If this material has a texture
	 * with a pre-multiplied alpha channel, set sourceBlend to GL_ONE, so that the pre-multiplied alpha of
	 * the source will blend with the destination correctly.
	 *
	 * Opaque materials can be managed more efficiently than translucent materials. If a material really
	 * does not allow other materials to be seen behind it, you should ensure that the sourceBlend and
	 * destinationBlend properties are set to GL_ONE and GL_ZERO, respectively, to optimize rendering
	 * performance. The performance improvement is small, but can add up if a large number of opaque
	 * objects are rendered as if they were translucent.
	 *
	 * Materials support different using blending functions for the RGB components and the alpha component.
	 * To set separate source blend values for RGB and alpha, use the sourceBlendRGB and sourceBlendAlpha
	 * properties instead. Setting this property sets the sourceBlendRGB and sourceBlendAlpha properties
	 * to the same value. Querying this property returns the value of the sourceBlendRGB property.
	 *
	 * The initial value is determined by the value of the class-side property
	 * defaultBlendFunc, which can be modified by the setDefaultBlendFunc: method.
	 */
	GLenum						getSourceBlend();
	void						setSourceBlend( GLenum sourceBlend );

	/**
	 * The blending function to be applied to the destination material. This property must be set to
	 * one of the valid GL blending functions.
	 *
	 * The value in this property combines with the value in the sourceBlend property to determine the
	 * way that materials are combined when one (the source) is drawn over another (the destination).
	 * Features such as transparency can cause the two to blend together in various ways.
	 *
	 * Although you can set this property directly, you can also allow this material to manage the
	 * value of this property automatically, based on the values of the isOpaque and opacity properties.
	 * See the notes for those properties for more information.
	 *
	 * If you want the source to completely cover the destination, set destinationBlend to GL_ZERO.
	 *
	 * If you want to have the destination show through the source, either by setting the diffuse
	 * alpha below one, or by covering this material with a texture that contains an alpha channel
	 * (including a pre-multiplied alpha channel), set the destinationBlend to GL_ONE_MINUS_SRC_ALPHA.
	 *
	 * Opaque materials can be managed more efficiently than translucent materials. If a material really
	 * does not allow other materials to be seen behind it, you should ensure that the sourceBlend and
	 * destinationBlend properties are set to GL_ONE and GL_ZERO, respectively, to optimize rendering
	 * performance. The performance improvement is small, but can add up if a large number of opaque
	 * objects are rendered as if they were translucent.
	 *
	 * Materials support different using blending functions for the RGB components and the alpha component.
	 * To set separate destination blend values for RGB and alpha, use the destinationBlendRGB and 
	 * destinationBlendAlpha properties instead. Setting this property sets the destinationBlendRGB and
	 * destinationBlendAlpha properties to the same value. Querying this property returns the value of
	 * the destinationBlendRGB property.
	 *
	 * The initial value is determined by the value of the class-side property
	 * defaultBlendFunc, which can be modified by the setDefaultBlendFunc: method.
	 */
	GLenum						getDestinationBlend();
	void						setDestinationBlend( GLenum destinationBlend );

	/**
	 * The blending function to be applied to the RGB components of the source material (this material).
	 * This property must be set to one of the valid GL blending functions.
	 *
	 * In conjunction with the sourceBlendAlpha property, this property allows the RGB blending and
	 * alpha blending to be specified separately. Alternately, you can use the sourceBlend property
	 * to set both RGB and alpha blending functions to the same value.
	 *
	 * See the notes of the sourceBlend property for more information about material blending.
	 */
	GLenum						getSourceBlendRGB();
	void						setSourceBlendRGB( GLenum sourceBlendRGB );

	/**
	 * The blending function to be applied to the RGB components of the destination material.
	 * This property must be set to one of the valid GL blending functions.
	 *
	 * In conjunction with the destinationBlendAlpha property, this property allows the RGB blending 
	 * and alpha blending to be specified separately. Alternately, you can use the destinationBlend
	 * property to set both RGB and alpha blending functions to the same value.
	 *
	 * See the notes of the destinationBlend property for more information about material blending.
	 */
	GLenum						getDestinationBlendRGB();
	void						setDestinationBlendRGB( GLenum destinationBlend );

	/**
	 * The blending function to be applied to the alpha components of the source material (this material).
	 * This property must be set to one of the valid GL blending functions.
	 *
	 * In conjunction with the sourceBlendRGB property, this property allows the RGB blending and
	 * alpha blending to be specified separately. Alternately, you can use the sourceBlend property
	 * to set both RGB and alpha blending functions to the same value.
	 *
	 * See the notes of the sourceBlend property for more information about material blending.
	 */
	GLenum						getSourceBlendAlpha();
	void						setSourceBlendAlpha( GLenum sourceBlendAlpha );

	/**
	 * The blending function to be applied to the alpha components of the destination material.
	 * This property must be set to one of the valid GL blending functions.
	 *
	 * In conjunction with the destinationBlendRGB property, this property allows the RGB blending
	 * and alpha blending to be specified separately. Alternately, you can use the destinationBlend
	 * property to set both RGB and alpha blending functions to the same value.
	 *
	 * See the notes of the destinationBlend property for more information about material blending.
	 */
	GLenum						getDestinationBlendAlpha();
	void						setDestinationBlendAlpha( GLenum destinationBlendAlpha );

	/**
	 * Indicates whether this material is opaque.
	 *
	 * This method returns YES if the values of the sourceBlend and destinationBlend
	 * properties are GL_ONE and GL_ZERO, respectively, otherwise this method returns NO.
	 *
	 * Setting this property to YES sets the value of the sourceBlend property to GL_ONE and the value
	 * of the destinationBlend to GL_ZERO. Setting this property to YES is a convenient way to force
	 * the source to completely cover the destination, even if the diffuse alpha value is less than one,
	 * and even if the texture contains translucency.
	 *
	 * If the sourceBlend and destinationBlend properties have not been set to something else, setting
	 * this property to NO sets the value of the destinationBlend property to GL_ONE_MINUS_SRC_ALPHA,
	 * and sets the sourceBlend property to GL_SRC_ALPHA if the alpha channel of the diffuseColor is
	 * below one and this material does not contain a texture that contains pre-multiplied alpha, in
	 * which case sourceBlend is left at GL_ONE.
	 *
	 * Setting the value of this property does not change the alpha values of any of the material colors. 
	 *
	 * The state of this property is also affected by setting the opacity property. As a convenience,
	 * setting the opacity property to a value less than kCCOpacityFull will automatically cause this
	 * isOpaque property to be set to NO, which, as described above, will also affect the sourceBlend and
	 * destinationBlend properties, so that the translucency will be blended correctly.
	 *
	 * Setting the opacity property to kCCOpacityFull will automatically cause this isOpaque property to be set
	 * to YES (affecting the sourceBlend and destinationBlend properties), unless the shouldBlendAtFullOpacity 
	 * property is set to YES, in which case the isOpaque property will be left set to NO.
	 *
	 * Setting this property can be thought of as a convenient way to switch between the two most
	 * common types of blending combinations. For finer control of blending, set the sourceBlend
	 * and destinationBlend properties and the alpha values of the individual material colors directly,
	 * and avoid making changes to this property, or the opacity property.
	 *
	 * Opaque materials can be managed slightly more efficiently than translucent materials.
	 * If a material really does not allow other materials to be seen behind it, you should
	 * ensure that this property is set to YES. The performance improvement is small, but
	 * can add up if a large number of opaque objects are rendered as if they were translucent.
	 */
	bool						isOpaque();
	void						setIsOpaque( bool opaque );

	/**
	 * Indicates whether blending should be applied even when the material is at full opacity.
	 *
	 * If this property is set to NO, when the opacity property of this material is set to full
	 * opacity (kCCOpacityFull), the isOpaque property will be set to YES, which in turn will set
	 * the sourceBlend property to GL_ONE and the destinationBlend property to GL_ZERO, effectively
	 * turning blending off. This allows a node that is opaque to be faded, but when fading is
	 * complete, and full opacity is restored, the node will be set fully opaque, which improves
	 * performance and the affects the rendering order of the node relative to other nodes.
	 *
	 * If this property is set to YES, when the opacity property of this material is set to full
	 * opacity (kCCOpacityFull), the isOpaque property will be set to NO, which will leave the
	 * sourceBlend and destination properties at their current values, generally leaving blending 
	 * enabled. This is useful when using fading on a material that contains textures that in turn
	 * contain transparency. When this material reaches full opacity, blending will be left enabled, 
	 * and the transparent sections of the textures will continue to be rendered correctly.
	 *
	 * The initial value of this property is NO. Whenever a texture is added to, or removed from,
	 * this material, the value is set to the value of the hasTextureAlpha property, reflecting
	 * whether any of the textures contain an alpha channel. If you know that the alpha channel
	 * in each texture does not contain transparency, and that this material will be displayed at
	 * full opacity, you can set this property back to NO to benefit from improved performance
	 * in sorting and rendering opaque materials.
	 */
	bool						shouldBlendAtFullOpacity();
	void						setShouldBlendAtFullOpacity( bool shouldBlendAtFullOpacity );

	/**
	 * Indicates the alpha test function that is used to determine if a pixel should be
	 * drawn, based on the value of its alpha component.
	 *
	 * The value of this property must be one of the following values:
	 *   - GL_ALWAYS: The pixel is always drawn, regardless of its alpha value.
	 *   - GL_GREATER - The pixel is drawn only if its alpha value is greater than the
	 *     value in the reference property.
	 *   - GL_GEQUAL - The pixel is drawn only if its alpha value is greater than or equal
	 *     to the value in the reference property.
	 *   - GL_LESS - The pixel is drawn only if its alpha value is less than the value
	 *     in the reference property.
	 *   - GL_LEQUAL - The pixel is drawn only if its alpha value is less than or equal
	 *     to the value in the reference property.
	 *   - GL_EQUAL - The pixel is drawn only if its alpha value is equal to the value
	 *     in the reference property.
	 *   - GL_NOTEQUAL - The pixel is drawn only if its alpha value is not equal to the
	 *     value in the reference property.
	 *   - GL_NEVER: The pixel is never drawn.
	 *
	 * The initial value of this property is GL_ALWAYS, indicating that each pixel will
	 * always be drawn, regardless of its alpha value.
	 * 
	 * For most situations, alpha testing is not necessary, and you can leave the value of this
	 * property at its initial value. Alpha testing can sometimes be useful when drawing overlapping
	 * objects that each contain transparency, and it is not possible to rely only on drawing order
	 * and depth testing to mediate whether a pixel should be drawn.
	 *
	 * Although you can set this property directly, since the most common values are either GL_ALWAYS
	 * or GL_GREATER, you can use the shouldDrawLowAlpha property as a shortcut to switch between
	 * these two values.
	 *
	 * Alpha testing within the GL engine is automatically disabled if this property
	 * is set to GL_ALWAYS, and enabled for any other value.
	 */
	GLenum						getAlphaTestFunction();
	void						setAlphaTestFunction( GLenum function );

	/**
	 * Indicates the reference value used by the alpha test function to compare against
	 * the alpha value of each pixel to determine if it should be drawn.
	 *
	 * The value of this property must be between zero and one, inclusive. The value
	 * is clamped by the GL engine if it is set to a value outside this range.
	 *
	 * The initial value of this property is zero.
	 *
	 * The value of this property has no effect if the value of the alphaTestFunction
	 * property is either GL_ALWAYS or GL_NEVER.
	 *
	 * See the notes for the alphaTestFunction property for more information on alpha testing.
	 */
	GLfloat						getAlphaTestReference();
	void						setAlphaTestReference( GLfloat alphaTestReference );

	/**
	 * Indicates whether alpha testing should be used to determine if pixels with
	 * lower alpha values should be drawn.
	 *
	 * This property is really a shortcut for setting the alphaTestFunction to either
	 * of its two most common values. Setting this property to YES will set the
	 * alphaTestFunction propery to GL_ALWAYS. Setting this property to NO will set
	 * the alphaTestFunction property to GL_GREATER.
	 *
	 * If the value of this property is set to YES, each pixel will be drawn regardless
	 * of the value of its alpha component. If the value of this property is set to NO,
	 * the value of the alpha component of each pixel will be compared against the value
	 * in the alphaTestReference property, and only those pixel alpha values that are
	 * greater than that reference value will be drawn. You can set the value of the
	 * alphaTestReference property to determine the cutoff level.
	 *
	 * Reading the value of this property will return YES if the value of the alphaTestFunction
	 * is any of GL_ALWAYS, GL_LESS or GL_LEQUAL, otherwise it returns NO.
	 *
	 * The initial value of this property is YES, indicating that pixels with lower
	 * alpha values will be drawn.
	 * 
	 * For most situations, alpha testing is not necessary, and you can leave the value
	 * of this property set to YES. Alpha testing can sometimes be useful when drawing
	 * overlapping objects that each contain transparency, and it is not possible to rely
	 * only on drawing order and depth testing to mediate whether a pixel should be drawn.
	 */
	bool						shouldDrawLowAlpha();
	void						setShouldDrawLowAlpha( bool shouldDrawLowAlpha );

	/**
	 * The color of this material, as a CCColorRef.
	 *
	 * The function of this property depends on the value of the shouldUseLighting property.
	 *
	 * If the shouldUseLighting property is set to YES, querying this property returns a value 
	 * derived from the diffuseColor property, and setting this property modifies the value of
	 * both the ambientColor and diffuseColor properties.
	 *
	 * If the shouldUseLighting property is set to NO, querying this property returns a value
	 * derived from the emissionColor property, and setting this property modifies the value of
	 * the emissionColor properties.
	 *
	 * When setting this property, the alpha of each of the affected properties is not modified.
	 */
	CCColorRef					getColor();
	void						setColor( const CCColorRef& color ); 

	/**
	 * The opacity of this material.
	 *
	 * Querying this property returns the alpha component of either the diffuseColor property or 
	 * the emissionColor property, depending on whether the shouldUseLighting property is set to
	 * YES or NO, respectively.
	 *
	 * When setting this property, the value is converted to a floating point number between 0 and 1,
	 * and is set into the alpha component of the ambientColor, diffuseColor, specularColor, and
	 * emissionColor properties. The RGB components of each of those properties remain unchanged.
	 *
	 * Setting this property also affects the isOpaque property. As a convenience, setting this opacity
	 * property to a value less than kCCOpacityFull will automatically cause the isOpaque property to be 
	 * set to NO, which will also affect the sourceBlend and destinationBlend properties, so that the 
	 * translucency will be blended correctly. See the notes of the isOpaque property for more information.
	 *
	 * However, setting this opacity property to kCCOpacityFull will NOT automatically cause the isOpaque 
	 * property to be set to YES. Even if the opacity of the material is full, the texture may contain 
	 * translucency, which would be ignored if the isOpaque property were to be set to YES.
	 * 
	 * Conversely, setting the value of this opacity property to kCCOpacityFull will automatically cause the
	 * isOpaque property to be set to YES, which will affect the sourceBlen and destinationBlend properties
	 * so that blending will be turned off. See the notes of the isOpaque property for more information.
	 *
	 * Setting this property can be thought of as a convenient way to make simple changes to the opacity
	 * of a material, using the most common types of blending combinations. For finer control of blending,
	 * set the sourceBlend and destinationBlend properties, and the alpha values of the individual colors
	 * directly, and avoid making changes to this property.
	 */
	CCOpacity					getOpacity();
	void						setOpacity( CCOpacity opacity );

	/**
	 * Implementation of the CCBlendProtocol blendFunc property.
	 *
	 * This is a convenience property that gets and sets the sourceBlend and destinationBlend 
	 * properties using a single structure.
	 *
	 * Materials support different using blending functions for the RGB components and the alpha component.
	 * To set separate blend values for RGB and alpha, use the blendFuncRGB and blendFuncAlpha properties
	 * instead. Setting this property sets the blendFuncRGB and blendFuncAlpha properties to the same value.
	 * Querying this property returns the value of the blendFuncRGB property.
	 */
	ccBlendFunc					getBlendFunc();
	void						setBlendFunc( const ccBlendFunc& blendFunc );

	/**
	 * The blending function to be applied to the RGB components.
	 *
	 * This is a convenience property that gets and sets both the sourceBlendRGB and
	 * destinationBlendRGB properties using a single structure.
	 *
	 * In conjunction with the blendFuncAlpha property, this property allows the RGB blending and
	 * alpha blending to be specified separately. Alternately, you can use the blendFunc property
	 * to set both RGB and alpha blending functions to the same value.
	 *
	 * See the notes of the sourceBlend and destinationBlend properties for more information
	 * about material blending.
	 */
	ccBlendFunc					getBlendFuncRGB();
	void						setBlendFuncRGB( const ccBlendFunc& blendFunc );

	/**
	 * The blending function to be applied to the alpha component.
	 *
	 * This is a convenience property that gets and sets both the sourceBlendAlpha and
	 * destinationBlendAlpha properties using a single structure.
	 *
	 * In conjunction with the blendFuncRGB property, this property allows the RGB blending and
	 * alpha blending to be specified separately. Alternately, you can use the blendFunc property
	 * to set both RGB and alpha blending functions to the same value.
	 *
	 * See the notes of the sourceBlend and destinationBlend properties for more information 
	 * about material blending.
	 */
	ccBlendFunc					getBlendFuncAlpha();
	void						setBlendFuncAlpha( const ccBlendFunc& blendFunc );

	/**
	 * Returns the default GL material source and destination blend function used for new instances.
	 * This affects both the RGB and alpha blending components.
	 *
	 * The initial value is {GL_ONE, GL_ZERO}.
	 */
	static ccBlendFunc			getDefaultBlendFunc();

	/** 
	 * Sets the default GL material source and destination blend function used for new instances. 
	 * This affects both the RGB and alpha blending components.
	 */
	static void					setDefaultBlendFunc( const ccBlendFunc& aBlendFunc );

	/**
	 * Returns the number of textures attached to this material, regardless of whether
	 * the textures were attached using the texture property or the addTexture: method.
	 */
	GLuint						getTextureCount();

	/**
	 * When using a single texture for this material, this property holds that texture.
	 *
	 * This property may be left nil if no texture is needed.
	 *
	 * When using multiple textures for this material, this property holds the first
	 * texture. You can add additional textures using the addTexture: method.
	 *
	 * As a convenience, this property can also be set using the addTexture: method,
	 * which will set this property if it has not been set already. This is useful when
	 * using multi-texturing, because it allows all textures attached to this material
	 * to be handled the same way.
	 *
	 * The texture held by this property will be processed by the first GL texture unit
	 * (texture unit zero).
	 *
	 * Once this material has been added to a mesh node, changes to this property should
	 * be made through the same property on the mesh node itself, and not made to this
	 * property directly, in order to keep the mesh aligned with the orientation and
	 * usable area of the textures. See the notes for the same property on CC3MeshNode
	 * for more information.
	 *
	 * When building for iOS, raw PNG and TGA images are pre-processed by Xcode to pre-multiply
	 * alpha, and to reorder the pixel component byte order, to optimize the image for the iOS 
	 * platform. If you want to avoid this pre-processing for PNG or TGA files, for textures 
	 * such as normal maps or lighting maps, that you don't want to be modified, you can prepend
	 * a 'p' to the file extension ("ppng" or "ptga") to cause Xcode to skip this pre-processing
	 * and to use a loader that does not pre-multiply the alpha. You can also use this for other
	 * file types as well. See the notes for the CC3STBImage useForFileExtensions class-side
	 * property for more info.
	 */
	CC3Texture*					getTexture();
	void						setTexture( CC3Texture* texture );

	/**
	 * In most situations, the material will use a single CC3Texture in the texture property.
	 * However, if multi-texturing is used, additional CC3Texture instances can be provided
	 * by adding them using this method.
	 *
	 * When multiple textures are attached to a material, when drawing, the material will
	 * combine these textures together using configurations contained in the textureUnit
	 * property of each texture.
	 *
	 * As a consistency convenience, if the texture property has not yet been set directly,
	 * the first texture added using this method will appear in that property.
	 *
	 * Textures are processed by GL texture units in the order they are added to the material.
	 * The first texture added (or set directly into the texture property) will be processed
	 * by GL texture unit zero. Subsequent textures added with this method will be processed
	 * by subsequent texture units, in the order they were added.
	 *
	 * The maximum number of texture units available is platform dependent, but will be
	 * at least two. The maximum number of texture units available can be read from the
	 * CC3OpenGL.sharedGL.maxNumberOfTextureUnits property. If you attempt to add more than
	 * this number of textures to the material, the additional textures will be ignored,
	 * and an informational message to that fact will be logged.
	 *
	 * Once this material has been added to a mesh node, new textures should be added
	 * through the same method on the mesh node itself, instead of this method, in order
	 * to keep the mesh aligned with the orientation and usable area of the textures.
	 * See the notes for the same method on CC3MeshNode for more information.
	 *
	 * When building for iOS, raw PNG and TGA images are pre-processed by Xcode to pre-multiply
	 * alpha, and to reorder the pixel component byte order, to optimize the image for the iOS
	 * platform. If you want to avoid this pre-processing for PNG or TGA files, for textures
	 * such as normal maps or lighting maps, that you don't want to be modified, you can prepend
	 * a 'p' to the file extension ("ppng" or "ptga") to cause Xcode to skip this pre-processing
	 * and to use a loader that does not pre-multiply the alpha. You can also use this for other
	 * file types as well. See the notes for the CC3STBImage useForFileExtensions class-side
	 * property for more info.
	 */
	virtual void				addTexture( CC3Texture* aTexture );

	/**
	 * Removes the specified texture from this material.
	 *
	 * If the specified texture is that in the texture property, that property is set to nil.
	 */
	void						removeTexture( CC3Texture* aTexture );

	/** Removes all textures from this material. */
	void						removeAllTextures();

	/**
	 * Returns the texture with the specified name, that was added either via the texture
	 * property or via the addTexture: method. Returns nil if such a texture cannot be found.
	 */
	CC3Texture*					getTextureNamed( const char* aName );

	/**
	 * Returns the texture that will be processed by the texture unit with the specified index.
	 * Texture unit indices start at zero.
	 *
	 * The value returned will be nil if there are no textures, or if the specified index is
	 * greater than one less than the value of the textureCount property.
	 */
	CC3Texture*					getTextureForTextureUnit( GLuint texUnit );

	/**
	 * Sets the texture that will be processed by the texture unit with the specified index,
	 * which should be a number between zero, and the value of the textureCount property.
	 * 
	 * If the specified index is less than the number of texture units added already, the
	 * specified texture will replace the one assigned to that texture unit. Otherwise, this
	 * implementation will invoke the addTexture: method to add the texture to this material.
	 *
	 * If the specified texture unit index is zero, the value of the texture property will
	 * be changed to the specified texture.
	 *
	 * Once this material has been added to a mesh node, changing a texture should be
	 * performed through the same method on the mesh node itself, instead of this method,
	 * in order to keep the mesh aligned with the orientation and usable area of the
	 * textures. See the notes for the same method on CC3MeshNode for more information.
	 */
	void						setTexture( CC3Texture* aTexture, GLuint texUnit );

	/**
	 * Returns whether any of the textures used by this material have an alpha channel, representing opacity.
	 *
	 * Returns YES if any of the textures contained in this instance has an alpha channel.
	 *
	 * See also the notes of the shouldBlendAtFullOpacity property for the effects of using a
	 * texture with an alpha channel.
	 */
	bool						hasTextureAlpha();

	/**
	 * Returns whether the alpha channel has already been multiplied into each of the RGB
	 * color channels, in any of the textures used by this material.
	 *
	 * Returns YES if any of the textures contained in this instance has pre-mulitiplied alpha.
	 *
	 * See also the notes of the shouldApplyOpacityToColor property for the effects of using textures
	 * with pre-multiplied alpha.
	 */
	bool						hasTexturePremultipliedAlpha();

	/**
	 * Returns whether the opacity of each of the material colors (ambient, diffuse, specular and emission)
	 * should be blended (multiplied) by its alpha value prior to being submitted to the GL engine.
	 *
	 * This property returns YES if the sourceBlendRGB property is set to GL_ONE and the hasPremultipliedAlpha
	 * property returns YES, otherwise this property returns NO. The combination of full source blending
	 * and pre-multiplied texture alpha can be made translucent by blending each color with its alpha value.
	 *
	 * If this property returns YES, each of the material colors will automatically be blended with its
	 * alpha component prior to being submitted to the GL engine.
	 */
	bool						shouldApplyOpacityToColor();

	/**
	 * Returns a cube-map texture if this material contains such a texture, or nil if it does not.
	 *
	 * This is a convenience property that returns the first cube-map texture that was added.
	 */
	CC3Texture*					getTextureCube();

	/** 
	 * Returns whether this material contains a texture that is a six-sided cube-map texture.
	 *
	 * Returns YES only if one of the textures that was added to this material (either through the
	 * texture property or the addTexture: method) returns YES from its isTextureCube property.
	 * Otherwise, this property returns NO.
	 */
	bool						hasTextureCube();

	/**
	 * Returns whether this material contains a texture that is configured as a bump-map.
	 *
	 * Returns YES only if one of the textures that was added to this material (either
	 * through the texture property or the addTexture: method) returns YES from its
	 * isBumpMap property. Otherwise, this property returns NO.
	 */
	bool						hasBumpMap();

	/**
	 * The direction, in local tangent coordinates, of the light source that is to
	 * interact with any texture contained in this material that has been configured
	 * as a bump-map.
	 *
	 * Bump-maps are textures that store a normal vector (XYZ coordinates) in
	 * the RGB components of each texture pixel, instead of color information.
	 * These per-pixel normals interact with the value of this lightDirection
	 * property (through a dot-product), to determine the luminance of the pixel.
	 *
	 * Setting this property sets the equivalent property in all textures contained
	 * within this material.
	 *
	 * Reading this value returns the value of the equivalent property in the first
	 * texture that is configrued as a bump-map. Otherwise kCC3VectorZero is returned.
	 *
	 * The value of this property must be in the tangent-space coordinates associated
	 * with the texture UV space, in practice, this property is typically not set
	 * directly. Instead, you can use the globalLightPosition property of the mesh
	 * node that is making use of this texture.
	 */
	CC3Vector					getLightDirection();
	void						setLightDirection( const CC3Vector& direction );

	/**
	 * Allocates and initializes an autoreleased unnamed instance with an automatically
	 * generated unique tag value. The tag value is generated using a call to nextTag.
	 */
	static CC3Material*			material();

	/** Allocates and initializes an unnamed autoreleased instance with the specified tag. */
	static CC3Material*			materialWithTag( GLuint aTag );

	/**
	 * Allocates and initializes an autoreleased instance with the specified name and an
	 * automatically generated unique tag value. The tag value is generated using a call to nextTag.
	 */
	static CC3Material*			materialWithName( const std::string& aName );

	/** Allocates and initializes an autoreleased instance with the specified tag and name. */
	static CC3Material*			materialWithTag( GLuint aTag, const std::string& aName );

	/**
	 * Allocates and initializes an autoreleased unnamed instance with an automatically
	 * generated unique tag value. The tag value is generated using a call to nextTag.
	 *
	 * The returned instance will have a specularColor of { 1.0, 1.0, 1.0, 1.0 } and a
	 * shininess of 75.0.
	 */
	static CC3Material*			shiny();

	/**
	 * Allocates and initializes an autoreleased unnamed instance with an automatically
	 * generated unique tag value. The tag value is generated using a call to nextTag.
	 *
	 * The returned instance will have both diffuseColor and specularColor set to
	 * { 1.0, 1.0, 1.0, 1.0 } and a shininess of 75.0.
	 */
	static CC3Material*			shinyWhite();

	/**
	 * Applies this material to the GL engine. The specified visitor encapsulates
	 * the frustum of the currently active camera, and certain drawing options.
	 *
	 * This implementation first determines if this material is different than the material
	 * that was last bound to the GL engine. If this material is indeed different, this method
	 * applies the material to the GL engine, otherwise it does nothing.
	 *
	 * Draws this texture to the GL engine as follows:
	 *  - Applies the blending properties to the GL engine
	 *  - Applies the various lighting and color properties to the GL engine
	 *  - Binds the texture property to the GL engine as texture unit zero.
	 *  - Binds any additional textures added using addTexture: to additional texture units.
	 *  - Disables any unused texture units.
	 *
	 * If the texture property is nil, and there are no overlays, all texture units
	 * in the GL engine will be disabled.
	 *
	 * This method is invoked automatically during node drawing. Usually, the application
	 * never needs to invoke this method directly.
	 */
	void						drawWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Unbinds all materials from the GL engine.
	 *
	 * Disables material blending in the GL engine.
	 *
	 * This method is invoked automatically from the CC3MeshNode instance.
	 * Usually, the application never needs to invoke this method directly.
	 */
	static void					unbindWithVisitor( CC3NodeDrawingVisitor* visitor );

	CCColorRef					getDisplayedColor();
	bool						isCascadeColorEnabled();
	void						setCascadeColorEnabled( bool cascadeColorEnabled );
	void						updateDisplayedColor( const CCColorRef& color );
	CCOpacity					getDisplayedOpacity();
	void						setCascadeOpacityEnabled( bool cascadeOpacityEnabled );
	void						updateDisplayedOpacity( CCOpacity opacity );

	void						drawTexturesWithVisitor( CC3NodeDrawingVisitor* visitor );
	bool						isCascadeOpacityEnabled();
	void						texturesHaveChanged();
	void						applyColorsWithVisitor( CC3NodeDrawingVisitor* visitor );
	void						applyAlphaTestWithVisitor( CC3NodeDrawingVisitor* visitor );
	void						applyBlendWithVisitor( CC3NodeDrawingVisitor* visitor );

	void						initWithTag( GLuint aTag, const std::string& aName );
	void						initWithTag( GLuint aTag );

	void						populateFrom( CC3Material* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	virtual std::string			getNameSuffix();

protected:
	CC3Texture*					m_pTexture;
	CCArray*					m_textureOverlays;
	ccColor4F					m_ambientColor;
	ccColor4F					m_diffuseColor;
	ccColor4F					m_specularColor;
	ccColor4F					m_emissionColor;
	float						m_fShininess;
	float						m_fReflectivity;
	GLenum						m_alphaTestFunction;
	float						m_fAlphaTestReference;
	ccBlendFunc					m_blendFuncRGB;
	ccBlendFunc					m_blendFuncAlpha;
	bool						m_shouldUseLighting : 1;
	bool						m_shouldBlendAtFullOpacity : 1;
};

NS_COCOS3D_END

#endif
