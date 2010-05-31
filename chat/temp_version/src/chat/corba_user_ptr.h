#pragma once

#include "chat.idl.h"

struct corba_user_ptr;

namespace std
{
   void swap(corba_user_ptr & a, corba_user_ptr & b);
}

struct corba_user_ptr
{
   corba_user_ptr()
      : ptr_(Chat::User::_nil())
   {}

   corba_user_ptr(corba_user_ptr const & rhs)
      : ptr_(Chat::User::_duplicate(rhs.ptr_))
   {}

   explicit corba_user_ptr(Chat::User_ptr ptr)
      : ptr_(ptr)
   {}

   ~corba_user_ptr()
   {
      if (!CORBA::is_nil(ptr_))
         CORBA::release(ptr_);
   }

   corba_user_ptr & operator=(corba_user_ptr rhs)
   {
      std::swap(*this, rhs);
      return *this;
   }

   Chat::User_ptr operator->() const
   {
      assert(!CORBA::is_nil(ptr_));
      return ptr_;
   }

   Chat::User_ptr get() const
   {
      return ptr_;
   }

   Chat::User_ptr release()
   {
      Chat::User_ptr t = ptr_;
      ptr_ = Chat::User::_nil();
      return t;
   }

   static corba_user_ptr make(Chat::User_ptr ptr)
   {
      return corba_user_ptr(Chat::User::_duplicate(ptr));
   }

   Chat::User_ptr ptr_;
};

bool operator==(corba_user_ptr const & a, corba_user_ptr const & b)
{
   return a.ptr_->_is_equivalent(b.ptr_);
}

bool operator!=(corba_user_ptr const & a, corba_user_ptr const & b)
{
   return !(a == b);
}

namespace std
{
   void swap(corba_user_ptr & a, corba_user_ptr & b)
   {
      std::swap(a.ptr_, b.ptr_);
   }
}
