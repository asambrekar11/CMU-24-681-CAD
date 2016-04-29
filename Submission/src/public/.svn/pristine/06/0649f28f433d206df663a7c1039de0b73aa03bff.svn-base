/* ////////////////////////////////////////////////////////////

File Name: ystightarray.h
Copyright (c) 2015 Soji Yamakawa.  All rights reserved.
http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////// */

#ifndef YSTIGHTARRAY_IS_INCLUDED
#define YSTIGHTARRAY_IS_INCLUDED

#include "ysrange.h"

/*! A variable length (doubling) array class.

    Unlike YsTightArray class, this class does not hold pre-allocation buffer.

    The functionality of YsTightArray class is similar to the vector class of the standard-template library.

    NOTE: Currently SizeType is YSSIZE_T by default, but will be changed to size_t in the near future to fully
    adapt to 64-bit environment.  Also indexes will be changed from int to SizeType.  Not a practical
    concern until we really start using array indexes beyond 32bit boundary.

    \tparam T Type of the items stored in the array
    \tparam SizeType Type of the variable used for specifying an index
    \sa YsString
    \sa YsTightArray
    */
template <class T,class SizeType=YSSIZE_T>
class YsTightArray
{
   // CAUTION!
// Whenever add an item, do not forget to modify operator=
public:
// STL-like iterator support begin >>
public:
	class iterator
	{
	public:
		YsTightArray <T,SizeType> *array;
		YSSIZE_T index,dir;
		inline iterator &operator++()
		{
			index+=dir;
			return *this;
		}
		inline iterator operator++(int)
		{
			iterator copy=*this;
			index+=dir;
			return copy;
		}
		inline iterator &operator--()
		{
			index-=dir;
			return *this;
		}
		inline iterator operator--(int)
		{
			iterator copy=*this;
			index-=dir;
			return copy;
		}
		inline bool operator==(const iterator &from)
		{
			return (array==from.array && index==from.index);
		}
		inline bool operator!=(const iterator &from)
		{
			return (array!=from.array || index!=from.index);
		}
		inline T &operator*()
		{
			return (*array)[index];
		}
	};

	class const_iterator
	{
	public:
		const YsTightArray <T,SizeType> *array;
		YSSIZE_T index,dir;
		inline const_iterator &operator++()
		{
			index+=dir;
			return *this;
		}
		inline const_iterator operator++(int)
		{
			iterator copy=*this;
			index+=dir;
			return copy;
		}
		inline const_iterator &operator--()
		{
			index-=dir;
			return *this;
		}
		inline const_iterator operator--(int)
		{
			iterator copy=*this;
			index-=dir;
			return copy;
		}
		inline bool operator==(const const_iterator &from)
		{
			return (array==from.array && index==from.index);
		}
		inline bool operator!=(const const_iterator &from)
		{
			return (array!=from.array || index!=from.index);
		}
		inline const T &operator*()
		{
			return (*array)[index];
		}
	};

public:
	/*! Support for STL-like iterator */
	iterator begin()
	{
		iterator iter;
		iter.array=this;
		iter.index=0;
		iter.dir=1;
		return iter;
	}

	/*! Support for STL-like iterator */
	iterator end()
	{
		iterator iter;
		iter.array=this;
		iter.index=GetN();
		iter.dir=1;
		return iter;
	}

	/*! Support for STL-like iterator */
	const_iterator begin() const
	{
		const_iterator iter;
		iter.array=this;
		iter.index=0;
		iter.dir=1;
		return iter;
	}

	/*! Support for STL-like iterator */
	const_iterator end() const
	{
		const_iterator iter;
		iter.array=this;
		iter.index=GetN();
		iter.dir=1;
		return iter;
	}

	/*! Support for STL-like iterator */
	iterator rbegin()
	{
		iterator iter;
		iter.array=this;
		iter.index=GetN()-1;
		iter.dir=-1;
		return iter;
	}

	/*! Support for STL-like iterator */
	iterator rend()
	{
		iterator iter;
		iter.array=this;
		iter.index=-1;
		iter.dir=-1;
		return iter;
	}

	/*! Support for STL-like iterator */
	const_iterator rbegin() const
	{
		const_iterator iter;
		iter.array=this;
		iter.index=GetN()-1;
		iter.dir=-1;
		return iter;
	}

	/*! Support for STL-like iterator */
	const_iterator rend() const
	{
		const_iterator iter;
		iter.array=this;
		iter.index=-1;
		iter.dir=-1;
		return iter;
	}



	/*! Index enumerator enables range-based for using an index.  It can be written as:
	    YsArray <T> array;
	    for(auto idx : array.AllIndex())
	    {
	        // Do something for array[idx]
	    }
	    Useful when something needs to be done before and after an array item. */
	inline YsIndexRange AllIndex(void) const;
// STL-like iterator support end <<



public:
	/*! Default constructor.  Creates an empty array. */
	YsTightArray();

	/*! A constructor that makes an array from a raw C++ array. 
	    \param n [In] Number of array items
	    \param v [In] Raw C++ array to be copied to this array
	    */
	YsTightArray(SizeType n,const T v[]);

	/*! Default copy constructor that creates an YsTightArray object from another YsTightArray object.
	    */
	YsTightArray(const YsTightArray <T,SizeType> &from);

	/*! A copy constructor that creates an YsTightArray object from another YsTightArray object of different SizeType.
	    This copy constructor is supposed to cover the case SizeType==SizeType2.  However, it seems
	    that C++ deals with the case where the input parameter has the exactly the same type, and this
	    constructo may not be called SizeType==SizeType2.  (At least that is what happened in 
	    Visual Studio 2008.  A separate default constructor is written just to be absolutely sure that
	    an appropriate constructor is called.
	    */
	template <class SizeType2>
	YsTightArray(const YsTightArray <T,SizeType2> &from);

	/*! Default destructor.
	    */
	~YsTightArray();

	/*! Default copy operator that safely copies object from the incoming YsTightArray.
	    */
	const YsTightArray <T,SizeType> &operator=(const YsTightArray <T,SizeType> &from);

	/*! A copy operator that copies from a YsTightArray class with different SizeType.
	    \tparam SizeType2 Integral data type for storing size of the incoming array.
	    */
	template <class SizeType2>
	const YsTightArray <T,SizeType> &operator=(const YsTightArray <T,SizeType2> &from);

	/*! An operator that allows constant access to the array items. */
	inline const T &operator[](YSSIZE_T idx) const;
	// Memo: idx must have YSSIZE_T, or parameter deduction gets confused when SizeType is a smaller integer.

	/*! An operator that allows non-constant access to the array items. */
	inline T &operator[](YSSIZE_T idx);
	// Memo: idx must have YSSIZE_T, or parameter deduction gets confused when SizeType is a smaller integer.

	/*! A cast operator that returns constant pointer to the array. */
	inline operator const T *() const;

	/*! A cast operator that returns non-constant pointer to the array. */
	inline operator T *();

	/*! This function returns the length of the array. */
	inline SizeType GetN(void) const;

	/*! This function adds an item to the end of the array. */
	inline YSRESULT Append(const T &dat);

	/*! This function returns a reference to the first item of the array. */
	inline T &GetTop(void);

	/*! This function returns a constant reference to the first item of the array. */
	inline const T &GetTop(void) const;

	/*! This function returns a non-constant reference to the last item of the array. */
	inline T &GetEnd(void);

	/*! This function returns a constant reference to the last item of the array. */
	inline const T &GetEnd(void) const;

	/*! This function adds multiple items given by nItm and itm to the end of the array. 
	   \param nDat [In] Number of items to be added.
	   \param dat [In] Items to be added.
	   */
	YSRESULT Append(SizeType nDat,const T dat[]);

	/*! This function adds multiple items given by ary to the end of the array. 
	   */
	template <class SizeType2>
	YSRESULT Append(const YsTightArray <T,SizeType2> &ary);


	/*! This function copies the last item of the array to the idx-th item, and shrinks
	    the length of the array by one.  When the order of the items in the array does not
	    matter, this function quickly deletes idx-th item in the array.
	    \param idx [In] Index to the item to be deleted.
	   */
	YSRESULT DeleteBySwapping(SizeType idx);

	/*! This function deletes the last item of the array. */
	YSRESULT DeleteLast(void);

	/*! This function increases the length of the array by one.  The content of the last item
	    is undetermined immediately after this function. */
	YSRESULT Increment(void);

	/*! This function clears the array.  The length of the array becomes zero. 
	    The internally-allocated memory chunk stays the same.  If you want to explicitly delete
	    the memory chunk, use ClearDeep function instead.  */
	YSRESULT Clear(void);

	/*! This function clears the array.  The length of the array becomes zero.  Internal buffer
	    will also be deleted.*/
	YSRESULT ClearDeep(void);

	/*! This function makes an array with length of nv, and copies dat[0] to dat[nDat-1] to the array. 
	    \param nDat [In] New length of the array.
	    \param dat [In] Items to be copied to the array.
	    */
	YSRESULT Set(SizeType nDat,const T dat[]);

	/*! This function copies a value to idx-th item in the array.
	    \param idx [In] Location of the item that receives a new value
	    \param dat [In] Incoming value 
	    */
	inline YSRESULT SetItem(SizeType idx,const T &dat);

	/*! This function creates an array with length of one, and copies the incoming item to the array. */
	YSRESULT MakeUnitLength(const T &dat);

	/*! This function returns a non-constant reference to the idx-th item in the array. */
	inline T &GetEditableItem(SizeType idx);

	/*! This function returns a constant reference to the idx-th item in the array. */
	inline const T &GetItem(SizeType idx) const;

	/*! This function returns a constant pointer to the array.  
	    It is useful when casting operator occasionally becomes ambiguous. */
	inline const T *GetArray(void) const;

	/*! This function returns a non-constant pointer to the array. 
	    It is useful when casting operator occasionally becomes ambiguous. */
	inline T *GetEditableArray(void);

	/*! This function checks the length of internal buffer.  */
	inline SizeType GetNAvailable(void) const;

	/*! This function checks the length of the unused portion of the internal buffer.  */
	inline SizeType GetNUnused(void) const;

	/*! This function return YSTRUE if 0<=id && id<GetN() */
	inline YSBOOL IsInRange(SizeType id) const;  // 2008/07/06

	/*! This function return YSTRUE if the array includes dat.  If the compare operator
	    is not defined for class T, the compiler will give an error. */
	inline YSBOOL IsIncluded(const T &dat) const; // 2010/01/20

	/*! This function changes the length of the array.  If the new length is longer than
	    the current length, items beyond the current length will not be initialized. */
	YSRESULT Resize(SizeType newSize);

	/*! This function inserts dat to the array.  dat will become idx-th item after this
	    function. */
	YSRESULT Insert(SizeType idx,const T &dat);

	/*! This function deletes idx-th item.  Latter items will be shifted (the order doesn't change). */
	YSRESULT Delete(SizeType idx);

	/*! This function numItem items starting from idxFrom-th item.  Latter items will be shifted (the order doesn't change). */
	YSRESULT Delete(SizeType idxFrom,SizeType numItem);

// \cond
protected:
	SizeType nv,nAvailable;
	T *vv;
	inline YSRESULT Alloc(SizeType n,YSBOOL cpy);
	inline YSRESULT Shrink(void);
// \endcond
};


// STL-like iterator support begin >>
template <class T,class SizeType>
inline typename YsTightArray <T,SizeType>::iterator begin(YsTightArray <T,SizeType> &array)
{
	return array.begin();
}

template <class T,class SizeType>
inline typename YsTightArray <T,SizeType>::iterator end(YsTightArray <T,SizeType> &array)
{
	return array.end();
}

template <class T,class SizeType>
inline typename YsTightArray <T,SizeType>::const_iterator begin(const YsTightArray <T,SizeType> &array)
{
	return array.begin();
}

template <class T,class SizeType>
inline typename YsTightArray <T,SizeType>::const_iterator end(const YsTightArray <T,SizeType> &array)
{
	return array.end();
}

template <class T,class SizeType>
inline typename YsTightArray <T,SizeType>::iterator rbegin(YsTightArray <T,SizeType> &array)
{
	return array.rbegin();
}

template <class T,class SizeType>
inline typename YsTightArray <T,SizeType>::iterator rend(YsTightArray <T,SizeType> &array)
{
	return array.rend();
}

template <class T,class SizeType>
inline typename YsTightArray <T,SizeType>::const_iterator rbegin(const YsTightArray <T,SizeType> &array)
{
	return array.rbegin();
}

template <class T,class SizeType>
inline typename YsTightArray <T,SizeType>::const_iterator rend(const YsTightArray <T,SizeType> &array)
{
	return array.rend();
}


template <class T,class SizeType>
inline YsIndexRange YsTightArray <T,SizeType>::AllIndex(void) const
{
	YsIndexRange allIndex(GetN());
	return allIndex;
}
// STL-like iterator support end <<


template <class T,class SizeType>
inline const T &YsTightArray<T,SizeType>::operator[](YSSIZE_T idx) const
{
	// return GetItem(idx);  2003/01/29 Don't check bound.
	return vv[idx];
}

template <class T,class SizeType>
inline T &YsTightArray<T,SizeType>::operator[](YSSIZE_T idx)
{
	return vv[idx];
	// return GetEditableItem(idx);  2003/01/29 Don't check bound.
}

template <class T,class SizeType>
YsTightArray<T,SizeType>::operator const T *() const
{
	return GetArray();
}

template <class T,class SizeType>
YsTightArray<T,SizeType>::operator T *()
{
	return GetEditableArray();
}

template <class T,class SizeType>
inline SizeType YsTightArray<T,SizeType>::GetN(void) const
{
	return nv;
}

template <class T,class SizeType>
inline YSRESULT YsTightArray<T,SizeType>::Append(const T &dat)
{
	if(&dat<vv || (vv+nv)<=&dat)
	{
		if(Alloc(nv+1,YSTRUE)==YSOK)
		{
			vv[nv-1]=dat;
			return YSOK;
		}
	}
	else
	{
		T tmp;
		tmp=dat;
		if(Alloc(nv+1,YSTRUE)==YSOK)
		{
			vv[nv-1]=tmp;
			return YSOK;
		}
	}
	return YSERR;
}

template <class T,class SizeType>
inline T &YsTightArray<T,SizeType>::GetTop(void)
{
	if(0<nv)
	{
		return vv[0];
	}
	else
	{
		YsErrOut("YsTightArray::GetTop()\n  The array is empty.\n");
		return *((T *)NULL);
	}
}

template <class T,class SizeType>
inline const T &YsTightArray<T,SizeType>::GetTop(void) const
{
	if(0<nv)
	{
		return vv[0];
	}
	else
	{
		YsErrOut("YsTightArray::GetTop()\n  The array is empty.\n");
		return *((T *)NULL);
	}
}

template <class T,class SizeType>
inline T &YsTightArray<T,SizeType>::GetEnd(void)
{
	if(0<nv)
	{
		return vv[nv-1];
	}
	else
	{
		YsErrOut("YsTightArray::GetEnd()\n  The array is empty.\n");
		return *((T *)NULL);
	}
}

template <class T,class SizeType>
inline const T &YsTightArray<T,SizeType>::GetEnd(void) const
{
	if(0<nv)
	{
		return vv[nv-1];
	}
	else
	{
		YsErrOut("YsTightArray::GetEnd()\n  The array is empty.\n");
		return *((T *)NULL);
	}
}

template <class T,class SizeType>
inline YSRESULT YsTightArray<T,SizeType>::SetItem(SizeType idx,const T &v)
{
	if(0<=idx && idx<nv)
	{
		vv[idx]=v;
		return YSOK;
	}
	return YSERR;
}

template <class T,class SizeType>
YSRESULT YsTightArray <T,SizeType>::MakeUnitLength(const T &dat)
{
	return Set(1,&dat);
}

template <class T,class SizeType>
inline T &YsTightArray<T,SizeType>::GetEditableItem(SizeType i)
{
	return vv[i];
}

template <class T,class SizeType>
inline const T &YsTightArray<T,SizeType>::GetItem(SizeType i) const
{
	return vv[i];
}

template <class T,class SizeType>
inline const T *YsTightArray<T,SizeType>::GetArray(void) const
{
	return vv;
}

template <class T,class SizeType>
inline T *YsTightArray<T,SizeType>::GetEditableArray(void)
{
	return vv;
}

template <class T,class SizeType>
inline SizeType YsTightArray <T,SizeType>::GetNAvailable(void) const
{
	return nAvailable;
}


template <class T,class SizeType>
inline SizeType YsTightArray <T,SizeType>::GetNUnused(void) const
{
	return nAvailable-nv;
}

template <class T,class SizeType>
inline YSBOOL YsTightArray<T,SizeType>::IsInRange(SizeType id) const
{
	if(0<=id && id<GetN())
	{
		return YSTRUE;
	}
	return YSFALSE;
}

template <class T,class SizeType>
inline YSBOOL YsTightArray<T,SizeType>::IsIncluded(const T &x) const
{
	for(SizeType i=0; i<GetN(); i++)
	{
		if(vv[i]==x)
		{
			return YSTRUE;
		}
	}
	return YSFALSE;
}

// \cond
template <class T,class SizeType>
inline YSRESULT YsTightArray<T,SizeType>::Alloc(SizeType n,YSBOOL cpy)
{
	if(n<=nAvailable)
	{
		nv=n;
		return YSOK;
	}
	else
	{
		T *neo;
		SizeType toAlloc;

		toAlloc=YsGreater <SizeType> (1,nAvailable*2);
		while(toAlloc<n)
		{
			toAlloc=toAlloc*2;
			if(toAlloc<0) // Overflow
			{
				goto INTOVERFLOW;
			}
		}
		neo=new T [toAlloc];
		if(neo!=NULL)
		{
			if(cpy==YSTRUE && nv>0)    // nv==0 -> don't have to copy
			{
				for(SizeType i=0; i<nv; i++)
				{
					neo[i]=vv[i];
				}
			}
			if(NULL!=vv)
			{
				delete [] vv;
			}
			nv=n;
			vv=neo;
			nAvailable=toAlloc;
			return YSOK;
		}
		else
		{
			goto OUTOFMEM;
		}
// Will never fall into this place
	}
// Will never fall into this place

OUTOFMEM:
	YsExceptionHandler::Exception(YsExceptionHandler::OUTOFMEMORY,"YsTightArray::Alloc()");
	YsErrOut("YsTightArray::Alloc()\n");
	YsErrOut("  Out of memory!!!\n");
	return YSERR;

INTOVERFLOW:
	YsErrOut("YsTightArray::Alloc()\n");
	YsErrOut("  Integer Overflow!!!\n");
	return YSERR;
}

template <class T,class SizeType>
inline YSRESULT YsTightArray <T,SizeType>::Shrink(void)
{
	if(0==nv)
	{
		if(NULL!=vv)
		{
			delete [] vv;
			vv=NULL;
		}
		nAvailable=0;
	}
	else if(NULL!=vv && nv<nAvailable/2)
	{
		SizeType newSize=nAvailable/2;
		while(newSize>1 && newSize>nv*2)
		{
			newSize/=2;
		}
		T *newArray=new T [newSize];
		if(newArray!=NULL)
		{
			for(SizeType i=0; i<nv; i++)
			{
				newArray[i]=vv[i];
			}
			delete [] vv;
			vv=newArray;
			nAvailable=newSize;
		}
		else
		{
			YsExceptionHandler::Exception(YsExceptionHandler::OUTOFMEMORY,"YsTightArray::Shrink()");
			return YSERR;
		}
	}
	return YSOK;
}
// \endcond

template <class T,class SizeType>
YsTightArray<T,SizeType>::YsTightArray()
{
	nv=0;
	nAvailable=0;
	vv=NULL;
}

template <class T,class SizeType>
YsTightArray<T,SizeType>::YsTightArray(SizeType nv,const T v[])
{
	this->nv=0;
	this->nAvailable=0;
	this->vv=NULL;
	Set(nv,v);
}

template <class T,class SizeType>
YsTightArray<T,SizeType>::YsTightArray(const YsTightArray <T,SizeType> &from)
{
	nv=0;
	nAvailable=0;
	vv=NULL;
	Set(from.GetN(),from);
}

template <class T,class SizeType>
template <class SizeType2>
YsTightArray<T,SizeType>::YsTightArray(const YsTightArray <T,SizeType2> &from)
{
	nv=0;
	nAvailable=0;
	vv=NULL;
	Set((SizeType)from.GetN(),from);
}

template <class T,class SizeType>
YsTightArray<T,SizeType>::~YsTightArray()
{
	if(vv!=NULL)
	{
		delete [] vv;
	}
}

template <class T,class SizeType>
const YsTightArray <T,SizeType> &YsTightArray<T,SizeType>::operator=(const YsTightArray <T,SizeType> &from)
{
	if(this->GetArray()!=from.GetArray())
	{
		Set(from.GetN(),from.GetArray());
	}
	return *this;
}

template <class T,class SizeType>
template <class SizeType2>
const YsTightArray <T,SizeType> &YsTightArray<T,SizeType>::operator=(const YsTightArray <T,SizeType2> &from)
{
	if(this->GetArray()!=from.GetArray())
	{
		Set((SizeType)from.GetN(),from.GetArray());
	}
	return *this;
}

template <class T,class SizeType>
YSRESULT YsTightArray<T,SizeType>::Set(SizeType NV,const T V[])
{
	if(NV==0)
	{
		nv=0;
		return YSOK;
	}
	else
	{
		if(Alloc(NV,YSFALSE)==YSOK)
		{
			if(V!=NULL)
			{
				for(SizeType i=0; i<NV; i++)
				{
					vv[i]=V[i];
				}
			}
			return YSOK;
		}
		else
		{
			YsErrOut("YsTightArray::Set()\n  Low Memory Warning!!\n");
			return YSERR;
		}
	}
}

template <class T,class SizeType>
YSRESULT YsTightArray<T,SizeType>::Resize(SizeType neoSize)
{
	return Alloc(neoSize,YSTRUE);
}

template <class T,class SizeType>
YSRESULT YsTightArray<T,SizeType>::Append(SizeType nItm,const T itm[])
{
	SizeType i,cpyTop;
	cpyTop=nv;
	if(Alloc(nv+nItm,YSTRUE)==YSOK)
	{
		for(i=0; i<nItm; i++)
		{
			vv[cpyTop+i]=itm[i];
		}
		return YSOK;
	}
	return YSERR;
}

template <class T,class SizeType>
template <class SizeType2>
YSRESULT YsTightArray<T,SizeType>::Append(const YsTightArray <T,SizeType2> &ary)
{
	return Append((SizeType)ary.GetN(),ary.GetArray());
}

template <class T,class SizeType>
YSRESULT YsTightArray<T,SizeType>::Delete(SizeType id)
{
	if(0<=id && id<nv)
	{
		SizeType i;
		for(i=id; i<nv-1; i++)
		{
			vv[i]=vv[i+1];
		}
		nv--;
		Shrink();
		return YSOK;
	}
	return YSERR;
}

template <class T,class SizeType>
YSRESULT YsTightArray <T,SizeType>::Delete(SizeType idFrom,SizeType numItem)
{
	if(GetN()<=idFrom+numItem)
	{
		Resize(idFrom);
	}
	else
	{
		SizeType i;
		for(i=idFrom; i+numItem<GetN(); i++)
		{
			vv[i]=vv[i+numItem];
		}
		Resize(GetN()-numItem);
	}
	return YSOK;
}

template <class T,class SizeType>
YSRESULT YsTightArray<T,SizeType>::DeleteBySwapping(SizeType id)
{
	if(0<=id && id<nv)
	{
		vv[id]=vv[nv-1];
		nv--;
		Shrink();
		return YSOK;
	}
	return YSERR;
}

template <class T,class SizeType>
YSRESULT YsTightArray<T,SizeType>::DeleteLast(void)
{
	if(nv>0)
	{
		nv--;
		Shrink();
		return YSOK;
	}
	return YSERR;
}

template <class T,class SizeType>
YSRESULT YsTightArray<T,SizeType>::Increment(void)
{
	return Alloc(GetN()+1,YSTRUE);
}

template <class T,class SizeType>
YSRESULT YsTightArray<T,SizeType>::Clear(void)
{
	return Set(0,NULL);
}

template <class T,class SizeType>
YSRESULT YsTightArray<T,SizeType>::ClearDeep(void)
{
	if(NULL!=vv)
	{
		delete vv;
	}
	nv=0;
	nAvailable=0;
	vv=NULL;
	return YSOK;
}

template <class T,class SizeType>
YSRESULT YsTightArray<T,SizeType>::Insert(SizeType id,const T &dat)
{
	if(0<=id && id<=nv && Alloc(nv+1,YSTRUE)==YSOK)
	{
		SizeType i;
		for(i=nv-1; i>id; i--)
		{
			vv[i]=vv[i-1];
		}
		vv[id]=dat;
		return YSOK;
	}
	return YSERR;
}

////////////////////////////////////////////////////////////

template <class T,const int MinimumLength,class SizeType>
template <class SizeType2>
YsArray<T,MinimumLength,SizeType>::YsArray(const YsTightArray <T,SizeType2> &from)
{
	nv=0;
	nAvailable=0;
	vv=NULL;
	Set((SizeType)from.GetN(),from);
}

template <class T,const int MinimumLength,class SizeType>
template <class SizeType2>
const YsArray <T,MinimumLength,SizeType> &YsArray<T,MinimumLength,SizeType>::operator=(const YsTightArray <T,SizeType2> &from)
{
	Set((SizeType)from.GetN(),from.GetArray());
	return *this;
}

template <class T,const int MinimumLength,class SizeType>
template <class SizeType2>
YSRESULT YsArray<T,MinimumLength,SizeType>::Append(const YsTightArray <T,SizeType2> &ary)
{
	return Add((SizeType)ary.GetN(),ary.GetArray());
}

template <class T,const int MinimumLength,class SizeType>
template <class SizeType2>
YSRESULT YsArray<T,MinimumLength,SizeType>::Add(const YsTightArray <T,SizeType2> &ary)
{
	return Add((SizeType)ary.GetN(),ary.GetArray());
}

#endif
