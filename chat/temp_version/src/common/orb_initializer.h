#pragma once

#include <boost/noncopyable.hpp>

struct orb_initializer : boost::noncopyable
{
   orb_initializer(int argc, char ** argv)
      : orb_(CORBA::ORB_init(argc, argv))
   {}

   CORBA::ORB_ptr get()
   {
      return orb_;
   }

   ~orb_initializer()
   {
      orb_->destroy();
   }

   CORBA::ORB_var orb_;
};
