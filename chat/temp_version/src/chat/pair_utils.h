#pragma once

template <typename U>
struct first_is_t
{
   first_is_t(U const & u)
      : u_(u)
   {}

   template <typename V>
   bool operator()(std::pair<U, V> const & p) const
   {
      return p.first == u_;
   }

private:
   U u_;
};

template <typename U>
struct second_is_t
{
   second_is_t(U const & u)
      : u_(u)
   {}

   template <typename V>
   bool operator()(std::pair<V, U> const & p) const
   {
      return p.second == u_;
   }

private:
   U u_;
};

template <typename U>
first_is_t<U> first_is(U const & u)
{
   return first_is_t<U>(u);
}

template <typename U>
second_is_t<U> second_is(U const & u)
{
   return second_is_t<U>(u);
}
