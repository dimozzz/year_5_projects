#include <assert.h>

#include <algorithm>
#include <sstream>
#include <utility>
#include <vector>

#include <boost/thread.hpp>
#include "corba_user_ptr.h"
#include "pair_utils.h"

typedef std::pair<corba_user_ptr, std::string> user_name_t;

typedef std::vector<user_name_t> user_name_vec;

bool insert(user_name_vec & v, corba_user_ptr const & u, std::string const & name)
{
   if (std::find_if(v.begin(), v.end(), first_is(u)) != v.end())
      return false;
   if (std::find_if(v.begin(), v.end(), second_is(name)) != v.end())
      return false;

   v.push_back(user_name_t(u, name));
   return true;
}

void erase(user_name_vec & v, corba_user_ptr const & u)
{
   user_name_vec::iterator i = std::find_if(v.begin(), v.end(), first_is(u));
   if (i == v.end())
   {
      std::cerr << "vector_bimap::erase: unknown user" << std::endl;
      return;
   }

   assert(std::find_if(i + 1, v.end(), first_is(u)) == v.end());

   v.erase(i);
}

user_name_vec::iterator find(user_name_vec & v, corba_user_ptr u)
{
   return std::find_if(v.begin(), v.end(), first_is(u));
}

struct Server_i : POA_Chat::Server
{
    typedef boost::lock_guard<boost::mutex> lock_t;

    Server_i(CORBA::ORB_ptr orb)
        : orb_(orb)
    {}

    ::CORBA::Boolean _cxx_register(::Chat::User_ptr u, const char* name)
    {
        lock_t lock(m_);
        return insert(v_, corba_user_ptr::make(u), name);
    }

    void quit(::Chat::User_ptr u)
    {
        lock_t lock(m_);
        erase(v_, corba_user_ptr::make(u));
    }

    void send(::Chat::User_ptr u, const char* message)
    {
        if (std::string(message) == "/server-quit")
        {
            orb_->shutdown(false);
            return;
        }

        lock_t lock(m_);
        std::string u_name;

        {
            user_name_vec::iterator ui = find(v_, corba_user_ptr::make(u));
            if (ui == v_.end())
                u_name = "<unregistered user>";
            else
                u_name = ui->second;
        }

        user_name_vec::const_iterator i = v_.begin();
        while (i != v_.end())
        {
            try
            {
               i->first->receive(u_name.c_str(), message);
               ++i;
            }
            catch (CORBA::Exception const & e)
            {
               std::cerr << "Caught a CORBA::" << e._name() << " in send (kick user \"" << i->second << "\")." << std::endl;
               i = v_.erase(i);
            }
        }
    }

private:
    CORBA::ORB_ptr orb_;
    user_name_vec v_;
    boost::mutex m_;
};
