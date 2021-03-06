#include <assert.h>
#include <algorithm>
#include <sstream>
#include <utility>
#include <vector>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH
#include <boost/function.hpp>
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
        , should_quit_(false)
        , thread_(boost::bind(&Server_i::thread_proc, this))
		, time_(0)
    {}

    ~Server_i()
    {
        {
            lock_t lock(m_);
            should_quit_ = true;
        }
        thread_.join();
    }

    void thread_proc()
    {
        for (;;)
        {
            lock_t lock(m_);
            if (should_quit_)
                return;

            /*for ( int i = 0; i != 100; ++i )
                boost::this_thread::yield();*/
			boost::xtime xt;
			boost::xtime_get(&xt, boost::TIME_UTC);
			xt.nsec += 20000;
			boost::thread::sleep(xt);
            prune_dead();
        }
    }

    void prune_dead()
    {
        remove_users(deliver("alive", ""));
    }

    ::CORBA::Boolean _cxx_register(::Chat::User_ptr u, const char* name)
    {
        lock_t lock(m_);

        bool res = insert(v_, corba_user_ptr::make(u), name);
        if (res)
        {
            std::cerr << "user " << name << " registred\n";
            for (user_name_vec::const_iterator it = v_.begin(); it != boost::prior(v_.end()); ++it)
            {
                u->addUser(it->second.c_str());
            }
            remove_users(for_each_user(add_user_t(name)));
        }
        return res;
    }

private:
    struct add_user_t
    {
        add_user_t(const char* name) : name_(name) {}

        void operator() (corba_user_ptr & user)
        {
           user->addUser(name_);
        }
    private:
        const char* name_;
    };


    struct remove_users_t
    {
       remove_users_t( std::vector<std::string> const * removing_users)
            : removing_users_(*removing_users)
       {}

       void operator() (corba_user_ptr & user)
       {
          foreach (std::string const & name, removing_users_)
            user->removeUser(name.c_str());
       }

    private:
       std::vector<std::string> const & removing_users_;
    };

    void remove_users(std::vector<std::string> removing_users)
    {
        while (!removing_users.empty()) 
        {
            std::vector<std::string> tmp = for_each_user(remove_users_t(&removing_users));
            removing_users.swap(tmp);
        }
    }

public:

    void quit(::Chat::User_ptr u)
    {
        lock_t lock(m_);

        user_name_vec::iterator i = std::find_if(v_.begin(), v_.end(), first_is(corba_user_ptr::make(u)));
        if (i == v_.end())
        {
           remove_users(deliver("server", "can't erase user: unknown user"));
           return;
        }

        assert(std::find_if(i + 1, v_.end(), first_is(corba_user_ptr::make(u))) == v_.end());
        std::cerr << "user " << i->second << " quited\n";

        remove_users(std::vector<std::string>(1, i->second));

        v_.erase(i);
    }

private:

    typedef boost::function<void(corba_user_ptr &)> user_functor_t;

    std::vector<std::string> for_each_user(user_functor_t f)
    {
        std::vector<std::string> res;
        user_name_vec::iterator i = v_.begin();
        while (i != v_.end())
        {
            try
            {
                f(i->first);
                ++i;
            }
            catch (CORBA::Exception const & e)
            {
                std::cerr << "Caught a CORBA::" << e._name() << " in send (kick user \"" << i->second << "\")." << std::endl;
                res.push_back(std::string(i->second));
                i = v_.erase(i);
            }
        }
        return res;
    }

public:

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

        remove_users(deliver(u_name, message, time_));
		++time_;
    }

private:

    struct deliver_t
    {
       deliver_t(const char * from, const char * message, int time)
            : from_(from), message_(message), time_(time)
       {}

       void operator() (corba_user_ptr & user)
       {
          user->receive(from_, message_, time_);
       }

    private:
       const char * from_;
       const char * message_;
	   const int time_;
    };

	std::vector<std::string> deliver(std::string const & from, std::string const & message)
	{
		return deliver(from, message, 0);
	}

    std::vector<std::string> deliver(std::string const & from, std::string const & message, int time)
    {
       if ( !( ( from == "alive" ) && ( message == "" ) ) ) 
            std::cerr << from << ": " << message << std::endl;

       return for_each_user(deliver_t(from.c_str(), message.c_str(), time));
    }

private:
    CORBA::ORB_ptr orb_;
    user_name_vec v_;
    boost::mutex m_;
    bool should_quit_;
    boost::thread thread_;
	int time_;
};
