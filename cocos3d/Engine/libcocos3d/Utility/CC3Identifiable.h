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
#ifndef _CCL_CC3IDENTIFIABLE_H_
#define _CCL_CC3IDENTIFIABLE_H_

NS_COCOS3D_BEGIN

class CC3PODResource;
class CC3Identifiable : public CC3Cacheable
{
public:
	CC3Identifiable();
	virtual ~CC3Identifiable();
	
	/**
	 * An arbitrary identification. Useful for keeping track of instances. Unique tags are not explicitly
	 * required, but are highly recommended. In most cases, it is best to just let the tag be assigned
	 * automatically by using an initializer that does not explicitly set the tag.
	 */
	GLuint						getTag();
	void						setTag( GLuint tag );

	/**
	 * An arbitrary name for this object. It is not necessary to give all identifiable objects
	 * a name, but can be useful for retrieving objects at runtime, and for identifying objects
	 * during development. 
	 *
	 * In general, names need not be unique, are not automatically assigned, and leaving the name
	 * as nil is acceptable.
	 *
	 * Some subclasses are designed so that their instances can be cached. For instances of those
	 * subclasses, the name is required, and must be unique.
	 */
	virtual std::string			getName();
	virtual void				setName( const std::string& name );

	/**
	 * If this instance does not already have a name, it is derived from the name of the specified
	 * other CC3Identifiable, if it has one. If this instance has a name already, or if the other
	 * CC3Identifiable does not have a name, the name of this instance is not changed.
	 *
	 * Typically, this is invoked when one CC3Identifiable is added as a component to another.
	 *
	 * This implementation concatenates the value of the nameSuffix property of this instance onto
	 * the name of the specified CC3Identifiable, and sets that into the name property of this
	 * instance. If a subclass returns nil from the nameSuffix property, no name is generated.
	 *
	 * Returns whether the name of this instance was changed.
	 */
	bool						deriveNameFrom( CC3Identifiable* another );

	/**
	 * If this instance does not already have a name, it is derived from the name of the specified
	 * other CC3Identifiable, if it has one. If this instance has a name already, or if the other
	 * CC3Identifiable does not have a name, the name of this instance is not changed.
	 *
	 * Typically, this is invoked when one CC3Identifiable is added as a component to another.
	 *
	 * This implementation concatenates the value of the specified suffix onto the name of the
	 * specified CC3Identifiable, and sets that into the name property of this instance.
	 *
	 * Returns whether the name of this instance was changed.
	 */
	bool						deriveNameFrom( CC3Identifiable* another, const std::string& suffix );

	/**
	 * Returns a string to concatenate to the name of another CC3Identifiable to automatically
	 * create a useful name for this instance.
	 *
	 * This property is used by the deriveNameFrom: method.
	 * 
	 * This implementation simply raises an assertion exception. Each concrete subclass should
	 * override this property to return a useful identifiable name suffix. A subclass can return
	 * nil from this property to indicate that automatic naming should not be performed.
	 */
	virtual std::string			getNameSuffix();

	/**
	 * Application-specific data associated with this object.
	 *
	 * You can use this property to add any additional information you want to an instance of 
	 * CC3Identifiable or its concrete subclasses (CC3Node, CC3Mesh, CC3Material, CC3Texture, etc.).
	 * 
	 * If you have non-object data to attach, such as a structure, or a pointer to data in a memory
	 * space (perhaps loaded from a file), you can wrap it in an instance of NSData and attach it here.
	 *
	 * To assist in managing this data, the methods initUserData is invoked automatically during the
	 * initialization and deallocation of each instance of this class. You can override this method
	 * by subclassing, or by adding extention categories to the concrete subclasses of CC3Identifiable,
	 * (CC3Node, CC3Mesh, CC3Material, CC3Texture, etc.), to create or retrieve the data, and attach
	 * it to the new instance.
	 *
	 * When copying instances of CC3Identifiable and its subclasses, the copyUserDataFrom: method
	 * is invoked in the new copy so that it can copy the data from the original instance to the
	 * new instance copy.
	 *
	 * In this abstract class, the copyUserDataFrom: method simply sets the userData property of
	 * the copy to the object returned from the same property in the original instance, without
	 * creating a copy of the userData object. If you need the userData object to be copied as
	 * well, you can override the copyUserDataFrom: method by subclassing, or by adding extention
	 * categories to the concrete subclasses of CC3Identifiable, (CC3Node, CC3Mesh, CC3Material,
	 * CC3Texture, etc.), to copy the userData object and assign it to the new instance.
	 */
	void*						getUserData();
	void						setUserData( void* data );

	/**
	 * Invoked automatically from the init* family of methods to initialize the userData property
	 * of this instance.
	 *
	 * This implementation simply sets the userData property to nil. You can override this method
	 * in a subclass or by creating extension categories for the concrete subclasses, (CC3Node,
	 * CC3Mesh, CC3Material, CC3Texture, etc.), if the userData can be initialized and retained
	 * in a self-contained manner.
	 *
	 * Alternately, you can leave this method unimplemented, and set the userData property from
	 * outside this instance.
	 */
	void						initUserData();

	/**
	 * Invoked automatically after this instance has been created as a copy of the specified
	 * instance, to copy the userData property from the original instance to this instance.
	 *
	 * In this abstract class, this method simply sets the userData property of this instance
	 * to the object returned from the same property in the specified original instance, without
	 * creating a copy of the userData object. If you need the userData object to be copied as 
	 * well, you can override this method by subclassing, or by adding extention categories to
	 * the concrete subclasses of CC3Identifiable, (CC3Node, CC3Mesh, CC3Material, CC3Texture,
	 * etc.), to copy the userData object and assign it to the new instance.
	 */
	void						copyUserDataFrom( CC3Identifiable* another );

	/**
	 * Initializes this unnamed instance with an automatically generated unique tag value.
	 * The tag value will be generated automatically via the method nextTag.
	 */
	virtual bool				init();

	/** Initializes this unnamed instance with the specified tag. */
	virtual void				initWithTag( GLuint aTag );

	/**
	 * Initializes this instance with the specified name and an automatically generated unique
	 * tag value. The tag value will be generated automatically via the method nextTag.
	 */
	virtual void				initWithName( const std::string& name );

	/**
	 * Initializes this instance with the specified tag and name.
	 * When overriding initialization, subclasses typically need only override this initializer.
	 */
	virtual void				initWithTag( GLuint aTag, const std::string& aName );

	/**
	 * Returns a newly allocated (retained) copy of this instance. The new copy will have
	 * the same name as this instance, but will have a unique tag.
	 *
	 * The returned instance is retained. It is the responsiblity of the caller to manage
	 * the lifecycle of the returned instance and perform the corresponding invocation of
	 * the release method at the appropriate time.
	 *
	 * This copy operation is a deep copy. Copies of most of the content of the original
	 * will be created as well. For structural subclasses, such as CC3Node, copies will
	 * be made of each structual element (eg- child nodes). Some exceptions are made.
	 * For instance, copies are generally not made for fixed, voluminous content such as
	 * mesh data. In addition, subclasses may excuse themselves from being copied through
	 * the shouldIncludeInDeepCopy property.
	 *
	 * This copy operation is a deep copy. Copies of most of the content of the original
	 * will be created as well. For structural subclasses, such as CC3Node, copies will
	 * be made of each structual element (eg- child nodes). Some exceptions are made.
	 * For instance, copies are generally not made for fixed, voluminous content such as
	 * mesh data. In addition, subclasses may excuse themselves from being copied through
	 * the shouldIncludeInDeepCopy property.
	 *
	 * The copy... methods may often be used to duplicate an instance many times, to create
	 * large number of similar instances to populate a game. To help you verify that you are
	 * correctly releasing and deallocating all these copies, you can use the instanceCount
	 * class method to get a current count of the total number of instances of all subclasses
	 * of CC3Identifiable,
	 *
	 * Subclasses that extend copying should not override this method, but should override
	 * the populateFrom: method instead.
	 */
	 virtual CCObject*			copy();

	/**
	 * Returns a newly allocated (retained) copy of this instance. The new copy
	 * will have its name set to the specified name, and will have a unique tag.
	 *
	 * The returned instance is retained. It is the responsiblity of the caller to manage
	 * the lifecycle of the returned instance and perform the corresponding invocation of
	 * the release method at the appropriate time.
	 *
	 * This copy operation is a deep copy. Copies of most of the content of the original
	 * will be created as well. For structural subclasses, such as CC3Node, copies will
	 * be made of each structual element (eg- child nodes). Some exceptions are made.
	 * For instance, copies are generally not made for fixed, voluminous content such as
	 * mesh data. In addition, subclasses may excuse themselves from being copied through
	 * the shouldIncludeInDeepCopy property.
	 *
	 * The copy... methods may often be used to duplicate an instance many times, to create
	 * large number of similar instances to populate a game. To help you verify that you are
	 * correctly releasing and deallocating all these copies, you can use the instanceCount
	 * class method to get a current count of the total number of instances of all subclasses
	 * of CC3Identifiable,
	 *
	 * Subclasses that extend copying should not override this method, but should override
	 * the populateFrom: method instead.
	 */
	virtual CCObject*			copyWithName( const std::string& aName );

	/**
	 * Returns a newly allocated (retained) copy of this instance. The new copy
	 * will have its name set to the specified name, and will have a unique tag.
	 *
	 * The returned instance is retained. It is the responsiblity of the caller to manage
	 * the lifecycle of the returned instance and perform the corresponding invocation of
	 * the release method at the appropriate time.
	 *
	 * This copy operation is a deep copy. Copies of most of the content of the original
	 * will be created as well. For structural subclasses, such as CC3Node, copies will
	 * be made of each structual element (eg- child nodes). Some exceptions are made.
	 * For instance, copies are generally not made for fixed, voluminous content such as
	 * mesh data. In addition, subclasses may excuse themselves from being copied through
	 * the shouldIncludeInDeepCopy property.
	 *
	 * The copy... methods may often be used to duplicate an instance many times, to create
	 * large number of similar instances to populate a game. To help you verify that you are
	 * correctly releasing and deallocating all these copies, you can use the instanceCount
	 * class method to get a current count of the total number of instances of all subclasses
	 * of CC3Identifiable,
	 *
	 * Subclasses that extend copying should not override this method, but should override
	 * the populateFrom: method instead.
	 */
	virtual CCObject*			copyWithZone( CCZone* zone, const std::string& aName );
	virtual CCObject*			copyWithZone( CCZone* zone );

	/**
	 * Template method that populates this instance from the specified other instance.
	 *
	 * This method is invoked automatically during object copying via the copy or
	 * copyWithZone: method. In most situations, the application should use the
	 * copy method, and should never need to invoke this method directly.
	 * 
	 * Subclasses that add additional instance state (instance variables) should extend
	 * copying by overriding this method to copy that additional state. Superclass that
	 * override this method should be sure to invoke the superclass implementation to
	 * ensure that superclass state is copied as well.
	 */
	void						populateFrom( CC3Identifiable* another );

	/**
	 * Returns whether this instance should be included in a deep copy.
	 *
	 * This method simply returns YES by default, and in most cases this is sufficient.
	 * However, for some structural subclasses (notably subclasses of CC3Node) it may
	 * be desirable to not copy some components.
	 *
	 * This property is not universally automatically applied or honoured. It is
	 * up to the invoker and invokee to agree on when to make use of this property.
	 */
	virtual bool				shouldIncludeInDeepCopy();
	
	/**
	 * Returns a unique tag value to identify instances. This value is unique across all instances
	 * of all subclasses. The initial value returned will be one, and subsequent calls will increment
	 * the value retuned on each call. The starting value can be reset back to one via the
	 * resetTagAllocation method.
	 */
	virtual GLuint				nextTag();

	/** Resets the allocation of new tags to resume at one again. */
	virtual void				resetTagAllocation();

	/**
	 * Indicates the total number of active instances, over all subclasses, that have been allocated
	 * and initialized, but not deallocated. This can be useful when creating hordes of 3D objects,
	 * to verify that your application is properly deallocating them again when you are done with them.
	 */
	static GLint				getInstanceCount();	

	/**
	 * Returns a string containing a more complete description of this object.
	 *
	 * This implementation simply invokes the description method. Subclasses with more
	 * substantial content can override to provide much more information.
	 */
	virtual std::string			fullDescription();


protected:
	/**
	 * An arbitrary identification. Useful for keeping track of instances. Unique tags are not explicitly
	 * required, but are highly recommended. In most cases, it is best to just let the tag be assigned
	 * automatically by using an initializer that does not explicitly set the tag.
	*/
	unsigned int				m_nTag;
	/**
	 * An arbitrary name for this node. It is not necessary to give all identifiable objects a name,
	 * but can be useful for retrieving objects at runtime, and for identifying objects during development.
	 * Names need not be unique, are not automatically assigned, and leaving the name as nil is acceptable.
	*/
	std::string					m_sName;

	void*						m_userData;
};

NS_COCOS3D_END

#endif
