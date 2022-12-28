#pragma once

#include <rose/hash.h>
#include <rose/typetraits.h>
#include <serializer/serializer.h>

///////////////////////////////////////////////////////////////////
//  AUTOGEN                                                      //
//  command:
//    rose.parser -I world.h -O world.serializer.h
///////////////////////////////////////////////////////////////////

namespace rose {
  namespace ecs {
    void        serialize(Vector3 &o, ISerializer &s);
    void      deserialize(Vector3 &o, IDeserializer &s);
  }
  hash_value         hash(const Vector3 &o);
  template<>
  struct type_id<Vector3> {
    inline static hash_value VALUE = 865855757241434760ULL;
  };
  void construct_defaults(      Vector3 &o); // implement me
}
bool operator==(const Vector3 &lhs, const Vector3 &rhs);
bool operator!=(const Vector3 &lhs, const Vector3 &rhs);


struct                World;
namespace rose {
  namespace ecs {
    void        serialize(World &o, ISerializer &s);
    void      deserialize(World &o, IDeserializer &s);
  }
  hash_value         hash(const World &o);
  template<>
  struct type_id<World> {
    inline static hash_value VALUE = 2258745035795743098ULL;
  };
  void construct_defaults(      World &o); // implement me
}
bool operator==(const World &lhs, const World &rhs);
bool operator!=(const World &lhs, const World &rhs);


#ifdef IMPL_SERIALIZER

    #ifndef IMPL_SERIALIZER_UTIL
    #define IMPL_SERIALIZER_UTIL
    #include <cstring>

    namespace {
    //internal helper methods
    template<class T>
    bool rose_parser_equals(const T& lhs, const T& rhs) {
      return lhs == rhs;
    }

    template<class T, size_t N>
    bool rose_parser_equals(const T(&lhs)[N], const T(&rhs)[N]) {
      for (size_t i = 0; i != N; ++i) {
        if (lhs[i] != rhs[i]) return false;
      }
      return true;
    }

    template<size_t N>
    bool rose_parser_equals(const char(&lhs)[N], const char(&rhs)[N]) {
      for (size_t i = 0; i != N; ++i) {
        if (lhs[i] != rhs[i]) return false;
        if (lhs[i] == 0) return true;
      }
      return true;
    }

    template<class T>
    bool rose_parser_equals(const std::vector<T> &lhs, const std::vector<T> &rhs) {
      if (lhs.size() != rhs.size()) return false;
      for (size_t i = 0; i != lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) return false;
      }
      return true;
    }

    template<class TL, class TR>
    void assign(TL& lhs, TR&& rhs) {
      lhs = rhs;
    }

    template<class T>
    void construct_default(std::vector<T> & v) {
      c.clear();
    }
    }
    #endif
  
///////////////////////////////////////////////////////////////////
//  struct Vector3
///////////////////////////////////////////////////////////////////
bool operator==(const Vector3 &lhs, const Vector3 &rhs) {
  return
    rose_parser_equals(lhs.x, rhs.x) &&
    rose_parser_equals(lhs.y, rhs.y) &&
    rose_parser_equals(lhs.z, rhs.z) ;
}

bool operator!=(const Vector3 &lhs, const Vector3 &rhs) {
  return
    !rose_parser_equals(lhs.x, rhs.x) ||
    !rose_parser_equals(lhs.y, rhs.y) ||
    !rose_parser_equals(lhs.z, rhs.z) ;
}

void rose::ecs::serialize(Vector3 &o, ISerializer &s) {
  if(s.node_begin("Vector3", rose::hash("Vector3"), &o)) {
    s.key("x");
    serialize(o.x, s);
    s.key("y");
    serialize(o.y, s);
    s.key("z");
    serialize(o.z, s);
    s.node_end();
  }
  s.end();
}

void rose::ecs::deserialize(Vector3 &o, IDeserializer &s) {
  //implement me
  //construct_defaults(o);

  while (s.next_key()) {
    switch (s.hash_key()) {
      case rose::hash("x"):
        deserialize(o.x, s);
        break;
      case rose::hash("y"):
        deserialize(o.y, s);
        break;
      case rose::hash("z"):
        deserialize(o.z, s);
        break;
      default: s.skip_key(); break;
    }
  }
}

rose::hash_value rose::hash(const Vector3 &o) {
  rose::hash_value h = rose::hash(o.x);
  h = rose::xor64(h);
  h ^= rose::hash(o.y);
  h = rose::xor64(h);
  h ^= rose::hash(o.z);
  return h;
}
///////////////////////////////////////////////////////////////////
//  struct World
///////////////////////////////////////////////////////////////////
bool operator==(const World &lhs, const World &rhs) {
  return
    rose_parser_equals(lhs.cubePosition, rhs.cubePosition) &&
    rose_parser_equals(lhs.ballPosition, rhs.ballPosition) &&
    rose_parser_equals(lhs.ballVelocity, rhs.ballVelocity) &&
    rose_parser_equals(lhs.points, rhs.points) ;
}

bool operator!=(const World &lhs, const World &rhs) {
  return
    !rose_parser_equals(lhs.cubePosition, rhs.cubePosition) ||
    !rose_parser_equals(lhs.ballPosition, rhs.ballPosition) ||
    !rose_parser_equals(lhs.ballVelocity, rhs.ballVelocity) ||
    !rose_parser_equals(lhs.points, rhs.points) ;
}

void rose::ecs::serialize(World &o, ISerializer &s) {
  if(s.node_begin("World", rose::hash("World"), &o)) {
    s.key("cubePosition");
    serialize(o.cubePosition, s);
    s.key("ballPosition");
    serialize(o.ballPosition, s);
    s.key("ballVelocity");
    serialize(o.ballVelocity, s);
    s.key("points");
    serialize(o.points, s);
    s.node_end();
  }
  s.end();
}

void rose::ecs::deserialize(World &o, IDeserializer &s) {
  //implement me
  //construct_defaults(o);

  while (s.next_key()) {
    switch (s.hash_key()) {
      case rose::hash("cubePosition"):
        deserialize(o.cubePosition, s);
        break;
      case rose::hash("ballPosition"):
        deserialize(o.ballPosition, s);
        break;
      case rose::hash("ballVelocity"):
        deserialize(o.ballVelocity, s);
        break;
      case rose::hash("points"):
        deserialize(o.points, s);
        break;
      default: s.skip_key(); break;
    }
  }
}

rose::hash_value rose::hash(const World &o) {
  rose::hash_value h = rose::hash(o.cubePosition);
  h = rose::xor64(h);
  h ^= rose::hash(o.ballPosition);
  h = rose::xor64(h);
  h ^= rose::hash(o.ballVelocity);
  h = rose::xor64(h);
  h ^= rose::hash(o.points);
  return h;
}

#endif
