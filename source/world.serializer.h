#pragma once

#include <new>
#include <rose/hash.h>
#include <rose/typetraits.h>
#include <rose/serializer.h>
#include <rose/world.h>

///////////////////////////////////////////////////////////////////
//  AUTOGEN
//  command:
//    rose.parser -I world.h -O world.serializer.h
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//  predef enum StoneState
///////////////////////////////////////////////////////////////////
namespace rose {
inline const char * to_string(const StoneState & e);
inline void serialize(StoneState& o, ISerializer& s);
inline void deserialize(StoneState& o, IDeserializer& s);
inline hash_value       hash(const StoneState& o);
} //namespace rose

///////////////////////////////////////////////////////////////////
//  predef enum WorldState
///////////////////////////////////////////////////////////////////
namespace rose {
inline const char * to_string(const WorldState & e);
inline void serialize(WorldState& o, ISerializer& s);
inline void deserialize(WorldState& o, IDeserializer& s);
inline hash_value       hash(const WorldState& o);
} //namespace rose

///////////////////////////////////////////////////////////////////
//  predef struct Color
///////////////////////////////////////////////////////////////////
namespace rose {
inline bool equals(const Color &lhs, const Color &rhs);
inline bool operator==(const Color &lhs, const Color &rhs) { return equals(lhs, rhs); }
inline bool operator!=(const Color &lhs, const Color &rhs) { return !equals(lhs, rhs); }
inline hash_value hash(const Color &o);

template <>
struct type_id<Color>;
template <>
inline const reflection::TypeInfo & reflection::get_type_info<Color>();
} //namespace rose


///////////////////////////////////////////////////////////////////
//  predef struct Vector3
///////////////////////////////////////////////////////////////////
namespace rose {
inline bool equals(const Vector3 &lhs, const Vector3 &rhs);
inline bool operator==(const Vector3 &lhs, const Vector3 &rhs) { return equals(lhs, rhs); }
inline bool operator!=(const Vector3 &lhs, const Vector3 &rhs) { return !equals(lhs, rhs); }
inline hash_value hash(const Vector3 &o);

template <>
struct type_id<Vector3>;
template <>
inline const reflection::TypeInfo & reflection::get_type_info<Vector3>();
} //namespace rose


///////////////////////////////////////////////////////////////////
//  predef struct Stone
///////////////////////////////////////////////////////////////////
namespace rose {
inline bool equals(const Stone &lhs, const Stone &rhs);
inline bool operator==(const Stone &lhs, const Stone &rhs) { return equals(lhs, rhs); }
inline bool operator!=(const Stone &lhs, const Stone &rhs) { return !equals(lhs, rhs); }
inline void serialize(Stone &o, ISerializer &s);
inline void deserialize(Stone &o, IDeserializer &s);
inline hash_value hash(const Stone &o);

template <>
struct type_id<Stone>;
template <>
inline const reflection::TypeInfo & reflection::get_type_info<Stone>();
} //namespace rose


///////////////////////////////////////////////////////////////////
//  predef struct World
///////////////////////////////////////////////////////////////////
namespace rose {
inline bool equals(const World &lhs, const World &rhs);
inline bool operator==(const World &lhs, const World &rhs) { return equals(lhs, rhs); }
inline bool operator!=(const World &lhs, const World &rhs) { return !equals(lhs, rhs); }
inline void serialize(World &o, ISerializer &s);
inline void deserialize(World &o, IDeserializer &s);
inline hash_value hash(const World &o);

template <>
struct type_id<World>;
template <>
inline const reflection::TypeInfo & reflection::get_type_info<World>();
} //namespace rose



#ifndef IMPL_SERIALIZER_UTIL
#define IMPL_SERIALIZER_UTIL

///////////////////////////////////////////////////////////////////
// internal helper methods
///////////////////////////////////////////////////////////////////

namespace rose {
template<class T>
bool rose_parser_equals(const T& lhs, const T& rhs) {
  return lhs == rhs;
}

template<class T, size_t N>
bool rose_parser_equals(const T(&lhs)[N], const T(&rhs)[N]) {
  for (size_t i = 0; i != N; ++i) {
    if (!rose_parser_equals(lhs, rhs)) return false;
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
    if (!rose_parser_equals(lhs, rhs)) return false;
  }
  return true;
}

template<class T>
hash_value rose_parser_hash(const T & value) { return hash(value); }

template<class T>
hash_value rose_parser_hash(const std::vector<T>& v) {
  hash_value h = 0;
  for (const auto& o : v) {
    h ^= rose_parser_hash(o);
    h = xor64(h);
  }
  return h;
}

}
#endif
  
///////////////////////////////////////////////////////////////////
//  impl enum StoneState
///////////////////////////////////////////////////////////////////
inline const char * rose::to_string(const StoneState & e) {
    switch(e) {
        case StoneState::Alive: return "Alive";
        case StoneState::Dead: return "Dead";
        default: return "<UNKNOWN>";
    }
}
inline void rose::serialize(StoneState& o, ISerializer& s) {
  switch (o) {
    case StoneState::Alive: {
      char str[] = "Alive";
      serialize(str, s);
      break;
    }
    case StoneState::Dead: {
      char str[] = "Dead";
      serialize(str, s);
      break;
    }
    default: /* unknown */ break;
  }
}
inline void rose::deserialize(StoneState& o, IDeserializer& s) {
  char str[64];
  deserialize(str, s);
  rose::hash_value h = rose::hash(str);
  switch (h) {
  case rose::hash("Alive"): o = StoneState::Alive; break;
  case rose::hash("Dead"): o = StoneState::Dead; break;
  default: /*unknown value*/ break;
  }
}
inline rose::hash_value rose::hash(const StoneState& o) {
  return static_cast<rose::hash_value>(o);
}

///////////////////////////////////////////////////////////////////
//  impl enum WorldState
///////////////////////////////////////////////////////////////////
inline const char * rose::to_string(const WorldState & e) {
    switch(e) {
        case WorldState::NewGame: return "NewGame";
        case WorldState::Running: return "Running";
        case WorldState::Paused: return "Paused";
        default: return "<UNKNOWN>";
    }
}
inline void rose::serialize(WorldState& o, ISerializer& s) {
  switch (o) {
    case WorldState::NewGame: {
      char str[] = "NewGame";
      serialize(str, s);
      break;
    }
    case WorldState::Running: {
      char str[] = "Running";
      serialize(str, s);
      break;
    }
    case WorldState::Paused: {
      char str[] = "Paused";
      serialize(str, s);
      break;
    }
    default: /* unknown */ break;
  }
}
inline void rose::deserialize(WorldState& o, IDeserializer& s) {
  char str[64];
  deserialize(str, s);
  rose::hash_value h = rose::hash(str);
  switch (h) {
  case rose::hash("NewGame"): o = WorldState::NewGame; break;
  case rose::hash("Running"): o = WorldState::Running; break;
  case rose::hash("Paused"): o = WorldState::Paused; break;
  default: /*unknown value*/ break;
  }
}
inline rose::hash_value rose::hash(const WorldState& o) {
  return static_cast<rose::hash_value>(o);
}

///////////////////////////////////////////////////////////////////
//  impl struct Color
///////////////////////////////////////////////////////////////////
inline bool rose::equals(const Color &lhs, const Color &rhs) {
  return
    rose::rose_parser_equals(lhs.r, rhs.r) &&
    rose::rose_parser_equals(lhs.g, rhs.g) &&
    rose::rose_parser_equals(lhs.b, rhs.b) &&
    rose::rose_parser_equals(lhs.a, rhs.a);
}

inline rose::hash_value rose::hash(const Color &o) {
  rose::hash_value h = 0;
  h ^= rose::rose_parser_hash(o.r);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.g);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.b);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.a);
  return h;
}

template <>
struct rose::type_id<Color> {
    inline static rose::hash_value VALUE = 11375897551430744876ULL;
};

template <>
inline const rose::reflection::TypeInfo & rose::reflection::get_type_info<Color>() {
  static rose::reflection::TypeInfo info = {
    /*             unique_id */ rose::hash("Color"),
    /*           member_hash */ 11375897551430744876ULL,
    /*      memory_footprint */ sizeof(Color),
    /*      memory_alignment */ 16,
    /*                  name */ "Color",
    /*  fp_default_construct */ +[](void * ptr) { new (ptr) Color(); },
    /*   fp_default_destruct */ +[](void * ptr) { std::launder(reinterpret_cast<Color*>(ptr))->~Color(); },
    /*          fp_serialize */ +[](void * ptr, ISerializer & s) { ::rose::serialize(*std::launder(reinterpret_cast<Color*>(ptr)), s); },
    /*        fp_deserialize */ +[](void * ptr, IDeserializer & d) { ::rose::deserialize(*std::launder(reinterpret_cast<Color*>(ptr)), d); }
  };
  return info;
}

///////////////////////////////////////////////////////////////////
//  impl struct Vector3
///////////////////////////////////////////////////////////////////
inline bool rose::equals(const Vector3 &lhs, const Vector3 &rhs) {
  return
    rose::rose_parser_equals(lhs.x, rhs.x) &&
    rose::rose_parser_equals(lhs.y, rhs.y) &&
    rose::rose_parser_equals(lhs.z, rhs.z);
}

inline rose::hash_value rose::hash(const Vector3 &o) {
  rose::hash_value h = 0;
  h ^= rose::rose_parser_hash(o.x);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.y);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.z);
  return h;
}

template <>
struct rose::type_id<Vector3> {
    inline static rose::hash_value VALUE = 865855757241434760ULL;
};

template <>
inline const rose::reflection::TypeInfo & rose::reflection::get_type_info<Vector3>() {
  static rose::reflection::TypeInfo info = {
    /*             unique_id */ rose::hash("Vector3"),
    /*           member_hash */ 865855757241434760ULL,
    /*      memory_footprint */ sizeof(Vector3),
    /*      memory_alignment */ 16,
    /*                  name */ "Vector3",
    /*  fp_default_construct */ +[](void * ptr) { new (ptr) Vector3(); },
    /*   fp_default_destruct */ +[](void * ptr) { std::launder(reinterpret_cast<Vector3*>(ptr))->~Vector3(); },
    /*          fp_serialize */ +[](void * ptr, ISerializer & s) { ::rose::serialize(*std::launder(reinterpret_cast<Vector3*>(ptr)), s); },
    /*        fp_deserialize */ +[](void * ptr, IDeserializer & d) { ::rose::deserialize(*std::launder(reinterpret_cast<Vector3*>(ptr)), d); }
  };
  return info;
}

///////////////////////////////////////////////////////////////////
//  impl struct Stone
///////////////////////////////////////////////////////////////////
inline bool rose::equals(const Stone &lhs, const Stone &rhs) {
  return
    rose::rose_parser_equals(lhs.position, rhs.position) &&
    rose::rose_parser_equals(lhs.size, rhs.size) &&
    rose::rose_parser_equals(lhs.color, rhs.color) &&
    rose::rose_parser_equals(lhs.state, rhs.state);
}

inline void rose::serialize(Stone &o, ISerializer &s) {
  if(s.node_begin("Stone", hash("Stone"), &o)) {
    s.key("position");
    serialize(o.position, s);
    s.key("size");
    serialize(o.size, s);
    s.key("color");
    serialize(o.color, s);
    s.key("state");
    serialize(o.state, s);
    s.node_end();
  }
  s.end();
}

inline void rose::deserialize(Stone &o, IDeserializer &s) {
  while (s.next_key()) {
    switch (s.hash_key()) {
      case rose::hash("position"):
        deserialize(o.position, s);
        break;
      case rose::hash("size"):
        deserialize(o.size, s);
        break;
      case rose::hash("color"):
        deserialize(o.color, s);
        break;
      case rose::hash("state"):
        deserialize(o.state, s);
        break;
      default: s.skip_key(); break;
    }
  }
}

inline rose::hash_value rose::hash(const Stone &o) {
  rose::hash_value h = 0;
  h ^= rose::rose_parser_hash(o.position);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.size);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.color);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.state);
  return h;
}

template <>
struct rose::type_id<Stone> {
    inline static rose::hash_value VALUE = 1551552509579347363ULL;
};

template <>
inline const rose::reflection::TypeInfo & rose::reflection::get_type_info<Stone>() {
  static rose::reflection::TypeInfo info = {
    /*             unique_id */ rose::hash("Stone"),
    /*           member_hash */ 1551552509579347363ULL,
    /*      memory_footprint */ sizeof(Stone),
    /*      memory_alignment */ 16,
    /*                  name */ "Stone",
    /*  fp_default_construct */ +[](void * ptr) { new (ptr) Stone(); },
    /*   fp_default_destruct */ +[](void * ptr) { std::launder(reinterpret_cast<Stone*>(ptr))->~Stone(); },
    /*          fp_serialize */ +[](void * ptr, ISerializer & s) { ::rose::serialize(*std::launder(reinterpret_cast<Stone*>(ptr)), s); },
    /*        fp_deserialize */ +[](void * ptr, IDeserializer & d) { ::rose::deserialize(*std::launder(reinterpret_cast<Stone*>(ptr)), d); }
  };
  return info;
}

///////////////////////////////////////////////////////////////////
//  impl struct World
///////////////////////////////////////////////////////////////////
inline bool rose::equals(const World &lhs, const World &rhs) {
  return
    rose::rose_parser_equals(lhs.cubePosition, rhs.cubePosition) &&
    rose::rose_parser_equals(lhs.currentStick, rhs.currentStick) &&
    rose::rose_parser_equals(lhs.ballPosition, rhs.ballPosition) &&
    rose::rose_parser_equals(lhs.ballVelocity, rhs.ballVelocity) &&
    rose::rose_parser_equals(lhs.ballColor, rhs.ballColor) &&
    rose::rose_parser_equals(lhs.random, rhs.random) &&
    rose::rose_parser_equals(lhs.points, rhs.points) &&
    rose::rose_parser_equals(lhs.lifes, rhs.lifes) &&
    rose::rose_parser_equals(lhs.lifes2, rhs.lifes2) &&
    rose::rose_parser_equals(lhs.previous_pad_event, rhs.previous_pad_event) &&
    rose::rose_parser_equals(lhs.state, rhs.state) &&
    rose::rose_parser_equals(lhs.stones, rhs.stones);
}

inline void rose::serialize(World &o, ISerializer &s) {
  if(s.node_begin("World", hash("World"), &o)) {
    s.key("cubePosition");
    serialize(o.cubePosition, s);
    s.key("currentStick");
    serialize(o.currentStick, s);
    s.key("ballPosition");
    serialize(o.ballPosition, s);
    s.key("ballVelocity");
    serialize(o.ballVelocity, s);
    s.key("ballColor");
    serialize(o.ballColor, s);
    s.key("random");
    serialize(o.random, s);
    s.key("points");
    serialize(o.points, s);
    s.key("lifes");
    serialize(o.lifes, s);
    s.key("lifes2");
    serialize(o.lifes2, s);
    s.key("previous_pad_event");
    serialize(o.previous_pad_event, s);
    s.key("state");
    serialize(o.state, s);
    s.key("stones");
    serialize(o.stones, s);
    s.node_end();
  }
  s.end();
}

inline void rose::deserialize(World &o, IDeserializer &s) {
  while (s.next_key()) {
    switch (s.hash_key()) {
      case rose::hash("cubePosition"):
        deserialize(o.cubePosition, s);
        break;
      case rose::hash("currentStick"):
        deserialize(o.currentStick, s);
        break;
      case rose::hash("ballPosition"):
        deserialize(o.ballPosition, s);
        break;
      case rose::hash("ballVelocity"):
        deserialize(o.ballVelocity, s);
        break;
      case rose::hash("ballColor"):
        deserialize(o.ballColor, s);
        break;
      case rose::hash("random"):
        deserialize(o.random, s);
        break;
      case rose::hash("points"):
        deserialize(o.points, s);
        break;
      case rose::hash("lifes"):
        deserialize(o.lifes, s);
        break;
      case rose::hash("lifes2"):
        deserialize(o.lifes2, s);
        break;
      case rose::hash("previous_pad_event"):
        deserialize(o.previous_pad_event, s);
        break;
      case rose::hash("state"):
        deserialize(o.state, s);
        break;
      case rose::hash("stones"):
        deserialize(o.stones, s);
        break;
      default: s.skip_key(); break;
    }
  }
}

inline rose::hash_value rose::hash(const World &o) {
  rose::hash_value h = 0;
  h ^= rose::rose_parser_hash(o.cubePosition);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.currentStick);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.ballPosition);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.ballVelocity);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.ballColor);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.random);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.points);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.lifes);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.lifes2);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.previous_pad_event);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.state);
  h = rose::xor64(h);
  h ^= rose::rose_parser_hash(o.stones);
  return h;
}

template <>
struct rose::type_id<World> {
    inline static rose::hash_value VALUE = 6205194912455769452ULL;
};

template <>
inline const rose::reflection::TypeInfo & rose::reflection::get_type_info<World>() {
  static rose::reflection::TypeInfo info = {
    /*             unique_id */ rose::hash("World"),
    /*           member_hash */ 6205194912455769452ULL,
    /*      memory_footprint */ sizeof(World),
    /*      memory_alignment */ 16,
    /*                  name */ "World",
    /*  fp_default_construct */ +[](void * ptr) { new (ptr) World(); },
    /*   fp_default_destruct */ +[](void * ptr) { std::launder(reinterpret_cast<World*>(ptr))->~World(); },
    /*          fp_serialize */ +[](void * ptr, ISerializer & s) { ::rose::serialize(*std::launder(reinterpret_cast<World*>(ptr)), s); },
    /*        fp_deserialize */ +[](void * ptr, IDeserializer & d) { ::rose::deserialize(*std::launder(reinterpret_cast<World*>(ptr)), d); }
  };
  return info;
}

