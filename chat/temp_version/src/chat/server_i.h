#include <assert.h>

#include <algorithm>
#include <sstream>
#include <utility>
#include <vector>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH  

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

        bool res = insert(v_, corba_user_ptr::make(u), name);
        if (res)
        {
           std::stringstream ss;
           ss << "user \"" << name << "\" registered";
           deliver("server", ss.str().c_str());
           u->receive("server", "you are registred"); 
        }

        return res;
    }

    void quit(::Chat::User_ptr u)
    {
        lock_t lock(m_);

        user_name_vec::iterator i = std::find_if(v_.begin(), v_.end(), first_is(corba_user_ptr::make(u)));
        if (i == v_.end())
        {
           deliver("server", "can't erase user: unknown user");
           return;
        }

        assert(std::find_if(i + 1, v_.end(), first_is(corba_user_ptr::make(u))) == v_.end());

        {
           std::stringstream ss;
           ss << "user \"" << i->second << "\" left";
           deliver("server", ss.str().c_str());
        }

        v_.erase(i);
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
            {
                foreach(user_name_t const & user, v_)
                {
                    std::cout << user.first.ptr_ << "\t" << user.second;
                }
                u_name = "<unregistered user>";
            }
            else
                u_name = ui->second;
        }

        deliver(u_name, message);
    }

    void deliver(std::string const & from, std::string const & message)
    {
       user_name_vec::iterator i = v_.begin();
       while (i != v_.end())
       {
          try
          {
             i->first->receive(from.c_str(), message.c_str());
             ++i;
          }
          catch (CORBA::Exception const & e)
          {
             std::cerr << "Caught a CORBA::" << e._name() << " in send (kick user \"" << i->second << "\")." << std::endl;
             i = v_.erase(i);
          }
       }

       std::cerr << from << ": " << message << std::endl;
    }

private:
    CORBA::ORB_ptr orb_;
    user_name_vec v_;
    boost::mutex m_;
};
