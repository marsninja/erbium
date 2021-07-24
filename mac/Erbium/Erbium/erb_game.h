/*---------------------------------------------------------------------------
Erbium Game Engine

Implementation of the Erbium Assembler
Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#ifndef ErbGameHeader
#define ErbGameHeader

#include "erb_engine.h"
#include "erb_machine.h"
#include "molokai.h"
#include "erb_assembler.h"
#include <glut/glut.h>

using namespace std;
using namespace k;
using k::u_long;

namespace erb { namespace game {
//Game Object----------------------------------------------------------------
class game_object : public construct
{
private:
  u_long auto_color;
public:
  game_object();
  void draw();
};
    void draw(construct &dc);
//------------------------------------------------------------------------END
}}
#endif
