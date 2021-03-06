/*
 *                         Vortex OpenSplice
 *
 *   This software and documentation are Copyright 2006 to  ADLINK
 *   Technology Limited, its affiliated companies and licensors. All rights
 *   reserved.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */
#ifndef _SACPP_MAPPING_BVL_H_
#define _SACPP_MAPPING_BVL_H_

#include "cpp_dcps_if.h"
// Bounded variable-length sequence

template <class T, DDS::ULong max> class DDS_DCPSBVLSeq
{
public:

   typedef T& _subscript_type;
   typedef const T& _const_subscript_type;

   // DDS Standard

   static T * allocbuf (DDS::ULong n);
   static void freebuf (T * p);

   DDS_DCPSBVLSeq ();
   DDS_DCPSBVLSeq
   (
      DDS::ULong len,
      T * data,
      DDS::Boolean rel = FALSE
   );
   DDS_DCPSBVLSeq (const DDS_DCPSBVLSeq<T, max> &);
   ~DDS_DCPSBVLSeq ();

   DDS_DCPSBVLSeq<T, max> & operator = (const DDS_DCPSBVLSeq<T, max> &);

   DDS::ULong maximum () const;

   void length (DDS::ULong);
   DDS::ULong length () const;

   T & operator[] (DDS::ULong index);
   const T & operator[] (DDS::ULong index) const;

   DDS::Boolean release () const;
   void replace
   (
      DDS::ULong length,
      T * data,
      DDS::Boolean rel = FALSE
   );

   T * get_buffer (DDS::Boolean orphan = FALSE);
   const T * get_buffer () const;

private:

   DDS::ULong m_length;
   DDS::Boolean m_release;
   T * m_buffer;
};

template <class T, DDS::ULong max>
   inline T * DDS_DCPSBVLSeq<T, max>::allocbuf (DDS::ULong nelems)
{
   return new T [nelems];
}

template <class T, DDS::ULong max>
   inline void DDS_DCPSBVLSeq<T, max>::freebuf (T * buffer)
{
   delete [] buffer;
}

template <class T, DDS::ULong max> inline DDS_DCPSBVLSeq<T, max>::DDS_DCPSBVLSeq ()
:
   m_length (0),
   m_release (TRUE),
   m_buffer (NULL)
{}

template <class T, DDS::ULong max> inline DDS_DCPSBVLSeq<T, max>::DDS_DCPSBVLSeq
(
   DDS::ULong len,
   T * data,
   DDS::Boolean rel
)
:
   m_length (len),
   m_release (rel),
   m_buffer (data)
{
   assert (m_length <= max);
}

template <class T, DDS::ULong max> inline DDS_DCPSBVLSeq<T, max>::DDS_DCPSBVLSeq
(
   const DDS_DCPSBVLSeq<T,
   max> & that
)
:
   m_length (0),
   m_release (TRUE),
   m_buffer (NULL)
{
   *this = that;
}

template <class T, DDS::ULong max> inline DDS_DCPSBVLSeq<T, max>::~DDS_DCPSBVLSeq ()
{
   if (m_release && m_buffer)
   {
      freebuf (m_buffer);
   }
}

template <class T, DDS::ULong max> inline DDS_DCPSBVLSeq<T, max>&
DDS_DCPSBVLSeq<T, max>::operator= (const DDS_DCPSBVLSeq<T, max>& that)
{
   if (this != &that)
   {
      m_length = that.m_length;

      if (m_length) {
          m_buffer = allocbuf (max);
          for (DDS::ULong i = 0; i < m_length; i++)
          {
             m_buffer[i] = that.m_buffer[i];
          }
      }
   }

   return *this;
}

template <class T, DDS::ULong max> inline void
DDS_DCPSBVLSeq<T, max>::length (DDS::ULong nelems)
{
   assert (nelems <= max);
   m_length = nelems;
   if (m_length) {
       m_buffer = allocbuf (max);
   }
}

template <class T, DDS::ULong max> inline
DDS::ULong DDS_DCPSBVLSeq<T, max>::length () const
{
   return m_length;
}

template <class T, DDS::ULong max> inline DDS::Boolean
DDS_DCPSBVLSeq<T, max>::release () const
{
   return m_release;
}

template <class T, DDS::ULong max> inline DDS::ULong
DDS_DCPSBVLSeq<T, max>::maximum () const
{
   return max;
}

template <class T, DDS::ULong max> inline T &
DDS_DCPSBVLSeq<T, max>::operator[] (DDS::ULong index)
{
   assert (index < m_length);
   return m_buffer[index];
}

template <class T, DDS::ULong max> inline const T&
DDS_DCPSBVLSeq<T, max>::operator[] (DDS::ULong index) const
{
   assert(index < m_length);
   return m_buffer[index];
}

template <class T, DDS::ULong max> inline T*
DDS_DCPSBVLSeq<T, max>::get_buffer (DDS::Boolean orphan)
{
   T* ret = NULL;

   if (orphan)
   {
      if (m_release)
      {
         m_length = 0;
         m_release = TRUE;
         ret = m_buffer;
         m_buffer = NULL;
      }

      // if orphan && !m_release
      // return nil
   }
   else
   {
      ret = m_buffer;
   }

   return ret;
}

template <class T, DDS::ULong max> inline const T*
DDS_DCPSBVLSeq<T, max>::get_buffer () const
{
   return m_buffer;
}

template <class T, DDS::ULong max> inline void
DDS_DCPSBVLSeq<T, max>::replace (DDS::ULong length, T * data, DDS::Boolean rel)
{
   assert (length <= max);

   if (m_release && m_buffer)
   {
      freebuf(m_buffer);
   }

   m_length = length;
   m_buffer = (T*)data;
   m_release = rel;
}

#undef OS_API
#endif

