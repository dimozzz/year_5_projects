#include <assert.h>

#include <algorithm>
#include <sstream>
#include <utility>
#include <vector>

#include <boost/thread.hpp>

typedef std::pair<Chat::User_ptr, std::string> user_name_t;
struct user_is
{
    user_is(Chat::User_ptr u)
        : u_(u)
    {}

    bool operator()(user_name_t const & p) const
    {
        return p.first->_is_equivalent(u_);
    }

    Chat::User_ptr u_;
};

struct name_is
{
    name_is(std::string const & n)
        : n_(n)
    {}

    bool operator()(user_name_t const & p) const
    {
        return p.second == n_;
    }

    std::string n_;
};

typedef std::vector<user_name_t> user_name_vec;

bool insert(user_name_vec & v, Chat::User_ptr u, std::string const & name)
{
   if (std::find_if(v.begin(), v.end(), user_is(u)) != v.end())
      return false;
   if (std::find_if(v.begin(), v.end(), name_is(name)) != v.end())
      return false;

   v.push_back(user_name_t(Chat::User::_duplicate(u), name));
   return true;
}

void erase(user_name_vec & v, Chat::User_ptr u)
{
   user_name_vec::iterator i = std::find_if(v.begin(), v.end(), user_is(u));
   if (i == v.end())
   {
      std::cerr << "vector_bimap::erase: unknown user" << std::endl;
      return;
   }

   assert(std::find_if(i + 1, v.end(), user_is(u)) == v.end());

   v.erase(i);
}

user_name_vec::iterator find(user_name_vec & v, Chat::User_ptr u)
{
   return std::find_if(v.begin(), v.end(), user_is(u));
}

struct Server_i : POA_Chat::Server
{
    typedef boost::lock_guard<boost::mutex> lock_t;

    Server_i()
    {}

    ::CORBA::Boolean _cxx_register(::Chat::User_ptr u, const char* name)
    {
        lock_t lock(m_);
        return insert(v_, u, name);
    }

    void quit(::Chat::User_ptr u)
    {
        lock_t lock(m_);
        erase(v_, u);
    }

    void send(::Chat::User_ptr u, const char* message)
    {
        lock_t lock(m_);
        std::string u_name;

        {
            user_name_vec::iterator ui = find(v_, u);
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
    user_name_vec v_;
    boost::mutex m_;
};
