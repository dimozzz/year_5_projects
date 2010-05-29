#include <assert.h>

#include <algorithm>
#include <sstream>
#include <utility>
#include <vector>

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

struct vector_bimap
{
    typedef std::vector<user_name_t> user_name_vec;

    bool insert(Chat::User_ptr u, std::string const & name)
    {
        if (std::find_if(v_.begin(), v_.end(), user_is(u)) != v_.end())
            return false;
        if (std::find_if(v_.begin(), v_.end(), name_is(name)) != v_.end())
            return false;

        v_.push_back(user_name_t(Chat::User::_duplicate(u), name));
        return true;
    }

    void erase(Chat::User_ptr u)
    {
        user_name_vec::iterator i = std::find_if(v_.begin(), v_.end(), user_is(u));
        if (i == v_.end())
        {
            std::cerr << "vector_bimap::erase: unknown user" << std::endl;
            return;
        }

        assert(std::find_if(i + 1, v_.end(), user_is(u)) == v_.end());

        v_.erase(i);
    }

    user_name_vec::iterator find(Chat::User_ptr u)
    {
        return std::find_if(v_.begin(), v_.end(), user_is(u));
    }

    // TODO: release references
    user_name_vec v_;
};

struct Server_i : POA_Chat::Server
{
    Server_i()
    {}

    ::CORBA::Boolean _cxx_register(::Chat::User_ptr u, const char* name)
    {
        return v_.insert(u, name);
    }

    void alive(::Chat::User_ptr u)
    {
    }

    void quit(::Chat::User_ptr u)
    {
        v_.erase(u);
    }

    void send(::Chat::User_ptr u, const char* message)
    {
        vector_bimap::user_name_vec::iterator ui = v_.find(u);
        if (ui == v_.v_.end())
        {
            std::cerr << "send: unregistered user: " << u << std::endl;
            return;
        }

        for (vector_bimap::user_name_vec::const_iterator i = v_.v_.begin();
             i != v_.v_.end();
             ++i)
           i->first->receive(ui->second.c_str(), message);
    }

private:
    vector_bimap v_;
};
